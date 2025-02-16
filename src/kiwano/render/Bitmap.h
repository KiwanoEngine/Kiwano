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
#include <kiwano/platform/NativeObject.hpp>

namespace kiwano
{

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
 * @brief 像素格式
 * @details 像素格式指定了从内存加载位图时内存以何种方式排列
 */
enum class PixelFormat
{
    Bpp32RGBA,
    Bpp32BGRA,
};

/**
 * \~chinese
 * @brief 图像（位图或矢量图）
 */
class KGE_API Image : public NativeObject
{
public:
    Image();

    /// \~chinese
    /// @brief 获取像素插值方式
    InterpolationMode GetInterpolationMode() const;

    /// \~chinese
    /// @brief 设置像素插值方式
    void SetInterpolationMode(InterpolationMode mode);

    /// \~chinese
    /// @brief 设置默认的像素插值方式
    static void SetDefaultInterpolationMode(InterpolationMode mode);

    /// \~chinese
    /// @brief 获取默认的像素插值方式
    static InterpolationMode GetDefaultInterpolationMode();

protected:
    InterpolationMode interpolation_mode_;

    static InterpolationMode default_interpolation_mode_;
};

/**
 * \~chinese
 * @brief 位图
 */
class KGE_API Bitmap : public Image
{
public:
    Bitmap();

    /// \~chinese
    /// @brief 从本地文件创建位图
    Bitmap(StringView file_path);

    /// \~chinese
    /// @brief 从资源创建位图
    Bitmap(const Resource& res);

    /// \~chinese
    /// @brief 从内存加载位图位图
    Bitmap(const PixelSize& size, const BinaryData& data, PixelFormat format);

    virtual ~Bitmap();

    /// \~chinese
    /// @brief 加载本地文件
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief 加载资源
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief 从内存加载位图位图
    bool Load(const PixelSize& size, const BinaryData& data, PixelFormat format);

    /// \~chinese
    /// @brief 获取位图宽度
    float GetWidth() const;

    /// \~chinese
    /// @brief 获取位图高度
    float GetHeight() const;

    /// \~chinese
    /// @brief 获取位图大小
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
    /// @brief 设置大小
    void SetSize(const Size& size);

    /// \~chinese
    /// @brief 设置像素大小
    void SetSizeInPixels(const PixelSize& size);

    /// \~chinese
    /// @brief 拷贝位图
    /// @param copy_from 源位图
    void CopyFrom(RefPtr<Bitmap> copy_from);

    /// \~chinese
    /// @brief 拷贝位图
    /// @param copy_from 源位图
    /// @param src_rect 源位图裁剪矩形
    /// @param dest_point 拷贝至目标位置
    void CopyFrom(RefPtr<Bitmap> copy_from, const Rect& src_rect, const Point& dest_point);

private:
    Size      size_;
    PixelSize size_in_pixels_;
};

/** @} */

inline InterpolationMode Image::GetInterpolationMode() const
{
    return interpolation_mode_;
}

inline float Bitmap::GetWidth() const
{
    return size_.x;
}

inline float Bitmap::GetHeight() const
{
    return size_.y;
}

inline Size Bitmap::GetSize() const
{
    return size_;
}

inline uint32_t Bitmap::GetWidthInPixels() const
{
    return size_in_pixels_.x;
}

inline uint32_t Bitmap::GetHeightInPixels() const
{
    return size_in_pixels_.y;
}

inline PixelSize Bitmap::GetSizeInPixels() const
{
    return size_in_pixels_;
}

inline void Bitmap::SetSize(const Size& size)
{
    size_ = size;
}

inline void Bitmap::SetSizeInPixels(const PixelSize& size)
{
    size_in_pixels_ = size;
}

}  // namespace kiwano
