/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */
#pragma once

#include <CubismFramework.hpp>

/**
 * @brief  程序中用到的常量定义
 *
 */
namespace LAppDefine {

    using namespace Csm;

    extern const csmFloat32 ViewMaxScale;           // 最大缩放比例
    extern const csmFloat32 ViewMinScale;           // 最小缩放比例

    extern const csmFloat32 ViewLogicalLeft;        // 逻辑视图坐标系左边缘的值
    extern const csmFloat32 ViewLogicalRight;       // 逻辑视图坐标系右边缘的值

    extern const csmFloat32 ViewLogicalMaxLeft;     // 逻辑视图坐标系左边缘的最大值
    extern const csmFloat32 ViewLogicalMaxRight;    // 逻辑视图坐标系右边缘的最大值
    extern const csmFloat32 ViewLogicalMaxBottom;   // 逻辑视图坐标系下边缘的最大值
    extern const csmFloat32 ViewLogicalMaxTop;      // 逻辑视图坐标系上边缘的最大值

    extern const csmChar* ResourcesPath;            // 素材路径
    extern const csmChar* BackImageName;         // 背景图像文件
    extern const csmChar* GearImageName;         // 齿轮图像文件
    extern const csmChar* PowerImageName;        // 结束按钮图像文件

    //模型定义--------------------------------------------
    extern const csmChar* ModelDir[];               // 配置模型的目录名的排列 使目录名和model3.json的名字一致
    extern const csmInt32 ModelDirSize;             // 模型目录数组大小

                                                    // 与外部定义文件（json）匹配
    extern const csmChar* MotionGroupIdle;          // 怠速时要播放的运动列表
    extern const csmChar* MotionGroupTapBody;       // 点击身体时播放的动作列表

                                                    //与外部定义文件（json）匹配
    extern const csmChar* HitAreaNameHead;          // “标题”标签
    extern const csmChar* HitAreaNameBody;          // 判定中的“躯干”标签

                                                    // 运动优先级常数
    extern const csmInt32 PriorityNone;             // 运动优先级常数： 0
    extern const csmInt32 PriorityIdle;             // 运动优先级常数： 1
    extern const csmInt32 PriorityNormal;           // 运动优先级常数： 2
    extern const csmInt32 PriorityForce;            //运动优先级常数：3

                                                    //查看调试日志
    extern const csmBool DebugLogEnable;            // 启用或禁用调试日志显示
    extern const csmBool DebugTouchLogEnable;       // 启用或禁用用于调试触摸处理的日志显示

    //从框架输出的日志级别设置
    extern const CubismFramework::Option::LogLevel CubismLoggingLevel;

    
    extern const csmInt32 RenderTargetWidth;// 默认渲染目标宽度
    extern const csmInt32 RenderTargetHeight;// 默认渲染目标高度
}
