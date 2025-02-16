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
 * @brief ��ֵģʽ
 * @details ��ֵģʽָ����λͼ�����ź���תʱ������ɫ�ļ��㷽ʽ
 */
enum class InterpolationMode
{
    Linear,  ///< ˫���Բ�ֵ������Χ�ĸ����ؽ����������Բ�ֵ���㣬��ͼ��Ŵ�ʱ���ܻ�ģ��
    Nearest  ///< ���ڽ���ֵ��ȡ���ڽ������ص����ɫֵ
};

/// \~chinese
/// @brief ���ش�С
typedef math::Vec2T<uint32_t> PixelSize;

/**
 * \~chinese
 * @brief ���ظ�ʽ
 * @details ���ظ�ʽָ���˴��ڴ����λͼʱ�ڴ��Ժ��ַ�ʽ����
 */
enum class PixelFormat
{
    Bpp32RGBA,
    Bpp32BGRA,
};

/**
 * \~chinese
 * @brief ͼ��λͼ��ʸ��ͼ��
 */
class KGE_API Image : public NativeObject
{
public:
    Image();

    /// \~chinese
    /// @brief ��ȡ���ز�ֵ��ʽ
    InterpolationMode GetInterpolationMode() const;

    /// \~chinese
    /// @brief �������ز�ֵ��ʽ
    void SetInterpolationMode(InterpolationMode mode);

    /// \~chinese
    /// @brief ����Ĭ�ϵ����ز�ֵ��ʽ
    static void SetDefaultInterpolationMode(InterpolationMode mode);

    /// \~chinese
    /// @brief ��ȡĬ�ϵ����ز�ֵ��ʽ
    static InterpolationMode GetDefaultInterpolationMode();

protected:
    InterpolationMode interpolation_mode_;

    static InterpolationMode default_interpolation_mode_;
};

/**
 * \~chinese
 * @brief λͼ
 */
class KGE_API Bitmap : public Image
{
public:
    Bitmap();

    /// \~chinese
    /// @brief �ӱ����ļ�����λͼ
    Bitmap(StringView file_path);

    /// \~chinese
    /// @brief ����Դ����λͼ
    Bitmap(const Resource& res);

    /// \~chinese
    /// @brief ���ڴ����λͼλͼ
    Bitmap(const PixelSize& size, const BinaryData& data, PixelFormat format);

    virtual ~Bitmap();

    /// \~chinese
    /// @brief ���ر����ļ�
    bool Load(StringView file_path);

    /// \~chinese
    /// @brief ������Դ
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief ���ڴ����λͼλͼ
    bool Load(const PixelSize& size, const BinaryData& data, PixelFormat format);

    /// \~chinese
    /// @brief ��ȡλͼ���
    float GetWidth() const;

    /// \~chinese
    /// @brief ��ȡλͼ�߶�
    float GetHeight() const;

    /// \~chinese
    /// @brief ��ȡλͼ��С
    Size GetSize() const;

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
    /// @brief ���ô�С
    void SetSize(const Size& size);

    /// \~chinese
    /// @brief �������ش�С
    void SetSizeInPixels(const PixelSize& size);

    /// \~chinese
    /// @brief ����λͼ
    /// @param copy_from Դλͼ
    void CopyFrom(RefPtr<Bitmap> copy_from);

    /// \~chinese
    /// @brief ����λͼ
    /// @param copy_from Դλͼ
    /// @param src_rect Դλͼ�ü�����
    /// @param dest_point ������Ŀ��λ��
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
