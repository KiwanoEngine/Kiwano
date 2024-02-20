// Copyright (c) 2016-2018 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <kiwano/core/Common.h>
#include <kiwano/math/Math.h>
#include <kiwano/render/Texture.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief 精灵帧
 */
class KGE_API SpriteFrame
{
public:
    SpriteFrame();

    /// \~chinese
    /// @brief 创建精灵帧
    /// @param file_path 图像路径
    SpriteFrame(StringView file_path);

    /// \~chinese
    /// @brief 创建精灵帧
    /// @param res 图像资源
    SpriteFrame(const Resource& res);

    /// \~chinese
    /// @brief 创建精灵帧
    /// @param texture 纹理
    SpriteFrame(RefPtr<Texture> texture);

    /// \~chinese
    /// @brief 创建精灵帧
    /// @param file_path 图像路径
    /// @param crop_rect 裁剪矩形
    SpriteFrame(StringView file_path, const Rect& crop_rect);

    /// \~chinese
    /// @brief 创建精灵帧
    /// @param res 图像资源
    /// @param crop_rect 裁剪矩形
    SpriteFrame(const Resource& res, const Rect& crop_rect);

    /// \~chinese
    /// @brief 创建精灵帧
    /// @param texture 纹理
    /// @param crop_rect 裁剪矩形
    SpriteFrame(RefPtr<Texture> texture, const Rect& crop_rect);

    /// \~chinese
    /// @brief 加载图像
    /// @param file_path 图像路径
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief 加载图像
    /// @param res 图像资源
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 获取裁剪矩形
    const Rect& GetCropRect() const;

    /// \~chinese
    /// @brief 获取纹理
    RefPtr<Texture> GetTexture() const;

    /// \~chinese
    /// @brief 获取精灵帧大小
    Size GetSize() const;

    /// \~chinese
    /// @brief 裁剪精灵帧为矩形
    /// @param crop_rect 裁剪矩形
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief 设置纹理并重置裁剪矩形
    /// @param texture 纹理
    void SetTexture(RefPtr<Texture> texture);

    /// \~chinese
    /// @brief 按行列分割精灵帧
    /// @param cols 列数
    /// @param rows 行数
    /// @param max_num 最大帧数量，设-1为将分割后的图像全部作为序列帧
    /// @param padding_x X方向间隔
    /// @param padding_y Y方向间隔
    Vector<SpriteFrame> Split(int cols, int rows, int max_num = -1, float padding_x = 0, float padding_y = 0);

private:
    RefPtr<Texture> texture_;
    Rect            crop_rect_;
};

inline bool SpriteFrame::IsValid() const
{
    return texture_ && texture_->IsValid();
}

inline const Rect& SpriteFrame::GetCropRect() const
{
    return crop_rect_;
}

inline RefPtr<Texture> SpriteFrame::GetTexture() const
{
    return texture_;
}

inline Size SpriteFrame::GetSize() const
{
    return crop_rect_.GetSize();
}

}  // namespace kiwano
