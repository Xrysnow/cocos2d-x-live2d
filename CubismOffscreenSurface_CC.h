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

		csmBool CreateOffscreenFrame(csmUint32 displayBufferWidth, csmUint32 displayBufferHeight, cocos2d::RenderTexture* renderTexture = nullptr);

		void DestroyOffscreenFrame();

		cocos2d::Texture2D* GetColorBuffer() const;

		csmUint32 GetBufferWidth() const;

		csmUint32 GetBufferHeight() const;

		cocos2d::Size GetViewPortSize() const { return _viewPortSize; }

		csmBool IsValid() const;

		cocos2d::RenderTexture* GetOffscreenFrame() const;

	private:
		CubismRenderer_CC* _renderer = nullptr;
		cocos2d::Renderer* ccr = nullptr;
#ifdef CC_VERSION
		cocos2d::backend::RenderTarget* _oldRenderTarget = nullptr;
		cocos2d::backend::RenderTarget* _renderTarget = nullptr;
#else
		cocos2d::Texture2D* _oldColorAttachment = nullptr;
		cocos2d::Texture2D* _oldDepthAttachment = nullptr;
		cocos2d::Texture2D* _oldStencilAttachment = nullptr;
		cocos2d::RenderTargetFlag _oldRenderTargetFlag = {};
#endif // CC_VERSION

		cocos2d::RenderTexture*  _renderTexture = nullptr;         ///< レンダリングターゲットとしてのアドレス
		cocos2d::Texture2D*      _colorBuffer = nullptr;           ///< 描画の際使用するテクスチャとしてのアドレス
		csmUint32   _bufferWidth = 0;           ///< Create時に指定された幅
		csmUint32   _bufferHeight = 0;          ///< Create時に指定された高さ
		bool _isInheritedRenderTexture = false;
		cocos2d::Texture2D* _previousColorBuffer = nullptr; ///< 旧フレームバッファ
		cocos2d::Size _viewPortSize;
	};

}}}}
