#pragma once
#include "LAppModel.hpp"
#include <CubismFramework.hpp>
#include <Math/CubismMatrix44.hpp>
#include "cocos2d.h"
#include "ui/UIWidget.h"
#include <unordered_set>

namespace l2d
{
	class Model : public cocos2d::ui::Widget
	{
	protected:
		Model();
		~Model();
		bool _init(const std::string& dir, const std::string& fileName);
		void loadModelInfo();
	public:
		static Model* create(const std::string& dir, const std::string& fileName);

		// info

		cocos2d::Size getCanvasSize() const;
		std::string getDirectory() const;
		std::vector<std::string> getMotionGroupNames() const;
		size_t getMotionCount(const std::string& groupName);
		std::vector<std::string> getExpressionNames() const;
		std::string getSoundFileName(const std::string& groupName, int32_t index);

		// operation

		bool startMotion(const char* groupName, int32_t index, int32_t priority);
		bool startRandomMotion(const char* groupName, int32_t priority);
		bool areaHitTest(const char* hitAreaName, float x, float y);
		bool setExpression(const char* expressionName);
		bool setRandomExpression();
		void setAutoDragging(bool b);
		void setDragging(float x, float y);
		void setTouchDragging(float x, float y);
		void resetDragging();
		void setAcceleration(float x, float y, float z);

		// property

		void setModelOpacity(uint8_t opacity);
		uint8_t getModelOpacity() const;
		void setModelColor(const cocos2d::Color4B& color);
		cocos2d::Color4B getModelColor() const;

		void setDebugRectEnable(bool b);
		cocos2d::DrawNode* getDebugRectRenderer() const;
		void update(float delta) override;
	protected:
		void onEnter() override;
		void onExit() override;
		void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
		void updateHitBoxes();
		void drawDebugRects();
		bool hitTest(const cocos2d::Vec2& pt, const cocos2d::Camera* camera, cocos2d::Vec3* p) const override;
	public:

		// parameter

		int32_t getParameterCount() const;
		std::vector<std::string> getParameterNames() const;
		float getParameterMaximumValue(const std::string& name) const;
		float getParameterMinimumValue(const std::string& name) const;
		float getParameterDefaultValue(const std::string& name) const;
		float getParameterValue(const std::string& name) const;
		void  setParameterValue(const std::string& name, float value, float weight = 1.0f);
		void  addParameterValue(const std::string& name, float value, float weight = 1.0f);
		void  multiplyParameterValue(const std::string& name, float value, float weight = 1.0f);

		// part

		int32_t getPartCount() const;
		std::vector<std::string> getPartNames() const;
		float getPartOpacity(const std::string& name) const;
		void setPartOpacity(const std::string& name, float opacity);

		// drawable

		int32_t getDrawableCount() const;
		std::vector<std::string> getDrawableNames() const;
		float getDrawableOpacity(const std::string& name) const;
		int32_t getDrawableCulling(const std::string& name) const;
		int32_t getDrawableBlendMode(const std::string& name) const;

		// gravity, wind

		cocos2d::Vec2 getGravity() const;
		void setGravity(const cocos2d::Vec2& gravity);
		cocos2d::Vec2 getWind() const;
		void setWind(const cocos2d::Vec2& wind);

		// blinking, lip

		void setBlinkingInterval(float blinkigInterval);// in seconds
		void setBlinkingSettings(float closing, float closed, float opening);
		float getLipValue() const;
		void setLipValue(float value);

		CC_DISALLOW_COPY_AND_ASSIGN(Model);
	protected:

		LAppModel* model = nullptr;
		std::string homeDir;

		struct MotionInfo
		{
			std::string name;
			std::string fileName;
			std::string soundFileName;
			float duration = -1;
			float loopDuration = -1;
			float fadeInTime = -1;
			float fadeOutTime = -1;
			float weight = -1;
		};
		struct MotionGroupInfo
		{
			std::string name;
			std::vector<MotionInfo> motionInfos;
		};
		std::unordered_map<std::string, MotionGroupInfo> motionGroupInfo;
		std::vector<std::string> expressionNames;

		cocos2d::Color4F hitAreaColor = cocos2d::Color4F(1.0f, 0, 0, 0.2f);
		cocos2d::Color4F userDataAreaColor = cocos2d::Color4F(0, 0, 1.0f, 0.2f);
		cocos2d::EventListenerCustom* recreatListener = nullptr;
		cocos2d::Size canvasSize;
		cocos2d::Mat4 constTransform;
		Csm::CubismMatrix44 viewForDraw;
		std::unordered_map<std::string, cocos2d::Rect> hitBoxes;
		cocos2d::CallbackCommand drawCommandBefore;
		cocos2d::CallbackCommand drawCommandAfter;
		bool enableDebugRect = false;
		cocos2d::DrawNode* debugRenderer = nullptr;
		bool autoDragging = false;
		ccWidgetTouchCallback autoDraggingCallback;

		static std::unordered_set<Model*> instances;

		friend class Framework;
	};
}
