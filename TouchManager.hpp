/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once
/*
* @brief 处理鼠标点击事件
*/
class TouchManager
{
public:

    TouchManager();

    float GetCenterX() const { return _lastX; }
    float GetCenterY() const { return _lastY; }
    float GetDeltaX() const { return _deltaX; }
    float GetDeltaY() const{ return _deltaY; }
    float GetStartX() const{ return _startX; }
    float GetStartY() const{ return _startY; }
    float GetScale() const { return _scale; }
    float GetX() const{ return _lastX; }
    float GetY() const{ return _lastY; }
    float GetX1() const{ return _lastX1; }
    float GetY1() const{ return _lastY1; }
    float GetX2() const{ return _lastX2; }
    float GetY2() const{ return _lastY2; }
    bool IsSingleTouch() const { return _touchSingle; }
    bool IsFlickAvailable() const { return _flipAvailable; }
    void DisableFlick() { _flipAvailable = false; }

    /*
    * @brief 触摸开始时事件
    *
    * @param[in] deviceY    触摸画面的y值
    * @param[in] deviceX    触摸画面的x值
    */
    void TouchesBegan(float deviceX, float deviceY);

    /*
    * @brief 拖动事件
    *
    * @param[in] deviceX    触摸画面的x值
    * @param[in] deviceY      触摸画面的y值
    */
    void TouchesMoved(float deviceX, float deviceY);

    /*
    * @brief 拖动事件
    *
    * @param[in] deviceX1   第一个触摸画面的x值
    * @param[in] deviceY1   第一个触摸画面的y值
    * @param[in] deviceX2   第二个触摸画面的x值
    * @param[in] deviceY2   第二个触摸画面的y值
    */
    void TouchesMoved(float deviceX1, float deviceY1, float deviceX2, float deviceY2);

    /*
    * @brief 弗里克测距
    *
    * @return 闪烁距离
    */
    float GetFlickDistance() const;

private:
    /*
    * @brief 求出从点1到点2距离
    *
    * @param[in] x1 第一个触摸画面的x值
    * @param[in] y1 第一个触摸画面的y值
    * @param[in] x2 第二个触摸画面的x值
    * @param[in] y2 第二个触摸画面的y值
    * @return   两点的距离
    */
    float CalculateDistance(float x1, float y1, float x2, float y2) const;

    /*
    * 根据两个值求出移动量
    * 在不同方向的情况下移动量为0 在相同方向的情况下，参照绝对值小的值
    *
    * @param[in] v1    第一个移动量
    * @param[in] v2    第二个移动量
    *
    * @return   小移动量
    */
    float CalculateMovingAmount(float v1, float v2);

    float _startY;              // 开始触摸时的x值
    float _startX;              // 开始触摸时的y值
    float _lastX;               // 单次触摸时x的值
    float _lastY;               // 单次触摸时y的值
    float _lastX1;              // 双击时第一个x的值
    float _lastY1;              // 双击时第一个y的值
    float _lastX2;              // 双击时的第二个x值
    float _lastY2;              // 双击时的第二个y值
    float _lastTouchDistance;   //用2根以上触摸时手指的距离
    float _deltaX;              // 从上一个值到这次值的x的移动距离
    float _deltaY;              // 从上次值到这次值的y的移动距离
    float _scale;               // 在这个框架中相乘的放大率     放大操作中以外为1。
    bool _touchSingle;          // 单触时为真
    bool _flipAvailable;        // 翻转是否有效

};
