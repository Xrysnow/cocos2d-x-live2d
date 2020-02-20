#pragma once

#include "CubismRenderer.hpp"
#include "CubismFramework.hpp"
#include "Type/csmVector.hpp"
#include "Type/csmRectF.hpp"
#include "CubismOffscreenSurface_CC.h"
#include "cocos2d.h"

namespace Live2D { namespace Cubism {namespace Framework { namespace Rendering {
	class CubismRenderer_CC;
	class CubismClippingContext_CC;

	class CubismClippingManager_CC
	{
		friend class CubismShader_CC;
		friend class CubismRenderer_CC;

		CubismRenderer::CubismTextureColor* GetChannelFlagAsColor(csmInt32 channelNo);
		void CalcClippedDrawTotalBounds(
			CubismModel& model, CubismClippingContext_CC* clippingContext);
		CubismClippingManager_CC();
		virtual ~CubismClippingManager_CC();

		void Initialize(
			CubismModel& model, csmInt32 drawableCount,
			const csmInt32** drawableMasks, const csmInt32* drawableMaskCounts);
		void SetupClippingContext(
			CubismModel& model, CubismRenderer_CC* renderer,
			const cocos2d::Viewport& lastViewport);
		CubismClippingContext_CC* FindSameClip(
			const csmInt32* drawableMasks, csmInt32 drawableMaskCounts) const;
		void SetupLayoutBounds(csmInt32 usingClipCount) const;
		csmVector<CubismClippingContext_CC*>* GetClippingContextListForDraw();
		void SetClippingMaskBufferSize(csmInt32 size);
		csmInt32 GetClippingMaskBufferSize() const;

		csmInt32    _currentFrameNo;

		csmVector<CubismRenderer::CubismTextureColor*>  _channelColors;
		csmVector<CubismClippingContext_CC*>            _clippingContextListForMask;
		csmVector<CubismClippingContext_CC*>            _clippingContextListForDraw;
		csmInt32                                        _clippingMaskBufferSize;

		CubismMatrix44  _tmpMatrix;
		CubismMatrix44  _tmpMatrixForMask;
		CubismMatrix44  _tmpMatrixForDraw;
		csmRectF        _tmpBoundsOnModel;
	};

	class CubismClippingContext_CC
	{
		friend class CubismClippingManager_CC;
		friend class CubismShader_CC;
		friend class CubismRenderer_CC;

		CubismClippingContext_CC(CubismClippingManager_CC* manager,
			const csmInt32* clippingDrawableIndices, csmInt32 clipCount);
		virtual ~CubismClippingContext_CC();

		void AddClippedDrawable(csmInt32 drawableIndex);
		CubismClippingManager_CC* GetClippingManager();

		csmBool _isUsing;
		const csmInt32* _clippingIdList;
		csmInt32 _clippingIdCount;
		csmInt32 _layoutChannelNo;
		csmRectF* _layoutBounds;
		csmRectF* _allClippedDrawRect;
		CubismMatrix44 _matrixForMask;
		CubismMatrix44 _matrixForDraw;
		csmVector<csmInt32>* _clippedDrawableIndexList;

		CubismClippingManager_CC* _owner;
	};

	class CubismShader_CC
	{
		friend class CubismRenderer_CC;

		static CubismShader_CC* GetInstance();
		static void DeleteInstance();

		struct CubismShaderSet
		{
			cocos2d::backend::Program* ShaderProgram;
			int AttributePositionLocation;
			int AttributeTexCoordLocation;
			cocos2d::backend::UniformLocation UniformMatrixLocation;
			cocos2d::backend::UniformLocation UniformClipMatrixLocation;
			cocos2d::backend::UniformLocation SamplerTexture0Location;
			cocos2d::backend::UniformLocation SamplerTexture1Location;
			cocos2d::backend::UniformLocation UniformBaseColorLocation;
			cocos2d::backend::UniformLocation UnifromChannelFlagLocation;
		};

		CubismShader_CC();
		virtual ~CubismShader_CC();

		void SetupShaderProgram(CubismRenderer_CC* renderer
			, cocos2d::PipelineDescriptor* desc
			, cocos2d::Texture2D* texture
			, csmInt32 vertexCount, csmFloat32* vertexArray
			, csmFloat32* uvArray, csmFloat32 opacity
			, CubismRenderer::CubismBlendMode colorBlendMode
			, const CubismRenderer::CubismTextureColor& baseColor
			, csmBool isPremultipliedAlpha, CubismMatrix44& matrix4x4
			, csmBool invertedMask);
		void ReleaseShaderProgram();
		void GenerateShaders();
		static cocos2d::backend::Program* LoadShaderProgram(
			const csmChar* vertShaderSrc, const csmChar* fragShaderSrc);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	public:
		static void SetExtShaderMode(csmBool extMode, csmBool extPAMode);
	private:
		static csmBool  s_extMode;
		static csmBool  s_extPAMode;
#endif
		csmVector<CubismShaderSet*> _shaderSets;
	};

	class CubismRendererProfile_CC
	{
		friend class CubismRenderer_CC;

		CubismRendererProfile_CC();
		virtual ~CubismRendererProfile_CC() {}
		void Save();
		void Restore();

		cocos2d::Renderer* ccr = nullptr;
		bool _lastScissorTest;
		bool _lastStencilTest;
		bool _lastDepthTest;
		cocos2d::backend::CullMode _lastCullFace;
		cocos2d::backend::Winding _lastWinding;
		cocos2d::Viewport _lastViewport;
	};

	class CubismRenderer_CC : public CubismRenderer
	{
		friend class CubismRenderer;
		friend class CubismClippingManager_CC;
		friend class CubismShader_CC;
		friend class CubismOffscreenFrame_CC;

	public:
		CubismRenderer_CC(const CubismRenderer_CC&) = delete;
		CubismRenderer_CC& operator=(const CubismRenderer_CC&) = delete;

		void Initialize(CubismModel* model) override;
		void BindTexture(csmUint32 modelTextureNo, cocos2d::Texture2D* texture);
		void SetClippingMaskBufferSize(csmInt32 size);
		csmInt32 GetClippingMaskBufferSize() const;
		CubismOffscreenFrame_CC* GetOffscreenFrame() { return &_offscreenFrameBuffer; }

	protected:
		CubismRenderer_CC();
		virtual ~CubismRenderer_CC();
		void DoDrawModel() override;
		void DrawMesh(
			csmInt32 textureNo, csmInt32 indexCount, csmInt32 vertexCount
			, csmUint16* indexArray, csmFloat32* vertexArray, csmFloat32* uvArray
			, csmFloat32 opacity, CubismBlendMode colorBlendMode
			, csmBool invertedMask) override;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	public:
		static void SetExtShaderMode(csmBool extMdoe, csmBool extPAMode = false);
		static void ReloadShader();
#endif

	private:
		static void DoStaticRelease();

		void PreDraw();
		void PostDraw() {}

		void SaveProfile() override;
		void RestoreProfile() override;

		void SetClippingContextBufferForMask(CubismClippingContext_CC* clip);
		CubismClippingContext_CC* GetClippingContextBufferForMask() const;
		void SetClippingContextBufferForDraw(CubismClippingContext_CC* clip);
		CubismClippingContext_CC* GetClippingContextBufferForDraw() const;

	protected:
		void SetViewPort(const cocos2d::Viewport& value);
		void SetViewPort(int x, int y, unsigned int w, unsigned int h);
		void SetCullMode(cocos2d::backend::CullMode value);
		void AddCallBack(const std::function<void()>& callback);
		void ClearCommands();

	private:
		cocos2d::Renderer* ccr = nullptr;
		std::vector<std::shared_ptr<cocos2d::RenderCommand>> cmds;
		cocos2d::Map<csmInt32, cocos2d::Texture2D*> _textures;
		csmVector<csmInt32>        _sortedDrawableIndexList;
		CubismRendererProfile_CC   _rendererProfile;
		CubismClippingManager_CC*  _clippingManager;
		CubismClippingContext_CC*  _clippingContextBufferForMask;
		CubismClippingContext_CC*  _clippingContextBufferForDraw;
		CubismMatrix44             _mvpMatrix;
		CubismOffscreenFrame_CC    _offscreenFrameBuffer;
		cocos2d::backend::CullMode _lastCullMode;
	};

}}}}
