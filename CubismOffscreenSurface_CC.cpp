#include "CubismOffscreenSurface_CC.h"
#include "CubismRenderer_CC.h"

using namespace cocos2d;
using namespace cocos2d::backend;
using namespace Live2D::Cubism::Framework;
using namespace Live2D::Cubism::Framework::Rendering;

CubismOffscreenFrame_CC::CubismOffscreenFrame_CC()
	: _renderTexture(nullptr)
	, _colorBuffer(nullptr)
	, _bufferWidth(0)
	, _bufferHeight(0)
{
	ccr = Director::getInstance()->getRenderer();
}

void CubismOffscreenFrame_CC::BeginDraw()
{
	if (!IsValid())
		return;
	_renderer->AddCallBack([=]()
	{		
		_oldColorAttachment = ccr->getColorAttachment();
		_oldDepthAttachment = ccr->getDepthAttachment();
		_oldStencilAttachment = ccr->getStencilAttachment();
		_oldRenderTargetFlag = ccr->getRenderTargetFlag();
		ccr->setRenderTarget(RenderTargetFlag::COLOR, _colorBuffer, nullptr, nullptr);
	});
}

void CubismOffscreenFrame_CC::EndDraw()
{
	if (!IsValid())
		return;
	_renderer->AddCallBack([=]()
	{
		ccr->setRenderTarget(_oldRenderTargetFlag, _oldColorAttachment, _oldDepthAttachment, _oldStencilAttachment);
	});
}

void CubismOffscreenFrame_CC::Clear(float r, float g, float b, float a)
{
	if (!IsValid())
		return;
	ccr->clear(ClearFlag::COLOR, { r,g,b,a }, 1, 0, 0);
}

csmBool CubismOffscreenFrame_CC::CreateOffscreenFrame(
	csmUint32 displayBufferWidth, csmUint32 displayBufferHeight)
{
	DestroyOffscreenFrame();

	do
	{
		_renderTexture = RenderTexture::create(displayBufferWidth, displayBufferHeight);
		if(!_renderTexture)
			break;
		_renderTexture->retain();
		_colorBuffer = _renderTexture->getSprite()->getTexture();
		_colorBuffer->setTexParameters({
			SamplerFilter::LINEAR,SamplerFilter::LINEAR,
			SamplerAddressMode::CLAMP_TO_EDGE,SamplerAddressMode::CLAMP_TO_EDGE });
		_bufferWidth = displayBufferWidth;
		_bufferHeight = displayBufferHeight;
		return true;

	} while (false);

	DestroyOffscreenFrame();
	return false;
}

void CubismOffscreenFrame_CC::DestroyOffscreenFrame()
{
	CC_SAFE_RELEASE_NULL(_renderTexture);
	_colorBuffer = nullptr;
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
