#include "CubismRenderer_CC.h"
#include "Type/CubismBasicType.hpp"
#include "Model/CubismModel.hpp"
#include "renderer/backend/Device.h"
#include <cstring>
#include <vector>

#ifdef CC_USE_GFX
#include "renderer/backend/gfx/DeviceGFX.h"
using Type = cocos2d::backend::DeviceGFX::Type;
using Uniform = cocos2d::backend::DeviceGFX::Uniform;
using UniformList = cocos2d::backend::DeviceGFX::UniformList;
using UniformBlock = cocos2d::backend::DeviceGFX::UniformBlock;
using UniformBlockList = cocos2d::backend::DeviceGFX::UniformBlockList;
using UniformSamplerTexture = cocos2d::backend::DeviceGFX::UniformSamplerTexture;
using UniformSamplerTextureList = cocos2d::backend::DeviceGFX::UniformSamplerTextureList;
using Attribute = cocos2d::backend::DeviceGFX::Attribute;
using AttributeList = cocos2d::backend::DeviceGFX::AttributeList;
#endif

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

CubismDrawCommand_CC::CubismDrawCommand_CC()
{
	_command.init(0.0);
	_command.setDrawType(cocos2d::CustomCommand::DrawType::ELEMENT);
	_command.setPrimitiveType(cocos2d::CustomCommand::PrimitiveType::TRIANGLE);
}

CubismDrawCommand_CC::~CubismDrawCommand_CC()
{
	if(_drawBuffer)
		CSM_FREE(_drawBuffer);
}

void CubismDrawCommand_CC::CreateVertexBuffer(csmSizeInt stride, csmSizeInt count)
{
	_vbSize = stride * count;
	if (_drawBuffer)
		CSM_FREE(_drawBuffer);
	_drawBuffer = static_cast<csmUint8*>(CSM_MALLOC(_vbSize));
	_command.createVertexBuffer(stride, count, cocos2d::CustomCommand::BufferUsage::DYNAMIC);
}

void CubismDrawCommand_CC::CreateIndexBuffer(csmSizeInt count)
{
	_command.createIndexBuffer(cocos2d::backend::IndexFormat::U_SHORT, count,
	                           cocos2d::CustomCommand::BufferUsage::DYNAMIC);
}

void CubismDrawCommand_CC::UpdateVertexBuffer(void* data, void* uvData, csmSizeInt count)
{
	const auto vbuf = reinterpret_cast<Core::csmVector2*>(_drawBuffer);
	const auto pos = static_cast<Core::csmVector2*>(data);
	const auto uv = static_cast<Core::csmVector2*>(uvData);
	for (size_t i = 0; i < count; ++i)
	{
		vbuf[i * 2] = pos[i];
		vbuf[i * 2 + 1] = uv[i];
	}
	_command.updateVertexBuffer(_drawBuffer, _vbSize);
}

void CubismDrawCommand_CC::UpdateIndexBuffer(void* data, csmSizeInt count)
{
	_command.updateIndexBuffer(data, count * sizeof(csmInt16));
}

CubismClippingManager_CC::CubismClippingManager_CC() :
	_currentFrameNo(0)
	, _clippingMaskBufferSize(256, 256)
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
			cc = CSM_NEW CubismClippingContext_CC(this, model, drawableMasks[i], drawableMaskCounts[i]);
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
			renderer->SetViewPort(0, 0, _clippingMaskBufferSize.X, _clippingMaskBufferSize.Y);

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

			const csmFloat32 MARGIN = 0.05f;
			csmFloat32 scaleX = 0.0f;
			csmFloat32 scaleY = 0.0f;

			if (renderer->IsUsingHighPrecisionMask())
			{
				const csmFloat32 ppu = model.GetPixelsPerUnit();
				const csmFloat32 maskPixelWidth = clipContext->_owner->_clippingMaskBufferSize.X;
				const csmFloat32 maskPixelHeight = clipContext->_owner->_clippingMaskBufferSize.Y;
				const csmFloat32 physicalMaskWidth = layoutBoundsOnTex01->Width * maskPixelWidth;
				const csmFloat32 physicalMaskHeight = layoutBoundsOnTex01->Height * maskPixelHeight;


				_tmpBoundsOnModel.SetRect(allClippedDrawRect);

				if (_tmpBoundsOnModel.Width * ppu > physicalMaskWidth)
				{
					_tmpBoundsOnModel.Expand(allClippedDrawRect->Width * MARGIN, 0.0f);
					scaleX = layoutBoundsOnTex01->Width / _tmpBoundsOnModel.Width;
				}
				else
				{
					scaleX = ppu / physicalMaskWidth;
				}

				if (_tmpBoundsOnModel.Height * ppu > physicalMaskHeight)
				{
					_tmpBoundsOnModel.Expand(0.0f, allClippedDrawRect->Height * MARGIN);
					scaleY = layoutBoundsOnTex01->Height / _tmpBoundsOnModel.Height;
				}
				else
				{
					scaleY = ppu / physicalMaskHeight;
				}
			}
			else
			{
				// モデル座標上の矩形を、適宜マージンを付けて使う
				_tmpBoundsOnModel.SetRect(allClippedDrawRect);
				_tmpBoundsOnModel.Expand(allClippedDrawRect->Width * MARGIN, allClippedDrawRect->Height * MARGIN);
				//########## 本来は割り当てられた領域の全体を使わず必要最低限のサイズがよい
				// シェーダ用の計算式を求める。回転を考慮しない場合は以下のとおり
				// movePeriod' = movePeriod * scaleX + offX     [[ movePeriod' = (movePeriod - tmpBoundsOnModel.movePeriod)*scale + layoutBoundsOnTex01.movePeriod ]]
				scaleX = layoutBoundsOnTex01->Width / _tmpBoundsOnModel.Width;
				scaleY = layoutBoundsOnTex01->Height / _tmpBoundsOnModel.Height;
			}

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

					if (model.GetDrawableVertexCount(clipDrawIndex) <= 0)
					{
						continue;
					}

					// 今回専用の変換を適用して描く
					// チャンネルも切り替える必要がある(A,R,G,B)
					renderer->SetClippingContextBufferForMask(clipContext);
					renderer->DrawMeshCC(clipContext->_clippingCommandBufferList->At(i),
						model.GetDrawableTextureIndex(clipDrawIndex),
						model.GetDrawableVertexIndexCount(clipDrawIndex),
						model.GetDrawableVertexCount(clipDrawIndex),
						const_cast<csmUint16*>(model.GetDrawableVertexIndices(clipDrawIndex)),
						const_cast<csmFloat32*>(model.GetDrawableVertices(clipDrawIndex)),
						reinterpret_cast<csmFloat32*>(const_cast<Core::csmVector2*>(model.GetDrawableVertexUvs(clipDrawIndex))),
						model.GetMultiplyColor(clipDrawIndex),
						model.GetScreenColor(clipDrawIndex),
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
	csmFloat32 clippedDrawTotalMaxX = -FLT_MAX, clippedDrawTotalMaxY = -FLT_MAX;

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
		csmFloat32 maxX = -FLT_MAX, maxY = -FLT_MAX;

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

void CubismClippingManager_CC::SetClippingMaskBufferSize(csmFloat32 width, csmFloat32 height)
{
	_clippingMaskBufferSize = CubismVector2(width, height);
}

CubismVector2 CubismClippingManager_CC::GetClippingMaskBufferSize() const
{
	return _clippingMaskBufferSize;
}

/*********************************************************************************************************************
*                                      CubismClippingContext
********************************************************************************************************************/
CubismClippingContext_CC::CubismClippingContext_CC(
	CubismClippingManager_CC* manager, CubismModel& model, const csmInt32* clippingDrawableIndices, csmInt32 clipCount)
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
	_clippingCommandBufferList = CSM_NEW csmVector<CubismDrawCommand_CC*>();
	for (csmUint32 i = 0; i < _clippingIdCount; ++i)
	{
		const csmInt32 clippingId = _clippingIdList[i];
		const csmInt32 drawableVertexCount = model.GetDrawableVertexCount(clippingId);
		const csmInt32 drawableVertexIndexCount = model.GetDrawableVertexIndexCount(clippingId);
		auto drawCommandBuffer = CSM_NEW CubismDrawCommand_CC();
		drawCommandBuffer->CreateVertexBuffer(sizeof(csmFloat32) * 4, drawableVertexCount);      // Vertices + UVs
		drawCommandBuffer->CreateIndexBuffer(drawableVertexIndexCount);
		_clippingCommandBufferList->PushBack(drawCommandBuffer);
	}
}

CubismClippingContext_CC::~CubismClippingContext_CC()
{
	CSM_SAFE_DELETE_NULL(_layoutBounds);
	CSM_SAFE_DELETE_NULL(_allClippedDrawRect);
	CSM_SAFE_DELETE_NULL(_clippedDrawableIndexList);
	if (_clippingCommandBufferList)
	{
		for (csmUint32 i = 0; i < _clippingCommandBufferList->GetSize(); ++i)
		{
			CSM_DELETE(_clippingCommandBufferList->At(i));
			_clippingCommandBufferList->At(i) = nullptr;
		}
		CSM_DELETE(_clippingCommandBufferList);
		_clippingCommandBufferList = nullptr;
	}
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
	std::set<backend::Program*> programs;
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

#if defined(CC_PLATFORM_MOBILE)
#define FRAG_SHADER_HEADER "#version 100\nprecision mediump float;\n"
#define VERT_SHADER_HEADER "#version 100\n"
#else
#define FRAG_SHADER_HEADER "#version 120\n"
#define VERT_SHADER_HEADER "#version 120\n"
#endif

// SetupMask
static const csmChar* VertShaderSrcSetupMask =
#ifndef CC_USE_GFX
	FRAG_SHADER_HEADER
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
#else
	R"(
layout(location=0) in vec2 a_position;
layout(location=1) in vec2 a_texCoord;
layout(std140, binding=0) uniform VSBlock
{
	mat4 u_clipMatrix;
};
layout(location=0) out vec4 v_myPos;
layout(location=1) out vec2 v_texCoord;

void main()
{
	vec4 pos = vec4(a_position.x, a_position.y, 0.0, 1.0);
	gl_Position = u_clipMatrix * pos;
	v_myPos = u_clipMatrix * pos;
	v_texCoord = a_texCoord;
	v_texCoord.y = 1.0 - v_texCoord.y;
}
)";
static const UniformBlock VertShaderSrcSetupMaskBlock = {
	0,0,"VSBlock",
	UniformList({
		{"u_clipMatrix", Type::MAT4, 1},
	}),
	1
};
#endif

static const csmChar* FragShaderSrcSetupMask =
#ifndef CC_USE_GFX
	FRAG_SHADER_HEADER
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
#else
	R"(
layout(location=0) in vec4 v_myPos;
layout(location=1) in vec2 v_texCoord;
layout(std140, binding=1) uniform FSBlock
{
	vec4 u_channelFlag;
	vec4 u_baseColor;
};
layout(binding=2) uniform sampler2D s_texture0;
layout(location=0) out vec4 cc_FragColor;

void main()
{
	float isInside =
	  step(u_baseColor.x, v_myPos.x/v_myPos.w)
	* step(u_baseColor.y, v_myPos.y/v_myPos.w)
	* step(v_myPos.x/v_myPos.w, u_baseColor.z)
	* step(v_myPos.y/v_myPos.w, u_baseColor.w);
	cc_FragColor = u_channelFlag * texture(s_texture0 , v_texCoord).a * isInside;
}
)";
static const UniformBlock FragShaderSrcSetupMaskBlock = {
	0,1,"FSBlock",
	UniformList({
		{"u_channelFlag", Type::FLOAT4, 1},
		{"u_baseColor", Type::FLOAT4, 1},
	}),
	1
};
static const UniformSamplerTextureList FragShaderSrcSetupMaskTextures = {
	UniformSamplerTexture {
		0,2,"s_texture0",Type::SAMPLER2D,1}
};
#endif
#if CSM_RENDERER_EXT
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
#ifndef CC_USE_GFX
	VERT_SHADER_HEADER
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
#else
	R"(
layout(location=0) in vec2 a_position;
layout(location=1) in vec2 a_texCoord;
layout(std140, binding=0) uniform VSBlock
{
	mat4 u_matrix;
};
layout(location=0) out vec2 v_texCoord;

void main()
{
	vec4 pos = vec4(a_position.x, a_position.y, 0.0, 1.0);
	gl_Position = u_matrix * pos;
	v_texCoord = a_texCoord;
	v_texCoord.y = 1.0 - v_texCoord.y;
}
)";
static const UniformBlock VertShaderSrcBlock = {
	0,0,"VSBlock",
	UniformList({
		{"u_matrix", Type::MAT4, 1},
	}),
	1
};
#endif
// Normal & Add & Mult 共通（クリッピングされたものの描画用）
static const csmChar* VertShaderSrcMasked =
#ifndef CC_USE_GFX
	VERT_SHADER_HEADER
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
#else
	R"(
layout(location=0) in vec2 a_position;
layout(location=1) in vec2 a_texCoord;
layout(std140, binding=0) uniform VSBlock
{
	mat4 u_matrix;
	mat4 u_clipMatrix;
};
layout(location=0) out vec2 v_texCoord;
layout(location=1) out vec4 v_clipPos;

void main()
{
	vec4 pos = vec4(a_position.x, a_position.y, 0.0, 1.0);
	gl_Position = u_matrix * pos;
	v_clipPos = u_clipMatrix * pos;
	v_texCoord = a_texCoord;
	v_texCoord.y = 1.0 - v_texCoord.y;
}
)";
static const UniformBlock VertShaderSrcMaskedBlock = {
	0,0,"VSBlock",
	UniformList({
		{"u_matrix", Type::MAT4, 1},
		{"u_clipMatrix", Type::MAT4, 1},
	}),
	1
};
#endif
//----- フラグメントシェーダプログラム -----
// Normal & Add & Mult 共通
static const csmChar* FragShaderSrc =
#ifndef CC_USE_GFX
	FRAG_SHADER_HEADER
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform sampler2D s_texture0;" //_MainTex
	"uniform vec4 u_baseColor;" //v2f.color
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);"
	"vec4 color = texColor * u_baseColor;"
	"gl_FragColor = vec4(color.rgb * color.a, color.a);"
	"}";
#else
	R"(
layout(location=0) in vec2 v_texCoord;
layout(std140, binding=1) uniform FSBlock
{
	vec4 u_baseColor;
	vec4 u_multiplyColor;
	vec4 u_screenColor;
};
layout(binding=2) uniform sampler2D s_texture0;
layout(location=0) out vec4 cc_FragColor;

void main()
{
	vec4 texColor = texture(s_texture0 , v_texCoord);
	texColor.rgb = texColor.rgb * u_multiplyColor.rgb;
	texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);
	vec4 color = texColor * u_baseColor;
	cc_FragColor = vec4(color.rgb * color.a, color.a);
}
)";
static const UniformBlock FragShaderSrcBlock = {
	0,1,"FSBlock",
	UniformList({
		{"u_baseColor", Type::FLOAT4, 1},
		{"u_multiplyColor", Type::FLOAT4, 1},
		{"u_screenColor", Type::FLOAT4, 1},
	}),
	1
};
static const UniformSamplerTextureList FragShaderSrcTextures = {
	UniformSamplerTexture {
		0,2,"s_texture0",Type::SAMPLER2D,1}
};
#endif
#if CSM_RENDERER_EXT
static const csmChar* FragShaderSrcTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform sampler2D s_texture0;" //_MainTex
	"uniform vec4 u_baseColor;" //v2f.color
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);"
	"vec4 color = texColor * u_baseColor;"
	"gl_FragColor = vec4(color.rgb * color.a, color.a);"
	"}";
#endif

// Normal & Add & Mult 共通 （PremultipliedAlpha）
static const csmChar* FragShaderSrcPremultipliedAlpha =
#ifndef CC_USE_GFX
	FRAG_SHADER_HEADER
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform sampler2D s_texture0;" //_MainTex
	"uniform vec4 u_baseColor;" //v2f.color
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);"
	"gl_FragColor = texColor * u_baseColor;"
	"}";
#else
	R"(
layout(location=0) in vec2 v_texCoord;
layout(std140, binding=1) uniform FSBlock
{
	vec4 u_baseColor;
	vec4 u_multiplyColor;
	vec4 u_screenColor;
};
layout(binding=2) uniform sampler2D s_texture0;
layout(location=0) out vec4 cc_FragColor;

void main()
{
	vec4 texColor = texture(s_texture0 , v_texCoord);
	texColor.rgb = texColor.rgb * u_multiplyColor.rgb;
	texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);
	cc_FragColor = texColor * u_baseColor;
}
)";
#endif
#if CSM_RENDERER_EXT
static const csmChar* FragShaderSrcPremultipliedAlphaTegra =
	"#version 100\n"
	"#extension GL_NV_shader_framebuffer_fetch : enable\n"
	"precision mediump float;"
	"varying vec2 v_texCoord;" //v2f.texcoord
	"uniform sampler2D s_texture0;" //_MainTex
	"uniform vec4 u_baseColor;" //v2f.color
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"	
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);"
	"gl_FragColor = texColor * u_baseColor;"
	"}";
#endif

// Normal & Add & Mult 共通（クリッピングされたものの描画用）
static const csmChar* FragShaderSrcMask =
#ifndef CC_USE_GFX
	FRAG_SHADER_HEADER
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);"
	"vec4 col_formask = texColor * u_baseColor;"
	"col_formask.rgb = col_formask.rgb * col_formask.a ;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * maskVal;"
	"gl_FragColor = col_formask;"
	"}";
#else
	R"(
layout(location=0) in vec2 v_texCoord;
layout(location=1) in vec4 v_clipPos;
layout(std140, binding=1) uniform FSBlock
{
	vec4 u_channelFlag;
	vec4 u_baseColor;
	vec4 u_multiplyColor;
	vec4 u_screenColor;
};
layout(binding=2) uniform sampler2D s_texture0;
layout(binding=3) uniform sampler2D s_texture1;
layout(location=0) out vec4 cc_FragColor;

void main()
{
	vec4 texColor = texture(s_texture0 , v_texCoord);
	texColor.rgb = texColor.rgb * u_multiplyColor.rgb;
	texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);
	vec4 col_formask = texColor * u_baseColor;
	col_formask.rgb = col_formask.rgb * col_formask.a ;
	vec4 clipMask = (1.0 - texture(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;
	float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;
	col_formask = col_formask * maskVal;
	cc_FragColor = col_formask;
}
)";
static const UniformBlock FragShaderSrcMaskBlock = {
	0,1,"FSBlock",
	UniformList({
		{"u_channelFlag", Type::FLOAT4, 1},
		{"u_baseColor", Type::FLOAT4, 1},
		{"u_multiplyColor", Type::FLOAT4, 1},
		{"u_screenColor", Type::FLOAT4, 1},
	}),
	1
};
static const UniformSamplerTextureList FragShaderSrcMaskTextures = {
	UniformSamplerTexture {
		0,2,"s_texture0",Type::SAMPLER2D,1},
	UniformSamplerTexture {
		0,3,"s_texture1",Type::SAMPLER2D,1}
};
#endif
#if CSM_RENDERER_EXT
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
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);"
	"vec4 col_formask = texColor * u_baseColor;"
	"col_formask.rgb = col_formask.rgb  * col_formask.a ;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * maskVal;"
	"gl_FragColor = col_formask;"
	"}";
#endif

// Normal & Add & Mult 共通（クリッピングされて反転使用の描画用）
static const csmChar* FragShaderSrcMaskInverted =
#ifndef CC_USE_GFX
	FRAG_SHADER_HEADER
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);"
	"vec4 col_formask = texColor * u_baseColor;"
	"col_formask.rgb = col_formask.rgb * col_formask.a ;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * (1.0 - maskVal);"
	"gl_FragColor = col_formask;"
	"}";
#else
	R"(
layout(location=0) in vec2 v_texCoord;
layout(location=1) in vec4 v_clipPos;
layout(std140, binding=1) uniform FSBlock
{
	vec4 u_channelFlag;
	vec4 u_baseColor;
	vec4 u_multiplyColor;
	vec4 u_screenColor;
};
layout(binding=2) uniform sampler2D s_texture0;
layout(binding=3) uniform sampler2D s_texture1;
layout(location=0) out vec4 cc_FragColor;

void main()
{
	vec4 texColor = texture(s_texture0 , v_texCoord);
	texColor.rgb = texColor.rgb * u_multiplyColor.rgb;
	texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);
	vec4 col_formask = texColor * u_baseColor;
	col_formask.rgb = col_formask.rgb * col_formask.a ;
	vec4 clipMask = (1.0 - texture(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;
	float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;
	col_formask = col_formask * (1.0 - maskVal);
	cc_FragColor = col_formask;
}
)";
#endif
#if CSM_RENDERER_EXT
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
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = texColor.rgb + u_screenColor.rgb - (texColor.rgb * u_screenColor.rgb);"
	"vec4 col_formask = texColor * u_baseColor;"
	"col_formask.rgb = col_formask.rgb  * col_formask.a ;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * (1.0 - maskVal);"
	"gl_FragColor = col_formask;"
	"}";
#endif

// Normal & Add & Mult 共通（クリッピングされたものの描画用、PremultipliedAlphaの場合）
static const csmChar* FragShaderSrcMaskPremultipliedAlpha =
#ifndef CC_USE_GFX
	FRAG_SHADER_HEADER
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);"
	"vec4 col_formask = texColor * u_baseColor;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * maskVal;"
	"gl_FragColor = col_formask;"
	"}";
#else
	R"(
layout(location=0) in vec2 v_texCoord;
layout(location=1) in vec4 v_clipPos;
layout(std140, binding=1) uniform FSBlock
{
	vec4 u_channelFlag;
	vec4 u_baseColor;
	vec4 u_multiplyColor;
	vec4 u_screenColor;
};
layout(binding=2) uniform sampler2D s_texture0;
layout(binding=3) uniform sampler2D s_texture1;
layout(location=0) out vec4 cc_FragColor;

void main()
{
	vec4 texColor = texture(s_texture0 , v_texCoord);
	texColor.rgb = texColor.rgb * u_multiplyColor.rgb;
	texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);
	vec4 col_formask = texColor * u_baseColor;
	vec4 clipMask = (1.0 - texture(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;
	float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;
	col_formask = col_formask * maskVal;
	cc_FragColor = col_formask;
}
)";
#endif
#if CSM_RENDERER_EXT
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
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);"
	"vec4 col_formask = texColor * u_baseColor;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * maskVal;"
	"gl_FragColor = col_formask;"
	"}";
#endif

// Normal & Add & Mult 共通（クリッピングされて反転使用の描画用、PremultipliedAlphaの場合）
static const csmChar* FragShaderSrcMaskInvertedPremultipliedAlpha =
#ifndef CC_USE_GFX
	FRAG_SHADER_HEADER
	"varying vec2 v_texCoord;"
	"varying vec4 v_clipPos;"
	"uniform sampler2D s_texture0;"
	"uniform sampler2D s_texture1;"
	"uniform vec4 u_channelFlag;"
	"uniform vec4 u_baseColor;"
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);"
	"vec4 col_formask = texColor * u_baseColor;"
	"vec4 clipMask = (1.0 - texture2D(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;"
	"float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;"
	"col_formask = col_formask * (1.0 - maskVal);"
	"gl_FragColor = col_formask;"
	"}";
#else
	R"(
layout(location=0) in vec2 v_texCoord;
layout(location=1) in vec4 v_clipPos;
layout(std140, binding=1) uniform FSBlock
{
	vec4 u_channelFlag;
	vec4 u_baseColor;
	vec4 u_multiplyColor;
	vec4 u_screenColor;
};
layout(binding=2) uniform sampler2D s_texture0;
layout(binding=3) uniform sampler2D s_texture1;
layout(location=0) out vec4 cc_FragColor;

void main()
{
	vec4 texColor = texture(s_texture0 , v_texCoord);
	texColor.rgb = texColor.rgb * u_multiplyColor.rgb;
	texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);
	vec4 col_formask = texColor * u_baseColor;
	vec4 clipMask = (1.0 - texture(s_texture1, v_clipPos.xy / v_clipPos.w)) * u_channelFlag;
	float maskVal = clipMask.r + clipMask.g + clipMask.b + clipMask.a;
	col_formask = col_formask * (1.0 - maskVal);
	cc_FragColor = col_formask;
}
)";
#endif
#if CSM_RENDERER_EXT
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
	"uniform vec4 u_multiplyColor;"
	"uniform vec4 u_screenColor;"
	"void main()"
	"{"
	"vec4 texColor = texture2D(s_texture0 , v_texCoord);"
	"texColor.rgb = texColor.rgb * u_multiplyColor.rgb;"
	"texColor.rgb = (texColor.rgb + u_screenColor.rgb * texColor.a) - (texColor.rgb * u_screenColor.rgb);"
	"vec4 col_formask = texColor * u_baseColor;"
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

#if CSM_RENDERER_EXT
csmBool CubismShader_CC::s_extMode = false;
csmBool CubismShader_CC::s_extPAMode = false;
void CubismShader_CC::SetExtShaderMode(csmBool extMode, csmBool extPAMode) {
	s_extMode = extMode;
	s_extPAMode = extPAMode;
}
#endif

#ifdef CC_USE_GFX
static cocos2d::backend::Program* LoadGFXProgram(
	const std::string& name,
	const std::string& vs, const std::string& fs,
	const UniformBlock& vsb, const UniformBlock& fsb,
	const UniformSamplerTextureList& textures)
{
	using ShaderStage = DeviceGFX::ShaderStage;
	using ShaderStageFlagBit = DeviceGFX::ShaderStageFlagBit;
	const auto device = (DeviceGFX*)backend::Device::getInstance();
	DeviceGFX::ShaderInfo info;
	info.attributes = {
		DeviceGFX::Attribute{"a_position",VertexFormat::FLOAT2,false,0,false,0},
		DeviceGFX::Attribute{"a_texCoord",VertexFormat::FLOAT2,false,0,false,1},
	};
	info.name = name;
	info.stages = {
		ShaderStage{ShaderStageFlagBit::VERTEX, vs},
		ShaderStage{ShaderStageFlagBit::FRAGMENT, fs},
	};
	info.blocks = { vsb, fsb };
	info.samplerTextures = textures;
	return device->newProgram(info);
}
#endif

void CubismShader_CC::GenerateShaders()
{
	for (csmInt32 i = 0; i < ShaderCount; i++)
	{
		_shaderSets.PushBack(CSM_NEW CubismShaderSet());
	}

#if CSM_RENDERER_EXT
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

#ifndef CC_USE_GFX
	_shaderSets[0]->ShaderProgram = LoadShaderProgram(VertShaderSrcSetupMask, FragShaderSrcSetupMask);

	_shaderSets[1]->ShaderProgram = LoadShaderProgram(VertShaderSrc, FragShaderSrc);
	_shaderSets[2]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMask);
	_shaderSets[3]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskInverted);
	_shaderSets[4]->ShaderProgram = LoadShaderProgram(VertShaderSrc, FragShaderSrcPremultipliedAlpha);
	_shaderSets[5]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskPremultipliedAlpha);
	_shaderSets[6]->ShaderProgram = LoadShaderProgram(VertShaderSrcMasked, FragShaderSrcMaskInvertedPremultipliedAlpha);
#else
	_shaderSets[0]->ShaderProgram = LoadGFXProgram(
		"CSM_Shader0", 
		VertShaderSrcSetupMask, FragShaderSrcSetupMask,
		VertShaderSrcSetupMaskBlock, FragShaderSrcSetupMaskBlock,
		FragShaderSrcSetupMaskTextures);
	_shaderSets[1]->ShaderProgram = LoadGFXProgram(
		"CSM_Shader1",
		VertShaderSrc, FragShaderSrc,
		VertShaderSrcBlock, FragShaderSrcBlock,
		FragShaderSrcTextures);
	_shaderSets[2]->ShaderProgram = LoadGFXProgram(
		"CSM_Shader2",
		VertShaderSrcMasked, FragShaderSrcMask,
		VertShaderSrcMaskedBlock, FragShaderSrcMaskBlock,
		FragShaderSrcMaskTextures);
	_shaderSets[3]->ShaderProgram = LoadGFXProgram(
		"CSM_Shader3",
		VertShaderSrcMasked, FragShaderSrcMaskInverted,
		VertShaderSrcMaskedBlock, FragShaderSrcMaskBlock,
		FragShaderSrcMaskTextures);
	_shaderSets[4]->ShaderProgram = LoadGFXProgram(
		"CSM_Shader4",
		VertShaderSrc, FragShaderSrcPremultipliedAlpha,
		VertShaderSrcBlock, FragShaderSrcBlock,
		FragShaderSrcTextures);
	_shaderSets[5]->ShaderProgram = LoadGFXProgram(
		"CSM_Shader5",
		VertShaderSrcMasked, FragShaderSrcMaskPremultipliedAlpha,
		VertShaderSrcMaskedBlock, FragShaderSrcMaskBlock,
		FragShaderSrcMaskTextures);
	_shaderSets[6]->ShaderProgram = LoadGFXProgram(
		"CSM_Shader6",
		VertShaderSrcMasked, FragShaderSrcMaskInvertedPremultipliedAlpha,
		VertShaderSrcMaskedBlock, FragShaderSrcMaskBlock,
		FragShaderSrcMaskTextures);
#endif

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

	for (int i = 0; i <= 18; ++i)
	{
		_shaderSets[i]->AttributePositionLocation = _shaderSets[i]->ShaderProgram->getAttributeLocation("a_position");
		_shaderSets[i]->AttributeTexCoordLocation = _shaderSets[i]->ShaderProgram->getAttributeLocation("a_texCoord");
	}

	auto setLoaction = [&](int i, const std::set<std::string>& uniforms)
	{
		const auto ss = _shaderSets[i];
		const auto progrgam = ss->ShaderProgram;
		if (uniforms.count("s_texture0"))
			ss->SamplerTexture0Location = progrgam->getUniformLocation("s_texture0");
		if (uniforms.count("s_texture1"))
			ss->SamplerTexture1Location = progrgam->getUniformLocation("s_texture1");
		if (uniforms.count("u_matrix"))
			ss->UniformMatrixLocation = progrgam->getUniformLocation("u_matrix");
		if (uniforms.count("u_clipMatrix"))
			ss->UniformClipMatrixLocation = progrgam->getUniformLocation("u_clipMatrix");
		if (uniforms.count("u_baseColor"))
			ss->UniformBaseColorLocation = progrgam->getUniformLocation("u_baseColor");
		if (uniforms.count("u_channelFlag"))
			ss->UnifromChannelFlagLocation = progrgam->getUniformLocation("u_channelFlag");
		//NOTE: _shaderSets[0] dose not have these
		ss->UniformMultiplyColorLocation = progrgam->getUniformLocation("u_multiplyColor");
		ss->UniformScreenColorLocation = progrgam->getUniformLocation("u_screenColor");
	};

	// SetupMask
	setLoaction(0, {"s_texture0","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 通常
	setLoaction(1, { "s_texture0","u_matrix","u_baseColor" });

	// 通常（クリッピング）
	setLoaction(2, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 通常（クリッピング・反転）
	setLoaction(3, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 通常（PremultipliedAlpha）
	setLoaction(4, { "s_texture0","u_matrix","u_baseColor" });

	// 通常（クリッピング、PremultipliedAlpha）
	setLoaction(5, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 通常（クリッピング・反転、PremultipliedAlpha）
	setLoaction(6, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 加算
	setLoaction(7, { "s_texture0","u_matrix","u_baseColor" });

	// 加算（クリッピング）
	setLoaction(8, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 加算（クリッピング・反転）
	setLoaction(9, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 加算（PremultipliedAlpha）
	setLoaction(10, { "s_texture0","u_matrix","u_baseColor" });

	// 加算（クリッピング、PremultipliedAlpha）
	setLoaction(11, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 加算（クリッピング・反転、PremultipliedAlpha）
	setLoaction(12, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 乗算
	setLoaction(13, { "s_texture0","u_matrix","u_baseColor" });

	// 乗算（クリッピング）
	setLoaction(14, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 乗算（クリッピング・反転）
	setLoaction(15, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 乗算（PremultipliedAlpha）
	setLoaction(16, { "s_texture0","u_matrix","u_baseColor" });

	// 乗算（クリッピング、PremultipliedAlpha）
	setLoaction(17, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });

	// 乗算（クリッピング・反転、PremultipliedAlpha）
	setLoaction(18, { "s_texture0","s_texture1","u_matrix","u_clipMatrix","u_channelFlag","u_baseColor" });
}

void CubismShader_CC::SetupShaderProgram(CubismRenderer_CC* renderer
	, PipelineDescriptor* desc
	, Texture2D* texture
	, csmInt32 vertexCount, csmFloat32* vertexArray
	, csmFloat32* uvArray, csmFloat32 opacity
	, CubismRenderer::CubismBlendMode colorBlendMode
	, const CubismRenderer::CubismTextureColor& baseColor
	, CubismRenderer::CubismTextureColor multiplyColor
	, CubismRenderer::CubismTextureColor screenColor
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
	ProgramState* state = desc->programState;

	if (renderer->GetClippingContextBufferForMask() != nullptr) // マスク生成時
	{
		auto shaderSet = _shaderSets[ShaderNames_SetupMask];
		if (!state)
			state = new ProgramState(shaderSet->ShaderProgram);

		//テクスチャ設定
		state->setTexture(shaderSet->SamplerTexture0Location, 0, texture->getBackendTexture());

#ifndef CC_VERSION
		// 頂点配列の設定
		state->getVertexLayout()->setAttribute(AttributePositionName, shaderSet->AttributePositionLocation,
			VertexFormat::FLOAT2, 0, false);
		// テクスチャ頂点の設定
		state->getVertexLayout()->setAttribute(AttributeTexCoordName, shaderSet->AttributeTexCoordLocation,
			VertexFormat::FLOAT2, sizeof(float) * 2, false);
#endif // !CC_VERSION

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

		if (shaderSet->UniformMultiplyColorLocation)
		{
			const Vec4 multiply = { multiplyColor.R, multiplyColor.G, multiplyColor.B, multiplyColor.A };
			state->setUniform(shaderSet->UniformMultiplyColorLocation, &multiply, sizeof(float) * 4);
		}

		if (shaderSet->UniformScreenColorLocation)
		{
			const Vec4 screen = { screenColor.R, screenColor.G, screenColor.B, screenColor.A };
			state->setUniform(shaderSet->UniformScreenColorLocation, &screen, sizeof(float) * 4);
		}

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

		if (!state)
			state = new ProgramState(shaderSet->ShaderProgram);

#ifndef CC_VERSION
		// 頂点配列の設定
		state->getVertexLayout()->setAttribute(AttributePositionName, shaderSet->AttributePositionLocation,
			VertexFormat::FLOAT2, 0, false);
		// テクスチャ頂点の設定
		state->getVertexLayout()->setAttribute(AttributeTexCoordName, shaderSet->AttributeTexCoordLocation,
			VertexFormat::FLOAT2, sizeof(float) * 2, false);
#endif // !CC_VERSION

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

		const Vec4 vec4 = { baseColor.R, baseColor.G, baseColor.B, baseColor.A };
		state->setUniform(shaderSet->UniformBaseColorLocation,
			&vec4, sizeof(float) * 4);

		if (shaderSet->UniformMultiplyColorLocation)
		{
			const Vec4 multiply = { multiplyColor.R, multiplyColor.G, multiplyColor.B, multiplyColor.A };
			state->setUniform(shaderSet->UniformMultiplyColorLocation, &multiply, sizeof(float) * 4);
		}

		if (shaderSet->UniformScreenColorLocation)
		{
			const Vec4 screen = { screenColor.R, screenColor.G, screenColor.B, screenColor.A };
			state->setUniform(shaderSet->UniformScreenColorLocation, &screen, sizeof(float) * 4);
		}
	}
#ifndef CC_VERSION
	state->getVertexLayout()->setLayout(sizeof(csmFloat32) * 4);
#endif // !CC_VERSION
	desc->programState = state;
	auto& blend = desc->blendDescriptor;
	blend.blendEnabled = true;
	blend.sourceRGBBlendFactor = SRC_COLOR;
	blend.destinationRGBBlendFactor = DST_COLOR;
	blend.sourceAlphaBlendFactor = SRC_ALPHA;
	blend.destinationAlphaBlendFactor = DST_ALPHA;
}

backend::Program* CubismShader_CC::LoadShaderProgram(const std::string& vertShaderSrc, const std::string& fragShaderSrc)
{
	return backend::Device::getInstance()->newProgram(vertShaderSrc, fragShaderSrc);
}

/*********************************************************************************************************************
 *                                      CubismRenderer_CC
 ********************************************************************************************************************/

#if CSM_RENDERER_EXT
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
#ifdef CSM_DEBUG
	// make sure default texture is created
	Sprite::create();
#endif
}

CubismRenderer_CC::~CubismRenderer_CC()
{
	CSM_DELETE_SELF(CubismClippingManager_CC, _clippingManager);

	for (csmInt32 i = 0; i < _drawableDrawCommandBuffer.GetSize(); ++i)
	{
		CSM_DELETE(_drawableDrawCommandBuffer[i]);
	}
	_drawableDrawCommandBuffer.Clear();
	_textures.clear();
	if (_offscreenFrameBuffer.IsValid())
	{
		_offscreenFrameBuffer.DestroyOffscreenFrame();
	}
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

		const auto mbSize = _clippingManager->GetClippingMaskBufferSize();
		_offscreenFrameBuffer.CreateOffscreenFrame(mbSize.X, mbSize.Y);
	}

	_sortedDrawableIndexList.Resize(model->GetDrawableCount(), 0);

	_drawableDrawCommandBuffer.Resize(model->GetDrawableCount());
	for (csmInt32 i = 0; i < _drawableDrawCommandBuffer.GetSize(); ++i)
	{
		const csmInt32 drawableVertexCount = model->GetDrawableVertexCount(i);
		const csmInt32 drawableVertexIndexCount = model->GetDrawableVertexIndexCount(i);
		_drawableDrawCommandBuffer[i] = CSM_NEW CubismDrawCommand_CC();
		_drawableDrawCommandBuffer[i]->CreateVertexBuffer(sizeof(csmFloat32) * 4, drawableVertexCount);      // Vertices + UVs
		if (drawableVertexIndexCount > 0)
		{
			_drawableDrawCommandBuffer[i]->CreateIndexBuffer(drawableVertexIndexCount);
		}
	}

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

		const auto mbSize = _clippingManager->GetClippingMaskBufferSize();
		// サイズが違う場合はここで作成しなおし
		if (_offscreenFrameBuffer.GetBufferWidth() != static_cast<csmUint32>(mbSize.X) ||
			_offscreenFrameBuffer.GetBufferHeight() != static_cast<csmUint32>(mbSize.Y))
		{
			_offscreenFrameBuffer.DestroyOffscreenFrame();
			_offscreenFrameBuffer.CreateOffscreenFrame(
				static_cast<csmUint32>(mbSize.X),
				static_cast<csmUint32>(mbSize.Y));
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
				//const auto mbSize = _clippingManager->GetClippingMaskBufferSize();
				const auto mbSize = _offscreenFrameBuffer.GetViewPortSize();
				// 生成したFrameBufferと同じサイズでビューポートを設定
				//SetViewPort(0, 0, mbSize.X, mbSize.Y);
				SetViewPort(0, 0, mbSize.width, mbSize.height);

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

					if (GetModel()->GetDrawableVertexCount(clipDrawIndex) <= 0)
					{
						continue;
					}

					// 今回専用の変換を適用して描く
					// チャンネルも切り替える必要がある(A,R,G,B)
					SetClippingContextBufferForMask(clipContext);
					DrawMeshCC(clipContext->_clippingCommandBufferList->At(index),
						GetModel()->GetDrawableTextureIndex(clipDrawIndex),
						GetModel()->GetDrawableVertexIndexCount(clipDrawIndex),
						GetModel()->GetDrawableVertexCount(clipDrawIndex),
						const_cast<csmUint16*>(GetModel()->GetDrawableVertexIndices(clipDrawIndex)),
						const_cast<csmFloat32*>(GetModel()->GetDrawableVertices(clipDrawIndex)),
						reinterpret_cast<csmFloat32*>(const_cast<Core::csmVector2*>(GetModel()->GetDrawableVertexUvs(clipDrawIndex))),
						GetModel()->GetMultiplyColor(clipDrawIndex),
						GetModel()->GetScreenColor(clipDrawIndex),
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

		if (GetModel()->GetDrawableVertexIndexCount(drawableIndex) <= 0)
		{
			continue;
		}

		DrawMeshCC(_drawableDrawCommandBuffer.At(drawableIndex),
			GetModel()->GetDrawableTextureIndex(drawableIndex),
			GetModel()->GetDrawableVertexIndexCount(drawableIndex),
			GetModel()->GetDrawableVertexCount(drawableIndex),
			const_cast<csmUint16*>(GetModel()->GetDrawableVertexIndices(drawableIndex)),
			const_cast<csmFloat32*>(GetModel()->GetDrawableVertices(drawableIndex)),
			reinterpret_cast<csmFloat32*>(const_cast<Core::csmVector2*>(GetModel()->GetDrawableVertexUvs(drawableIndex))),
			GetModel()->GetMultiplyColor(drawableIndex),
			GetModel()->GetScreenColor(drawableIndex),
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
	assert(false);
}

void CubismRenderer_CC::DrawMeshCC(CubismDrawCommand_CC* command, csmInt32 textureNo, csmInt32 indexCount, csmInt32 vertexCount
	, csmUint16* indexArray, csmFloat32* vertexArray, csmFloat32* uvArray
	, const CubismTextureColor& multiplyColor, const CubismTextureColor& screenColor
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
#ifdef CSM_DEBUG
	// テクスチャマップからバインド済みテクスチャIDを取得
	// バインドされていなければダミーのテクスチャIDをセットする
	if (it != _textures.end())
		drawTexture = it->second;
	else
		drawTexture = Director::getInstance()->getTextureCache()->getTextureForKey("/cc_2x2_white_image");
#else
	drawTexture = it->second;
#endif

	auto cmd = command->GetCommand();
	command->UpdateIndexBuffer(indexArray, indexCount);
	command->UpdateVertexBuffer(vertexArray, uvArray, vertexCount);

	CubismShader_CC::GetInstance()->SetupShaderProgram(
		this, &cmd->getPipelineDescriptor(), drawTexture, vertexCount, vertexArray, uvArray
		, opacity, colorBlendMode, modelColorRGBA, multiplyColor, screenColor, IsPremultipliedAlpha()
		, _mvpMatrix, invertedMask
	);

	// ポリゴンメッシュを描画する
	ccr->addCommand(cmd);

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

void CubismRenderer_CC::SetClippingMaskBufferSize(csmFloat32 width, csmFloat32 height)
{
	//FrameBufferのサイズを変更するためにインスタンスを破棄・再作成する
	CSM_DELETE_SELF(CubismClippingManager_CC, _clippingManager);

	_clippingManager = CSM_NEW CubismClippingManager_CC();

	_clippingManager->SetClippingMaskBufferSize(width, height);

	_clippingManager->Initialize(
		*GetModel(),
		GetModel()->GetDrawableCount(),
		GetModel()->GetDrawableMasks(),
		GetModel()->GetDrawableMaskCounts()
	);
}

CubismVector2 CubismRenderer_CC::GetClippingMaskBufferSize() const
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
#ifdef CC_VERSION
	auto cmd = ccr->nextCallbackCommand();
	cmd->func = callback;
	ccr->addCommand(cmd);
#else
	auto cmd = std::make_shared<CallbackCommand>();
	cmds.push_back(cmd);
	cmd->init(0.f);
	cmd->func = callback;
	ccr->addCommand(cmd.get());
#endif // CC_VERSION
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
