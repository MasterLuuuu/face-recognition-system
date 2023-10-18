/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <CubismFramework.hpp>
#include <ICubismAllocator.hpp>

/**
* @brief 一个简易的基于malloc的内存分配器
*/
class LAppAllocator : public Csm::ICubismAllocator
{
    /**
    * @brief  分配内存区域
    *
    *       未被外部调用
    * @param[in]   size    要分配的大小
    * @return  指定的内存区域
    */
    void* Allocate(const Csm::csmSizeType size);

    /**
    * @brief   释放内存空间
    *
    *        未被外部调用
    * @param[in]   memory    要释放的内存
    */
    void Deallocate(void* memory);

    /**
    * @brief
    *       未被外部调用
    * @param[in]   size         要分配的大小
    * @param[in]   alignment    要分配的大小
    * @return  alignedAddress
    */
    void* AllocateAligned(const Csm::csmSizeType size, const Csm::csmUint32 alignment);

    /**
    * @brief        未被外部调用
    * @param[in]   alignedMemory    要释放的内存
    */
    void DeallocateAligned(void* alignedMemory);
};
