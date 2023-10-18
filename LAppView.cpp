﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppView.hpp"
#include <math.h>
#include <string>
#include "LAppPal.hpp"
#include "LAppDelegate.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppTextureManager.hpp"
#include "LAppDefine.hpp"
#include "LAppSprite.hpp"
#include "LAppModel.hpp"

using namespace std;
using namespace LAppDefine;

LAppView::LAppView():
    _back(NULL),
    _gear(NULL),
    _power(NULL),
    _renderSprite(NULL),
    _renderTarget(SelectTarget_None)
{
    _clearColor[0] = 1.0f;
    _clearColor[1] = 1.0f;
    _clearColor[2] = 1.0f;
    _clearColor[3] = 0.0f;

    // 用于将设备坐标转换为屏幕坐标
    _deviceToScreen = new CubismMatrix44();

    // 进行画面的显示的放大缩小和移动的变换的行列
    _viewMatrix = new CubismViewMatrix();
}

LAppView::~LAppView()
{
    _renderBuffer.DestroyOffscreenFrame();

    delete _renderSprite;
    delete _viewMatrix;
    delete _deviceToScreen;
    delete _touchManager;
    delete _back;
    delete _gear;
    delete _power;
}

void LAppView::Initialize()
{
    int width, height;
    LAppDelegate::GetClientSize(width, height);

    if(width==0 || height==0)
    {
        return;
    }

    float ratio = static_cast<float>(height) / static_cast<float>(width);
    float left = ViewLogicalLeft;
    float right = ViewLogicalRight;
    float bottom = -ratio;
    float top = ratio;

    _viewMatrix->SetScreenRect(left, right, bottom, top); //与设备相对应的屏幕范围。X左端、X右端、Y下端、Y上端

    float screenW = fabsf(left - right);
    _deviceToScreen->LoadIdentity(); // 初始化为单位矩阵
    _deviceToScreen->ScaleRelative(screenW / width, -screenW / width);//相对设置当前矩阵的放大率
    _deviceToScreen->TranslateRelative(-width * 0.5f, -height * 0.5f);//以当前矩阵的位置为起点移动

    // 设置显示范围
    _viewMatrix->SetMaxScale(ViewMaxScale); 
    _viewMatrix->SetMinScale(ViewMinScale); 

    // 可显示的最大范围
    _viewMatrix->SetMaxScreenRect(
        ViewLogicalMaxLeft,
        ViewLogicalMaxRight,
        ViewLogicalMaxBottom,
        ViewLogicalMaxTop
    );
}

void LAppView::Render()
{
    LAppLive2DManager* live2DManager = LAppLive2DManager::GetInstance();
    if (!live2DManager)
    {
        return;
    }

    // Sprite绘制
    int width, height;
    LAppDelegate::GetInstance()->GetClientSize(width, height);

    if (_back)
    {
        _back->Render(width, height);
    }
    if (_gear)
    {
        _gear->Render(width, height);
    }
    if (_power)
    {
        _power->Render(width, height);
    }

    // Cubism更新・描画
    live2DManager->OnUpdate();

    // 将每个模型的绘制目标作为纹理
    if (_renderTarget == SelectTarget_ModelFrameBuffer && _renderSprite)
    {
        for(csmUint32 i=0; i<live2DManager->GetModelNum(); i++)
        {
            float alpha = GetSpriteAlpha(i); // 作为样本落差
            _renderSprite->SetColor(1.0f, 1.0f, 1.0f, alpha);

            LAppModel *model = live2DManager->GetModel(i);
            if (model)
            {
                _renderSprite->RenderImmidiate(width, height, model->GetRenderBuffer().GetTextureView());
            }
        }
    }
}

void LAppView::InitializeSprite()
{
    // 绘图区域大小
    int width, height;
    LAppDelegate::GetInstance()->GetClientSize(width, height);

    LAppTextureManager* textureManager = LAppDelegate::GetInstance()->GetTextureManager();
    const string resourcesPath = ResourcesPath;

    float x = 0.0f;
    float y = 0.0f;
    float fWidth = 0.0f;
    float fHeight = 0.0f;

    string imageName = resourcesPath + BackImageName;
    LAppTextureManager::TextureInfo* backgroundTexture = textureManager->CreateTextureFromPngFile(imageName, false);
    x = width * 0.5f;
    y = height * 0.5f;
    fWidth = static_cast<float>(backgroundTexture->width * 2);
    fHeight = static_cast<float>(height) * 0.95f;
    _back = new LAppSprite(x, y, fWidth, fHeight, backgroundTexture->id);

    imageName = resourcesPath + GearImageName;
    LAppTextureManager::TextureInfo* gearTexture = textureManager->CreateTextureFromPngFile(imageName, false);
    x = static_cast<float>(width - gearTexture->width * 0.5f);
    y = static_cast<float>(height - gearTexture->height * 0.5f);
    fWidth = static_cast<float>(gearTexture->width);
    fHeight = static_cast<float>(gearTexture->height);
    _gear = new LAppSprite(x, y, fWidth, fHeight, gearTexture->id);

    imageName = resourcesPath + PowerImageName;
    LAppTextureManager::TextureInfo* powerTexture = textureManager->CreateTextureFromPngFile(imageName, false);
    x = static_cast<float>(width - powerTexture->width * 0.5f);
    y = static_cast<float>(powerTexture->height * 0.5f);
    fWidth = static_cast<float>(powerTexture->width);
    fHeight = static_cast<float>(powerTexture->height);
    _power = new LAppSprite(x, y, fWidth, fHeight, powerTexture->id);

    x = width * 0.5f;
    y = height * 0.5f;
    _renderSprite = new LAppSprite(x, y, static_cast<float>(width), static_cast<float>(height), 0);
}

void LAppView::ReleaseSprite()
{
    delete _renderSprite;
    _renderSprite = NULL;

    delete _gear;
    _gear = NULL;

    delete _power;
    _power = NULL;

    delete _back;
    _back = NULL;
}

void LAppView::ResizeSprite()
{
    LAppTextureManager* textureManager = LAppDelegate::GetInstance()->GetTextureManager();
    if (!textureManager)
    {
        return;
    }

    // 绘图区域大小
    int width, height;
    LAppDelegate::GetInstance()->GetClientSize(width, height);

    float x = 0.0f;
    float y = 0.0f;
    float fWidth = 0.0f;
    float fHeight = 0.0f;

    if(_back)
    {
        Csm::csmUint64 id = _back->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
        if(texInfo)
        {
            x = width * 0.5f;
            y = height * 0.5f;
            fWidth = static_cast<float>(texInfo->width * 2);
            fHeight = static_cast<float>(height) * 0.95f;
            _back->ResetRect(x, y, fWidth, fHeight);
        }
    }

    if (_power)
    {
        Csm::csmUint64 id = _power->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = static_cast<float>(width - texInfo->width * 0.5f);
            y = static_cast<float>(texInfo->height * 0.5f);
            fWidth = static_cast<float>(texInfo->width);
            fHeight = static_cast<float>(texInfo->height);
            _power->ResetRect(x, y, fWidth, fHeight);
        }
    }

    if (_gear)
    {
        Csm::csmUint64 id = _gear->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = static_cast<float>(width - texInfo->width * 0.5f);
            y = static_cast<float>(height - texInfo->height * 0.5f);
            fWidth = static_cast<float>(texInfo->width);
            fHeight = static_cast<float>(texInfo->height);
            _gear->ResetRect(x, y, fWidth, fHeight);
        }
    }

    if (_renderSprite)
    {
        x = width * 0.5f;
        y = height * 0.5f;
        _renderSprite->ResetRect(x, y, static_cast<float>(width), static_cast<float>(height));
    }
}

void LAppView::PreModelDraw(LAppModel& refModel)
{
    // 用于绘制到其他渲染目标的帧缓冲区
    Csm::Rendering::CubismOffscreenFrame_D3D11* useTarget = NULL;

    if (_renderTarget != SelectTarget_None)
    {// 绘制到其他渲染目标时

        // 要使用的目标
        useTarget = (_renderTarget == SelectTarget_ViewFrameBuffer) ? &_renderBuffer : &refModel.GetRenderBuffer();

        if (!useTarget->IsValid())
        {// 绘制目标内部未创建时在此处创建
            int width, height;
            LAppDelegate::GetClientSize(width, height);

            if (width != 0 && height != 0)
            {
                // 模型绘图画布
                useTarget->CreateOffscreenFrame(LAppDelegate::GetInstance()->GetD3dDevice(),
                    static_cast<csmUint32>(width), static_cast<csmUint32>(height));
            }
        }

        // 开始渲染
        useTarget->BeginDraw(LAppDelegate::GetInstance()->GetD3dContext());
        useTarget->Clear(LAppDelegate::GetInstance()->GetD3dContext(), _clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]); // 背景清除颜色
    }
}

void LAppView::PostModelDraw(LAppModel& refModel)
{
    // 用于绘制到其他渲染目标的帧缓冲区
    Csm::Rendering::CubismOffscreenFrame_D3D11* useTarget = NULL;

    if (_renderTarget != SelectTarget_None)
    {// 绘制到其他渲染目标时

        // 要使用的目标
        useTarget = (_renderTarget == SelectTarget_ViewFrameBuffer) ? &_renderBuffer : &refModel.GetRenderBuffer();

        // 渲染结束
        useTarget->EndDraw(LAppDelegate::GetInstance()->GetD3dContext());

        //如果使用LAppView的帧缓冲区，则在Sprite中绘制
        if (_renderTarget == SelectTarget_ViewFrameBuffer && _renderSprite)
        {
            // Sprite绘制
            int width, height;
            LAppDelegate::GetInstance()->GetClientSize(width, height);

            _renderSprite->SetColor(1.0f, 1.0f, 1.0f, GetSpriteAlpha(0));
            _renderSprite->RenderImmidiate(width, height, useTarget->GetTextureView());
        }
    }
}

void LAppView::SwitchRenderingTarget(SelectTarget targetType)
{
    _renderTarget = targetType;
}

void LAppView::SetRenderTargetClearColor(float r, float g, float b)
{
    _clearColor[0] = r;
    _clearColor[1] = g;
    _clearColor[2] = b;
}

void LAppView::DestroyOffscreenFrame()
{
    _renderBuffer.DestroyOffscreenFrame();
}

float LAppView::GetSpriteAlpha(int assign) const
{
    // 根据assign的数值适当地决定
    float alpha = 0.25f + static_cast<float>(assign) * 0.5f; // 作为样本落差
    if (alpha > 1.0f)
    {
        alpha = 1.0f;
    }
    if (alpha < 0.1f)
    {
        alpha = 0.1f;
    }

    return alpha;
}
