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
 * @brief 精灵
 */
class KGE_API Sprite : public Actor
{
public:
    Sprite();

    /// \~chinese
    /// @brief 创建精灵
    /// @param image 图像
    /// @param src_rect 源矩形（裁剪矩形）
    Sprite(RefPtr<Image> image, const Rect& src_rect = Rect());

    /// \~chinese
    /// @brief 获取图像
    RefPtr<Image> GetImage() const;

    /// \~chinese
    /// @brief 获取位图（仅当设置为位图时生效）
    RefPtr<Bitmap> GetBitmap() const;

    /// \~chinese
    /// @brief 设置精灵图像
    /// @param image 图像
    /// @param src_rect 源矩形（裁剪矩形）
    /// @param reset_size 修正自身大小（目的矩形）
    void SetImage(RefPtr<Image> image, const Rect& src_rect = Rect(), bool reset_size = true);

    /// \~chinese
    /// @brief 设置精灵图像
    /// @param bitmap 位图
    /// @param src_rect 源矩形（裁剪矩形）
    /// @param reset_size 修正自身大小（目的矩形）
    void SetBitmap(RefPtr<Bitmap> bitmap, const Rect& src_rect = Rect(), bool reset_size = true);

    /// \~chinese
    /// @brief 获取源矩形
    Rect GetSourceRect() const;

    /// \~chinese
    /// @brief 设置源矩形
    /// @param src_rect 源矩形（裁剪矩形）
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
