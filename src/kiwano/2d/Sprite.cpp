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

#include <kiwano/2d/Sprite.h>
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

Sprite::Sprite() {}

Sprite::Sprite(RefPtr<Image> image, const Rect& src_rect)
{
    auto bitmap = RefPtr<Bitmap>(dynamic_cast<Bitmap*>(image.Get()));
    if (bitmap)
    {
        SetBitmap(bitmap, src_rect);
    }
    else
    {
        SetImage(image, src_rect);
    }
}

void Sprite::SetImage(RefPtr<Image> image, const Rect& src_rect, bool reset_size)
{
    image_    = image;
    src_rect_ = src_rect;
    if (reset_size && !src_rect.IsEmpty())
    {
        SetSize(src_rect.GetSize());
    }
    is_bitmap_ = false;
}

void Sprite::SetBitmap(RefPtr<Bitmap> bitmap, const Rect& src_rect, bool reset_size)
{
    image_    = bitmap;
    src_rect_ = src_rect;
    if (reset_size)
    {
        SetSize(src_rect.IsEmpty() ? bitmap->GetSize() : src_rect.GetSize());
    }
    is_bitmap_ = true;
}

void Sprite::OnRender(RenderContext& ctx)
{
    if (image_ && image_->IsValid())
    {
        auto src_rect = src_rect_.IsEmpty() ? nullptr : &src_rect_;
        if (is_bitmap_)
        {
            auto& bitmap = dynamic_cast<Bitmap&>(*image_);
            ctx.DrawBitmap(bitmap, src_rect, &GetBounds());
        }
        else
        {
            ctx.DrawImage(*image_, src_rect);
        }
    }
}

bool Sprite::CheckVisibility(RenderContext& ctx) const
{
    return image_ && image_->IsValid() && Actor::CheckVisibility(ctx);
}

}  // namespace kiwano
