/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */


#include "LAppLive2DManager.hpp"
#include <string>
#include <Rendering/D3D11/CubismRenderer_D3D11.hpp>
#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppDelegate.hpp"
#include "LAppModel.hpp"
#include "LAppView.hpp"

using namespace Csm;
using namespace LAppDefine;
using namespace std;

namespace {
    LAppLive2DManager* s_instance = NULL;

    void FinishedMotion(ACubismMotion* self)
    {
        LAppPal::PrintLog("Motion Finished: %x", self);
    }
}

LAppLive2DManager* LAppLive2DManager::GetInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new LAppLive2DManager();
    }

    return s_instance;
}

void LAppLive2DManager::ReleaseInstance()
{
    if (s_instance != NULL)
    {
        delete s_instance;
    }

    s_instance = NULL;
}

LAppLive2DManager::LAppLive2DManager()
    : _viewMatrix(NULL)
    , _sceneIndex(0)
{
    ChangeScene(_sceneIndex);
}

LAppLive2DManager::~LAppLive2DManager()
{
    ReleaseAllModel();
    // カウンターを待たず速攻破棄
    for (int i = _releaseModel.GetSize() - 1; i >= 0; i--)
    {
        if (_releaseModel[i]._model)
        {
            delete _releaseModel[i]._model;
        }
    }
    _releaseModel.Clear();

    CubismFramework::Dispose();
}

void LAppLive2DManager::ReleaseAllModel()
{
    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        //delete_models[i]；在这里不会消失

        // 计划删除的Mark
        _models[i]->DeleteMark();

        //2帧后删除
        ReleaseModel rel;
        rel._model = _models[i];
        rel._counter = 2;
        _releaseModel.PushBack(rel);
    }

    _models.Clear();
}

LAppModel* LAppLive2DManager::GetModel(csmUint32 no) const
{
    if (no < _models.GetSize())
    {
        return _models[no];
    }

    return NULL;
}

void LAppLive2DManager::OnDrag(csmFloat32 x, csmFloat32 y) const
{
    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        LAppModel* model = GetModel(i);

        model->SetDragging(x, y);
    }
}

void LAppLive2DManager::OnTap(csmFloat32 x, csmFloat32 y)
{
    if (DebugLogEnable)
    {
        LAppPal::PrintLog("[APP]tap point: {x:%.2f y:%.2f}", x, y);
    }

    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        if (_models[i]->HitTest(HitAreaNameHead, x, y))
        {
            if (DebugLogEnable)
            {
                LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameHead);
            }
            _models[i]->SetRandomExpression();
        }
        else if (_models[i]->HitTest(HitAreaNameBody, x, y))
        {
            if (DebugLogEnable)
            {
                LAppPal::PrintLog("[APP]hit area: [%s]", HitAreaNameBody);
            }
            _models[i]->StartRandomMotion(MotionGroupTapBody, PriorityNormal, FinishedMotion);
        }
    }
}

void LAppLive2DManager::OnUpdate() const
{
    int windowWidth, windowHeight;
    LAppDelegate::GetInstance()->GetClientSize(windowWidth, windowHeight);

    // 投影用マトリックス
    CubismMatrix44 projection = CubismMatrix44();
    if (windowWidth != 0 && windowHeight != 0)
    {
        projection.Scale(1.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight));
    }

    // 必要があればここで乗算
    if (_viewMatrix != NULL)
    {
        projection.MultiplyByMatrix(_viewMatrix);
    }

    // D3D11 フレーム先頭処理
    // 各フレームでの、Cubism SDK の処理前にコール
    Rendering::CubismRenderer_D3D11::StartFrame(LAppDelegate::GetInstance()->GetD3dDevice(), LAppDelegate::GetInstance()->GetD3dContext(), windowWidth, windowHeight);

    const CubismMatrix44    saveProjection = projection;
    const csmUint32 modelCount = _models.GetSize();
    for (csmUint32 i = 0; i < modelCount; ++i)
    {
        LAppModel* model = GetModel(i);
        projection = saveProjection;

        // モデル1体描画前コール
        LAppDelegate::GetInstance()->GetView()->PreModelDraw(*model);

        // Cubismモデルの描画
        model->Update();
        model->Draw(projection);///< 参照渡しなのでprojectionは変質する

        // モデル1体描画後コール
        LAppDelegate::GetInstance()->GetView()->PostModelDraw(*model);
    }

    // D3D11 フレーム終了処理
    // 各フレームでの、Cubism SDK の処理後にコール
    Rendering::CubismRenderer_D3D11::EndFrame(LAppDelegate::GetInstance()->GetD3dDevice());
}

void LAppLive2DManager::NextScene()
{
    csmInt32 no = (_sceneIndex + 1) % ModelDirSize;
    ChangeScene(no);
}

void LAppLive2DManager::ChangeScene(Csm::csmInt32 index)
{
    _sceneIndex = index;
    if (DebugLogEnable)
    {
        LAppPal::PrintLog("[APP]model index: %d", _sceneIndex);
    }

    // 从ModelDir[]中保留的目录名称
    // 决定model3.json的路径
    // 使目录名与model3.json的名字一致
    std::string model = ModelDir[index];
    std::string modelPath = ResourcesPath + model + "/";
    std::string modelJsonName = ModelDir[index];
    modelJsonName += ".model3.json";

    ReleaseAllModel();
    _models.PushBack(new LAppModel());
    _models[0]->LoadAssets(modelPath.c_str(), modelJsonName.c_str());

    /*
     * 提出进行模型半透明显示的样品
     * 此处为USE_RENDER_TARGET、USE_ODEL_RENDER_如果定义了TARGET
     * 在另一渲染目标上绘制模型，并将绘制结果作为纹理粘贴到另一目标上
     */
    {
#if defined(USE_RENDER_TARGET)
        //选择此选项可在LAppView目标上绘制
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_ViewFrameBuffer;
#elif defined(USE_MODEL_RENDER_TARGET)
        // 选择此选项可在每个LAppModel的目标上绘制
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_ModelFrameBuffer;
#else
        // 渲染到默认大型机缓冲区（正常）
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_None;
#endif

#if defined(USE_RENDER_TARGET) || defined(USE_MODEL_RENDER_TARGET)
        // 每个模型α中选择所需的墙类型
        _models.PushBack(new LAppModel());
        _models[1]->LoadAssets(modelPath.c_str(), modelJsonName.c_str());
        _models[1]->GetModelMatrix()->TranslateX(0.2f);
#endif

        LAppDelegate::GetInstance()->GetView()->SwitchRenderingTarget(useRenderTarget);

        // 选择其他渲染目标时的背景透明色
        float clearColor[3] = { 1.0f, 1.0f, 1.0f };
        LAppDelegate::GetInstance()->GetView()->SetRenderTargetClearColor(clearColor[0], clearColor[1], clearColor[2]);
    }
}

csmUint32 LAppLive2DManager::GetModelNum() const
{
    return _models.GetSize();
}

void LAppLive2DManager::EndFrame()
{
    // モデル解放監視
    for( int i= _releaseModel.GetSize()-1; i>=0; i--)
    {
        _releaseModel[i]._counter--;

        if (_releaseModel[i]._counter <= 0)
        {// モデル削除
            if (_releaseModel[i]._model)
            {
                delete _releaseModel[i]._model;
            }
            // コンテナも削除
            _releaseModel.Remove(i);
            continue;
        }
    }
}

void LAppLive2DManager::ResizedWindow()
{
    const csmUint32 modelCount = _models.GetSize();
    for (csmUint32 i = 0; i < modelCount; ++i)
    {
        _models[i]->GetRenderBuffer().DestroyOffscreenFrame();
    }
}
