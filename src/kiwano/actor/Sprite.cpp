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

#include <kiwano/actor/Sprite.h>
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

Sprite::Sprite(RefPtr<Texture> texture)
{
    SetFrame(SpriteFrame(texture));
}

Sprite::Sprite(StringView file_path, const Rect& crop_rect)
{
    SetFrame(SpriteFrame(file_path, crop_rect));
}

Sprite::Sprite(const Resource& res, const Rect& crop_rect)
{
    SetFrame(SpriteFrame(res, crop_rect));
}

Sprite::Sprite(RefPtr<Texture> texture, const Rect& crop_rect)
{
    SetFrame(SpriteFrame(texture, crop_rect));
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

    if (frame_.IsValid())
    {
        if (!render_comp_)
        {
            render_comp_ = new TextureRenderAbility();
            AddComponent(render_comp_);
        }

        render_comp_->SetTexture(frame_.GetTexture());
        render_comp_->SetSourceRect(frame_.GetCropRect());
    }
    else
    {
        Fail("Sprite::SetFrame failed");
    }
}

void Sprite::SetCropRect(const Rect& crop_rect)
{
    frame_.SetCropRect(crop_rect);
    render_comp_->SetSourceRect(crop_rect);
}

}  // namespace kiwano
