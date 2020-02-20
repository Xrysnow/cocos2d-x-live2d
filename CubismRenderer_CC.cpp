#include "CubismRenderer_CC.h"
#include "Type/CubismBasicType.hpp"
#include "Model/CubismModel.hpp"
#include "renderer/backend/Device.h"

#define CSM_SAFE_DELETE_NULL(p) do { if(p) {CSM_DELETE(p); p = nullptr;} } while (false)

using namespace cocos2d;
using namespace cocos2d::backend;
using namespace Live2D::Cubism::Framework;
using namespace Live2D::Cubism::Framework::Rendering;

/*********************************************************************************************************************
*                                      CubismClippingManager_CC
********************************************************************************************************************/
///< ファイルスコープの変数宣言
namespace {
	const csmInt32 ColorChannelCount = 4;   ///< 実験時に1チャンネルの場合は1、RGBだけの場合は3、アルファも含める場合は4
}

CubismClippingManager_CC::CubismClippingManager_CC() :
	_currentFrameNo(0)
	, _clippingMaskBufferSize(256)
{
	CubismRenderer::CubismTextureColor* tmp;
	tmp = CSM_NEW CubismRenderer::CubismTextureColor();
	tmp->R = 1.0f;
	tmp->G = 0.0f;
	tmp->B = 0.0f;
	tmp->A = 0.0f;
	_channelColors.PushBack(tmp);
	tmp = CSM_NEW CubismRenderer::CubismTextureColor();
	tmp->R = 0.0f;
	tmp->G = 1.0f;
	tmp->B = 0.0f;
	tmp->A = 0.0f;
	_channelColors.PushBack(tmp);
	tmp = CSM_NEW CubismRenderer::CubismTextureColor();
	tmp->R = 0.0f;
	tmp->G = 0.0f;
	tmp->B = 1.0f;
	tmp->A = 0.0f;
	_channelColors.PushBack(tmp);
	tmp = CSM_NEW CubismRenderer::CubismTextureColor();
	tmp->R = 0.0f;
	tmp->G = 0.0f;
	tmp->B = 0.0f;
	tmp->A = 1.0f;
	_channelColors.PushBack(tmp);
}

CubismClippingManager_CC::~CubismClippingManager_CC()
{
	for (csmUint32 i = 0; i < _clippingContextListForMask.GetSize(); i++)
	{
		if (_clippingContextListForMask[i]) CSM_DELETE_SELF(CubismClippingContext_CC, _clippingContextListForMask[i]);
		_clippingContextListForMask[i] = nullptr;
	}

	// _clippingContextListForDrawは_clippingContextListForMaskにあるインスタンスを指している。上記の処理により要素ごとのDELETEは不要。
	for (csmUint32 i = 0; i < _clippingContextListForDraw.GetSize(); i++)
	{
		_clippingContextListForDraw[i] = nullptr;
	}

	for (csmUint32 i = 0; i < _channelColors.GetSize(); i++)
	{
		if (_channelColors[i]) CSM_DELETE(_channelColors[i]);
		_channelColors[i] = nullptr;
	}
}

void CubismClippingManager_CC::Initialize(CubismModel& model, csmInt32 drawableCount, const csmInt32** drawableMasks, const csmInt32* drawableMaskCounts)
{
	//クリッピングマスクを使う描画オブジェクトを全て登録する
	//クリッピングマスクは、通常数個程度に限定して使うものとする
	for (csmInt32 i = 0; i < drawableCount; i++)
	{
		if (drawableMaskCounts[i] <= 0)
		{
			//クリッピングマスクが使用されていないアートメッシュ（多くの場合使用しない）
			_clippingContextListForDraw.PushBack(nullptr);
			continue;
		}

		// 既にあるClipContextと同じかチェックする
		CubismClippingContext_CC* cc = FindSameClip(drawableMasks[i], drawableMaskCounts[i]);
		if (cc == nullptr)
		{
			// 同一のマスクが存在していない場合は生成する
			cc = CSM_NEW CubismClippingContext_CC(this, drawableMasks[i], drawableMaskCounts[i]);
			_clippingContextListForMask.PushBack(cc);
		}

		cc->AddClippedDrawable(i);

		_clippingContextListForDraw.PushBack(cc);
	}
}

CubismClippingContext_CC* CubismClippingManager_CC::FindSameClip(const csmInt32* drawableMasks, csmInt32 drawableMaskCounts) const
{
	// 作成済みClippingContextと一致するか確認
	for (csmUint32 i = 0; i < _clippingContextListForMask.GetSize(); i++)
	{
		CubismClippingContext_CC* cc = _clippingContextListForMask[i];
		const csmInt32 count = cc->_clippingIdCount;
		if (count != drawableMaskCounts) continue; //個数が違う場合は別物
		csmInt32 samecount = 0;

		// 同じIDを持つか確認。配列の数が同じなので、一致した個数が同じなら同じ物を持つとする。
		for (csmInt32 j = 0; j < count; j++)
		{
			const csmInt32 clipId = cc->_clippingIdList[j];
			for (csmInt32 k = 0; k < count; k++)
			{
				if (drawableMasks[k] == clipId)
				{
					samecount++;
					break;
				}
			}
		}
		if (samecount == count)
		{
			return cc;
		}
	}
	return nullptr; //見つからなかった
}

void CubismClippingManager_CC::SetupClippingContext(CubismModel& model, CubismRenderer_CC* renderer,
	const Viewport& lastViewport)
{
	_currentFrameNo++;

	// 全てのクリッピングを用意する
	// 同じクリップ（複数の場合はまとめて１つのクリップ）を使う場合は１度だけ設定する
	csmInt32 usingClipCount = 0;
	for (csmUint32 clipIndex = 0; clipIndex < _clippingContextListForMask.GetSize(); clipIndex++)
	{
		// １つのクリッピングマスクに関して
		CubismClippingContext_CC* cc = _clippingContextListForMask[clipIndex];

		// このクリップを利用する描画オブジェクト群全体を囲む矩形を計算
		CalcClippedDrawTotalBounds(model, cc);

		if (cc->_isUsing)
		{
			usingClipCount++; //使用中としてカウント
		}
	}

	// マスク作成処理
	if (usingClipCount > 0)
	{
		if (!renderer->IsUsingHighPrecisionMask())
		{
			// 生成したFrameBufferと同じサイズでビューポートを設定
			renderer->SetViewPort(0, 0, _clippingMaskBufferSize, _clippingMaskBufferSize);

			// モデル描画時にDrawMeshNowに渡される変換（モデルtoワールド座標変換）
			CubismMatrix44 modelToWorldF = renderer->GetMvpMatrix();

			renderer->PreDraw(); // バッファをクリアする

			// _offscreenFrameBufferへ切り替え
			renderer->_offscreenFrameBuffer.BeginDraw();

			// マスクをクリアする
			// 1が無効（描かれない）領域、0が有効（描かれる）領域。（シェーダで Cd*Csで0に近い値をかけてマスクを作る。1をかけると何も起こらない）
			renderer->_offscreenFrameBuffer.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// 各マスクのレイアウトを決定していく
		SetupLayoutBounds(renderer->IsUsingHighPrecisionMask() ? 0 : usingClipCount);

		// 実際にマスクを生成する
		// 全てのマスクをどの様にレイアウトして描くかを決定し、ClipContext , ClippedDrawContext に記憶する
		for (csmUint32 clipIndex = 0; clipIndex < _clippingContextListForMask.GetSize(); clipIndex++)
		{
			// --- 実際に１つのマスクを描く ---
			CubismClippingContext_CC* clipContext = _clippingContextListForMask[clipIndex];
			csmRectF* allClippedDrawRect = clipContext->_allClippedDrawRect; //このマスクを使う、全ての描画オブジェクトの論理座標上の囲み矩形
			csmRectF* layoutBoundsOnTex01 = clipContext->_layoutBounds; //この中にマスクを収める

			// モデル座標上の矩形を、適宜マージンを付けて使う
			const csmFloat32 MARGIN = 0.05f;
			_tmpBoundsOnModel.SetRect(allClippedDrawRect);
			_tmpBoundsOnModel.Expand(allClippedDrawRect->Width * MARGIN, allClippedDrawRect->Height * MARGIN);
			//########## 本来は割り当てられた領域の全体を使わず必要最低限のサイズがよい

			// シェーダ用の計算式を求める。回転を考慮しない場合は以下のとおり
			// movePeriod' = movePeriod * scaleX + offX     [[ movePeriod' = (movePeriod - tmpBoundsOnModel.movePeriod)*scale + layoutBoundsOnTex01.movePeriod ]]
			const csmFloat32 scaleX = layoutBoundsOnTex01->Width / _tmpBoundsOnModel.Width;
			const csmFloat32 scaleY = layoutBoundsOnTex01->Height / _tmpBoundsOnModel.Height;

			// マスク生成時に使う行列を求める
			{
				// シェーダに渡す行列を求める <<<<<<<<<<<<<<<<<<<<<<<< 要最適化（逆順に計算すればシンプルにできる）
				_tmpMatrix.LoadIdentity();
				{
					// Layout0..1 を -1..1に変換
					_tmpMatrix.TranslateRelative(-1.0f, -1.0f);
					_tmpMatrix.ScaleRelative(2.0f, 2.0f);
				}
				{
					// view to Layout0..1
					_tmpMatrix.TranslateRelative(layoutBoundsOnTex01->X, layoutBoundsOnTex01->Y); //new = [translate]
					_tmpMatrix.ScaleRelative(scaleX, scaleY); //new = [translate][scale]
					_tmpMatrix.TranslateRelative(-_tmpBoundsOnModel.X, -_tmpBoundsOnModel.Y);
					//new = [translate][scale][translate]
				}
				// tmpMatrixForMask が計算結果
				_tmpMatrixForMask.SetMatrix(_tmpMatrix.GetArray());
			}

			//--------- draw時の mask 参照用行列を計算
			{
				// シェーダに渡す行列を求める <<<<<<<<<<<<<<<<<<<<<<<< 要最適化（逆順に計算すればシンプルにできる）
				_tmpMatrix.LoadIdentity();
				{
					_tmpMatrix.TranslateRelative(layoutBoundsOnTex01->X, layoutBoundsOnTex01->Y); //new = [translate]
					_tmpMatrix.ScaleRelative(scaleX, scaleY); //new = [translate][scale]
					_tmpMatrix.TranslateRelative(-_tmpBoundsOnModel.X, -_tmpBoundsOnModel.Y);
					//new = [translate][scale][translate]
				}

				_tmpMatrixForDraw.SetMatrix(_tmpMatrix.GetArray());
			}

			clipContext->_matrixForMask.SetMatrix(_tmpMatrixForMask.GetArray());

			clipContext->_matrixForDraw.SetMatrix(_tmpMatrixForDraw.GetArray());

			if (!renderer->IsUsingHighPrecisionMask())
			{
				const csmInt32 clipDrawCount = clipContext->_clippingIdCount;
				for (csmInt32 i = 0; i < clipDrawCount; i++)
				{
					const csmInt32 clipDrawIndex = clipContext->_clippingIdList[i];

					// 頂点情報が更新されておらず、信頼性がない場合は描画をパスする
					if (!model.GetDrawableDynamicFlagVertexPositionsDidChange(clipDrawIndex))
					{
						continue;
					}

					renderer->IsCulling(model.GetDrawableCulling(clipDrawIndex) != 0);

					// 今回専用の変換を適用して描く
					// チャンネルも切り替える必要がある(A,R,G,B)
					renderer->SetClippingContextBufferForMask(clipContext);
					renderer->DrawMesh(
						model.GetDrawableTextureIndices(clipDrawIndex),
						model.GetDrawableVertexIndexCount(clipDrawIndex),
						model.GetDrawableVertexCount(clipDrawIndex),
						const_cast<csmUint16*>(model.GetDrawableVertexIndices(clipDrawIndex)),
						const_cast<csmFloat32*>(model.GetDrawableVertices(clipDrawIndex)),
						reinterpret_cast<csmFloat32*>(const_cast<Core::csmVector2*>(model.GetDrawableVertexUvs(clipDrawIndex))),
						model.GetDrawableOpacity(clipDrawIndex),
						CubismRenderer::CubismBlendMode_Normal,   //クリッピングは通常描画を強制
						false   // マスク生成時はクリッピングの反転使用は全く関係がない
					);
				}
			}
		}

		if (!renderer->IsUsingHighPrecisionMask())
		{
			// --- 後処理 ---
			renderer->_offscreenFrameBuffer.EndDraw(); // 描画対象を戻す
			renderer->SetClippingContextBufferForMask(nullptr);
			renderer->SetViewPort(lastViewport.x, lastViewport.y, lastViewport.w, lastViewport.h);
		}
	}
}

void CubismClippingManager_CC::CalcClippedDrawTotalBounds(CubismModel& model, CubismClippingContext_CC* clippingContext)
{
	// 被クリッピングマスク（マスクされる描画オブジェクト）の全体の矩形
	csmFloat32 clippedDrawTotalMinX = FLT_MAX, clippedDrawTotalMinY = FLT_MAX;
	csmFloat32 clippedDrawTotalMaxX = FLT_MIN, clippedDrawTotalMaxY = FLT_MIN;

	// このマスクが実際に必要か判定する
	// このクリッピングを利用する「描画オブジェクト」がひとつでも使用可能であればマスクを生成する必要がある

	const csmInt32 clippedDrawCount = clippingContext->_clippedDrawableIndexList->GetSize();
	for (csmInt32 clippedDrawableIndex = 0; clippedDrawableIndex < clippedDrawCount; clippedDrawableIndex++)
	{
		// マスクを使用する描画オブジェクトの描画される矩形を求める
		const csmInt32 drawableIndex = (*clippingContext->_clippedDrawableIndexList)[clippedDrawableIndex];

		const csmInt32 drawableVertexCount = model.GetDrawableVertexCount(drawableIndex);
		csmFloat32* drawableVertexes = const_cast<csmFloat32*>(model.GetDrawableVertices(drawableIndex));

		csmFloat32 minX = FLT_MAX, minY = FLT_MAX;
		csmFloat32 maxX = FLT_MIN, maxY = FLT_MIN;

		csmInt32 loop = drawableVertexCount * Constant::VertexStep;
		for (csmInt32 pi = Constant::VertexOffset; pi < loop; pi += Constant::VertexStep)
		{
			csmFloat32 x = drawableVertexes[pi];
			csmFloat32 y = drawableVertexes[pi + 1];
			if (x < minX) minX = x;
			if (x > maxX) maxX = x;
			if (y < minY) minY = y;
			if (y > maxY) maxY = y;
		}

		//
		if (minX == FLT_MAX) continue; //有効な点がひとつも取れなかったのでスキップする

		// 全体の矩形に反映
		if (minX < clippedDrawTotalMinX) clippedDrawTotalMinX = minX;
		if (minY < clippedDrawTotalMinY) clippedDrawTotalMinY = minY;
		if (maxX > clippedDrawTotalMaxX) clippedDrawTotalMaxX = maxX;
		if (maxY > clippedDrawTotalMaxY) clippedDrawTotalMaxY = maxY;
	}
	if (clippedDrawTotalMinX == FLT_MAX)
	{
		clippingContext->_allClippedDrawRect->X = 0.0f;
		clippingContext->_allClippedDrawRect->Y = 0.0f;
		clippingContext->_allClippedDrawRect->Width = 0.0f;
		clippingContext->_allClippedDrawRect->Height = 0.0f;
		clippingContext->_isUsing = false;
	}
	else
	{
		clippingContext->_isUsing = true;
		csmFloat32 w = clippedDrawTotalMaxX - clippedDrawTotalMinX;
		csmFloat32 h = clippedDrawTotalMaxY - clippedDrawTotalMinY;
		clippingContext->_allClippedDrawRect->X = clippedDrawTotalMinX;
		clippingContext->_allClippedDrawRect->Y = clippedDrawTotalMinY;
		clippingContext->_allClippedDrawRect->Width = w;
		clippingContext->_allClippedDrawRect->Height = h;
	}
}

void CubismClippingManager_CC::SetupLayoutBounds(csmInt32 usingClipCount) const
{
	if (usingClipCount <= 0)
	{// この場合は一つのマスクターゲットを毎回クリアして使用する
		for (csmUint32 index = 0; index < _clippingContextListForMask.GetSize(); index++)
		{
			CubismClippingContext_CC* cc = _clippingContextListForMask[index];
			cc->_layoutChannelNo = 0; // どうせ毎回消すので固定で良い
			cc->_layoutBounds->X = 0.0f;
			cc->_layoutBounds->Y = 0.0f;
			cc->_layoutBounds->Width = 1.0f;
			cc->_layoutBounds->Height = 1.0f;
		}
		return;
	}

	// ひとつのRenderTextureを極力いっぱいに使ってマスクをレイアウトする
	// マスクグループの数が4以下ならRGBA各チャンネルに１つずつマスクを配置し、5以上6以下ならRGBAを2,2,1,1と配置する

	// RGBAを順番に使っていく。
	const csmInt32 div = usingClipCount / ColorChannelCount; //１チャンネルに配置する基本のマスク個数
	const csmInt32 mod = usingClipCount % ColorChannelCount; //余り、この番号のチャンネルまでに１つずつ配分する

	// RGBAそれぞれのチャンネルを用意していく(0:R , 1:G , 2:B, 3:A, )
	csmInt32 curClipIndex = 0; //順番に設定していく

	for (csmInt32 channelNo = 0; channelNo < ColorChannelCount; channelNo++)
	{
		// このチャンネルにレイアウトする数
		const csmInt32 layoutCount = div + (channelNo < mod ? 1 : 0);

		// 分割方法を決定する
		if (layoutCount == 0)
		{
			// 何もしない
		}
		else if (layoutCount == 1)
		{
			//全てをそのまま使う
			CubismClippingContext_CC* cc = _clippingContextListForMask[curClipIndex++];
			cc->_layoutChannelNo = channelNo;
			cc->_layoutBounds->X = 0.0f;
			cc->_layoutBounds->Y = 0.0f;
			cc->_layoutBounds->Width = 1.0f;
			cc->_layoutBounds->Height = 1.0f;
		}
		else if (layoutCount == 2)
		{
			for (csmInt32 i = 0; i < layoutCount; i++)
			{
				const csmInt32 xpos = i % 2;

				CubismClippingContext_CC* cc = _clippingContextListForMask[curClipIndex++];
				cc->_layoutChannelNo = channelNo;

				cc->_layoutBounds->X = xpos * 0.5f;
				cc->_layoutBounds->Y = 0.0f;
				cc->_layoutBounds->Width = 0.5f;
				cc->_layoutBounds->Height = 1.0f;
				//UVを2つに分解して使う
			}
		}
		else if (layoutCount <= 4)
		{
			//4分割して使う
			for (csmInt32 i = 0; i < layoutCount; i++)
			{
				const csmInt32 xpos = i % 2;
				const csmInt32 ypos = i / 2;

				CubismClippingContext_CC* cc = _clippingContextListForMask[curClipIndex++];
				cc->_layoutChannelNo = channelNo;

				cc->_layoutBounds->X = xpos * 0.5f;
				cc->_layoutBounds->Y = ypos * 0.5f;
				cc->_layoutBounds->Width = 0.5f;
				cc->_layoutBounds->Height = 0.5f;
			}
		}
		else if (layoutCount <= 9)
		{
			//9分割して使う
			for (csmInt32 i = 0; i < layoutCount; i++)
			{
				const csmInt32 xpos = i % 3;
				const csmInt32 ypos = i / 3;

				CubismClippingContext_CC* cc = _clippingContextListForMask[curClipIndex++];
				cc->_layoutChannelNo = channelNo;

				cc->_layoutBounds->X = xpos / 3.0f;
				cc->_layoutBounds->Y = ypos / 3.0f;
				cc->_layoutBounds->Width = 1.0f / 3.0f;
				cc->_layoutBounds->Height = 1.0f / 3.0f;
			}
		}
		else
		{
			CubismLogError("not supported mask count : %d", layoutCount);

			// 開発モードの場合は停止させる
			CSM_ASSERT(0);

			// 引き続き実行する場合、 SetupShaderProgramでオーバーアクセスが発生するので仕方なく適当に入れておく
			// もちろん描画結果はろくなことにならない
			for (csmInt32 i = 0; i < layoutCount; i++)
			{
				CubismClippingContext_CC* cc = _clippingContextListForMask[curClipIndex++];
				cc->_layoutChannelNo = 0;
				cc->_layoutBounds->X = 0.0f;
				cc->_layoutBounds->Y = 0.0f;
				cc->_layoutBounds->Width = 1.0f;
				cc->_layoutBounds->Height = 1.0f;
			}
		}
	}
}

CubismRenderer::CubismTextureColor* CubismClippingManager_CC::GetChannelFlagAsColor(csmInt32 channelNo)
{
	return _channelColors[channelNo];
}

csmVector<CubismClippingContext_CC*>* CubismClippingManager_CC::GetClippingContextListForDraw()
{
	return &_clippingContextListForDraw;
}

void CubismClippingManager_CC::SetClippingMaskBufferSize(csmInt32 size)
{
	_clippingMaskBufferSize = size;
}

csmInt32 CubismClippingManager_CC::GetClippingMaskBufferSize() const
{
	return _clippingMaskBufferSize;
}

/*********************************************************************************************************************
*                                      CubismClippingContext
********************************************************************************************************************/
CubismClippingContext_CC::CubismClippingContext_CC(
	CubismClippingManager_CC* manager, const csmInt32* clippingDrawableIndices, csmInt32 clipCount)
	:_isUsing(false)
{
	_owner = manager;

	// クリップしている（＝マスク用の）Drawableのインデックスリスト
	_clippingIdList = clippingDrawableIndices;

	// マスクの数
	_clippingIdCount = clipCount;

	_layoutChannelNo = 0;

	_allClippedDrawRect = CSM_NEW csmRectF();
	_layoutBounds = CSM_NEW csmRectF();

	_clippedDrawableIndexList = CSM_NEW csmVector<csmInt32>();
}

CubismClippingContext_CC::~CubismClippingContext_CC()
{
	CSM_SAFE_DELETE_NULL(_layoutBounds);
	CSM_SAFE_DELETE_NULL(_allClippedDrawRect);
	CSM_SAFE_DELETE_NULL(_clippedDrawableIndexList);
}

void CubismClippingContext_CC::AddClippedDrawable(csmInt32 drawableIndex)
{
	_clippedDrawableIndexList->PushBack(drawableIndex);
}

CubismClippingManager_CC* CubismClippingContext_CC::GetClippingManager()
{
	return _owner;
}

/*********************************************************************************************************************
*                                      CubismDrawProfile_OpenGL
********************************************************************************************************************/

CubismRendererProfile_CC::CubismRendererProfile_CC()
{
	ccr = cocos2d::Director::getInstance()->getRenderer();
}

void CubismRendererProfile_CC::Save()
{
	_lastScissorTest = ccr->getScissorTest();
	_lastStencilTest = ccr->getStencilTest();
	_lastDepthTest = ccr->getDepthTest();
	_lastCullFace = ccr->getCullMode();
	_lastWinding = ccr->getWinding();
	_lastViewport = ccr->getViewport();
}

void CubismRendererProfile_CC::Restore()
{
	ccr->setScissorTest(_lastScissorTest);
	ccr->setStencilTest(_lastStencilTest);
	ccr->setDepthTest(_lastDepthTest);
	ccr->setCullMode(_lastCullFace);
	ccr->setWinding(_lastWinding);
	ccr->setViewPort(_lastViewport.x, _lastViewport.y, _lastViewport.w, _lastViewport.h);
}

/*********************************************************************************************************************
*                                       CubismShader_CC
********************************************************************************************************************/
namespace {
	const csmInt32 ShaderCount = 19; ///< シェーダの数 = マスク生成用 + (通常 + 加算 + 乗算) * (マスク無 + マスク有 + マスク有反転 + マスク無の乗算済アルファ対応版 + マスク有の乗算済アルファ対応版 + マスク有反転の乗算済アルファ対応版)
	CubismShader_CC* s_instance;
}

enum ShaderNames
{
	// SetupMask
	ShaderNames_SetupMask,

	//Normal
	ShaderNames_Normal,
	ShaderNames_NormalMasked,
	ShaderNames_NormalMaskedInverted,
	ShaderNames_NormalPremultipliedAlpha,
	ShaderNames_NormalMaskedPremultipliedAlpha,
	ShaderNames_NormalMaskedInvertedPremultipliedAlpha,

	//Add
	ShaderNames_Add,
	ShaderNames_AddMasked,
	ShaderNames_AddMaskedInverted,
	ShaderNames_AddPremultipliedAlpha,
	ShaderNames_AddMaskedPremultipliedAlpha,
	ShaderNames_AddMaskedPremultipliedAlphaInverted,

	//Mult
	ShaderNames_Mult,
	ShaderNames_MultMasked,
	ShaderNames_MultMaskedInverted,
	ShaderNames_MultPremultipliedAlpha,
	ShaderNames_MultMaskedPremultipliedAlpha,
	ShaderNames_MultMaskedPremultipliedAlphaInverted,
};

void CubismShader_CC::ReleaseShaderProgram()
{
	std::set<Program*> programs;
	for (csmUint32 i = 0; i < _shaderSets.GetSize(); i++)
	{
		if (_shaderSets[i]->ShaderProgram)
		{
			programs.insert(_shaderSets[i]->ShaderProgram);
			_shaderSets[i]->ShaderProgram = nullptr;
			CSM_DELETE(_shaderSets[i]);
		}
	}
	for (auto& p : programs)
	{
		CC_SAFE_RELEASE(p);
	}
}

// SetupMask
static const csmChar* VertShaderSrcSetupMask =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
#else
	"#version 120\n"
#endif
	"attribute vec2 a_position;"
	"attribute vec2 a_texCoord;"
	"varying vec2 v_texCoord;"
	"varying vec4 v_myPos;"
	"uniform mat4 u_clipMatrix;"
	"void main()"
	"{"
	"vec4 pos = vec4(a_position.x, a_position.y, 0.0, 1.0);"
	"gl_Position = u_clipMatrix * pos;"
	"v_myPos = u_clipMatrix * pos;"
	"v_texCoord = a_texCoord;"
	"v_texCoord.y = 1.0 - v_texCoord.y;"
	"}";
static const csmChar* FragShaderSrcSetupMask =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
	"precision mediump float;"
#else
	"#version 120\n"
#endif
	"varying vec2 v_texCoord;"
	"varying vec4 v_myPos;"
	"uniform sampler2D s_texture0;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"float isInside = "
	"  step(u_baseColor.x, v_myPos.x/v_myPos.w)"
	"* step(u_baseColor.y, v_myPos.y/v_myPos.w)"
	"* step(v_myPos.x/v_myPos.w, u_baseColor.z)"
	"* step(v_myPos.y/v_myPos.w, u_baseColor.w);"

	"gl_FragColor = u_channelFlag * texture2D(s_texture0 , v_texCoord).a * isInside;"
	"}";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
static const csmChar* FragShaderSrcSetupMaskTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;"
	"varying vec4 v_myPos;"
	"uniform sampler2D s_texture0;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"float isInside = "
	"  step(u_baseColor.x, v_myPos.x/v_myPos.w)"
	"* step(u_baseColor.y, v_myPos.y/v_myPos.w)"
	"* step(v_myPos.x/v_myPos.w, u_baseColor.z)"
	"* step(v_myPos.y/v_myPos.w, u_baseColor.w);"

	"gl_FragColor = u_channelFlag * texture2D(s_texture0 , v_texCoord).a * isInside;"
	"}";
#endif

//----- バーテックスシェーダプログラム -----
// Normal & Add & Mult 共通
static const csmChar* VertShaderSrc =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
#else
	"#version 120\n"
#endif
	"attribute vec2 a_position;" //v.vertex
	"attribute vec2 a_texCoord;" //v.texcoord
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform mat4 u_matrix;"
	"void main()"
	"{"
	"vec4 pos = vec4(a_position.x, a_position.y, 0.0, 1.0);"
	"gl_Position = u_matrix * pos;"
	"v_texCoord = a_texCoord;"
	"v_texCoord.y = 1.0 - v_texCoord.y;"
	"}";

// Normal & Add & Mult 共通（クリッピングされたものの描画用）
static const csmChar* VertShaderSrcMasked =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
#else
	"#version 120\n"
#endif
	"attribute vec2 a_position;"
	"attribute vec2 a_texCoord;"
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform mat4 u_matrix;"
	"uniform mat4 u_clipMatrix;"
	"void main()"
	"{"
	"vec4 pos = vec4(a_position.x, a_position.y, 0.0, 1.0);"
	"gl_Position = u_matrix * pos;"
	"v_clipPos = u_clipMatrix * pos;"
	"v_texCoord = a_texCoord;"
	"v_texCoord.y = 1.0 - v_texCoord.y;"
	"}";

//----- フラグメントシェーダプログラム -----
// Normal & Add & Mult 共通
static const csmChar* FragShaderSrc =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
	"precision mediump float;"
#else
	"#version 120\n"
#endif
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform sampler2D s_texture0;" //_MainTex
	"uniform vec4 u_baseColor;" //v2f.color
	"void main()"
	"{"
	"vec4 color = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"gl_FragColor = vec4(color.rgb * color.a,  color.a);"
	"}";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
static const csmChar* FragShaderSrcTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform sampler2D s_texture0;" //_MainTex
	"uniform vec4 u_baseColor;" //v2f.color
	"void main()"
	"{"
	"vec4 color = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"gl_FragColor = vec4(color.rgb * color.a,  color.a);"
	"}";
#endif

// Normal & Add & Mult 共通 （PremultipliedAlpha）
static const csmChar* FragShaderSrcPremultipliedAlpha =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
	"precision mediump float;"
#else
	"#version 120\n"
#endif
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform sampler2D s_texture0;" //_MainTex
	"uniform vec4 u_baseColor;" //v2f.color
	"void main()"
	"{"
	"gl_FragColor = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"}";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
static const csmChar* FragShaderSrcPremultipliedAlphaTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform sampler2D s_texture0;" //_MainTex
	"uniform vec4 u_baseColor;" //v2f.color
	"void main()"
	"{"
	"gl_FragColor = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"}";
#endif

// Normal & Add & Mult 共通（クリッピングされたものの描画用）
static const csmChar* FragShaderSrcMask =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
	"precision mediump float;"
#else
	"#version 120\n"
#endif
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"vec4 col_formask = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"col_formask.rgb = col_formask.rgb  * col_formask.a ;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * maskVal;"
	"gl_FragColor = col_formask;"
	"}";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
static const csmChar* FragShaderSrcMaskTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"vec4 col_formask = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"col_formask.rgb = col_formask.rgb  * col_formask.a ;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * maskVal;"
	"gl_FragColor = col_formask;"
	"}";
#endif

// Normal & Add & Mult 共通（クリッピングされて反転使用の描画用）
static const csmChar* FragShaderSrcMaskInverted =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
	"precision mediump float;"
#else
	"#version 120\n"
#endif
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"vec4 col_formask = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"col_formask.rgb = col_formask.rgb  * col_formask.a ;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * (1.0 - maskVal);"
	"gl_FragColor = col_formask;"
	"}";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
static const csmChar* FragShaderSrcMaskInvertedTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"vec4 col_formask = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"col_formask.rgb = col_formask.rgb  * col_formask.a ;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * (1.0 - maskVal);"
	"gl_FragColor = col_formask;"
	"}";
#endif

// Normal & Add & Mult 共通（クリッピングされたものの描画用、PremultipliedAlphaの場合）
static const csmChar* FragShaderSrcMaskPremultipliedAlpha =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
	"precision mediump float;"
#else
	"#version 120\n"
#endif
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"vec4 col_formask = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * maskVal;"
	"gl_FragColor = col_formask;"
	"}";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
static const csmChar* FragShaderSrcMaskPremultipliedAlphaTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"vec4 col_formask = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * maskVal;"
	"gl_FragColor = col_formask;"
	"}";
#endif

// Normal & Add & Mult 共通（クリッピングされて反転使用の描画用、PremultipliedAlphaの場合）
static const csmChar* FragShaderSrcMaskInvertedPremultipliedAlpha =
#if defined(CC_PLATFORM_MOBILE)
	"#version 100\n"
	"precision mediump float;"
#else
	"#version 120\n"
#endif
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"vec4 col_formask = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * (1.0 - maskVal);"
	"gl_FragColor = col_formask;"
	"}";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
static const csmChar* FragShaderSrcMaskInvertedPremultipliedAlphaTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"void main()"
	"{"
	"vec4 col_formask = texture2D(s_texture0 , v_texCoord) * u_baseColor;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * (1.0 - maskVal);"
	"gl_FragColor = col_formask;"
	"}";
#endif

CubismShader_CC::CubismShader_CC()
{ }

CubismShader_CC::~CubismShader_CC()
{
	ReleaseShaderProgram();
}

CubismShader_CC* CubismShader_CC::GetInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = CSM_NEW CubismShader_CC();
	}
	return s_instance;
}

void CubismShader_CC::DeleteInstance()
{
	if (s_instance)
	{
		CSM_DELETE_SELF(CubismShader_CC, s_instance);
		s_instance = nullptr;
	}
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
csmBool CubismShader_CC::s_extMode = false;
csmBool CubismShader_CC::s_extPAMode = false;
void CubismShader_CC::SetExtShaderMode(csmBool extMode, csmBool extPAMode) {
	s_extMode = extMode;
	s_extPAMode = extPAMode;
}
#endif

void CubismShader_CC::GenerateShaders()
{
	for (csmInt32 i = 0; i < ShaderCount; i++)
	{
		_shaderSets.PushBack(CSM_NEW CubismShaderSet());
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if (s_extMode)
	{
		_shaderSets[0]->ShaderProgram = LoadShaderProgram(VertShaderSrcSetupMask, FragShaderSrcSetupMaskTegra);

		_shaderSets[1]->ShaderProgram = LoadShaderProgram(VertShaderSrc, FragShaderSrcTegra);
		_shaderSets[2]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskTegra);
		_shaderSets[3]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskInvertedTegra);
		_shaderSets[4]->ShaderProgram = LoadShaderProgram(VertShaderSrc, FragShaderSrcPremultipliedAlphaTegra);
		_shaderSets[5]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskPremultipliedAlphaTegra);
		_shaderSets[6]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskInvertedPremultipliedAlphaTegra);
	}
	else
	{
		_shaderSets[0]->ShaderProgram = LoadShaderProgram(VertShaderSrcSetupMask, FragShaderSrcSetupMask);

		_shaderSets[1]->ShaderProgram = LoadShaderProgram(VertShaderSrc, FragShaderSrc);
		_shaderSets[2]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMask);
		_shaderSets[3]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskInverted);
		_shaderSets[4]->ShaderProgram = LoadShaderProgram(VertShaderSrc, FragShaderSrcPremultipliedAlpha);
		_shaderSets[5]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskPremultipliedAlpha);
		_shaderSets[6]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskInvertedPremultipliedAlpha);
	}

	// 加算も通常と同じシェーダーを利用する
	_shaderSets[7]->ShaderProgram = _shaderSets[1]->ShaderProgram;
	_shaderSets[8]->ShaderProgram = _shaderSets[2]->ShaderProgram;
	_shaderSets[9]->ShaderProgram = _shaderSets[3]->ShaderProgram;
	_shaderSets[10]->ShaderProgram = _shaderSets[4]->ShaderProgram;
	_shaderSets[11]->ShaderProgram = _shaderSets[5]->ShaderProgram;
	_shaderSets[12]->ShaderProgram = _shaderSets[6]->ShaderProgram;

	// 乗算も通常と同じシェーダーを利用する
	_shaderSets[13]->ShaderProgram = _shaderSets[1]->ShaderProgram;
	_shaderSets[14]->ShaderProgram = _shaderSets[2]->ShaderProgram;
	_shaderSets[15]->ShaderProgram = _shaderSets[3]->ShaderProgram;
	_shaderSets[16]->ShaderProgram = _shaderSets[4]->ShaderProgram;
	_shaderSets[17]->ShaderProgram = _shaderSets[5]->ShaderProgram;
	_shaderSets[18]->ShaderProgram = _shaderSets[6]->ShaderProgram;

#else

	_shaderSets[0]->ShaderProgram = LoadShaderProgram(VertShaderSrcSetupMask, FragShaderSrcSetupMask);

	_shaderSets[1]->ShaderProgram = LoadShaderProgram(VertShaderSrc, FragShaderSrc);
	_shaderSets[2]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMask);
	_shaderSets[3]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskInverted);
	_shaderSets[4]->ShaderProgram = LoadShaderProgram(VertShaderSrc, FragShaderSrcPremultipliedAlpha);
	_shaderSets[5]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskPremultipliedAlpha);
	_shaderSets[6]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskInvertedPremultipliedAlpha);

	// 加算も通常と同じシェーダーを利用する
	_shaderSets[7]->ShaderProgram = _shaderSets[1]->ShaderProgram;
	_shaderSets[8]->ShaderProgram = _shaderSets[2]->ShaderProgram;
	_shaderSets[9]->ShaderProgram = _shaderSets[3]->ShaderProgram;
	_shaderSets[10]->ShaderProgram = _shaderSets[4]->ShaderProgram;
	_shaderSets[11]->ShaderProgram = _shaderSets[5]->ShaderProgram;
	_shaderSets[12]->ShaderProgram = _shaderSets[6]->ShaderProgram;

	// 乗算も通常と同じシェーダーを利用する
	_shaderSets[13]->ShaderProgram = _shaderSets[1]->ShaderProgram;
	_shaderSets[14]->ShaderProgram = _shaderSets[2]->ShaderProgram;
	_shaderSets[15]->ShaderProgram = _shaderSets[3]->ShaderProgram;
	_shaderSets[16]->ShaderProgram = _shaderSets[4]->ShaderProgram;
	_shaderSets[17]->ShaderProgram = _shaderSets[5]->ShaderProgram;
	_shaderSets[18]->ShaderProgram = _shaderSets[6]->ShaderProgram;
#endif


	// SetupMask
	_shaderSets[0]->AttributePositionLocation = _shaderSets[0]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[0]->AttributeTexCoordLocation = _shaderSets[0]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[0]->SamplerTexture0Location = _shaderSets[0]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[0]->UniformClipMatrixLocation = _shaderSets[0]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[0]->UnifromChannelFlagLocation = _shaderSets[0]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[0]->UniformBaseColorLocation = _shaderSets[0]->ShaderProgram->getUniformLocation("u_baseColor");

	// 通常
	_shaderSets[1]->AttributePositionLocation = _shaderSets[1]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[1]->AttributeTexCoordLocation = _shaderSets[1]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[1]->SamplerTexture0Location = _shaderSets[1]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[1]->UniformMatrixLocation = _shaderSets[1]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[1]->UniformBaseColorLocation = _shaderSets[1]->ShaderProgram->getUniformLocation("u_baseColor");

	// 通常（クリッピング）
	_shaderSets[2]->AttributePositionLocation = _shaderSets[2]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[2]->AttributeTexCoordLocation = _shaderSets[2]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[2]->SamplerTexture0Location = _shaderSets[2]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[2]->SamplerTexture1Location = _shaderSets[2]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[2]->UniformMatrixLocation = _shaderSets[2]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[2]->UniformClipMatrixLocation = _shaderSets[2]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[2]->UnifromChannelFlagLocation = _shaderSets[2]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[2]->UniformBaseColorLocation = _shaderSets[2]->ShaderProgram->getUniformLocation("u_baseColor");

	// 通常（クリッピング・反転）
	_shaderSets[3]->AttributePositionLocation = _shaderSets[3]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[3]->AttributeTexCoordLocation = _shaderSets[3]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[3]->SamplerTexture0Location = _shaderSets[3]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[3]->SamplerTexture1Location = _shaderSets[3]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[3]->UniformMatrixLocation = _shaderSets[3]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[3]->UniformClipMatrixLocation = _shaderSets[3]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[3]->UnifromChannelFlagLocation = _shaderSets[3]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[3]->UniformBaseColorLocation = _shaderSets[3]->ShaderProgram->getUniformLocation("u_baseColor");

	// 通常（PremultipliedAlpha）
	_shaderSets[4]->AttributePositionLocation = _shaderSets[4]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[4]->AttributeTexCoordLocation = _shaderSets[4]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[4]->SamplerTexture0Location = _shaderSets[4]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[4]->UniformMatrixLocation = _shaderSets[4]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[4]->UniformBaseColorLocation = _shaderSets[4]->ShaderProgram->getUniformLocation("u_baseColor");

	// 通常（クリッピング、PremultipliedAlpha）
	_shaderSets[5]->AttributePositionLocation = _shaderSets[5]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[5]->AttributeTexCoordLocation = _shaderSets[5]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[5]->SamplerTexture0Location = _shaderSets[5]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[5]->SamplerTexture1Location = _shaderSets[5]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[5]->UniformMatrixLocation = _shaderSets[5]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[5]->UniformClipMatrixLocation = _shaderSets[5]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[5]->UnifromChannelFlagLocation = _shaderSets[5]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[5]->UniformBaseColorLocation = _shaderSets[5]->ShaderProgram->getUniformLocation("u_baseColor");

	// 通常（クリッピング・反転、PremultipliedAlpha）
	_shaderSets[6]->AttributePositionLocation = _shaderSets[6]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[6]->AttributeTexCoordLocation = _shaderSets[6]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[6]->SamplerTexture0Location = _shaderSets[6]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[6]->SamplerTexture1Location = _shaderSets[6]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[6]->UniformMatrixLocation = _shaderSets[6]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[6]->UniformClipMatrixLocation = _shaderSets[6]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[6]->UnifromChannelFlagLocation = _shaderSets[6]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[6]->UniformBaseColorLocation = _shaderSets[6]->ShaderProgram->getUniformLocation("u_baseColor");

	// 加算
	_shaderSets[7]->AttributePositionLocation = _shaderSets[7]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[7]->AttributeTexCoordLocation = _shaderSets[7]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[7]->SamplerTexture0Location = _shaderSets[7]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[7]->UniformMatrixLocation = _shaderSets[7]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[7]->UniformBaseColorLocation = _shaderSets[7]->ShaderProgram->getUniformLocation("u_baseColor");

	// 加算（クリッピング）
	_shaderSets[8]->AttributePositionLocation = _shaderSets[8]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[8]->AttributeTexCoordLocation = _shaderSets[8]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[8]->SamplerTexture0Location = _shaderSets[8]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[8]->SamplerTexture1Location = _shaderSets[8]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[8]->UniformMatrixLocation = _shaderSets[8]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[8]->UniformClipMatrixLocation = _shaderSets[8]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[8]->UnifromChannelFlagLocation = _shaderSets[8]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[8]->UniformBaseColorLocation = _shaderSets[8]->ShaderProgram->getUniformLocation("u_baseColor");

	// 加算（クリッピング・反転）
	_shaderSets[9]->AttributePositionLocation = _shaderSets[9]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[9]->AttributeTexCoordLocation = _shaderSets[9]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[9]->SamplerTexture0Location = _shaderSets[9]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[9]->SamplerTexture1Location = _shaderSets[9]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[9]->UniformMatrixLocation = _shaderSets[9]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[9]->UniformClipMatrixLocation = _shaderSets[9]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[9]->UnifromChannelFlagLocation = _shaderSets[9]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[9]->UniformBaseColorLocation = _shaderSets[9]->ShaderProgram->getUniformLocation("u_baseColor");

	// 加算（PremultipliedAlpha）
	_shaderSets[10]->AttributePositionLocation = _shaderSets[10]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[10]->AttributeTexCoordLocation = _shaderSets[10]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[10]->SamplerTexture0Location = _shaderSets[10]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[10]->UniformMatrixLocation = _shaderSets[10]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[10]->UniformBaseColorLocation = _shaderSets[10]->ShaderProgram->getUniformLocation("u_baseColor");

	// 加算（クリッピング、PremultipliedAlpha）
	_shaderSets[11]->AttributePositionLocation = _shaderSets[11]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[11]->AttributeTexCoordLocation = _shaderSets[11]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[11]->SamplerTexture0Location = _shaderSets[11]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[11]->SamplerTexture1Location = _shaderSets[11]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[11]->UniformMatrixLocation = _shaderSets[11]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[11]->UniformClipMatrixLocation = _shaderSets[11]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[11]->UnifromChannelFlagLocation = _shaderSets[11]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[11]->UniformBaseColorLocation = _shaderSets[11]->ShaderProgram->getUniformLocation("u_baseColor");

	// 加算（クリッピング・反転、PremultipliedAlpha）
	_shaderSets[12]->AttributePositionLocation = _shaderSets[12]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[12]->AttributeTexCoordLocation = _shaderSets[12]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[12]->SamplerTexture0Location = _shaderSets[12]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[12]->SamplerTexture1Location = _shaderSets[12]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[12]->UniformMatrixLocation = _shaderSets[12]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[12]->UniformClipMatrixLocation = _shaderSets[12]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[12]->UnifromChannelFlagLocation = _shaderSets[12]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[12]->UniformBaseColorLocation = _shaderSets[12]->ShaderProgram->getUniformLocation("u_baseColor");

	// 乗算
	_shaderSets[13]->AttributePositionLocation = _shaderSets[13]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[13]->AttributeTexCoordLocation = _shaderSets[13]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[13]->SamplerTexture0Location = _shaderSets[13]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[13]->UniformMatrixLocation = _shaderSets[13]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[13]->UniformBaseColorLocation = _shaderSets[13]->ShaderProgram->getUniformLocation("u_baseColor");

	// 乗算（クリッピング）
	_shaderSets[14]->AttributePositionLocation = _shaderSets[14]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[14]->AttributeTexCoordLocation = _shaderSets[14]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[14]->SamplerTexture0Location = _shaderSets[14]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[14]->SamplerTexture1Location = _shaderSets[14]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[14]->UniformMatrixLocation = _shaderSets[14]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[14]->UniformClipMatrixLocation = _shaderSets[14]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[14]->UnifromChannelFlagLocation = _shaderSets[14]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[14]->UniformBaseColorLocation = _shaderSets[14]->ShaderProgram->getUniformLocation("u_baseColor");

	// 乗算（クリッピング・反転）
	_shaderSets[15]->AttributePositionLocation = _shaderSets[15]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[15]->AttributeTexCoordLocation = _shaderSets[15]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[15]->SamplerTexture0Location = _shaderSets[15]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[15]->SamplerTexture1Location = _shaderSets[15]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[15]->UniformMatrixLocation = _shaderSets[15]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[15]->UniformClipMatrixLocation = _shaderSets[15]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[15]->UnifromChannelFlagLocation = _shaderSets[15]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[15]->UniformBaseColorLocation = _shaderSets[15]->ShaderProgram->getUniformLocation("u_baseColor");

	// 乗算（PremultipliedAlpha）
	_shaderSets[16]->AttributePositionLocation = _shaderSets[16]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[16]->AttributeTexCoordLocation = _shaderSets[16]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[16]->SamplerTexture0Location = _shaderSets[16]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[16]->UniformMatrixLocation = _shaderSets[16]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[16]->UniformBaseColorLocation = _shaderSets[16]->ShaderProgram->getUniformLocation("u_baseColor");

	// 乗算（クリッピング、PremultipliedAlpha）
	_shaderSets[17]->AttributePositionLocation = _shaderSets[17]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[17]->AttributeTexCoordLocation = _shaderSets[17]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[17]->SamplerTexture0Location = _shaderSets[17]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[17]->SamplerTexture1Location = _shaderSets[17]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[17]->UniformMatrixLocation = _shaderSets[17]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[17]->UniformClipMatrixLocation = _shaderSets[17]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[17]->UnifromChannelFlagLocation = _shaderSets[17]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[17]->UniformBaseColorLocation = _shaderSets[17]->ShaderProgram->getUniformLocation("u_baseColor");

	// 乗算（クリッピング・反転、PremultipliedAlpha）
	_shaderSets[18]->AttributePositionLocation = _shaderSets[18]->ShaderProgram->getAttributeLocation("a_position");
	_shaderSets[18]->AttributeTexCoordLocation = _shaderSets[18]->ShaderProgram->getAttributeLocation("a_texCoord");
	_shaderSets[18]->SamplerTexture0Location = _shaderSets[18]->ShaderProgram->getUniformLocation("s_texture0");
	_shaderSets[18]->SamplerTexture1Location = _shaderSets[18]->ShaderProgram->getUniformLocation("s_texture1");
	_shaderSets[18]->UniformMatrixLocation = _shaderSets[18]->ShaderProgram->getUniformLocation("u_matrix");
	_shaderSets[18]->UniformClipMatrixLocation = _shaderSets[18]->ShaderProgram->getUniformLocation("u_clipMatrix");
	_shaderSets[18]->UnifromChannelFlagLocation = _shaderSets[18]->ShaderProgram->getUniformLocation("u_channelFlag");
	_shaderSets[18]->UniformBaseColorLocation = _shaderSets[18]->ShaderProgram->getUniformLocation("u_baseColor");
}

void CubismShader_CC::SetupShaderProgram(CubismRenderer_CC* renderer
	, PipelineDescriptor* desc
	, Texture2D* texture
	, csmInt32 vertexCount, csmFloat32* vertexArray
	, csmFloat32* uvArray, csmFloat32 opacity
	, CubismRenderer::CubismBlendMode colorBlendMode
	, const CubismRenderer::CubismTextureColor& baseColor
	, csmBool isPremultipliedAlpha, CubismMatrix44& matrix4x4
	, csmBool invertedMask)
{
	if (_shaderSets.GetSize() == 0)
	{
		GenerateShaders();
	}

	// Blending
	BlendFactor SRC_COLOR;
	BlendFactor DST_COLOR;
	BlendFactor SRC_ALPHA;
	BlendFactor DST_ALPHA;

	static std::string AttributePositionName = "a_position";
	static std::string AttributeTexCoordName = "a_texCoord";
	ProgramState* state;

	if (renderer->GetClippingContextBufferForMask() != nullptr) // マスク生成時
	{
		auto shaderSet = _shaderSets[ShaderNames_SetupMask];
		state = new ProgramState(shaderSet->ShaderProgram);

		//テクスチャ設定
		state->setTexture(shaderSet->SamplerTexture0Location, 0, texture->getBackendTexture());

		// 頂点配列の設定
		state->getVertexLayout()->setAttribute(AttributePositionName, shaderSet->AttributePositionLocation,
			VertexFormat::FLOAT2, 0, false);
		// テクスチャ頂点の設定
		state->getVertexLayout()->setAttribute(AttributeTexCoordName, shaderSet->AttributeTexCoordLocation,
			VertexFormat::FLOAT2, vertexCount * sizeof(float) * 2, false);

		// チャンネル
		const auto channelNo = renderer->GetClippingContextBufferForMask()->_layoutChannelNo;
		const auto colorChannel =
			renderer->GetClippingContextBufferForMask()->GetClippingManager()->GetChannelFlagAsColor(channelNo);
		Vec4 vec4 = { colorChannel->R, colorChannel->G, colorChannel->B, colorChannel->A };
		state->setUniform(shaderSet->UnifromChannelFlagLocation, &vec4, sizeof(float) * 4);
		state->setUniform(shaderSet->UniformClipMatrixLocation,
			renderer->GetClippingContextBufferForMask()->_matrixForMask.GetArray(),
			sizeof(float) * 16);

		const auto rect = renderer->GetClippingContextBufferForMask()->_layoutBounds;

		vec4.set(rect->X * 2.0f - 1.0f,
			rect->Y * 2.0f - 1.0f,
			rect->GetRight() * 2.0f - 1.0f,
			rect->GetBottom() * 2.0f - 1.0f);
		state->setUniform(shaderSet->UniformBaseColorLocation, &vec4, sizeof(float) * 4);

		SRC_COLOR = BlendFactor::ZERO;
		DST_COLOR = BlendFactor::ONE_MINUS_SRC_COLOR;
		SRC_ALPHA = BlendFactor::ZERO;
		DST_ALPHA = BlendFactor::ONE_MINUS_SRC_ALPHA;
	}
	else // マスク生成以外の場合
	{
		const csmBool masked = renderer->GetClippingContextBufferForDraw() != nullptr;  // この描画オブジェクトはマスク対象か
		const csmInt32 offset = (masked ? (invertedMask ? 2 : 1) : 0) + (isPremultipliedAlpha ? 3 : 0);

		CubismShaderSet* shaderSet;
		switch (colorBlendMode)
		{
		case CubismRenderer::CubismBlendMode_Normal:
		default:
			shaderSet = _shaderSets[ShaderNames_Normal + offset];
			SRC_COLOR = BlendFactor::ONE;
			DST_COLOR = BlendFactor::ONE_MINUS_SRC_ALPHA;
			SRC_ALPHA = BlendFactor::ONE;
			DST_ALPHA = BlendFactor::ONE_MINUS_SRC_ALPHA;
			break;

		case CubismRenderer::CubismBlendMode_Additive:
			shaderSet = _shaderSets[ShaderNames_Add + offset];
			SRC_COLOR = BlendFactor::ONE;
			DST_COLOR = BlendFactor::ONE;
			SRC_ALPHA = BlendFactor::ZERO;
			DST_ALPHA = BlendFactor::ONE;
			break;

		case CubismRenderer::CubismBlendMode_Multiplicative:
			shaderSet = _shaderSets[ShaderNames_Mult + offset];
			SRC_COLOR = BlendFactor::DST_COLOR;
			DST_COLOR = BlendFactor::ONE_MINUS_SRC_ALPHA;
			SRC_ALPHA = BlendFactor::ZERO;
			DST_ALPHA = BlendFactor::ONE;
			break;
		}

		state = new ProgramState(shaderSet->ShaderProgram);

		// 頂点配列の設定
		state->getVertexLayout()->setAttribute(AttributePositionName, shaderSet->AttributePositionLocation,
			VertexFormat::FLOAT2, 0, false);
		// テクスチャ頂点の設定
		state->getVertexLayout()->setAttribute(AttributeTexCoordName, shaderSet->AttributeTexCoordLocation,
			VertexFormat::FLOAT2, vertexCount * sizeof(float) * 2, false);

		if (masked)
		{
			// frameBufferに書かれたテクスチャ
			auto tex = renderer->_offscreenFrameBuffer.GetColorBuffer();
			state->setTexture(shaderSet->SamplerTexture1Location, 1, tex->getBackendTexture());

			// View座標をClippingContextの座標に変換するための行列を設定
			state->setUniform(shaderSet->UniformClipMatrixLocation,
				renderer->GetClippingContextBufferForDraw()->_matrixForDraw.GetArray(),
				sizeof(float) * 16);

			// 使用するカラーチャンネルを設定
			const auto channelNo = renderer->GetClippingContextBufferForDraw()->_layoutChannelNo;
			const auto colorChannel =
				renderer->GetClippingContextBufferForDraw()->GetClippingManager()->GetChannelFlagAsColor(channelNo);
			Vec4 vec4 = { colorChannel->R, colorChannel->G, colorChannel->B, colorChannel->A };
			state->setUniform(shaderSet->UnifromChannelFlagLocation,
				&vec4, sizeof(float) * 4);
		}

		//テクスチャ設定
		state->setTexture(shaderSet->SamplerTexture0Location, 0, texture->getBackendTexture());

		//座標変換
		state->setUniform(shaderSet->UniformMatrixLocation,
			matrix4x4.GetArray(),
			sizeof(float) * 16);

		Vec4 vec4 = { baseColor.R, baseColor.G, baseColor.B, baseColor.A };
		state->setUniform(shaderSet->UniformBaseColorLocation,
			&vec4, sizeof(float) * 4);
	}

	state->getVertexLayout()->setLayout(sizeof(float) * 2);
	desc->programState = state;
	auto& blend = desc->blendDescriptor;
	blend.blendEnabled = true;
	blend.sourceRGBBlendFactor = SRC_COLOR;
	blend.destinationRGBBlendFactor = DST_COLOR;
	blend.sourceAlphaBlendFactor = SRC_ALPHA;
	blend.destinationAlphaBlendFactor = DST_ALPHA;
}

Program* CubismShader_CC::LoadShaderProgram(const csmChar* vertShaderSrc, const csmChar* fragShaderSrc)
{
	return backend::Device::getInstance()->newProgram(vertShaderSrc, fragShaderSrc);
}

/*********************************************************************************************************************
 *                                      CubismRenderer_CC
 ********************************************************************************************************************/

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void CubismRenderer_CC::SetExtShaderMode(csmBool extMode, csmBool extPAMode)
{
	CubismShader_CC::SetExtShaderMode(extMode, extPAMode);
	CubismShader_CC::DeleteInstance();
}

void CubismRenderer_CC::ReloadShader()
{
	CubismShader_CC::DeleteInstance();
}
#endif

CubismRenderer* CubismRenderer::Create()
{
	return CSM_NEW CubismRenderer_CC();
}

void CubismRenderer::StaticRelease()
{
	CubismRenderer_CC::DoStaticRelease();
}

CubismRenderer_CC::CubismRenderer_CC()
	: _clippingManager(nullptr)
	, _clippingContextBufferForMask(nullptr)
	, _clippingContextBufferForDraw(nullptr)
	, _lastCullMode()
{
	_offscreenFrameBuffer._renderer = this;
	// テクスチャ対応マップの容量を確保しておく.
	_textures.reserve(32);
	ccr = Director::getInstance()->getRenderer();
#ifdef CSM_DEBU
	// make sure default texture is created
	Sprite::create();
#endif
}

CubismRenderer_CC::~CubismRenderer_CC()
{
	CSM_DELETE_SELF(CubismClippingManager_CC, _clippingManager);
}

void CubismRenderer_CC::DoStaticRelease()
{
	CubismShader_CC::DeleteInstance();
}

void CubismRenderer_CC::Initialize(CubismModel* model)
{
	if (model->IsUsingMasking())
	{
		_clippingManager = CSM_NEW CubismClippingManager_CC();  //クリッピングマスク・バッファ前処理方式を初期化
		_clippingManager->Initialize(
			*model,
			model->GetDrawableCount(),
			model->GetDrawableMasks(),
			model->GetDrawableMaskCounts()
		);

		_offscreenFrameBuffer.CreateOffscreenFrame(
			_clippingManager->GetClippingMaskBufferSize(),
			_clippingManager->GetClippingMaskBufferSize());
	}

	_sortedDrawableIndexList.Resize(model->GetDrawableCount(), 0);

	CubismRenderer::Initialize(model);  //親クラスの処理を呼ぶ
}

void CubismRenderer_CC::PreDraw()
{
	AddCallBack([=]()
	{
		ccr->setScissorTest(false);
		ccr->setStencilTest(false);
		ccr->setDepthTest(false);
		ccr->setWinding(backend::Winding::COUNTER_CLOCK_WISE);
	});

	//異方性フィルタリング。プラットフォームのOpenGLによっては未対応の場合があるので、未設定のときは設定しない
	if (GetAnisotropy() > 0.0f)
	{
		for (csmInt32 i = 0; i < _textures.size(); i++)
		{
			// not supported
		}
	}
	_mvpMatrix = GetMvpMatrix();
}

void CubismRenderer_CC::DoDrawModel()
{
	//------------ クリッピングマスク・バッファ前処理方式の場合 ------------
	if (_clippingManager != nullptr)
	{
		PreDraw();

		// サイズが違う場合はここで作成しなおし
		if (_offscreenFrameBuffer.GetBufferWidth() != static_cast<csmUint32>(_clippingManager->GetClippingMaskBufferSize()) ||
			_offscreenFrameBuffer.GetBufferHeight() != static_cast<csmUint32>(_clippingManager->GetClippingMaskBufferSize()))
		{
			_offscreenFrameBuffer.DestroyOffscreenFrame();
			_offscreenFrameBuffer.CreateOffscreenFrame(
				static_cast<csmUint32>(_clippingManager->GetClippingMaskBufferSize()),
				static_cast<csmUint32>(_clippingManager->GetClippingMaskBufferSize()));
		}

		_clippingManager->SetupClippingContext(*GetModel(), this,
			_rendererProfile._lastViewport);
	}

	// 上記クリッピング処理内でも一度PreDrawを呼ぶので注意!!
	PreDraw();

	const csmInt32 drawableCount = GetModel()->GetDrawableCount();
	const csmInt32* renderOrder = GetModel()->GetDrawableRenderOrders();

	// インデックスを描画順でソート
	for (csmInt32 i = 0; i < drawableCount; ++i)
	{
		const csmInt32 order = renderOrder[i];
		_sortedDrawableIndexList[order] = i;
	}

	// 描画
	for (csmInt32 i = 0; i < drawableCount; ++i)
	{
		const csmInt32 drawableIndex = _sortedDrawableIndexList[i];

		// Drawableが表示状態でなければ処理をパスする
		if (!GetModel()->GetDrawableDynamicFlagIsVisible(drawableIndex))
		{
			continue;
		}

		// クリッピングマスク
		CubismClippingContext_CC* clipContext = (_clippingManager != nullptr)
			? (*_clippingManager->GetClippingContextListForDraw())[drawableIndex]
			: nullptr;

		if (clipContext != nullptr && IsUsingHighPrecisionMask()) // マスクを書く必要がある
		{
			if (clipContext->_isUsing) // 書くことになっていた
			{
				// 生成したFrameBufferと同じサイズでビューポートを設定
				SetViewPort(0, 0,
					_clippingManager->GetClippingMaskBufferSize(),
					_clippingManager->GetClippingMaskBufferSize());

				PreDraw(); // バッファをクリアする

				_offscreenFrameBuffer.BeginDraw();

				// マスクをクリアする
				// 1が無効（描かれない）領域、0が有効（描かれる）領域。（シェーダで Cd*Csで0に近い値をかけてマスクを作る。1をかけると何も起こらない）
				_offscreenFrameBuffer.Clear(1.0f, 1.0f, 1.0f, 1.0f);
			}

			{
				const csmInt32 clipDrawCount = clipContext->_clippingIdCount;
				for (csmInt32 index = 0; index < clipDrawCount; index++)
				{
					const csmInt32 clipDrawIndex = clipContext->_clippingIdList[index];

					// 頂点情報が更新されておらず、信頼性がない場合は描画をパスする
					if (!GetModel()->GetDrawableDynamicFlagVertexPositionsDidChange(clipDrawIndex))
					{
						continue;
					}

					IsCulling(GetModel()->GetDrawableCulling(clipDrawIndex) != 0);

					// 今回専用の変換を適用して描く
					// チャンネルも切り替える必要がある(A,R,G,B)
					SetClippingContextBufferForMask(clipContext);
					DrawMesh(
						GetModel()->GetDrawableTextureIndices(clipDrawIndex),
						GetModel()->GetDrawableVertexIndexCount(clipDrawIndex),
						GetModel()->GetDrawableVertexCount(clipDrawIndex),
						const_cast<csmUint16*>(GetModel()->GetDrawableVertexIndices(clipDrawIndex)),
						const_cast<csmFloat32*>(GetModel()->GetDrawableVertices(clipDrawIndex)),
						reinterpret_cast<csmFloat32*>(const_cast<Core::csmVector2*>(GetModel()->GetDrawableVertexUvs(clipDrawIndex))),
						GetModel()->GetDrawableOpacity(clipDrawIndex),
						CubismRenderer::CubismBlendMode_Normal,   //クリッピングは通常描画を強制
						false // マスク生成時はクリッピングの反転使用は全く関係がない
					);
				}
			}

			{
				// --- 後処理 ---
				_offscreenFrameBuffer.EndDraw();
				SetClippingContextBufferForMask(nullptr);
				auto& vp = _rendererProfile._lastViewport;
				SetViewPort(vp.x, vp.y, vp.w, vp.h);

				PreDraw(); // バッファをクリアする
			}
		}

		// クリッピングマスクをセットする
		SetClippingContextBufferForDraw(clipContext);

		IsCulling(GetModel()->GetDrawableCulling(drawableIndex) != 0);

		DrawMesh(
			GetModel()->GetDrawableTextureIndices(drawableIndex),
			GetModel()->GetDrawableVertexIndexCount(drawableIndex),
			GetModel()->GetDrawableVertexCount(drawableIndex),
			const_cast<csmUint16*>(GetModel()->GetDrawableVertexIndices(drawableIndex)),
			const_cast<csmFloat32*>(GetModel()->GetDrawableVertices(drawableIndex)),
			reinterpret_cast<csmFloat32*>(const_cast<Core::csmVector2*>(GetModel()->GetDrawableVertexUvs(drawableIndex))),
			GetModel()->GetDrawableOpacity(drawableIndex),
			GetModel()->GetDrawableBlendMode(drawableIndex),
			GetModel()->GetDrawableInvertedMask(drawableIndex) // マスクを反転使用するか
		);
	}

	//
	PostDraw();

}

void CubismRenderer_CC::DrawMesh(csmInt32 textureNo, csmInt32 indexCount, csmInt32 vertexCount
	, csmUint16* indexArray, csmFloat32* vertexArray, csmFloat32* uvArray
	, csmFloat32 opacity, CubismBlendMode colorBlendMode, csmBool invertedMask)
{
	const auto it = _textures.find(textureNo);
#ifndef CSM_DEBUG
	// モデルが参照するテクスチャがバインドされていない場合は描画をスキップする
	if (it == _textures.end())
		return;
#endif

// 裏面描画の有効・無効
	if (IsCulling())
		SetCullMode(backend::CullMode::BACK);
	else
		SetCullMode(backend::CullMode::NONE);

	auto modelColorRGBA = GetModelColor();

	if (GetClippingContextBufferForMask() == nullptr) // マスク生成時以外
	{
		modelColorRGBA.A *= opacity;
		if (IsPremultipliedAlpha())
		{
			modelColorRGBA.R *= modelColorRGBA.A;
			modelColorRGBA.G *= modelColorRGBA.A;
			modelColorRGBA.B *= modelColorRGBA.A;
		}
	}

	// シェーダに渡すテクスチャID
	Texture2D* drawTexture;
#ifdef CSM_DEBU
	// テクスチャマップからバインド済みテクスチャIDを取得
	// バインドされていなければダミーのテクスチャIDをセットする
	if (it != _textures.end())
		drawTexture = it->second;
	else
		drawTexture = Director::getInstance()->getTextureCache()->getTextureForKey("/cc_2x2_white_image");
#else
	drawTexture = it->second;
#endif

	auto vbuf = new float[vertexCount * 4];
	const auto arrSize = vertexCount * sizeof(float) * 2;
	std::memcpy(vbuf, vertexArray, arrSize);
	std::memcpy(vbuf + vertexCount * 2, uvArray, arrSize);

	auto cmd = std::make_shared<CustomCommand>();
	cmds.push_back(cmd);
	cmd->init(0.f);
	cmd->setDrawType(CustomCommand::DrawType::ELEMENT);
	cmd->setPrimitiveType(PrimitiveType::TRIANGLE);
	cmd->createIndexBuffer(IndexFormat::U_SHORT, indexCount, BufferUsage::STATIC);
	cmd->createVertexBuffer(sizeof(float) * 4, vertexCount, BufferUsage::STATIC);
	cmd->updateIndexBuffer(indexArray, indexCount * sizeof(csmUint16));
	cmd->updateVertexBuffer(vbuf, vertexCount * sizeof(float) * 4);
	delete[] vbuf;

	CubismShader_CC::GetInstance()->SetupShaderProgram(
		this, &cmd->getPipelineDescriptor(), drawTexture, vertexCount, vertexArray, uvArray
		, opacity, colorBlendMode, modelColorRGBA, IsPremultipliedAlpha()
		, _mvpMatrix, invertedMask
	);

	// ポリゴンメッシュを描画する
	ccr->addCommand(cmd.get());

	// 後処理
	SetClippingContextBufferForDraw(nullptr);
	SetClippingContextBufferForMask(nullptr);
}

void CubismRenderer_CC::SaveProfile()
{
	// SaveProfile is called at first
	ClearCommands();
	_lastCullMode = (backend::CullMode)-1;
	AddCallBack([=]()
	{
		_rendererProfile.Save();
	});
}

void CubismRenderer_CC::RestoreProfile()
{
	AddCallBack([=]()
	{
		_rendererProfile.Restore();
	});
}

void CubismRenderer_CC::BindTexture(csmUint32 modelTextureNo, Texture2D* texture)
{
	if (texture)
		_textures.insert(modelTextureNo, texture);
}

void CubismRenderer_CC::SetClippingMaskBufferSize(csmInt32 size)
{
	//FrameBufferのサイズを変更するためにインスタンスを破棄・再作成する
	CSM_DELETE_SELF(CubismClippingManager_CC, _clippingManager);

	_clippingManager = CSM_NEW CubismClippingManager_CC();

	_clippingManager->SetClippingMaskBufferSize(size);

	_clippingManager->Initialize(
		*GetModel(),
		GetModel()->GetDrawableCount(),
		GetModel()->GetDrawableMasks(),
		GetModel()->GetDrawableMaskCounts()
	);
}

csmInt32 CubismRenderer_CC::GetClippingMaskBufferSize() const
{
	return _clippingManager->GetClippingMaskBufferSize();
}

void CubismRenderer_CC::SetClippingContextBufferForMask(CubismClippingContext_CC* clip)
{
	_clippingContextBufferForMask = clip;
}

CubismClippingContext_CC* CubismRenderer_CC::GetClippingContextBufferForMask() const
{
	return _clippingContextBufferForMask;
}

void CubismRenderer_CC::SetClippingContextBufferForDraw(CubismClippingContext_CC* clip)
{
	_clippingContextBufferForDraw = clip;
}

CubismClippingContext_CC* CubismRenderer_CC::GetClippingContextBufferForDraw() const
{
	return _clippingContextBufferForDraw;
}

void CubismRenderer_CC::SetViewPort(const Viewport& value)
{
	AddCallBack([=]()
	{
		ccr->setViewPort(value.x, value.y, value.w, value.h);
	});
}

void CubismRenderer_CC::SetViewPort(int x, int y, unsigned w, unsigned h)
{
	AddCallBack([=]()
	{
		ccr->setViewPort(x, y, w, h);
	});
}

void CubismRenderer_CC::SetCullMode(backend::CullMode value)
{
	if (value == _lastCullMode)
		return;
	_lastCullMode = value;
	AddCallBack([=]()
	{
		ccr->setCullMode(value);
	});
}

void CubismRenderer_CC::AddCallBack(const std::function<void()>& callback)
{
	auto cmd = std::make_shared<CallbackCommand>();
	cmds.push_back(cmd);
	cmd->init(0.f);
	cmd->func = callback;
	ccr->addCommand(cmd.get());
}

void CubismRenderer_CC::ClearCommands()
{
	for (auto&cmd : cmds)
	{
		const auto state = cmd->getPipelineDescriptor().programState;
		CC_SAFE_RELEASE(state);
	}
	cmds.clear();
}
