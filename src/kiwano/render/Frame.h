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
#include <kiwano/base/ObjectBase.h>
#include <kiwano/render/Texture.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(Frame);

/**
 * \~chinese
 * @brief 图像帧
 */
class KGE_API Frame : public ObjectBase
{
public:
    /// \~chinese
    /// @brief 创建图像帧
    /// @param file_path 图像路径
    Frame(const String& file_path);

    /// \~chinese
    /// @brief 创建图像帧
    /// @param res 图像资源
    Frame(const Resource& res);

    /// \~chinese
    /// @brief 创建图像帧
    /// @param texture 纹理
    Frame(TexturePtr texture);

    /// \~chinese
    /// @brief 构建空图像帧
    Frame();

    /// \~chinese
    /// @brief 加载图像
    /// @param file_path 图像路径
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief 加载图像
    /// @param res 图像资源
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief 裁剪图像帧为矩形
    /// @param crop_rect 裁剪矩形定义
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief 设置纹理
    /// @param texture 纹理
    void SetTexture(TexturePtr texture);

    /// \~chinese
    /// @brief 是否有效
    bool IsValid() const;

    /// \~chinese
    /// @brief 获取宽度
    float GetWidth() const;

    /// \~chinese
    /// @brief 获取高度
    float GetHeight() const;

    /// \~chinese
    /// @brief 获取大小
    Size GetSize() const;

    /// \~chinese
    /// @brief 获取裁剪位置
    Point GetCropPoint() const;

    /// \~chinese
    /// @brief 获取裁剪矩形
    const Rect& GetCropRect() const;

    /// \~chinese
    /// @brief 获取图像原宽度
    float GetSourceWidth() const;

    /// \~chinese
    /// @brief 获取图像原高度
    float GetSourceHeight() const;

    /// \~chinese
    /// @brief 获取图像原大小
    Size GetSourceSize() const;

    /// \~chinese
    /// @brief 获取纹理
    TexturePtr GetTexture() const;

private:
    TexturePtr texture_;
    Rect       crop_rect_;
};

inline bool Frame::IsValid() const
{
    return texture_ && texture_->IsValid();
}

inline float Frame::GetWidth() const
{
    return crop_rect_.GetWidth();
}

inline float Frame::GetHeight() const
{
    return crop_rect_.GetHeight();
}

inline Size Frame::GetSize() const
{
    return crop_rect_.GetSize();
}

inline Point Frame::GetCropPoint() const
{
    return crop_rect_.GetLeftTop();
}

inline const Rect& Frame::GetCropRect() const
{
    return crop_rect_;
}

inline float Frame::GetSourceWidth() const
{
    if (texture_)
    {
        return texture_->GetWidth();
    }
    return 0.0f;
}

inline float Frame::GetSourceHeight() const
{
    if (texture_)
    {
        return texture_->GetHeight();
    }
    return 0.0f;
}

inline Size Frame::GetSourceSize() const
{
    if (texture_)
    {
        return texture_->GetSize();
    }
    return Size();
}

inline TexturePtr Frame::GetTexture() const
{
    return texture_;
}

}  // namespace kiwano
