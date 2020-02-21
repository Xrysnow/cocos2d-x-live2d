﻿#include "L2DModel.h"
#include "L2DFramework.h"
#include "Id/CubismIdManager.hpp"
#include "Live2DCubismCore.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

using namespace l2d;
using namespace cocos2d;
using namespace Live2D::Cubism::Framework;

constexpr float CubismCanvasSize = 1024.f;
constexpr float CubismCanvasScale = CubismCanvasSize / 2;
static CubismMatrix44 CubismMatrixIdentity = {};
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

	// the full canvas size is 1024*1024, correspond to vertex position range (-1,-1) to (1,1)
	// the canvas size of model is in vertex coord

	const auto w = model->GetModel()->GetCanvasWidth();
	const auto h = model->GetModel()->GetCanvasHeight();
	canvasSize.width = w * CubismCanvasScale;
	canvasSize.height = h * CubismCanvasScale;

	Mat4 trans;
	Mat4::createTranslation(w / 2, h / 2, 0.f, &trans);
	Mat4 scale;
	Mat4::createScale(CubismCanvasScale, CubismCanvasScale, 1.f, &scale);
	constTransform = scale * trans;

	debugRenderer->setPosition(canvasSize.width / 2, canvasSize.height / 2);
	debugRenderer->setScale(CubismCanvasScale);

	updateHitBoxes();

	setContentSize(canvasSize);
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
		const auto p = convertToNodeSpace(Vec2(x, y));
		return it->second.containsPoint((p - canvasSize / 2) / CubismCanvasScale);
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

	const auto& proj = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	auto tr = proj * transform * constTransform;

	memcpy(viewForDraw.GetArray(), tr.m, 16 * sizeof(float));
	updateHitBoxes();

	auto viewCopy = viewForDraw;
	model->Draw(viewCopy);
	if (enableDebugRect)
		drawDebugRects();

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
	auto p = convertToNodeSpace(Vec2(x, y));
	p.x = p.x / canvasSize.width * 2 - 1;
	p.y = p.y / canvasSize.height * 2 - 1;
	p.clamp(-Vec2::ONE, Vec2::ONE);
	model->SetDragging(p.x, p.y);
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
	return canvasSize;
}

void Model::updateHitBoxes()
{
	auto setting = model->GetModelSetting();
	for (auto i = 0; i < setting->GetHitAreasCount(); i++)
	{
		const auto id = setting->GetHitAreaId(i);
		const auto name = setting->GetHitAreaName(i);
		const auto area = model->GetDrawableArea(
			model->GetModel()->GetDrawableIndex(id), CubismMatrixIdentity, CubismVector2(2, 2));
		hitBoxes[name] = { area.X - 1, area.Y - 1, area.Width, area.Height };
	}
}

Rect Model::getCanvasRect() const
{
	const auto w = canvasSize.width;
	const auto h = canvasSize.height;
	return { -w / 2,-h / 2,w,h };
}

void Model::drawDebugRects()
{
	debugRenderer->clear();
	// draw user data areas
	const auto& userDataAreas = model->GetUserDataAreas(CubismMatrixIdentity, CubismVector2(2, 2));
	for (auto j = 0U; j < userDataAreas.GetSize(); ++j)
	{
		const auto area = userDataAreas[j];
		auto rect = Rect(area.X - 1, area.Y - 1, area.Width, area.Height);
		debugRenderer->drawSolidRect(rect.origin, rect.origin + rect.size, userDataAreaColor);
	}
	// draw hit boxes
	for (const auto& it : hitBoxes)
	{
		const auto rect = it.second;
		debugRenderer->drawSolidRect(rect.origin, rect.origin + rect.size, hitAreaColor);
	}
	// draw model canvas
	const auto halfSize = Vec2(canvasSize / (CubismCanvasScale * 2));
	debugRenderer->drawSolidRect(-halfSize, halfSize, Color4F(1.0f, 1.0f, 1.0f, 0.2f));
	// draw full canvas
	//debugRenderer->drawSolidRect(-Vec2::ONE, Vec2::ONE, Color4F(1.0f, 0.0f, 0.0f, 0.2f));
}

bool Model::hitTest(const Vec2& pt, const Camera* camera, Vec3* p) const
{
#if 0
	// only test hit boxes
	auto local = convertToNodeSpace(pt);
	local = (local - canvasSize / 2) / CubismCanvasScale;
	for (auto& it : hitBoxes)
	{
		if (it.second.containsPoint(local))
			return true;
	}
#else
	return Widget::hitTest(pt, camera, p);
#endif
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
