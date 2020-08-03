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
#include <kiwano/core/Resource.h>
#include <kiwano/render/NativeObject.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Texture);

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief 插值模式
 * @details 插值模式指定了位图在缩放和旋转时像素颜色的计算方式
 */
enum class InterpolationMode
{
    Linear,  ///< 双线性插值，对周围四个像素进行两次线性插值计算，在图像放大时可能会模糊
    Nearest  ///< 最邻近插值，取最邻近的像素点的颜色值
};

/// \~chinese
/// @brief 像素大小
typedef math::Vec2T<uint32_t> PixelSize;

/**
 * \~chinese
 * @brief 纹理
 */
class KGE_API Texture : public NativeObject
{
public:
    /// \~chinese
    /// @brief 预加载本地图片
    static TexturePtr Preload(const String& file_path);

    /// \~chinese
    /// @brief 预加载图片资源
    static TexturePtr Preload(const Resource& res);

    Texture();

    /// \~chinese
    /// @brief 从本地文件创建纹理
    Texture(const String& file_path);

    /// \~chinese
    /// @brief 从资源创建纹理
    Texture(const Resource& res);

    virtual ~Texture();

    /// \~chinese
    /// @brief 加载本地文件
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief 加载资源
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief 获取纹理宽度
    float GetWidth() const;

    /// \~chinese
    /// @brief 获取纹理高度
    float GetHeight() const;

    /// \~chinese
    /// @brief 获取纹理大小
    Size GetSize() const;

    /// \~chinese
    /// @brief 获取像素宽度
    uint32_t GetWidthInPixels() const;

    /// \~chinese
    /// @brief 获取像素高度
    uint32_t GetHeightInPixels() const;

    /// \~chinese
    /// @brief 获取像素大小
    PixelSize GetSizeInPixels() const;

    /// \~chinese
    /// @brief 获取像素插值方式
    InterpolationMode GetBitmapInterpolationMode() const;
    /// \~chinese
    /// @brief 设置大小
    void SetSize(const Size& size);

    /// \~chinese
    /// @brief 设置像素大小
    void SetSizeInPixels(const PixelSize& size);

    /// \~chinese
    /// @brief 设置像素插值方式
    void SetInterpolationMode(InterpolationMode mode);

    /// \~chinese
    /// @brief 拷贝纹理
    /// @param copy_from 源纹理
    void CopyFrom(TexturePtr copy_from);

    /// \~chinese
    /// @brief 拷贝纹理
    /// @param copy_from 源纹理
    /// @param src_rect 源纹理裁剪矩形
    /// @param dest_point 拷贝至目标位置
    void CopyFrom(TexturePtr copy_from, const Rect& src_rect, const Point& dest_point);

    /// \~chinese
    /// @brief 设置默认的像素插值方式
    static void SetDefaultInterpolationMode(InterpolationMode mode);

    /// \~chinese
    /// @brief 获取默认的像素插值方式
    static InterpolationMode GetDefaultInterpolationMode();

private:
    Size              size_;
    PixelSize         size_in_pixels_;
    InterpolationMode interpolation_mode_;

    static InterpolationMode default_interpolation_mode_;
};

/** @} */

inline float Texture::GetWidth() const
{
    return size_.x;
}

inline float Texture::GetHeight() const
{
    return size_.y;
}

inline Size Texture::GetSize() const
{
    return size_;
}

inline uint32_t Texture::GetWidthInPixels() const
{
    return size_in_pixels_.x;
}

inline uint32_t Texture::GetHeightInPixels() const
{
    return size_in_pixels_.y;
}

inline PixelSize Texture::GetSizeInPixels() const
{
    return size_in_pixels_;
}

inline InterpolationMode Texture::GetBitmapInterpolationMode() const
{
    return interpolation_mode_;
}

inline void Texture::SetSize(const Size& size)
{
    size_ = size;
}

inline void Texture::SetSizeInPixels(const PixelSize& size)
{
    size_in_pixels_ = size;
}

}  // namespace kiwano
