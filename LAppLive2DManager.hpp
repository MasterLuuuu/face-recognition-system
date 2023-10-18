/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <Rendering/D3D11/CubismNativeInclude_D3D11.hpp>
#include <CubismFramework.hpp>
#include <Math/CubismMatrix44.hpp>
#include <Type/csmVector.hpp>
#include <Type/csmMap.hpp>



class LAppModel;

/**
* @brief 对模型进行管理
*
*/
class LAppLive2DManager
{

public:

    /**
     * @brief 要释放的模型存储
     */
    struct ReleaseModel
    {
        ReleaseModel()
        {
            _model = NULL;
            _counter = 0;
        }

        LAppModel*  _model;
        int         _counter;
    };

    /**
    * @brief  返回一个类的实例(单个)
    *          如果未生成实例，则在内部生成实例
    *
    * @return  类实例
    */
    static LAppLive2DManager* GetInstance();

    /**
    * @brief   释放类实例（单个）。
    *
    */
    static void ReleaseInstance();

    /**
    * @brief   返回当前场景中保留的模型
    *
    * @param[in]   no  模型列表索引值
    * @return      返回模型实例   如果索引值超出范围，则返回空值。
    */
    LAppModel* GetModel(Csm::csmUint32 no) const;

    /**
    * @brief   释放当前场景中保留的所有模型
    *
    */
    void ReleaseAllModel();

    /**
    * @brief   拖动屏幕时的操作
    *
    * @param[in]   x   画面的X坐标
    * @param[in]   y   画面的Y坐标
    */
    void OnDrag(Csm::csmFloat32 x, Csm::csmFloat32 y) const;

    /**
    * @brief   点击画面时的处理
    *
    * @param[in]   x   画面的X坐标
    * @param[in]   y   画面的Y坐标
    */
    void OnTap(Csm::csmFloat32 x, Csm::csmFloat32 y);

    /**
    * @brief   更新画面时的处理
    *         进行模型的更新处理及描绘处理
    */
    void OnUpdate() const;

    /**
    * @brief   切换到下一个场景
    *           在示例应用程序中切换模型集
    */
    void NextScene();

    /**
    * @brief   切换场景 
    *           在示例应用程序中切换模型集
    */
    void ChangeScene(Csm::csmInt32 index);

    /**
     * @brief   处理帧末端
     */
    void EndFrame();

    /**
     * @brief   得到模型个数
     * @return  所持模型个数
     */
    Csm::csmUint32 GetModelNum() const;

    /**
     * @brief   窗口大小变更时调用的处理
     */
    void ResizedWindow();

private:
  
    LAppLive2DManager();

    virtual ~LAppLive2DManager();

    Csm::CubismMatrix44*        _viewMatrix; // 用于模型绘制的视图矩阵
    Csm::csmVector<LAppModel*>  _models; // 模型实例容器
    Csm::csmInt32               _sceneIndex; // 要显示的场景索引值

    Csm::csmVector<ReleaseModel>  _releaseModel; // 计划释放模型实例的容器

    Csm::csmMap<LAppModel*, int>  _releaseModels; // 模型实例容器（释放保留、释放计数）
};
