/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <CubismFramework.hpp>
#include <Model/CubismUserModel.hpp>
#include <ICubismModelSetting.hpp>
#include <Type/csmRectF.hpp>
#include <Rendering/D3D11/CubismOffscreenSurface_D3D11.hpp>



/**
 * @brief 单个模型的动作与渲染等
 */
class LAppModel : public Csm::CubismUserModel
{
public:
    LAppModel();

    virtual ~LAppModel();

    /**
     * @brief 根据model3.json所在的目录和文件路径生成模型
     *
     */
    void LoadAssets(const Csm::csmChar* dir, const  Csm::csmChar* fileName);

    /**
     * @brief 重建渲染器
     *
     */
    void ReloadRenderer();

    /**
     * @brief   更新模型的过程     根据模型参数确定绘图状态
     *
     */
    void Update();

    /**
     * @brief   绘制模型的过程     传递要绘制模型的空间的视图投影矩阵
     *
     * @param[in]  matrix  View-Projection行列
     */
    void Draw(Csm::CubismMatrix44& matrix);

    /**
     * @brief   开始播放由参数指定的运动
     *
     * @param[in]   group                      运动组名称
     * @param[in]   no                          组中的编号
     * @param[in]   priority                    优先度
     * @param[in]   onFinishedMotionHandler     在运动播放结束时调用的回调函数     如果为空，则不调用
     * @return                                  返回已开始运动的标识号     用于确定单独的运动是否结束的IsFinished（）的参数。无法开始时为“-1”
     */
    Csm::CubismMotionQueueEntryHandle StartMotion(const Csm::csmChar* group, Csm::csmInt32 no, Csm::csmInt32 priority, Csm::ACubismMotion::FinishedMotionCallback onFinishedMotionHandler = NULL);

    /**
     * @brief   开始播放随机选取的运动
     *
     * @param[in]   group                       运动组名称
     * @param[in]   priority                   优先度
     * @param[in]   onFinishedMotionHandler     在运动播放结束时调用的回调函数     如果为空，则不调用
     * @return                                  返回已开始运动的标识号     用于确定单独的运动是否结束的IsFinished（）的参数。无法开始时为“-1”
     */
    Csm::CubismMotionQueueEntryHandle StartRandomMotion(const Csm::csmChar* group, Csm::csmInt32 priority, Csm::ACubismMotion::FinishedMotionCallback onFinishedMotionHandler = NULL);

    /**
     * @brief   设置参数指定的表情运动
     *
     * @param   expressionID    表情运动ID
     */
    void SetExpression(const Csm::csmChar* expressionID);

    /**
     * @brief   设置随机选择的表情运动
     *
     */
    void SetRandomExpression();

    /**
    * @brief   接受活动的开火
    *
    */
    virtual void MotionEventFired(const Live2D::Cubism::Framework::csmString& eventValue);

    /**
     * @brief    命中判定测试
     *           根据指定ID的顶点列表计算矩形    判定坐标是否在矩形范围内
     *
     * @param[in]   hitAreaName     测试命中判定的对象ID
     * @param[in]   x               进行判定X坐标
     * @param[in]   y               进行判定Y坐标
     */
    virtual Csm::csmBool HitTest(const Csm::csmChar* hitAreaName, Csm::csmFloat32 x, Csm::csmFloat32 y);

    /**
     * @brief   标记模型删除
     *          现在，作为角色只是抑制显示
     */
    void DeleteMark() { _deleteModel = true; }


    /**
     * @brief   获取要在其他目标上绘制的缓冲区
     */
    Csm::Rendering::CubismOffscreenFrame_D3D11& GetRenderBuffer();


protected:
    /**
     *  @brief  绘制模型的过程     传递要绘制模型的空间的视图投影矩阵
     *
     */
    void DoDraw();

private:
    /**
     * @brief 由model3.json生成模型
     *        根据model3.json的描述，进行模型生成、运动、物理运算等组件生成
     *
     * @param[in]   setting     ICubismModelSetting实例
     *
     */
    void SetupModel(Csm::ICubismModelSetting* setting);

    /**
     * @brief 加载纹理
     *
     */
    void SetupTextures();

    /**
     * @brief   从组名批量加载运动数据
     *           从内部的模型设置获取运动数据的名称
     *
     * @param[in]   group  运动数据组名称
     */
    void PreloadMotionGroup(const Csm::csmChar* group);

    /**
     * @brief   从组名中一次性释放运动数据
     *          从内部的模型设置中获取运动数据的名称
     *
     * @param[in]   group  运动数据组名称
     */
    void ReleaseMotionGroup(const Csm::csmChar* group) const;

    /**
    * @brief 释放所有运动数据
    */
    void ReleaseMotions();

    /**
    * @brief 释放所有表情数据
    */
    void ReleaseExpressions();

    Csm::ICubismModelSetting* _modelSetting; // 模型设置信息
    Csm::csmString _modelHomeDir; // 模型设置所在的目录
    Csm::csmFloat32 _userTimeSeconds; // 增量时间累计值[秒]
    Csm::csmVector<Csm::CubismIdHandle> _eyeBlinkIds; // 为模型设置的眨眼功能参数ID
    Csm::csmVector<Csm::CubismIdHandle> _lipSyncIds; // 为模型设置的唇同步功能参数标识
    Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>   _motions; // 导入的运动列表
    Csm::csmMap<Csm::csmString, Csm::ACubismMotion*>   _expressions; // 导入的表情列表
    Csm::csmVector<Csm::csmRectF> _hitArea;
    Csm::csmVector<Csm::csmRectF> _userArea;
    const Csm::CubismId* _idParamAngleX; //参数ID: ParamAngleX
    const Csm::CubismId* _idParamAngleY; //参数ID: ParamAngleY
    const Csm::CubismId* _idParamAngleZ; //参数ID: ParamAngleZ
    const Csm::CubismId* _idParamBodyAngleX; // 参数ID: ParamBodyAngleX
    const Csm::CubismId* _idParamEyeBallX; // 参数ID: ParamEyeBallX
    const Csm::CubismId* _idParamEyeBallY; // 参数ID: ParamEyeBallXY

    Csm::csmVector<Csm::csmUint64> _bindTextureId; // 纹理标识

    bool _deleteModel;  // 不调用实体消失预定标志Draw

    Csm::Rendering::CubismOffscreenFrame_D3D11 _renderBuffer;   // 帧缓冲区以外的目标
};



