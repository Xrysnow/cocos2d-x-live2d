#include "Model.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "LogSystem.h"
#include "XLive2D.h"
#include "Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp"

using namespace std;
using namespace l2d;
using namespace cocos2d;
using namespace Live2D::Cubism::Framework;

Model::Model()
{
}

Model::~Model()
{
	if (_running)
	{
		Model::onExit();
	}
	if(_recreatListener)
		Director::getInstance()->getEventDispatcher()->removeEventListener(_recreatListener);
	CC_SAFE_DELETE(model);
}

Model* Model::create(const std::string& dir, const std::string& fileName)
{
	auto ret = new (std::nothrow) Model();
	if (XLive2D::lazyInit() && ret && ret->_init(dir, fileName))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	LERROR("create XLive2DModel failed");
	return nullptr;
}

bool Model::_init(const std::string& dir, const std::string& fileName)
{
	if (!Widget::init()) return false;
	debugRenderer = DrawNode::create();
	if (!debugRenderer) return false;
	addChild(debugRenderer, 1);

	model = new (std::nothrow) LAppModel();
	if (!model) return false;
	auto d = dir;
	const auto end = d[d.size() - 1];
	if (end != '/' && end != '\\')
		d += "/";
	_recreatListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [&](EventCustom*)
	{
		model->ReloadRenderer();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_recreatListener, 2);
	auto ret = model->LoadAssets(d.c_str(), fileName.c_str());
	if (!ret)
	{
		LERROR("cannot load model '%s' at [%s]", fileName.c_str(), d.c_str());
	}
	model->GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->IsPremultipliedAlpha(false);

	const auto window = Director::getInstance()->getWinSize();
	auto tr = Mat4::IDENTITY;
	Vec3 translation;
	tr.getTranslation(&translation);
	tr.scale(1024 / window.width, 1024 / window.height, 0);
	// scale to make winsize => (-1, 1)
	tr.m[12] = translation.x / window.width * 2 - 1;
	tr.m[13] = translation.y / window.height * 2 - 1;
	//tr.m[14] = 0;
	memcpy(viewForDraw.GetArray(), tr.m, 16 * sizeof(float));
	updateHitBoxes();

	setIgnoreAnchorPointForPosition(true);
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setContentSize(getCanvasRect().size);
	return ret;
}

bool Model::startMotion(const char* group, int32_t no, int32_t priority)
{
	return model->StartMotion(group, no, priority) != InvalidMotionQueueEntryHandleValue;
}

bool Model::startRandomMotion(const char* group, int32_t priority)
{
	return model->StartRandomMotion(group, priority) != InvalidMotionQueueEntryHandleValue;
}

bool Model::areaHitTest(const char* hitAreaName, float x, float y)
{
	const auto it = hitBoxes.find(hitAreaName);
	if (it != hitBoxes.end())
	{
		return it->second.containsPoint(convertToNodeSpace(Vec2(x, y)));
	}
	return false;
}

bool Model::setExpression(const char* expressionID)
{
	return model->SetExpression(expressionID);
}

bool Model::setRandomExpression()
{
	return model->SetRandomExpression();
}

void Model::onEnter()
{
	Widget::onEnter();
	model->Update();
}

void Model::onExit()
{
	Widget::onExit();
}

void Model::update(float delta)
{
	Widget::update(delta);
	model->Update();
}

void Model::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
	if (!_visible)
		return;
	getNodeToParentTransform();
	_drawCommand.init(_globalZOrder);
	_drawCommand.func = CC_CALLBACK_0(Model::onDrawModel, this, transform, flags);
	renderer->addCommand(&_drawCommand);

	Widget::draw(renderer, transform, flags);
}

void Model::setModelOpacity(GLubyte opacity)
{
	model->SetOpacity(opacity / 255.f);
}

GLubyte Model::getModelOpacity() const
{
	return model->GetOpacity() * 255;
}

void Model::setModelColor(const Color4B& color)
{
	model->GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->SetModelColor(
		color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}

Color4B Model::getModelColor() const
{
	const auto color = model->GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->GetModelColor();
	return Color4B(color.R, color.G, color.B, color.A);
}

void Model::setDragging(float x, float y)
{
	//const auto rect = getCanvasRect();
	//const auto off_x = rect.getMidX();
	//const auto off_y = rect.getMidY();
	//const auto xx = max(-1.f, min((x - off_x) / rect.size.width, 1.f));
	//const auto yy = max(-1.f, min((y - off_y) / rect.size.height, 1.f));
	const auto p = convertToNodeSpace(Vec2(x, y));
	const auto sz = getContentSize() / 2;
	const auto xx = max(-1.f, min(p.x / sz.width, 1.f));
	const auto yy = max(-1.f, min(p.y / sz.height, 1.f));
	model->SetDragging(xx, yy);
}

void Model::setAcceleration(float x, float y, float z)
{
	model->SetAcceleration(x, y, z);
}

void Model::setOnHitCallback(int handler)
{
	onHitCallback = [=](float x,float y)
	{
		auto stack = LuaEngine::getInstance()->getLuaStack();
		stack->pushFloat(x);
		stack->pushFloat(y);
		stack->executeFunctionByHandler(handler, 2);
		stack->clean();
	};
}

void Model::setOnDraggingCallback(int handler)
{
	onDraggingCallback = [=](float x, float y)
	{
		auto stack = LuaEngine::getInstance()->getLuaStack();
		stack->pushFloat(x);
		stack->pushFloat(y);
		stack->executeFunctionByHandler(handler, 2);
		stack->clean();
	};
}

Size Model::getCanvasSize() const
{
	return { model->GetModel()->GetCanvasWidth(),model->GetModel()->GetCanvasHeight() };
}

void Model::onDrawModel(const Mat4& transform, uint32_t flags)
{
	auto director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	const auto window = director->getWinSize();
	auto tr = transform;
	Vec3 translation;
	tr.getTranslation(&translation);
	tr.scale(1024 / window.width, 1024 / window.height, 0);
	// scale to make winsize => (-1, 1)
	tr.m[12] = translation.x / window.width * 2 - 1;
	tr.m[13] = translation.y / window.height * 2 - 1;
	//tr.m[14] = 0;

	memcpy(viewForDraw.GetArray(), tr.m, 16 * sizeof(float));
	updateHitBoxes();

	auto viewCopy = viewForDraw;
	model->Draw(viewCopy);
	if (enableDebugRect)
	{
		drawDebugRects();
	}

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void Model::updateHitBoxes()
{
	const auto window = Director::getInstance()->getWinSize();
	const auto scale_x = window.width / getScaleX()*0.5;
	const auto scale_y = window.height / getScaleY()*0.5;
	auto setting = model->GetModelSetting();
	for (auto i = 0; i < setting->GetHitAreasCount(); i++)
	{
		const auto id = setting->GetHitAreaId(i);
		const auto name = setting->GetHitAreaName(i);
		const auto area = model->GetDrawableArea(
			model->GetModel()->GetDrawableIndex(id), viewForDraw, CubismVector2(2, 2));
		auto rect = Rect(area.X - 1, area.Y - 1, area.Width, area.Height);
		rect.origin.x *= scale_x;
		rect.origin.y *= scale_y;
		rect.size.width *= scale_x;
		rect.size.height *= scale_y;
		hitBoxes[name] = rect;
	}
}

Rect Model::getCanvasRect() const
{
	const auto window = Director::getInstance()->getWinSize();
	const auto scale_x = window.width / getScaleX()*0.5;
	const auto scale_y = window.height / getScaleY()*0.5;
	CubismMatrix44 currentMatrix(viewForDraw);
	currentMatrix.MultiplyByMatrix(model->GetModelMatrix());
	const auto size = getCanvasSize();
	const float right = size.width / 2;
	const float left = -right;
	const float bottom = size.height / 2;
	const float top = -bottom;
	const auto arr = currentMatrix.GetArray();
	const auto convertLeft = (left * arr[0] + top * arr[1]) * scale_x;
	const auto convertTop = (left * arr[4] + top * arr[5]) * scale_y;
	const auto convertRight = (right * arr[0] + bottom * arr[1]) * scale_x;
	const auto convertBottom = (right * arr[4] + bottom * arr[5]) * scale_y;
	// origin is left-bottom
	return Rect(std::min(convertLeft, convertRight), std::min(convertBottom, convertTop),
		std::abs(convertRight - convertLeft), std::abs(convertTop - convertBottom));
}

void Model::drawDebugRects()
{
	debugRenderer->clear();
	const auto window = Director::getInstance()->getWinSize();
	const auto scale_x = window.width / getScaleX()*0.5;
	const auto scale_y = window.height / getScaleY()*0.5;
	//TODO: verify userDataAreas rects
	const auto& userDataAreas = model->GetUserDataAreas(viewForDraw, CubismVector2(2, 2));
	for (auto j = 0U; j < userDataAreas.GetSize(); ++j)
	{
		const auto area = userDataAreas[j];
		auto rect = Rect(area.X - 1, area.Y - 1, area.Width, area.Height);
		rect.origin.x *= scale_x;
		rect.origin.y *= scale_y;
		rect.size.width *= scale_x;
		rect.size.height *= scale_y;
		debugRenderer->drawSolidRect(rect.origin, rect.origin + rect.size, userDataAreaColor);
	}

	for (const auto& it : hitBoxes)
	{
		const auto rect = it.second;
		debugRenderer->drawSolidRect(rect.origin, rect.origin + rect.size, hitAreaColor);
	}
	const auto riv = getCanvasRect();
	debugRenderer->drawSolidRect(riv.origin, riv.origin + riv.size, Color4F(1.0f, 1.0f, 1.0f, 0.2f));
}

bool Model::hitTest(const cocos2d::Vec2& pt, const cocos2d::Camera* camera, cocos2d::Vec3* p) const
{
	const auto size = getContentSize();
	const auto anch = getAnchorPoint();
	Rect rect(-size.width * anch.x, -size.height * anch.y, size.width, size.height);
	return isScreenPointInRect(pt, camera, getWorldToNodeTransform(), rect, nullptr);
}

float Model::getParameterMaximumValue(const char* name) const
{
	auto id = CubismId(name);
	auto m = model->GetModel();
	return m->GetParameterMaximumValue(m->GetParameterIndex(&id));
}

float Model::getParameterMinimumValue(const char* name) const
{
	auto id = CubismId(name);
	auto m = model->GetModel();
	return m->GetParameterMinimumValue(m->GetParameterIndex(&id));
}

float Model::getParameterValue(const char* name) const
{
	auto id = CubismId(name);
	auto m = model->GetModel();
	return m->GetParameterValue(m->GetParameterIndex(&id));
}

void Model::setParameterValue(const char* name, float value, float weight)
{
	auto id = CubismId(name);
	model->GetModel()->SetParameterValue(&id, value, weight);
}

void Model::addParameterValue(const char* name, float value, float weight)
{
	auto id = CubismId(name);
	model->GetModel()->AddParameterValue(&id, value, weight);
}

void Model::multiplyParameterValue(const char* name, float value, float weight)
{
	auto id = CubismId(name);
	model->GetModel()->MultiplyParameterValue(&id, value, weight);
}

Vec2 Model::getGravity() const
{
	const auto phy = model->GetPhysics();
	if (phy)
	{
		const auto g = phy->GetOptions().Gravity;
		return Vec2(g.X, g.Y);
	}
	return Vec2();
}

void Model::setGravity(const Vec2& gravity)
{
	auto phy = model->GetPhysics();
	if (phy)
	{
		auto op = phy->GetOptions();
		op.Gravity = CubismVector2(gravity.x, gravity.y);
		phy->SetOptions(op);
	}
}

Vec2 Model::getWind() const
{
	const auto phy = model->GetPhysics();
	if (phy)
	{
		const auto g = phy->GetOptions().Wind;
		return Vec2(g.X, g.Y);
	}
	return Vec2();
}

void Model::setWind(const Vec2& wind)
{
	auto phy = model->GetPhysics();
	if (phy)
	{
		auto op = phy->GetOptions();
		op.Wind = CubismVector2(wind.x, wind.y);
		phy->SetOptions(op);
	}
}

void Model::setBlinkingInterval(float blinkigInterval)
{
	auto blk = model->GetEyeBlink();
	if (blk)
	{
		blk->SetBlinkingInterval(blinkigInterval);
	}
}

void Model::setBlinkingSettings(float closing, float closed, float opening)
{
	auto blk = model->GetEyeBlink();
	if (blk)
	{
		blk->SetBlinkingSettings(closing, closed, opening);
	}
}

float Model::getLipValue() const
{
	return model->GetLipValue();
}

void Model::setLipValue(float value)
{
	model->SetLipValue(value);
}
