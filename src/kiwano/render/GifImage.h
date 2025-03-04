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
#include <kiwano/core/Time.h>
#include <kiwano/render/Bitmap.h>

namespace kiwano
{

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief GIF图像
 */
class KGE_API GifImage : public NativeObject
{
public:
    GifImage();

    /// \~chinese
    /// @brief 创建GIF图片
    GifImage(StringView file_path);

    /// \~chinese
    /// @brief 创建GIF图片
    GifImage(const Resource& res);

    /// \~chinese
    /// @brief 加载本地GIF图片
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief 加载GIF资源
    bool Load(const Resource& res);

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
    /// @brief 获取帧数量
    uint32_t GetFramesCount() const;

public:
    /// \~chinese
    /// @brief GIF帧的处置方式
    enum class DisposalType
    {
        Unknown,     ///< 未知
        None,        ///< 不处理
        Background,  ///< 背景
        Previous     ///< 恢复前一帧
    };

    /// \~chinese
    /// @brief GIF帧
    struct Frame
    {
        Duration       delay;          ///< 帧延迟
        RefPtr<Bitmap> bitmap;         ///< 帧图像
        Rect           rect;           ///< 绘制区域
        DisposalType   disposal_type;  ///< 处置方式

        Frame();
    };

    /// \~chinese
    /// @brief 获取GIF帧
    /// @param index 帧下标
    Frame GetFrame(uint32_t index);

private:
    bool GetGlobalMetadata();

private:
    uint32_t  frames_count_;
    PixelSize size_in_pixels_;
};

/** @} */

inline GifImage::Frame::Frame()
    : disposal_type(DisposalType::Unknown)
{
}

inline uint32_t GifImage::GetWidthInPixels() const
{
    return size_in_pixels_.x;
}

inline uint32_t GifImage::GetHeightInPixels() const
{
    return size_in_pixels_.y;
}

inline PixelSize GifImage::GetSizeInPixels() const
{
    return size_in_pixels_;
}

inline uint32_t GifImage::GetFramesCount() const
{
    return frames_count_;
}

}  // namespace kiwano
