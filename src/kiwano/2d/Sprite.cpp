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

Sprite::Sprite(const String& file_path)
{
    Load(file_path);
}

Sprite::Sprite(const Resource& res)
{
    Load(res);
}

Sprite::Sprite(TexturePtr texture)
{
    SetTexture(texture);
}

Sprite::Sprite(const String& file_path, const Rect& crop_rect)
    : Sprite(file_path)
{
    SetCropRect(crop_rect);
}

Sprite::Sprite(const Resource& res, const Rect& crop_rect)
    : Sprite(res)
{
    SetCropRect(crop_rect);
}

Sprite::Sprite(TexturePtr texture, const Rect& crop_rect)
{
    SetTexture(texture);
    SetCropRect(crop_rect);
}

Sprite::~Sprite() {}

bool Sprite::Load(const String& file_path)
{
    TexturePtr texture = MakePtr<Texture>(file_path);
    if (texture && texture->IsValid())
    {
        SetTexture(texture);
        return true;
    }
    Fail("Sprite::Load failed");
    return false;
}

bool Sprite::Load(const Resource& res)
{
    TexturePtr texture = MakePtr<Texture>(res);
    if (texture && texture->IsValid())
    {
        SetTexture(texture);
        return true;
    }
    Fail("Sprite::Load failed");
    return false;
}

void Sprite::SetTexture(TexturePtr texture)
{
    texture_ = texture;

    Size texture_size;
    if (texture_)
    {
        texture_size = texture_->GetSize();
    }
    // ÖØÖÃ²Ã¼ô¾ØÐÎ
    SetCropRect(Rect(Point(), texture_size));
}

void Sprite::OnRender(RenderContext& ctx)
{
    if (texture_ && texture_->IsValid())
    {
        ctx.DrawTexture(*texture_, &crop_rect_, &GetBounds());
    }
}

bool Sprite::CheckVisibility(RenderContext& ctx) const
{
    return texture_ && texture_->IsValid() && Actor::CheckVisibility(ctx);
}
}  // namespace kiwano
