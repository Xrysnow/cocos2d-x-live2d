#pragma once
#include "cocos2d.h"
#include "LAppModel.hpp"
#include <CubismFramework.hpp>
#include <Math/CubismMatrix44.hpp>
#include "ui/UIWidget.h"

namespace l2d
{
	class Model : public cocos2d::ui::Widget
	{
	private:
		Model();
	public:
		~Model();
		static Model* create(const std::string& dir, const std::string& fileName);
	protected:
		bool _init(const std::string& dir, const std::string& fileName);
	public:
		bool startMotion(const char* group, int32_t no, int32_t priority);
		bool startRandomMotion(const char* group, int32_t priority);

		bool areaHitTest(const char* hitAreaName, float x, float y);
		bool setExpression(const char* expressionID);
		bool setRandomExpression();

		std::vector<std::string> getMotionNames() const { return model->GetMotionNames(); } // group_no
		std::vector<std::string> getExpressionNames() const { return model->GetExpressionNames(); }
		std::vector<std::string> getMotionGroupNames() const { return model->GetMotionGroupNames(); }

		void onEnter() override;
		void onExit() override;

		void update(float delta) override;

		void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

		void setModelOpacity(GLubyte opacity);
		GLubyte getModelOpacity() const;

		void setModelColor(const cocos2d::Color4B& color);
		cocos2d::Color4B getModelColor() const;

		void setDragging(float x, float y);
		void setAcceleration(float x, float y, float z);

		void setOnHitCallback(std::function<void(float, float)> cb) { onHitCallback = cb; }
		void setOnHitCallback(int handler);
		void setOnDraggingCallback(std::function<void(float, float)> cb) { onDraggingCallback = cb; }
		void setOnDraggingCallback(int handler);

		void setDebugRectEnable(bool b) { enableDebugRect = b; }

		cocos2d::Size getCanvasSize() const;
		cocos2d::Rect getCanvasRect() const;

		cocos2d::DrawNode* getDebugRectRenderer() const { return debugRenderer; }

	protected:
		void onDrawModel(const cocos2d::Mat4& transform, uint32_t flags);
		void updateHitBoxes();
		void drawDebugRects();

		bool hitTest(const cocos2d::Vec2& pt, const cocos2d::Camera* camera, cocos2d::Vec3* p) const override;
	public:

		float getParameterMaximumValue(const char* name) const;
		float getParameterMinimumValue(const char* name) const;
		float getParameterValue(const char* name) const;
		void  setParameterValue(const char* name, float value, float weight = 1.0f);
		void  addParameterValue(const char* name, float value, float weight = 1.0f);
		void  multiplyParameterValue(const char* name, float value, float weight = 1.0f);

		cocos2d::Vec2 getGravity() const;
		void setGravity(const cocos2d::Vec2& gravity);
		cocos2d::Vec2 getWind() const;
		void setWind(const cocos2d::Vec2& wind);

		void setBlinkingInterval(float blinkigInterval);// in seconds
		void setBlinkingSettings(float closing, float closed, float opening);

		float getLipValue() const;
		void setLipValue(float value);
	private:

		LAppModel* model = nullptr;
		std::function<void(float, float)> onHitCallback = nullptr;
		std::function<void(float, float)> onDraggingCallback = nullptr;
		cocos2d::Color4F hitAreaColor = cocos2d::Color4F(1.0f, 0, 0, 0.2f);
		cocos2d::Color4F userDataAreaColor = cocos2d::Color4F(0, 0, 1.0f, 0.2f);

		cocos2d::EventListenerCustom* _recreatListener = nullptr;

		Csm::CubismMatrix44 viewForDraw;
		std::unordered_map<std::string, cocos2d::Rect> hitBoxes;

		cocos2d::CustomCommand _drawCommand;
		bool enableDebugRect = false;

		cocos2d::DrawNode* debugRenderer = nullptr;

		CC_DISALLOW_COPY_AND_ASSIGN(Model);
	};
}
