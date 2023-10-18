/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <Rendering/D3D11/CubismNativeInclude_D3D11.hpp>
#include <Type/csmVector.hpp>
#include <Rendering/D3D11/CubismOffscreenSurface_D3D11.hpp>
#include <Math/CubismMatrix44.hpp>
#include <Math/CubismViewMatrix.hpp>
#include "CubismFramework.hpp"


class TouchManager;
class LAppSprite;
class LAppModel;

/**
* @brief 提供整体的画布以及管理sprite的渲染
*/
class LAppView
{
public:

    /**
     * @brief 将LAppModel渲染
     */
    enum SelectTarget
    {
        SelectTarget_None,                // 渲染到默认帧缓冲区
        SelectTarget_ModelFrameBuffer,    // 渲染到LAppModel各自具有的帧缓冲区
        SelectTarget_ViewFrameBuffer,     // 渲染到具有LAppView的帧缓冲区
    };

    LAppView();

    ~LAppView();

    /**
    * @brief 初始化
    */
    void Initialize();

    /**
    * @brief 绘制
    */
    void Render();

    /**
    * @brief 进行图像的初始化
    */
    void InitializeSprite();

    /**
    * @brief        Sprite开放
    */
    void ReleaseSprite();

    /**
    * @brief 重新调整sprite大小
    */
    void ResizeSprite();
    /**
     * @brief   在绘制一个模型之前调用
     */
    void PreModelDraw(LAppModel& refModel);

    /**
     * @brief  绘制一个模型后立即调用
     */
    void PostModelDraw(LAppModel& refModel);

    /**
     * @brief   在动态输入提示中单击
     *           绘图时α决定
     */
    float GetSpriteAlpha(int assign) const;

    /**
     * @brief 切换渲染目标
     */
    void SwitchRenderingTarget(SelectTarget targetType);

    /**
     * @brief 将渲染目标切换为非默认值时的背景透明色设置
     * @param[in]   r   红(0.0~1.0)
     * @param[in]   g   绿(0.0~1.0)
     * @param[in]   b   蓝(0.0~1.0)
     */
    void SetRenderTargetClearColor(float r, float g, float b);

    /**
     * @brief   销毁内部渲染缓冲区
     */
    void DestroyOffscreenFrame();

private:
    TouchManager* _touchManager;                 // 触摸管理器
    Csm::CubismMatrix44* _deviceToScreen;    // 从设备到屏幕的矩阵
    Csm::CubismViewMatrix* _viewMatrix;     

    LAppSprite* _back;                       // 背景图像
    LAppSprite* _gear;                       // 齿轮图像
    LAppSprite* _power;                    // 电源图像

    // 用于将目标渲染为不同目标
    LAppSprite* _renderSprite;                                  // 在某些模式下绘制渲染缓冲区纹理
    Csm::Rendering::CubismOffscreenFrame_D3D11 _renderBuffer;   // 在某些模式下渲染Cubism模型结果
    SelectTarget _renderTarget;     // 目标选项
    float _clearColor[4];           // 渲染目标的透明颜色
};
