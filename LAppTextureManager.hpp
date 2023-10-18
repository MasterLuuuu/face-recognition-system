/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <wincodec.h> // for IWICImagingFactory
#include <WICTextureLoader.h>

#include <string>

#include <Rendering/D3D11/CubismNativeInclude_D3D11.hpp>
#include <Type/CubismBasicType.hpp>
#include <Type/csmVector.hpp>


/**
* @brief 纹理管理类
*
* 进行图像读取、管理的类
*/
class LAppTextureManager
{
public:

    /**
    * @brief 画像信息结构体
    */
    struct TextureInfo
    {
        Csm::csmUint64 id;      // 纹理标识
        int width;              ///< 宽度
        int height;             ///< 高度
        std::string fileName;   // 文件名
    };

    LAppTextureManager();

    ~LAppTextureManager();


    /**
    * @brief 预倍增处理
    *
    * @param[in] red  画像的Red値
    * @param[in] green  画像的Green値
    * @param[in] blue  画像的Blue値
    * @param[in] alpha  画像的Alpha値
    *
    * @return 预倍增处理后颜色值
    */
    inline unsigned int Premultiply(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    {
        return static_cast<unsigned>(\
            (red * (alpha + 1) >> 8) | \
            ((green * (alpha + 1) >> 8) << 8) | \
            ((blue * (alpha + 1) >> 8) << 16) | \
            (((alpha)) << 24)   \
            );
    }

    /**
     * @brief 图像读取
     *
     * @param[in]   fileName    导入的图像文件路径名
     * @param[in]   isPreMult   加载时像素的计算并重新存储值基本上可以为假
     * @param[in]   maxSize     纹理的最大尺寸为0时，采用图像的宽度和高度
     * @return 图像信息   读取失败时返回空值
     */
    TextureInfo* CreateTextureFromPngFile(std::string fileName, bool isPreMult, UINT maxSize = 0);

    /**
    * @brief 图像释放
    *
    * 释放阵列中存在的所有图像
    */
    void ReleaseTextures();

    /**
     * @brief 图像释放
     *
     * 释放指定纹理ID的图像
     * @param[in] textureId  要释放的纹理ID
     **/
    void ReleaseTexture(Csm::csmUint64 textureId);

    /**
     * @brief 获取纹理采样器
     *
     * @param   textureId[in]       指定想取得纹理ID CreatextureFromGnfFile中返回的info的即可
     * @param   retTexture[out]    成功时，指向IDirect3DTexture9的指针返回
     * @return  如果纹理存在，则返回真值
     */
    bool GetTexture(Csm::csmUint64 textureId, ID3D11ShaderResourceView*& retTexture) const;

    /**
     * @brief 从文件名获得纹理信息
     *
     * @param   textureId[in]       要获取的纹理ID
     * @return  如果纹理存在，则返回TextureInfo
     */
    TextureInfo* GetTextureInfoById(Csm::csmUint64 textureId) const;

private:

    Csm::csmVector<ID3D11Resource*>             _textures;          // DX11纹理
    Csm::csmVector<ID3D11ShaderResourceView*>   _textureView;       // DX11纹理视图
    Csm::csmVector<TextureInfo*>                _texturesInfo;      // 纹理信息
    Csm::csmUint64   _sequenceId;    // 要授予的连续标识
};
