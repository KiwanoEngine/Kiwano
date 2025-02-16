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

Sprite::Sprite(StringView file_path)
{
    Load(file_path);
}

Sprite::Sprite(const Resource& res)
{
    Load(res);
}

Sprite::Sprite(RefPtr<Bitmap> bitmap)
{
    SetFrame(SpriteFrame(bitmap));
}

Sprite::Sprite(StringView file_path, const Rect& crop_rect)
{
    SetFrame(SpriteFrame(file_path, crop_rect));
}

Sprite::Sprite(const Resource& res, const Rect& crop_rect)
{
    SetFrame(SpriteFrame(res, crop_rect));
}

Sprite::Sprite(RefPtr<Bitmap> bitmap, const Rect& crop_rect)
{
    SetFrame(SpriteFrame(bitmap, crop_rect));
}

Sprite::Sprite(const SpriteFrame& frame)
{
    SetFrame(frame);
}

Sprite::~Sprite() {}

bool Sprite::Load(StringView file_path)
{
    SpriteFrame frame(file_path);
    if (frame.IsValid())
    {
        SetFrame(frame);
        return true;
    }
    Fail("Sprite::Load failed");
    return false;
}

bool Sprite::Load(const Resource& res)
{
    SpriteFrame frame(res);
    if (frame.IsValid())
    {
        SetFrame(frame);
        return true;
    }
    Fail("Sprite::Load failed");
    return false;
}

void Sprite::SetFrame(const SpriteFrame& frame)
{
    frame_ = frame;
    SetSize(frame_.GetSize());

    if (!frame_.IsValid())
    {
        Fail("Sprite::SetFrame failed");
    }
}

void Sprite::OnRender(RenderContext& ctx)
{
    if (frame_.IsValid())
    {
        ctx.DrawBitmap(*frame_.GetBitmap(), &frame_.GetCropRect(), &GetBounds());
    }
}

bool Sprite::CheckVisibility(RenderContext& ctx) const
{
    return frame_.IsValid() && Actor::CheckVisibility(ctx);
}

}  // namespace kiwano
