/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <Windows.h>
#include <string>
#include <CubismFramework.hpp>
#include <Type/CubismBasicType.hpp>

//========================================================
//  帧缓冲区以外的目标
//========================================================
namespace Csm = Live2D::Cubism::Framework;

/**
* @brief 某些辅助函数
*/
class LAppPal
{
public:

    /**
     * @brief 启动计时器
     */
    static void StartTimer();

    /**
    * @brief 将文件导入为字节数据
    *
    * @param[in]   filePath    目标文件路径
    * @param[out]  outSize     文件大小
    * @return                  字节数据
    */
    static Csm::csmByte* LoadFileAsBytes(const std::string filePath, Csm::csmSizeInt* outSize);


    /**
    * @brief 释放字节数据
    *
    * @param[in]   byteData    要释放的字节数据
    */
    static void ReleaseBytes(Csm::csmByte* byteData);

    /**
    * @biref   获取增量时间（与上一帧的差值）
    *
    * @return  增量时间[ms]
    *
    */
    static Csm::csmFloat32 GetDeltaTime();

    static void UpdateTime();

    /**
    * @brief 输出日志
    *
    *
    */
    static void PrintLog(const char*, ...);

    /**
    * @brief 输出日志 wchar版
    *
    * 输出日志
    *
    */
    static void PrintLogW(const wchar_t*, ...);

    /**
    * @brief 输出消息
    *
    * @param[in]   message  文字列
    *
    */
    static void PrintMessage(const Csm::csmChar* message);

    /**
     * @brief   坐标变换
     *          全屏 LT(-bufferX, bufferY) RB(bufferX, -bufferY) → 窗口鼠标 LT(0, 0) RB(bufferX, bufferY)
     *          未被外部调用
     */
    static void CoordinateFullScreenToWindow(float clientWidth, float clientHeight, float fullScreenX, float fullScreenY, float& retWindowX, float& retWindowY);

    /**
     * @brief    坐标变换
     *          窗口鼠标 LT(0, 0) RB(bufferX, bufferY) → 全屏 LT(-bufferX, bufferY) RB(bufferX, -bufferY)
     */
    static void CoordinateWindowToFullScreen(float clientWidth, float clientHeight, float windowX, float windowY, float& retFullScreenX, float& retFullScreenY);

    /**
     * @brief   从Mbc转换为Wchar
     */
    static void MbcToWchar(const char* src, size_t srcLength, wchar_t* dest, size_t destLength);
};

