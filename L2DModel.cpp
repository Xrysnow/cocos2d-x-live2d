#include "L2DModel.h"
#include "L2DFramework.h"
#include "Id/CubismIdManager.hpp"
#include "Live2DCubismCore.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

using namespace l2d;
using namespace cocos2d;
using namespace Live2D::Cubism::Framework;

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

	Live2D::Cubism::Core::csmVector2 tmpSizeInPixels;
	Live2D::Cubism::Core::csmVector2 tmpOriginInPixels;
	csmFloat32 tmpPixelsPerUnit;
	csmReadCanvasInfo(model->GetModel()->GetModel(),
		&tmpSizeInPixels, &tmpOriginInPixels, &tmpPixelsPerUnit);

	canvasSizePixel.setSize(tmpSizeInPixels.X, tmpSizeInPixels.Y);
	originPixel.set(tmpOriginInPixels.X, tmpOriginInPixels.Y);
	pixelsPerUnit = model->GetModel()->GetPixelsPerUnit();

	// the full canvas size corresponds to vertex position range (-1,-1) to (1,1)

	Mat4 trans;
	Mat4::createTranslation(
		canvasSizePixel.width / pixelsPerUnit / 2,
		canvasSizePixel.height / pixelsPerUnit / 2, 0.f, &trans);
	Mat4 scale;
	Mat4::createScale(pixelsPerUnit, pixelsPerUnit, 1.f, &scale);
	constTransform = scale * trans;
	debugRenderer->setPosition(canvasSizePixel.width / 2, canvasSizePixel.height / 2);
	debugRenderer->setScale(pixelsPerUnit);
	updateHitBoxes();
	setContentSize(canvasSizePixel);
	setAnchorPoint(originPixel / pixelsPerUnit / 2);

	loadModelInfo();
	return true;
}

void Model::loadModelInfo()
{
	homeDir = model->GetHomeDir();
	auto& motions = model->GetMotions();
	auto setting = model->GetModelSetting();
	for (auto i = 0; i < setting->GetMotionGroupCount(); i++)
	{
		MotionGroupInfo ginfo;
		const auto groupNmae = setting->GetMotionGroupName(i);
		const auto count = setting->GetMotionCount(groupNmae);
		std::vector<MotionInfo> motionInfos;
		for (auto j = 0; j < count; j++)
		{
			MotionInfo info;
			const auto name = StringUtils::format("%s_%d", groupNmae, i);
			info.name = name;
			info.fileName = setting->GetMotionFileName(groupNmae, j);
			auto motion = motions[name.c_str()];
			if (motion)
			{
				info.duration = motion->GetDuration();
				info.loopDuration = motion->GetLoopDuration();
				info.fadeInTime = motion->GetFadeInTime();
				info.fadeOutTime = motion->GetFadeOutTime();
				info.weight = motion->GetWeight();
			}
			info.soundFileName = setting->GetMotionSoundFileName(groupNmae, i);
			ginfo.motionInfos.push_back(info);
		}
		ginfo.name = groupNmae;
		motionGroupInfo[ginfo.name] = ginfo;
	}
	auto& expressions = model->GetExpressions();
	for (auto it = expressions.Begin(); it != expressions.End(); ++it)
	{
		expressionNames.emplace_back(it->First.GetRawString());
	}
}

Size Model::getCanvasSize() const
{
	return canvasSizePixel;
}

Vec2 Model::getCanvasOrigin() const
{
	return originPixel;
}

float Model::getPixelsPerUnit() const
{
	return pixelsPerUnit;
}

std::string Model::getDirectory() const
{
	return homeDir;
}

std::vector<std::string> Model::getMotionGroupNames() const
{
	std::vector<std::string> ret;
	for (auto& it : motionGroupInfo)
		ret.push_back(it.first);
	return ret;
}

size_t Model::getMotionCount(const std::string& groupName)
{
	const auto it = motionGroupInfo.find(groupName);
	if (it != motionGroupInfo.end())
		return it->second.motionInfos.size();
	return 0;
}

std::vector<std::string> Model::getExpressionNames() const
{
	return expressionNames;
}

std::string Model::getSoundFileName(const std::string& groupName, int32_t index)
{
	const auto it = motionGroupInfo.find(groupName);
	if (it != motionGroupInfo.end())
	{
		if (0 <= index && index < it->second.motionInfos.size())
			return it->second.motionInfos[index].soundFileName;
	}
	return {};
}

bool Model::startMotion(const char* groupName, int32_t index, int32_t priority)
{
	return model->StartMotion(groupName, index, priority) != InvalidMotionQueueEntryHandleValue;
}

bool Model::startRandomMotion(const char* groupName, int32_t priority)
{
	return model->StartRandomMotion(groupName, priority) != InvalidMotionQueueEntryHandleValue;
}

bool Model::areaHitTest(const char* hitAreaName, float x, float y)
{
	const auto it = hitBoxes.find(hitAreaName);
	if (it != hitBoxes.end())
	{
		const auto p = convertToNodeSpace(Vec2(x, y));
		return it->second.containsPoint((p - canvasSizePixel / 2) / pixelsPerUnit);
	}
	return false;
}

bool Model::setExpression(const char* expressionName)
{
	return model->SetExpression(expressionName);
}

bool Model::setRandomExpression()
{
	return model->SetRandomExpression();
}

void Model::setAutoDragging(bool b)
{
	if (b)
	{
		if(!autoDraggingCallback)
		{
			autoDraggingCallback = [=](Ref*, TouchEventType e)
			{
				switch (e)
				{
				case TouchEventType::BEGAN: break;
				case TouchEventType::MOVED:
					{
						const auto p = getTouchMovePosition();
						setTouchDragging(p.x, p.y);
					}
					break;
				case TouchEventType::ENDED:
				case TouchEventType::CANCELED:
					resetDragging();
					break;
				default: ;
				}
			};
		}
		if (!autoDragging)
			addTouchEventListener(autoDraggingCallback);
	}
	else
	{
		// only remove when enabled
		if (autoDragging)
			addTouchEventListener(nullptr);
	}
	autoDragging = b;
}

void Model::setDragging(float x, float y)
{
	model->SetDragging(x, y);
}

void Model::setTouchDragging(float x, float y)
{
	auto p = convertToNodeSpace(Vec2(x, y));
	p.x = p.x / canvasSizePixel.width * 2 - 1;
	p.y = p.y / canvasSizePixel.height * 2 - 1;
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

void Model::setModelOpacity(uint8_t opacity)
{
	model->SetOpacity(opacity / 255.f);
}

uint8_t Model::getModelOpacity() const
{
	return model->GetOpacity() * 255.f;
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

void Model::update(float delta)
{
	Widget::update(delta);
	model->Update();
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

void Model::setDebugRectEnable(bool b)
{
	enableDebugRect = b;
}

DrawNode* Model::getDebugRectRenderer() const
{
	return debugRenderer;
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
	const auto halfSize = Vec2(canvasSizePixel / (pixelsPerUnit * 2));
	debugRenderer->drawSolidRect(-halfSize, halfSize, Color4F(1.0f, 1.0f, 1.0f, 0.2f));
	// draw full canvas
	//debugRenderer->drawSolidRect(-Vec2::ONE, Vec2::ONE, Color4F(1.0f, 0.0f, 0.0f, 0.2f));
}

bool Model::hitTest(const Vec2& pt, const Camera* camera, Vec3* p) const
{
#if 0
	// only test hit boxes
	auto local = convertToNodeSpace(pt);
	local = (local - canvasSizePixel / 2) / pixelsPerUnit;
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

float Model::getParameterMax(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetParameterMaximumValue(m->GetParameterIndex(csm_id(name)));
}

float Model::getParameterMin(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetParameterMinimumValue(m->GetParameterIndex(csm_id(name)));
}

float Model::getParameterDefault(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetParameterDefaultValue(m->GetParameterIndex(csm_id(name)));
}

float Model::getParameter(const std::string& name) const
{
	auto m = model->GetModel();
	return m->GetParameterValue(m->GetParameterIndex(csm_id(name)));
}

void Model::setParameter(const std::string& name, float value, float weight)
{
	model->GetModel()->SetParameterValue(csm_id(name), value, weight);
}

void Model::addParameter(const std::string& name, float value, float weight)
{
	model->GetModel()->AddParameterValue(csm_id(name), value, weight);
}

void Model::multiplyParameter(const std::string& name, float value, float weight)
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
	return model->GetModel()->GetDrawableOpacity(getDrawableIndex(name));
}

int32_t Model::getDrawableCulling(const std::string& name) const
{
	return model->GetModel()->GetDrawableCulling(getDrawableIndex(name));
}

int32_t Model::getDrawableBlendMode(const std::string& name) const
{
	return (int32_t)model->GetModel()->GetDrawableBlendMode(getDrawableIndex(name));
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

void Model::setBlinkingInterval(float seconds)
{
	auto blk = model->GetEyeBlink();
	if (blk)
	{
		blk->SetBlinkingInterval(seconds);
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

int32_t Model::getDrawableIndex(const std::string& name) const
{
	return model->GetModel()->GetDrawableIndex(csm_id(name));
}
