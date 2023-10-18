/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <wrl/client.h>
#include <d3dcompiler.h>

#include <Rendering/D3D11/CubismNativeInclude_D3D11.hpp>
#include <CubismFramework.hpp>

#include "LAppAllocator.hpp"

class LAppView;
class LAppTextureManager;

/**
 * @brief   整个程序中若干对象的生命期管理,
 *   以及用户交互部分
 */
class LAppDelegate
{
public:

    /**
     * @brief   返回一个类的实例（单个）
     *           如果未生成实例，则在内部生成实例
     *
     * @return  类实例
     */
    static LAppDelegate* GetInstance();

    /**
     * @brief   释放类实例
     *
     */
    static void ReleaseInstance();

    /**
     * @brief   获取D3D设备
     */
    static ID3D11Device*   GetD3dDevice();

    /**
     * @brief   获取D3D设备上下文
     */
    static ID3D11DeviceContext*   GetD3dContext();

    /**
     * @brief   窗口客户端区域获取
     *        未被外部调用
     */
    static void GetWindowRect(RECT& rect);

    /**
     * @brief   窗口客户端区域的宽度、高度获取
     */
    static void GetClientSize(int& rWidth, int& rHeight);

    /**
     * @brief   初始化APP所需的内容
     *        只被main调用
     */
    bool Initialize();

    /**
     *  @brief 释放处理
     */
    void Release();

    /**
    * @brief  运行处理
    *         只被main调用
    */
    void Run();

    /**
     * @brief   获取View
     */
    LAppView* GetView() { return _view; }

    /**
    * @brief   是否退出应用程序
    */
    bool GetIsEnd() { return _isEnd; }

    /**
    * @brief   退出应用程序
    */
    void AppEnd() { _isEnd = true; }

    LAppTextureManager* GetTextureManager() { return _textureManager; }

    /**
     * @brief   框架最初的行动
     */
    void StartFrame();

    /**
     * @brief   框架最后的行动
     */
    void EndFrame();

    /**
     * @brief   创建着色器并且创建顶点
     */
    bool CreateShader();

    /**
     * @brief   在上下文中设置着色器
     */
    void SetupShader();

    /**
     * @brief   确保CreateShader的对象开放
     */
    void ReleaseShader();

private:

    enum DeviceStep
    {
        DeviceStep_None,      // 平常
        DeviceStep_Lost,      // 丢失
        DeviceStep_Size,      // 调整大小
    };

    /**
     * @brief   窗口过程
     */
    static LRESULT WINAPI MsgProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

    LAppDelegate();

    ~LAppDelegate();

    /**
    * @brief   初始化Cubism SDK
    */
    void InitializeCubism();

    /**
     * @brief   渲染目标创建内部函数
     */
    bool CreateRenderTarget(UINT width, UINT height);

    /**
     * @brief   调整设备大小
     */
    void ResizeDevice();

    LAppAllocator _cubismAllocator;              // Cubism SDK Allocator
    Csm::CubismFramework::Option _cubismOption;  // Cubism SDK Option

    LAppView* _view;                             
    bool _captured;                              // 是否单击
    float _mouseX;                               // 鼠标X坐标
    float _mouseY;                               // 鼠标Y坐标
    bool _isEnd;                                 // APP是否结束
    LAppTextureManager* _textureManager;         // 纹理管理器

    HWND                    _windowHandle;  // 窗口句柄
    ID3D11Device*           _device;        //当前设备
    ID3D11DeviceContext*    _deviceContext; // D3D绘图上下文
    IDXGISwapChain*         _swapChain;     //当前交换链
    WNDCLASSEX              _windowClass;   // 窗口类
    DeviceStep              _deviceStep;    // 设备尺寸变更等步骤
    DXGI_SWAP_CHAIN_DESC    _presentParameters; //交换链的当前参数

    ID3D11RenderTargetView* _renderTargetView;  //绘画目标视图
    ID3D11Texture2D*        _depthTexture;      //2D 纹理
    ID3D11DepthStencilView* _depthStencilView;  //深度模具视图
    ID3D11DepthStencilState* _depthState;       // 深度模具对象

    ID3D11RasterizerState*  _rasterizer;    //光栅化器
    ID3D11SamplerState*     _samplerState;  //采样器

    ID3D11VertexShader*     _vertexShader;  // 绘制着色器
    ID3D11PixelShader*      _pixelShader;   // 绘制着色器
    ID3D11BlendState*       _blendState;    // 绘制混合状态
    ID3D11InputLayout*      _vertexFormat;  //绘图类型声明
};
