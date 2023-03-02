#include "CubismOffscreenSurface_CC.h"
#include "CubismRenderer_CC.h"
#include "renderer/backend/Device.h"
#ifdef CC_VERSION
#include "renderer/backend/RenderTarget.h"
#endif // CC_VERSION

using namespace cocos2d;
using namespace Live2D::Cubism::Framework;
using namespace Live2D::Cubism::Framework::Rendering;

CubismOffscreenFrame_CC::CubismOffscreenFrame_CC()
{
	ccr = Director::getInstance()->getRenderer();
}

void CubismOffscreenFrame_CC::BeginDraw()
{
	if (!IsValid())
		return;
	_renderer->AddCallBack([=]()
	{
#ifdef CC_VERSION
		_oldRenderTarget = ccr->getRenderTarget();
		ccr->setRenderTarget(_renderTarget);
#else
		_oldColorAttachment = ccr->getColorAttachment();
		_oldDepthAttachment = ccr->getDepthAttachment();
		_oldStencilAttachment = ccr->getStencilAttachment();
		_oldRenderTargetFlag = ccr->getRenderTargetFlag();
		ccr->setRenderTarget(RenderTargetFlag::COLOR, _colorBuffer, nullptr, nullptr);
#endif // CC_VERSION
	});
}

void CubismOffscreenFrame_CC::EndDraw()
{
	if (!IsValid())
		return;
	_renderer->AddCallBack([=]()
	{
#ifdef CC_VERSION
			ccr->setRenderTarget(_oldRenderTarget);
#else
		ccr->setRenderTarget(_oldRenderTargetFlag, _oldColorAttachment, _oldDepthAttachment, _oldStencilAttachment);
#endif // CC_VERSION
	});
}

void CubismOffscreenFrame_CC::Clear(float r, float g, float b, float a)
{
	if (!IsValid())
		return;
	ccr->clear(ClearFlag::COLOR, { r,g,b,a }, 1, 0, 0);
}

csmBool CubismOffscreenFrame_CC::CreateOffscreenFrame(
	csmUint32 displayBufferWidth, csmUint32 displayBufferHeight, RenderTexture* renderTexture)
{
	DestroyOffscreenFrame();

	do
	{
		if (!renderTexture)
		{
			_renderTexture = RenderTexture::create(
				displayBufferWidth, displayBufferHeight
#ifdef CC_VERSION
				, false
#endif // CC_VERSION
			);
			if(!_renderTexture)
				break;
			_renderTexture->retain();
			_colorBuffer = _renderTexture->getSprite()->getTexture();
			_colorBuffer->setTexParameters({
				backend::SamplerFilter::LINEAR,
				backend::SamplerFilter::LINEAR,
				backend::SamplerAddressMode::CLAMP_TO_EDGE,
				backend::SamplerAddressMode::CLAMP_TO_EDGE });
			_isInheritedRenderTexture = false;
		}
		else
		{
			_renderTexture = renderTexture;
			_colorBuffer = renderTexture->getSprite()->getTexture();
			_isInheritedRenderTexture = true;
		}
		_viewPortSize.setSize(
			_colorBuffer->getContentSizeInPixels().width,
			_colorBuffer->getContentSizeInPixels().height);
		_bufferWidth = displayBufferWidth;
		_bufferHeight = displayBufferHeight;
#ifdef CC_VERSION
		_renderTarget = cocos2d::backend::Device::getInstance()->newRenderTarget(TargetBufferFlags::COLOR, _colorBuffer->getBackendTexture());
#endif // CC_VERSION

		return true;
	} while (false);
	DestroyOffscreenFrame();
	return false;
}

void CubismOffscreenFrame_CC::DestroyOffscreenFrame()
{
	if(!_isInheritedRenderTexture)
		CC_SAFE_RELEASE_NULL(_renderTexture);
	_colorBuffer = nullptr;
	CC_SAFE_DELETE(_renderTarget);
}

Texture2D* CubismOffscreenFrame_CC::GetColorBuffer() const
{
	return _colorBuffer;
}

csmUint32 CubismOffscreenFrame_CC::GetBufferWidth() const
{
	return _bufferWidth;
}

csmUint32 CubismOffscreenFrame_CC::GetBufferHeight() const
{
	return _bufferHeight;
}

csmBool CubismOffscreenFrame_CC::IsValid() const
{
	return _renderTexture != nullptr;
}

RenderTexture* CubismOffscreenFrame_CC::GetOffscreenFrame() const
{
	if (IsValid())
		return _renderTexture;
	return nullptr;
}
