/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <Rendering/D3D11/CubismNativeInclude_D3D11.hpp>

#include <CubismFramework.hpp>
#include <Type/CubismBasicType.hpp>



/**
* @brief 模型以外的图像的渲染(背景图像，图标等)
*/
class LAppSprite
{
public:
    /**
    * @brief Rect 结构体
    */
    struct Rect
    {
    public:
        float left;     
        float right;    
        float up;     
        float down;    
    };

    struct SpriteVertex
    {
        float x, y;     // Position
        float u, v;     // UVs
    };

    LAppSprite();

    /**
     *
     * @param[in]       x            x坐标
     * @param[in]       y            y坐标
     * @param[in]       width        宽度
     * @param[in]       height       高度
     * @param[in]       textureId   纹理ID
     */
    LAppSprite(float x, float y, float width, float height, Csm::csmUint64 textureId);

    ~LAppSprite();

    /**
     * @brief Getter 纹理ID
     * @return 返回纹理ID
     */
    Csm::csmUint64 GetTextureId() { return _textureId; }

    /**
     * @brief 绘制
     * @param[in]        width   宽度
     * @param[in]        height  高度
     */
    void Render(int width, int height) const;

    /**
     * @brief 纹理绘制
     * @param[in]        width   宽度
     * @param[in]        height  高度
     * @param[in]        resourceView   使用纹理
     */
    void RenderImmidiate(int width, int height, ID3D11ShaderResourceView* resourceView) const;

    /**
     * @brief 命中检查
     *
     * @param[in]       pointX    x坐标
     * @param[in]       pointY    y坐标
     */
    bool IsHit(float pointX, float pointY) const;

    /**
     * @brief 重新调整大小
     *
     * @param[in]       x            x坐标
     * @param[in]       y            y坐标
     * @param[in]       width        宽度
     * @param[in]       height       高度
     */
    void ResetRect(float x, float y, float width, float height);

    /**
     * @brief 颜色设定
     *
     * @param[in]       r (0.0~1.0)
     * @param[in]       g (0.0~1.0)
     * @param[in]       b (0.0~1.0)
     * @param[in]       a (0.0~1.0)
     */
    void SetColor(float r, float g, float b, float a);

    // 4顶点·6索引仅用于四边形绘制
    static const int VERTEX_NUM = 4;
    static const int INDEX_NUM = 6;

private:

    Csm::csmUint64 _textureId;  // 纹理ID
    Rect _rect;                 // 矩形

    ID3D11Buffer* _vertexBuffer;    // 顶点缓冲区
    ID3D11Buffer* _indexBuffer;     // 索引缓冲器
    ID3D11Buffer* _constantBuffer;  // 常量缓冲器

    DirectX::XMFLOAT4   _color;     // 拼接颜色
};

