﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppPal.hpp"
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "LAppDefine.hpp"
#include "LAppDelegate.hpp"

using namespace Csm;
using namespace std;
using namespace LAppDefine;

namespace {
    LARGE_INTEGER s_frequency;
    LARGE_INTEGER s_lastFrame;
    double s_deltaTime = 0.0;
}

void LAppPal::StartTimer()
{
    QueryPerformanceFrequency(&s_frequency); //如果安装的硬件支持高分辨率性能计数器，则返回值为非零值。
}

csmByte* LAppPal::LoadFileAsBytes(const string filePath, csmSizeInt* outSize)
{
    //filePath;//
    const char* path = filePath.c_str();

    int size = 0;
    struct stat statBuf;
    if (stat(path, &statBuf) == 0)
    {
        size = statBuf.st_size;
    }

    std::fstream file;
    char* buf = new char[size];

    file.open(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        if (DebugLogEnable)
        {
            PrintLog("file open error");
        }
        return NULL;
    }
    file.read(buf, size);
    file.close();

    *outSize = size;
    return reinterpret_cast<csmByte*>(buf);
}

void LAppPal::ReleaseBytes(csmByte* byteData)
{
    delete[] byteData;
}

csmFloat32  LAppPal::GetDeltaTime()
{
    return static_cast<csmFloat32>(s_deltaTime);
}

void LAppPal::UpdateTime()
{
    if (s_frequency.QuadPart == 0)
    {
        StartTimer();
        QueryPerformanceCounter(&s_lastFrame); //检索性能计数器的当前值
        s_deltaTime = 0.0f;
        return;
    }

    LARGE_INTEGER current; // LARGE_INTEGER 表示 64 位带符号整数值
    QueryPerformanceCounter(&current);

    const LONGLONG BASIS = 1000000;
    LONGLONG dwTime = ((current.QuadPart - s_lastFrame.QuadPart) * (LONGLONG)BASIS / s_frequency.QuadPart);
    s_deltaTime = (double)dwTime / (double)(BASIS);
    s_lastFrame = current;
}

void LAppPal::PrintLog(const char* format, ...)
{
    va_list args;
    char buf[256];
    va_start(args, format);
    _vsnprintf_s(buf, sizeof(buf), format, args);
    OutputDebugStringA((LPCSTR)buf);
    OutputDebugStringA("\n");   // 单独添加换行符
    va_end(args);
}

void LAppPal::PrintLogW(const wchar_t* format, ...)
{
    va_list args;
    wchar_t buf[256];
    va_start(args, format);
    _vsnwprintf_s(buf, sizeof(buf), format, args);
    OutputDebugString((LPCSTR)buf);
    OutputDebugString("\n");   // 改行を別途付与します
    va_end(args);
}

void LAppPal::PrintMessage(const csmChar* message)
{
    PrintLog("%s", message);
}

void LAppPal::CoordinateWindowToFullScreen(float clientWidth, float clientHeight, float windowX, float windowY, float& retFullScreenX, float& retFullScreenY)
{
    retFullScreenX = retFullScreenY = 0.0f;

    const float width = static_cast<float>(clientWidth);
    const float height = static_cast<float>(clientHeight);

    if (width == 0.0f || height == 0.0f) return;

    retFullScreenX = 2.0f * windowX - width;
    retFullScreenY = (2.0f * windowY - height) * -1.0f;
}

void LAppPal::MbcToWchar(const char* src, size_t srcLength, wchar_t* dest, size_t destLength)
{
    if (srcLength<1 || destLength<2) return;

    memset(dest, 0, destLength);
    MultiByteToWideChar(CP_ACP, 0, src, static_cast<int>(srcLength), dest, static_cast<int>(destLength));
}
