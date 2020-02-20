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
#include <kiwano/render/Renderer.h>

namespace kiwano
{

SpritePtr Sprite::Create(const String& file_path)
{
    SpritePtr ptr = memory::New<Sprite>();
    if (ptr)
    {
        if (!ptr->Load(file_path))
            return nullptr;
    }
    return ptr;
}

SpritePtr Sprite::Create(const Resource& res)
{
    SpritePtr ptr = memory::New<Sprite>();
    if (ptr)
    {
        if (!ptr->Load(res))
            return nullptr;
    }
    return ptr;
}

SpritePtr Sprite::Create(FramePtr frame)
{
    SpritePtr ptr = memory::New<Sprite>();
    if (ptr)
    {
        ptr->SetFrame(frame);
    }
    return ptr;
}

SpritePtr Sprite::Create(const String& file_path, const Rect& crop_rect)
{
    SpritePtr ptr = Sprite::Create(file_path);
    if (ptr)
    {
        ptr->SetCropRect(crop_rect);
    }
    return ptr;
}

SpritePtr Sprite::Create(const Resource& res, const Rect& crop_rect)
{
    SpritePtr ptr = Sprite::Create(res);
    if (ptr)
    {
        ptr->SetCropRect(crop_rect);
    }
    return ptr;
}

Sprite::Sprite() {}

Sprite::~Sprite() {}

bool Sprite::Load(const String& file_path, bool autoresize)
{
    FramePtr frame = Frame::Create(file_path);
    if (frame)
    {
        SetFrame(frame, autoresize);
        return true;
    }
    return false;
}

bool Sprite::Load(const Resource& res, bool autoresize)
{
    FramePtr frame = Frame::Create(res);
    if (frame)
    {
        SetFrame(frame, autoresize);
        return true;
    }
    return false;
}

float Sprite::GetSourceWidth() const
{
    if (frame_)
    {
        return frame_->GetSourceWidth();
    }
    return 0.0f;
}

float Sprite::GetSourceHeight() const
{
    if (frame_)
    {
        return frame_->GetSourceHeight();
    }
    return 0.0f;
}

Size Sprite::GetSourceSize() const
{
    if (frame_)
    {
        return frame_->GetSourceSize();
    }
    return Size();
}

Rect Sprite::GetCropRect() const
{
    if (frame_)
    {
        return frame_->GetCropRect();
    }
    return Rect();
}

void Sprite::SetCropRect(const Rect& crop_rect)
{
    if (frame_)
    {
        frame_->SetCropRect(crop_rect);
    }
}

void Sprite::SetFrame(FramePtr frame, bool autoresize)
{
    if (frame_ != frame)
    {
        frame_ = frame;
        if (frame_ && autoresize)
        {
            SetSize(frame_->GetSize());
        }
    }
}

void Sprite::OnRender(RenderContext& ctx)
{
    if (frame_ && frame_->IsValid())
    {
        ctx.DrawTexture(*frame_->GetTexture(), &frame_->GetCropRect(), &GetBounds());
    }
}

bool Sprite::CheckVisibility(RenderContext& ctx) const
{
    return frame_ && frame_->IsValid() && Actor::CheckVisibility(ctx);
}
}  // namespace kiwano
