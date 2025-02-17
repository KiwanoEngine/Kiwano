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
#include <kiwano/2d/Actor.h>

namespace kiwano
{

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief ����
 */
class KGE_API Sprite : public Actor
{
public:
    Sprite();

    /// \~chinese
    /// @brief ��������
    /// @param image ͼ��
    /// @param src_rect Դ���Σ��ü����Σ�
    Sprite(RefPtr<Image> image, const Rect& src_rect = Rect());

    /// \~chinese
    /// @brief ��ȡͼ��
    RefPtr<Image> GetImage() const;

    /// \~chinese
    /// @brief ��ȡλͼ����������Ϊλͼʱ��Ч��
    RefPtr<Bitmap> GetBitmap() const;

    /// \~chinese
    /// @brief ���þ���ͼ��
    /// @param image ͼ��
    /// @param src_rect Դ���Σ��ü����Σ�
    /// @param reset_size ���������С��Ŀ�ľ��Σ�
    void SetImage(RefPtr<Image> image, const Rect& src_rect = Rect(), bool reset_size = true);

    /// \~chinese
    /// @brief ���þ���ͼ��
    /// @param bitmap λͼ
    /// @param src_rect Դ���Σ��ü����Σ�
    /// @param reset_size ���������С��Ŀ�ľ��Σ�
    void SetBitmap(RefPtr<Bitmap> bitmap, const Rect& src_rect = Rect(), bool reset_size = true);

    /// \~chinese
    /// @brief ��ȡԴ����
    Rect GetSourceRect() const;

    /// \~chinese
    /// @brief ����Դ����
    /// @param src_rect Դ���Σ��ü����Σ�
    void SetSourceRect(const Rect& src_rect);

    void OnRender(RenderContext& ctx) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    bool          is_bitmap_ = false;
    RefPtr<Image> image_;
    Rect          src_rect_;
};

/** @} */

inline RefPtr<Image> Sprite::GetImage() const
{
    return image_;
}

inline RefPtr<Bitmap> Sprite::GetBitmap() const
{
    return is_bitmap_ ? RefPtr<Bitmap>(dynamic_cast<Bitmap*>(image_.Get())) : nullptr;
}

inline Rect Sprite::GetSourceRect() const
{
    return src_rect_;
}

inline void Sprite::SetSourceRect(const Rect& src_rect)
{
    src_rect_ = src_rect;
}

}  // namespace kiwano
