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

#include <kiwano/render/Renderer.h>
#include <kiwano/render/Texture.h>
#include <kiwano/render/TextureCache.h>
#include <functional>  // std::hash

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
#include <kiwano/render/DirectX/NativePtr.h>
#endif

namespace kiwano
{

InterpolationMode Texture::default_interpolation_mode_ = InterpolationMode::Linear;

TexturePtr Texture::Preload(const String& file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (TexturePtr ptr = TextureCache::GetInstance().GetTexture(hash_code))
    {
        return ptr;
    }
    TexturePtr ptr = MakePtr<Texture>();
    if (ptr && ptr->Load(file_path))
    {
        TextureCache::GetInstance().AddTexture(hash_code, ptr);
    }
    return ptr;
}

TexturePtr Texture::Preload(const Resource& res)
{
    size_t hash_code = res.GetId();
    if (TexturePtr ptr = TextureCache::GetInstance().GetTexture(hash_code))
    {
        return ptr;
    }
    TexturePtr ptr = MakePtr<Texture>();
    if (ptr && ptr->Load(res))
    {
        TextureCache::GetInstance().AddTexture(hash_code, ptr);
    }
    return ptr;
}

Texture::Texture(const String& file_path)
    : Texture()
{
    Load(file_path);
}

Texture::Texture(const Resource& res)
    : Texture()
{
    Load(res);
}

Texture::Texture()
    : interpolation_mode_(default_interpolation_mode_)
{
}

Texture::~Texture() {}

bool Texture::Load(const String& file_path)
{
    Renderer::GetInstance().CreateTexture(*this, file_path);
    return IsValid();
}

bool Texture::Load(const Resource& res)
{
    Renderer::GetInstance().CreateTexture(*this, res.GetData());
    return IsValid();
}

void Texture::CopyFrom(TexturePtr copy_from)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (IsValid() && copy_from)
    {
        auto native         = NativePtr::Get<ID2D1Bitmap>(this);
        auto native_to_copy = NativePtr::Get<ID2D1Bitmap>(copy_from);

        HRESULT hr = native->CopyFromBitmap(nullptr, native_to_copy.Get(), nullptr);

        KGE_THROW_IF_FAILED(hr, "Copy texture data failed");
    }
#else
    return;  // not supported
#endif
}

void Texture::CopyFrom(TexturePtr copy_from, const Rect& src_rect, const Point& dest_point)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    if (IsValid() && copy_from)
    {
        auto native         = NativePtr::Get<ID2D1Bitmap>(this);
        auto native_to_copy = NativePtr::Get<ID2D1Bitmap>(copy_from);

        HRESULT hr =
            native->CopyFromBitmap(&D2D1::Point2U(uint32_t(dest_point.x), uint32_t(dest_point.y)), native_to_copy.Get(),
                                   &D2D1::RectU(uint32_t(src_rect.GetLeft()), uint32_t(src_rect.GetTop()),
                                                uint32_t(src_rect.GetRight()), uint32_t(src_rect.GetBottom())));

        KGE_THROW_IF_FAILED(hr, "Copy texture data failed");
    }
#else
    return;  // not supported
#endif
}

void Texture::SetInterpolationMode(InterpolationMode mode)
{
    interpolation_mode_ = mode;
    switch (mode)
    {
    case InterpolationMode::Linear:
        break;
    case InterpolationMode::Nearest:
        break;
    default:
        break;
    }
}

void Texture::SetDefaultInterpolationMode(InterpolationMode mode)
{
    default_interpolation_mode_ = mode;
}

InterpolationMode Texture::GetDefaultInterpolationMode()
{
    return default_interpolation_mode_;
}

}  // namespace kiwano
