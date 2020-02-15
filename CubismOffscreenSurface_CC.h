#pragma once
#include "Type/csmMap.hpp"
#include "2d/CCRenderTexture.h"

namespace Live2D { namespace Cubism { namespace Framework { namespace Rendering {
	class CubismRenderer_CC;

	class CubismOffscreenFrame_CC
	{
		friend class CubismRenderer_CC;
	public:
		CubismOffscreenFrame_CC();

		void BeginDraw();

		void EndDraw();

		void Clear(float r, float g, float b, float a);

		csmBool CreateOffscreenFrame(csmUint32 displayBufferWidth, csmUint32 displayBufferHeight);

		void DestroyOffscreenFrame();

		cocos2d::Texture2D* GetColorBuffer() const;

		csmUint32 GetBufferWidth() const;

		csmUint32 GetBufferHeight() const;

		csmBool IsValid() const;

		cocos2d::RenderTexture* GetOffscreenFrame() const;

	private:
		CubismRenderer_CC* _renderer;
		cocos2d::Renderer* ccr;
		cocos2d::Texture2D* _oldColorAttachment = nullptr;
		cocos2d::Texture2D* _oldDepthAttachment = nullptr;
		cocos2d::Texture2D* _oldStencilAttachment = nullptr;
		cocos2d::RenderTargetFlag _renderTargetFlags;
		cocos2d::RenderTargetFlag _oldRenderTargetFlag;

		cocos2d::RenderTexture*  _renderTexture;         ///< レンダリングターゲットとしてのアドレス
		cocos2d::Texture2D*      _colorBuffer;           ///< 描画の際使用するテクスチャとしてのアドレス
		csmUint32   _bufferWidth;           ///< Create時に指定された幅
		csmUint32   _bufferHeight;          ///< Create時に指定された高さ
	};

}}}}
