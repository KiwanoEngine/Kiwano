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
 * @brief ����
 */
class KGE_API Texture : public NativeObject
{
public:
    /// \~chinese
    /// @brief Ԥ���ر���ͼƬ
    static TexturePtr Preload(const String& file_path);

    /// \~chinese
    /// @brief Ԥ����ͼƬ��Դ
    static TexturePtr Preload(const Resource& res);

    Texture();

    /// \~chinese
    /// @brief �ӱ����ļ���������
    Texture(const String& file_path);

    /// \~chinese
    /// @brief ����Դ��������
    Texture(const Resource& res);

    virtual ~Texture();

    /// \~chinese
    /// @brief ���ر����ļ�
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief ������Դ
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief ��ȡ������
    float GetWidth() const;

    /// \~chinese
    /// @brief ��ȡ����߶�
    float GetHeight() const;

    /// \~chinese
    /// @brief ��ȡ�����С
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
    /// @brief ��ȡ���ز�ֵ��ʽ
    InterpolationMode GetBitmapInterpolationMode() const;
    /// \~chinese
    /// @brief ���ô�С
    void SetSize(const Size& size);

    /// \~chinese
    /// @brief �������ش�С
    void SetSizeInPixels(const PixelSize& size);

    /// \~chinese
    /// @brief �������ز�ֵ��ʽ
    void SetInterpolationMode(InterpolationMode mode);

    /// \~chinese
    /// @brief ��������
    /// @param copy_from Դ����
    void CopyFrom(TexturePtr copy_from);

    /// \~chinese
    /// @brief ��������
    /// @param copy_from Դ����
    /// @param src_rect Դ����ü�����
    /// @param dest_point ������Ŀ��λ��
    void CopyFrom(TexturePtr copy_from, const Rect& src_rect, const Point& dest_point);

    /// \~chinese
    /// @brief ����Ĭ�ϵ����ز�ֵ��ʽ
    static void SetDefaultInterpolationMode(InterpolationMode mode);

    /// \~chinese
    /// @brief ��ȡĬ�ϵ����ز�ֵ��ʽ
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
