﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

//Cubism Framework
#include "LAppDefine.hpp"
#include "LAppModel.hpp"
#include "LAppPal.hpp"
#include <CubismModelSettingJson.hpp>
#include <Motion/CubismMotion.hpp>
#include <Physics/CubismPhysics.hpp>
#include <CubismDefaultParameterId.hpp>
#include <Utils/CubismString.hpp>
#include <Id/CubismIdManager.hpp>
#include "Motion/CubismMotionQueueEntry.hpp"
#include "CubismRenderer_CC.h"

//cocos2d
#include "base/CCDirector.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCTextureCache.h"

using namespace std;
using namespace Csm;
using namespace Csm::Constant;
using namespace Csm::DefaultParameterId;
using namespace LAppDefine;
using namespace cocos2d;

//TODO: audio

namespace {
	csmByte* CreateBuffer(const csmChar* path, csmSizeInt* size)
	{
		if (DebugLogEnable)LAppPal::PrintLog("[L2D] create buffer: %s ", path);
		return LAppPal::LoadFileAsBytes(path, size);
	}

	void DeleteBuffer(csmByte* buffer, const csmChar* path = "")
	{
		if (DebugLogEnable) LAppPal::PrintLog("[L2D] delete buffer: %s", path);
		LAppPal::ReleaseBytes(buffer);
	}
}

LAppModel::LAppModel()
	: _modelSetting(nullptr), _userTimeSeconds(0.0f), _renderSprite(nullptr), _lipValue(0.0f)
{
	if (DebugLogEnable)
	{
		_debugMode = true;
	}

	_idParamAngleX = CubismFramework::GetIdManager()->GetId(ParamAngleX);
	_idParamAngleY = CubismFramework::GetIdManager()->GetId(ParamAngleY);
	_idParamAngleZ = CubismFramework::GetIdManager()->GetId(ParamAngleZ);
	_idParamBodyAngleX = CubismFramework::GetIdManager()->GetId(ParamBodyAngleX);
	_idParamEyeBallX = CubismFramework::GetIdManager()->GetId(ParamEyeBallX);
	_idParamEyeBallY = CubismFramework::GetIdManager()->GetId(ParamEyeBallY);

	_clearColor[0] = 1.0f;
	_clearColor[1] = 1.0f;
	_clearColor[2] = 1.0f;
	_clearColor[3] = 0.0f;
}

LAppModel::~LAppModel()
{
	if (_debugMode)
	{
		if(_modelSetting)
			LAppPal::PrintLog("[L2D] delete model: %s", _modelSetting->GetModelFileName());
		else
			LAppPal::PrintLog("[L2D] delete model");
	}
	// _renderSprite is owned by _renderBuffer
    _renderBuffer.DestroyOffscreenFrame();

    ReleaseMotions();
    ReleaseExpressions();

	if (_modelSetting)
	{
	    for (csmInt32 i = 0; i < _modelSetting->GetMotionGroupCount(); i++)
	    {
	        const csmChar* group = _modelSetting->GetMotionGroupName(i);
	        ReleaseMotionGroup(group);
	    }
	    CSM_DELETE(_modelSetting);
	}

    // clean
	_loadedTextures.clear();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool LAppModel::LoadAssets(const csmChar* dir, const csmChar* fileName)
{
    _modelHomeDir = dir;

    if (_debugMode)LAppPal::PrintLog("[L2D] load model setting: %s", fileName);

    csmSizeInt size;
    const csmString path = csmString(dir) + fileName;

    csmByte* buffer = CreateBuffer(path.GetRawString(), &size);
	if(!buffer)
	{
		if (_debugMode)LAppPal::PrintLog("[L2D] failed to load model setting: %s", fileName);
		return false;
	}
    auto setting = new CubismModelSettingJson(buffer, size);
    DeleteBuffer(buffer, path.GetRawString());

	if (!setting->GetJsonPointer()) return false;

    SetupModel(setting);

    CreateRenderer();

    SetupTextures();
	return true;
}


void LAppModel::SetupModel(ICubismModelSetting* setting)
{
    _updating = true;
    _initialized = false;

    _modelSetting = setting;

    csmByte* buffer;
    csmSizeInt size;
    //Cubism Model
    if (strcmp(_modelSetting->GetModelFileName(), "") != 0)
    {
        csmString path = _modelSetting->GetModelFileName();
        path = _modelHomeDir + path;

        if (_debugMode)LAppPal::PrintLog("[L2D] create model: %s", setting->GetModelFileName());

        buffer = CreateBuffer(path.GetRawString(), &size);
		if (buffer)
		{
			_mocVersion = Live2D::Cubism::Core::csmGetMocVersion(buffer, size);
	        LoadModel(buffer, size);
	        DeleteBuffer(buffer, path.GetRawString());
		}
		else
		{
			if (_debugMode)LAppPal::PrintLog("[L2D] failed to load model at %s", path.GetRawString());			
		}
    }

    //Expression
    if (_modelSetting->GetExpressionCount() > 0)
    {
        const csmInt32 count = _modelSetting->GetExpressionCount();
        for (csmInt32 i = 0; i < count; i++)
        {
            csmString name = _modelSetting->GetExpressionName(i);
            csmString path = _modelSetting->GetExpressionFileName(i);
            path = _modelHomeDir + path;

            buffer = CreateBuffer(path.GetRawString(), &size);
            if (buffer)
            {
	            ACubismMotion* motion = LoadExpression(buffer, size, name.GetRawString());

	            if (_expressions[name] != nullptr)
	            {
	                ACubismMotion::Delete(_expressions[name]);
	                _expressions[name] = nullptr;
	            }
	            _expressions[name] = motion;

	            DeleteBuffer(buffer, path.GetRawString());
            }
            else
            {
				if (_debugMode)LAppPal::PrintLog("[L2D] failed to load expression at %s", path.GetRawString());
            }
        }
    }

    //Physics
    if (strcmp(_modelSetting->GetPhysicsFileName(), "") != 0)
    {
        csmString path = _modelSetting->GetPhysicsFileName();
        path = _modelHomeDir + path;

        buffer = CreateBuffer(path.GetRawString(), &size);
        if (buffer)
        {
			LoadPhysics(buffer, size);
			DeleteBuffer(buffer, path.GetRawString());
        }
        else
        {
			if (_debugMode)LAppPal::PrintLog("[L2D] failed to load physics at %s", path.GetRawString());
        }
    }

    //Pose
    if (strcmp(_modelSetting->GetPoseFileName(), "") != 0)
    {
        csmString path = _modelSetting->GetPoseFileName();
        path = _modelHomeDir + path;

        buffer = CreateBuffer(path.GetRawString(), &size);
		if (buffer)
		{
			LoadPose(buffer, size);
	        DeleteBuffer(buffer, path.GetRawString());
		}
		else
		{
			if (_debugMode)LAppPal::PrintLog("[L2D] failed to load pose at %s", path.GetRawString());
		}
    }

    //EyeBlink
    if (_modelSetting->GetEyeBlinkParameterCount() > 0)
    {
        _eyeBlink = CubismEyeBlink::Create(_modelSetting);
    }

    //Breath
    {
        _breath = CubismBreath::Create();

        csmVector<CubismBreath::BreathParameterData> breathParameters;

        breathParameters.PushBack(CubismBreath::BreathParameterData(_idParamAngleX,     0.0f, 15.0f,    6.5345f,    0.5f));
        breathParameters.PushBack(CubismBreath::BreathParameterData(_idParamAngleY,     0.0f, 8.0f,     3.5345f,    0.5f));
        breathParameters.PushBack(CubismBreath::BreathParameterData(_idParamAngleZ,     0.0f, 10.0f,    5.5345f,    0.5f));
        breathParameters.PushBack(CubismBreath::BreathParameterData(_idParamBodyAngleX, 0.0f, 4.0f,     15.5345f,   0.5f));
        breathParameters.PushBack(CubismBreath::BreathParameterData(
			CubismFramework::GetIdManager()->GetId(ParamBreath),     0.5f, 0.5f,     3.2345f,    0.5f));

        _breath->SetParameters(breathParameters);
    }

    //UserData
    if (strcmp(_modelSetting->GetUserDataFile(), "") != 0)
    {
        csmString path = _modelSetting->GetUserDataFile();
        path = _modelHomeDir + path;
        buffer = CreateBuffer(path.GetRawString(), &size);
		if (buffer)
		{
			LoadUserData(buffer,size);
			DeleteBuffer(buffer, path.GetRawString());
		}
		else
		{
			if (_debugMode)LAppPal::PrintLog("[L2D] failed to load user data at %s", path.GetRawString());
		}
    }

    // EyeBlinkIds
    {
        csmInt32 eyeBlinkIdCount = _modelSetting->GetEyeBlinkParameterCount();
        for (csmInt32 i = 0; i < eyeBlinkIdCount; ++i)
        {
            _eyeBlinkIds.PushBack(_modelSetting->GetEyeBlinkParameterId(i));
        }
    }

    // LipSyncIds
    {
        csmInt32 lipSyncIdCount = _modelSetting->GetLipSyncParameterCount();
        for (csmInt32 i = 0; i < lipSyncIdCount; ++i)
        {
            _lipSyncIds.PushBack(_modelSetting->GetLipSyncParameterId(i));
        }
    }

    //Layout
    csmMap<csmString, csmFloat32> layout;
    _modelSetting->GetLayoutMap(layout);
    _modelMatrix->SetupFromLayout(layout);

    _model->SaveParameters();

    for (csmInt32 i = 0; i < _modelSetting->GetMotionGroupCount(); i++)
    {
        const csmChar* group = _modelSetting->GetMotionGroupName(i);
        PreloadMotionGroup(group);
    }

    _motionManager->StopAllMotions();

    _updating = false;
    _initialized = true;
}

void LAppModel::PreloadMotionGroup(const csmChar* group)
{
    const csmInt32 count = _modelSetting->GetMotionCount(group);

    for (csmInt32 i = 0; i < count; i++)
    {
        //ex) idle_0
        csmString name = Utils::CubismString::GetFormatedString("%s_%d", group, i);
        csmString path = _modelSetting->GetMotionFileName(group, i);
        path = _modelHomeDir + path;

        if (_debugMode)LAppPal::PrintLog("[L2D] load motion: %s => [%s_%d] ", path.GetRawString(), group, i);

        csmByte* buffer;
        csmSizeInt size;
        buffer = CreateBuffer(path.GetRawString(), &size);
        if (!buffer)
        {
			if (_debugMode)LAppPal::PrintLog("[L2D] failed to load motion at %s", path.GetRawString());
			continue;
        }
        CubismMotion* tmpMotion = static_cast<CubismMotion*>(LoadMotion(buffer, size, name.GetRawString()));

        csmFloat32 fadeTime = _modelSetting->GetMotionFadeInTimeValue(group, i);
        if (fadeTime >= 0.0f)
        {
            tmpMotion->SetFadeInTime(fadeTime);
        }

        fadeTime = _modelSetting->GetMotionFadeOutTimeValue(group, i);
        if (fadeTime >= 0.0f)
        {
            tmpMotion->SetFadeOutTime(fadeTime);
        }
        tmpMotion->SetEffectIds(_eyeBlinkIds, _lipSyncIds);

        if (_motions[name] != nullptr)
        {
            ACubismMotion::Delete(_motions[name]);
        }
        _motions[name] = tmpMotion;

        DeleteBuffer(buffer, path.GetRawString());

        std::string voice = _modelSetting->GetMotionSoundFileName(group, i);
        if (!voice.empty())
        {
            if (_soundHandler)
                _soundHandler(voice, SoundEventType::Load);
        }
    }
}

void LAppModel::ReleaseMotionGroup(const csmChar* group) const
{
    const csmInt32 count = _modelSetting->GetMotionCount(group);
    for (csmInt32 i = 0; i < count; i++)
    {
        std::string voice = _modelSetting->GetMotionSoundFileName(group, i);
        if (!voice.empty())
        {
            if (_soundHandler)
                _soundHandler(voice, SoundEventType::Unload);
        }
    }
}

/**
* @brief すべてのモーションデータの解放
*
* すべてのモーションデータを解放する。
*/
void LAppModel::ReleaseMotions()
{
    for (auto iter = _motions.Begin(); iter != _motions.End(); ++iter)
    {
        ACubismMotion::Delete(iter->Second);
    }

    _motions.Clear();
}

/**
* @brief すべての表情データの解放
*
* すべての表情データを解放する。
*/
void LAppModel::ReleaseExpressions()
{
    for (auto iter = _expressions.Begin(); iter != _expressions.End(); ++iter)
    {
        ACubismMotion::Delete(iter->Second);
    }

    _expressions.Clear();
}

void LAppModel::Update()
{
    const csmFloat32 deltaTimeSeconds = LAppPal::GetDeltaTime();
    _userTimeSeconds += deltaTimeSeconds;

    _dragManager->Update(deltaTimeSeconds);
    _dragX = _dragManager->GetX();
    _dragY = _dragManager->GetY();

    // モーションによるパラメータ更新の有無
    csmBool motionUpdated = false;

    //-----------------------------------------------------------------
    _model->LoadParameters(); // 前回セーブされた状態をロード
    if (_motionManager->IsFinished())
    {
        // モーションの再生がない場合、待機モーションの中からランダムで再生する
        StartRandomMotion(MotionGroupIdle, PriorityIdle);
    }
    else
    {
        motionUpdated = _motionManager->UpdateMotion(_model, deltaTimeSeconds); // モーションを更新
    }
    _model->SaveParameters(); // 状態を保存
    //-----------------------------------------------------------------

    // まばたき
    if (!motionUpdated)
    {
        if (_eyeBlink != nullptr)
        {
            // メインモーションの更新がないとき
            _eyeBlink->UpdateParameters(_model, deltaTimeSeconds); // 目パチ
        }
    }

    if (_expressionManager != nullptr)
    {
        _expressionManager->UpdateMotion(_model, deltaTimeSeconds); // 表情でパラメータ更新（相対変化）
    }

    //ドラッグによる変化
    //ドラッグによる顔の向きの調整
    _model->AddParameterValue(_idParamAngleX, _dragX * 30); // -30から30の値を加える
    _model->AddParameterValue(_idParamAngleY, _dragY * 30);
    _model->AddParameterValue(_idParamAngleZ, _dragX * _dragY * -30);

    //ドラッグによる体の向きの調整
    _model->AddParameterValue(_idParamBodyAngleX, _dragX * 10); // -10から10の値を加える

    //ドラッグによる目の向きの調整
    _model->AddParameterValue(_idParamEyeBallX, _dragX); // -1から1の値を加える
    _model->AddParameterValue(_idParamEyeBallY, _dragY);

    // 呼吸など
    if (_breath != nullptr)
    {
        _breath->UpdateParameters(_model, deltaTimeSeconds);
    }

    // 物理演算の設定
    if (_physics != nullptr)
    {
        _physics->Evaluate(_model, deltaTimeSeconds);
    }

    // リップシンクの設定
    if (_lipSync)
    {
        csmFloat32 value = _lipValue; // リアルタイムでリップシンクを行う場合、システムから音量を取得して0～1の範囲で値を入力します。

        for (csmUint32 i = 0; i < _lipSyncIds.GetSize(); ++i)
        {
            _model->AddParameterValue(_lipSyncIds[i], value, 0.8f);
        }
    }

    // ポーズの設定
    if (_pose != nullptr)
    {
        _pose->UpdateParameters(_model, deltaTimeSeconds);
    }

    _model->Update();

}

CubismMotionQueueEntryHandle LAppModel::StartMotion(const csmChar* group, csmInt32 no, csmInt32 priority, ACubismMotion::FinishedMotionCallback onFinishedMotionHandler)
{
    if (priority == PriorityForce)
    {
        _motionManager->SetReservePriority(priority);
    }
    else if (!_motionManager->ReserveMotion(priority))
    {
        if (_debugMode) LAppPal::PrintLog("[L2D] can't start motion.");
        return InvalidMotionQueueEntryHandleValue;
    }

    const csmString fileName = _modelSetting->GetMotionFileName(group, no);
	if (fileName.GetLength() == 0)
	{
		if (_debugMode)LAppPal::PrintLog("[L2D] can't find motion file of [%s_%d]", group, no);
		return InvalidMotionQueueEntryHandleValue;
	}

    //ex) idle_0
    csmString name = Utils::CubismString::GetFormatedString("%s_%d", group, no);
    CubismMotion* motion = static_cast<CubismMotion*>(_motions[name.GetRawString()]);
    csmBool autoDelete = false;

    if (motion == nullptr)
    {
        csmString path = fileName;
        path = _modelHomeDir + path;

        csmByte* buffer;
        csmSizeInt size;
        buffer = CreateBuffer(path.GetRawString(), &size);
		if(!buffer)
		{
			if (_debugMode)LAppPal::PrintLog("[L2D] can't create buffer for motion [%s_%d]", group, no);
			return InvalidMotionQueueEntryHandleValue;
		}
        motion = static_cast<CubismMotion*>(LoadMotion(buffer, size, nullptr, onFinishedMotionHandler));
        csmFloat32 fadeTime = _modelSetting->GetMotionFadeInTimeValue(group, no);
        if (fadeTime >= 0.0f)
        {
            motion->SetFadeInTime(fadeTime);
        }

        fadeTime = _modelSetting->GetMotionFadeOutTimeValue(group, no);
        if (fadeTime >= 0.0f)
        {
            motion->SetFadeOutTime(fadeTime);
        }
        motion->SetEffectIds(_eyeBlinkIds, _lipSyncIds);
        autoDelete = true; // 終了時にメモリから削除

        DeleteBuffer(buffer, path.GetRawString());
    }
    else
    {
        motion->SetFinishedMotionHandler(onFinishedMotionHandler);
    }

    //voice
    const std::string voice = _modelSetting->GetMotionSoundFileName(group, no);
    if (!voice.empty())
    {
        if (_soundHandler)
            _soundHandler(voice, SoundEventType::Play);
    }

    if (_debugMode)LAppPal::PrintLog("[L2D] start motion: [%s_%d]", group, no);
    return  _motionManager->StartMotionPriority(motion, autoDelete, priority);
}

CubismMotionQueueEntryHandle LAppModel::StartRandomMotion(const csmChar* group, csmInt32 priority, ACubismMotion::FinishedMotionCallback onFinishedMotionHandler)
{
    if (_modelSetting->GetMotionCount(group) == 0)
    {
        return InvalidMotionQueueEntryHandleValue;
    }

    csmInt32 no = rand() % _modelSetting->GetMotionCount(group);

    return StartMotion(group, no, priority, onFinishedMotionHandler);
}

void LAppModel::DoDraw()
{
    if (_model == nullptr)return;

    GetRenderer<Rendering::CubismRenderer>()->DrawModel();
}

void LAppModel::Draw(CubismMatrix44& matrix)
{
    if (_model == nullptr)return;

    if (_renderBuffer.IsValid())
    {
        _renderBuffer.BeginDraw();
        _renderBuffer.Clear(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
    }

    matrix.MultiplyByMatrix(_modelMatrix);

    GetRenderer<Rendering::CubismRenderer>()->SetMvpMatrix(&matrix);

    DoDraw();

    if (_renderBuffer.IsValid())
    {
        _renderBuffer.EndDraw();
    }
}

csmBool LAppModel::HitTest(const csmChar* hitAreaName, csmFloat32 x, csmFloat32 y)
{
    if (_opacity < 1)return false; // 透明時は当たり判定なし。
    const csmInt32 count = _modelSetting->GetHitAreasCount();
    for (csmInt32 i = 0; i < count; i++)
    {
        if (strcmp(_modelSetting->GetHitAreaName(i), hitAreaName) == 0)
        {
            const CubismIdHandle drawID = _modelSetting->GetHitAreaId(i);
            return IsHit(drawID, x, y);
        }
    }
    return false; // 存在しない場合はfalse
}

csmBool LAppModel::SetExpression(const csmChar* expressionID)
{
    ACubismMotion* motion = _expressions[expressionID];
    if (_debugMode) LAppPal::PrintLog("[L2D] expression: [%s]", expressionID);
    if (motion != nullptr)
    {
	    const auto h = _expressionManager->StartMotionPriority(motion, false, PriorityForce);
		return h != InvalidMotionQueueEntryHandleValue;
    }
    else
    {
        if (_debugMode) LAppPal::PrintLog("[L2D] expression[%s] is null ", expressionID);
		return false;
    }
}

csmBool LAppModel::SetRandomExpression()
{
    if (_expressions.GetSize() == 0)
		return false;

    csmInt32 no = rand() % _expressions.GetSize();
	csmInt32 i = 0;
    for (auto map_ite = _expressions.Begin(); map_ite != _expressions.End(); ++map_ite)
    {
        if (i == no)
        {
            csmString name = (*map_ite).First;
            return SetExpression(name.GetRawString());
        }
        i++;
    }
	return false;
}

void LAppModel::ReloadRenderer()
{
    DeleteRenderer();

    CreateRenderer();

    SetupTextures();
}

void LAppModel::SetupTextures()
{
	_loadedTextures.clear();
    for (csmInt32 modelTextureNumber = 0; modelTextureNumber < _modelSetting->GetTextureCount(); modelTextureNumber++)
    {
        // テクスチャ名が空文字だった場合はロード・バインド処理をスキップ
        if (strcmp(_modelSetting->GetTextureFileName(modelTextureNumber), "") == 0) continue;

        //OpenGLのテクスチャユニットにテクスチャをロードする
        csmString texturePath = _modelSetting->GetTextureFileName(modelTextureNumber);
        texturePath = _modelHomeDir + texturePath;

        // Cocos2d-x
        // テクスチャをファイルからロードする. ロードできなければnullptrが返る
        Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(std::string(texturePath.GetRawString()));

        // テクスチャが読めていなければバインド処理をスキップ
        if(!texture) continue;

        const Texture2D::TexParams texParams = {
        	backend::SamplerFilter::LINEAR,
        	backend::SamplerFilter::LINEAR,
			backend::SamplerAddressMode::CLAMP_TO_EDGE,
			backend::SamplerAddressMode::CLAMP_TO_EDGE };
        texture->setTexParameters(texParams);
        texture->generateMipmap();
		_loadedTextures.pushBack(texture);

        GetRenderer<Rendering::CubismRenderer_CC>()->BindTexture(modelTextureNumber, texture);
    }
}

void LAppModel::MotionEventFired(const csmString& eventValue)
{
	if (_debugMode) LAppPal::PrintLog("%s is fired", eventValue.GetRawString());
}

const csmVector<csmRectF>& LAppModel::GetHitAreas(const Csm::CubismMatrix44& vpMatrix, const CubismVector2& windowSize)
{
    _hitArea.Clear();

    for (csmInt32 i = 0; i < _modelSetting->GetHitAreasCount(); ++i)
    {
        _hitArea.PushBack(GetDrawableArea(GetModel()->GetDrawableIndex(_modelSetting->GetHitAreaId(i)),vpMatrix, windowSize));
    }
    return _hitArea;
}

const csmVector<csmRectF>& LAppModel::GetUserDataAreas(const CubismMatrix44& vpMatrix, const CubismVector2& windowSize)
{
    _userArea.Clear();

    if ( _modelUserData == nullptr)
    {
        return    _userArea;
    }

    const auto& ans = _modelUserData->GetArtMeshUserDatas();

    for ( csmUint32 i = 0; i < ans.GetSize(); ++i)
    {
        _userArea.PushBack(GetDrawableArea(GetModel()->GetDrawableIndex(ans[i]->TargetId), vpMatrix, windowSize));
    }

    return _userArea;
}

csmRectF LAppModel::GetDrawableArea(csmInt32 drawableIndex,
	const CubismMatrix44& vpMatrix, const CubismVector2& windowSize) const
{
    CubismMatrix44 currentMatrix(vpMatrix);         //Matrixをコピーしないと変質が次回に影響する
    currentMatrix.MultiplyByMatrix(_modelMatrix);

    const csmInt32 count = _model->GetDrawableVertexCount(drawableIndex);
    const csmFloat32* vertices = _model->GetDrawableVertices(drawableIndex);

    csmFloat32 left = vertices[0];
    csmFloat32 right = vertices[0];
    csmFloat32 top = vertices[1];
    csmFloat32 bottom = vertices[1];

    for (csmInt32 j = 1; j < count; ++j)
    {
        CubismVector2 pos;

        pos.X = vertices[Constant::VertexOffset + j * Constant::VertexStep];
        pos.Y = vertices[Constant::VertexOffset + j * Constant::VertexStep + 1];

        if (pos.X < left)
        {
            left = pos.X; // Min x
        }

        if (pos.X > right)
        {
            right = pos.X; // Max x
        }

        if (pos.Y < top)
        {
            top = pos.Y; // Min y
        }

        if (pos.Y > bottom)
        {
            bottom = pos.Y; // Max y
        }
    }

    csmFloat32 convertLeft   = left  * currentMatrix.GetArray()[0] + top    * currentMatrix.GetArray()[1];
    convertLeft   = convertLeft   * windowSize.X / 2 + windowSize.X / 2;
    csmFloat32 convertTop    = left  * currentMatrix.GetArray()[4] + top    * currentMatrix.GetArray()[5];
    convertTop    = convertTop    * windowSize.Y / 2 + windowSize.Y / 2;
    csmFloat32 convertRight  = right * currentMatrix.GetArray()[0] + bottom * currentMatrix.GetArray()[1];
    convertRight  = convertRight  * windowSize.X / 2 + windowSize.X / 2;
    csmFloat32 convertBottom = right * currentMatrix.GetArray()[4] + bottom * currentMatrix.GetArray()[5];
    convertBottom = convertBottom * windowSize.Y / 2 + windowSize.Y / 2;

    return csmRectF(convertLeft, convertTop, (convertRight - convertLeft), (convertBottom - convertTop));
}

void LAppModel::MakeRenderingTarget()
{
    // RenderTexture::createは描画タイミングで呼ぶとAssert扱いになるので注意すること
    if (!_renderSprite && !_renderBuffer.IsValid())
    {
		const auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
		int frameW = frameSize.width;
    	int frameH = frameSize.height;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        // Retina対策でこっちからとる
        GLViewImpl *glimpl = (GLViewImpl *)Director::getInstance()->getOpenGLView();
        glfwGetFramebufferSize(glimpl->getWindow(), &frameW, &frameH);
#endif

		const auto visibleSize = Director::getInstance()->getVisibleSize();
		const auto origin = Director::getInstance()->getVisibleOrigin();

        // レンダリングバッファの描画先をそのテクスチャにする
        const auto success = _renderBuffer.CreateOffscreenFrame(frameW, frameH);
		if (success)
		{
			_renderSprite = _renderBuffer.GetOffscreenFrame();
			_renderSprite->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
			_renderSprite->getSprite()->getTexture()->setAntiAliasTexParameters();
			_renderSprite->getSprite()->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
			_renderSprite->getSprite()->setOpacityModifyRGB(false);
		}
    }
}

void LAppModel::SetSpriteColor(float r, float g, float b, float a)
{
    if (_renderSprite)
    {
        _renderSprite->getSprite()->setColor(Color3B(uint8_t(255.0f * r), uint8_t(255.0f * g), uint8_t(255.0f * b)));
        _renderSprite->getSprite()->setOpacity(uint8_t(255.0f * a));
    }
}
