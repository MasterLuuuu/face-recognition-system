/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppDefine.hpp"
//#include <CubismFramework.hpp>

namespace LAppDefine {

    using namespace Csm;

    // 画面
    const csmFloat32 ViewMaxScale = 2.0f;
    const csmFloat32 ViewMinScale = 0.8f;

    const csmFloat32 ViewLogicalLeft = -1.0f;
    const csmFloat32 ViewLogicalRight = 1.0f;

    const csmFloat32 ViewLogicalMaxLeft = -2.0f;
    const csmFloat32 ViewLogicalMaxRight = 2.0f;
    const csmFloat32 ViewLogicalMaxBottom = -2.0f;
    const csmFloat32 ViewLogicalMaxTop = 2.0f;

    const csmChar* ResourcesPath = "Resources/";// 相对路径

    const csmChar* BackImageName = "background.png";// 模型后面的背景图像文件
    const csmChar* GearImageName = "icon_gear.png";// 歯车
    const csmChar* PowerImageName = "close.png";// 结束按钮

    // 模型定义------------------------------------------
    // 放置模型的目录名数组
    // 使目录名与model3.json的名字一致
    const csmChar* ModelDir[] = {
        "jocker"
    };
    const csmInt32 ModelDirSize = sizeof(ModelDir) / sizeof(const csmChar*);

    //与外部定义文件（json）匹配
    const csmChar* MotionGroupIdle = "Idle"; 
    const csmChar* MotionGroupTapBody = "TapBody"; // 点击身体的时候

    //与外部定义文件（json）匹配
    const csmChar* HitAreaNameHead = "Head";
    const csmChar* HitAreaNameBody = "Body";

    // 优先级
    const csmInt32 PriorityNone = 0;
    const csmInt32 PriorityIdle = 1;
    const csmInt32 PriorityNormal = 2;
    const csmInt32 PriorityForce = 3;

    //调试日志显示选项
    const csmBool DebugLogEnable = true;
    const csmBool DebugTouchLogEnable = false;

    const CubismFramework::Option::LogLevel CubismLoggingLevel = CubismFramework::Option::LogLevel_Verbose; //从框架输出的日志级别设置
    const csmInt32 RenderTargetWidth = 1900;// 默认渲染目标宽度
    const csmInt32 RenderTargetHeight = 1000;  //默认渲染目标高度
}
