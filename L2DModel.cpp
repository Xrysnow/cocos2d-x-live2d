#include "L2DModel.h"
#include "L2DFramework.h"
#include "Id/CubismIdManager.hpp"
#include "Live2DCubismCore.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

using namespace l2d;
using namespace cocos2d;
using namespace Live2D::Cubism::Framework;

std::unordered_set<Model*> Model::instances;

CubismIdHandle csm_id(const std::string& str)
{
	return CubismFramework::GetIdManager()->GetId(str.c_str());
}

Model::Model()
{
	instances.insert(this);
}

Model::~Model()
{
	if (_running)
	{
		Model::onExit();
	}
	if(recreatListener)
		Director::getInstance()->getEventDispatcher()->removeEventListener(recreatListener);
	CC_SAFE_DELETE(model);
	instances.erase(this);
}

Model* Model::create(const std::string& dir, const std::string& fileName)
{
	auto ret = new (std::nothrow) Model();
	if (Framework::lazyInit() && ret && ret->_init(dir, fileName))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
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
	if (!model->LoadAssets(d.c_str(), fileName.c_str()))
	{
		cocos2d::log("can't load model '%s' at [%s]", fileName.c_str(), d.c_str());
		return false;
	}
	recreatListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [&](EventCustom*)
	{
		model->ReloadRenderer();
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(recreatListener, 2);
	model->GetRenderer<Rendering::CubismRenderer>()->IsPremultipliedAlpha(false);

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
	return true;
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
	drawCommandBefore.init(_globalZOrder);
	drawCommandBefore.func = [=]()
	{
		Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
		Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
	};
	renderer->addCommand(&drawCommandBefore);

	const auto window = Director::getInstance()->getWinSize();
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
		drawDebugRects();

	drawCommandAfter.init(_globalZOrder);
	drawCommandAfter.func = []()
	{
		Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	};
	renderer->addCommand(&drawCommandAfter);

	Widget::draw(renderer, transform, flags);
}

void Model::setModelOpacity(uint8_t opacity)
{
	model->SetOpacity(opacity / 255.f);
}

uint8_t Model::getModelOpacity() const
{
	return model->GetOpacity() * 255;
}

void Model::setModelColor(const Color4B& color)
{
	model->GetRenderer<Rendering::CubismRenderer>()->SetModelColor(
		color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}

Color4B Model::getModelColor() const
{
	const auto color = model->GetRenderer<Rendering::CubismRenderer>()->GetModelColor();
	return { uint8_t(color.R*255.f), uint8_t(color.G*255.f), uint8_t(color.B*255.f), uint8_t(color.A*255.f) };
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
	const auto xx = std::max(-1.f, std::min(p.x / sz.width, 1.f));
	const auto yy = std::max(-1.f, std::min(p.y / sz.height, 1.f));
	model->SetDragging(xx, yy);
}

void Model::resetDragging()
{
	model->SetDragging(0, 0);
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
	return isScreenPointInRect(pt, camera, getWorldToNodeTransform(),
		{ -size.width * anch.x, -size.height * anch.y, size.width, size.height }, nullptr);
}

int32_t Model::getParameterCount() const
{
	return model->GetModel()->GetParameterCount();
}

std::vector<std::string> Model::getParameterNames() const
{
	const auto m = model->GetModel()->GetModel();
	const auto parameterIds = csmGetParameterIds(m);
	const auto parameterCount = csmGetParameterCount(m);
	std::vector<std::string> ret;
	ret.reserve(parameterCount);
	for (auto i = 0; i < parameterCount; ++i)
		ret.emplace_back(parameterIds[i]);
	return ret;
}

float Model::getParameterMaximumValue(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetParameterMaximumValue(m->GetParameterIndex(csm_id(name)));
}

float Model::getParameterMinimumValue(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetParameterMinimumValue(m->GetParameterIndex(csm_id(name)));
}

float Model::getParameterDefaultValue(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetParameterDefaultValue(m->GetParameterIndex(csm_id(name)));
}

float Model::getParameterValue(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetParameterValue(m->GetParameterIndex(csm_id(name)));
}

void Model::setParameterValue(const std::string& name, float value, float weight)
{
	model->GetModel()->SetParameterValue(csm_id(name), value, weight);
}

void Model::addParameterValue(const std::string& name, float value, float weight)
{
	model->GetModel()->AddParameterValue(csm_id(name), value, weight);
}

void Model::multiplyParameterValue(const std::string& name, float value, float weight)
{
	model->GetModel()->MultiplyParameterValue(csm_id(name), value, weight);
}

int32_t Model::getPartCount() const
{
	return model->GetModel()->GetPartCount();
}

std::vector<std::string> Model::getPartNames() const
{
	const auto m = model->GetModel()->GetModel();
	const auto partIds = csmGetPartIds(m);
	const auto partCount = csmGetPartCount(m);
	std::vector<std::string> ret;
	ret.reserve(partCount);
	for (auto i = 0; i < partCount; ++i)
		ret.emplace_back(partIds[i]);
	return ret;
}

float Model::getPartOpacity(const std::string& name) const
{
	return model->GetModel()->GetPartOpacity(csm_id(name));
}

void Model::setPartOpacity(const std::string& name, float opacity)
{
	model->GetModel()->SetPartOpacity(csm_id(name), opacity);
}

int32_t Model::getDrawableCount() const
{
	return model->GetModel()->GetDrawableCount();
}

std::vector<std::string> Model::getDrawableNames() const
{
	const auto m = model->GetModel()->GetModel();
	const auto drawableIds = csmGetDrawableIds(m);
	const auto drawableCount = csmGetDrawableCount(m);
	std::vector<std::string> ret;
	ret.reserve(drawableCount);
	for (auto i = 0; i < drawableCount; ++i)
		ret.emplace_back(drawableIds[i]);
	return ret;
}

float Model::getDrawableOpacity(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetDrawableOpacity(m->GetDrawableIndex(csm_id(name)));
}

int32_t Model::getDrawableCulling(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetDrawableCulling(m->GetDrawableIndex(csm_id(name)));
}

int32_t Model::getDrawableBlendMode(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetDrawableBlendMode(m->GetDrawableIndex(csm_id(name)));
}

Vec2 Model::getGravity() const
{
	const auto phy = model->GetPhysics();
	if (phy)
	{
		const auto g = phy->GetOptions().Gravity;
		return {g.X, g.Y};
	}
	return {};
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
		return {g.X, g.Y};
	}
	return {};
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
