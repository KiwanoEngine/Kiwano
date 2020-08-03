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
#include <kiwano/render/Texture.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(GifImage);

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief GIFͼ��
 */
class KGE_API GifImage : public NativeObject
{
public:
    /// \~chinese
    /// @brief Ԥ���ر���GIFͼƬ
    static GifImagePtr Preload(const String& file_path);

    /// \~chinese
    /// @brief Ԥ����GIFͼƬ��Դ
    static GifImagePtr Preload(const Resource& res);

    GifImage();

    /// \~chinese
    /// @brief ����GIFͼƬ
    GifImage(const String& file_path);

    /// \~chinese
    /// @brief ����GIFͼƬ
    GifImage(const Resource& res);

    /// \~chinese
    /// @brief ���ر���GIFͼƬ
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief ����GIF��Դ
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief ��ȡ���ؿ��
    uint32_t GetWidthInPixels() const;

    /// \~chinese
    /// @brief ��ȡ���ظ߶�
    uint32_t GetHeightInPixels() const;

    /// \~chinese
    /// @brief ��ȡ���ش�С
    PixelSize GetSizeInPixels() const;

    /// \~chinese
    /// @brief ��ȡ֡����
    uint32_t GetFramesCount() const;

public:
    /// \~chinese
    /// @brief GIF֡�Ĵ��÷�ʽ
    enum class DisposalType
    {
        Unknown,     ///< δ֪
        None,        ///< ������
        Background,  ///< ����
        Previous     ///< �ָ�ǰһ֡
    };

    /// \~chinese
    /// @brief GIF֡
    struct Frame
    {
        Duration     delay;          ///< ֡�ӳ�
        TexturePtr   texture;        ///< ֡ͼ��
        Rect         rect;           ///< ��������
        DisposalType disposal_type;  ///< ���÷�ʽ

        Frame();
    };

    /// \~chinese
    /// @brief ��ȡGIF֡
    /// @param index ֡�±�
    Frame GetFrame(uint32_t index);

private:
    bool GetGlobalMetadata();

private:
    uint32_t frames_count_;
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
