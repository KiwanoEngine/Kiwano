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

#include <functional>  // std::hash
#include <kiwano/render/Renderer.h>
#include <kiwano/event/WindowEvent.h>

namespace kiwano
{

Renderer::Renderer()
    : vsync_(true)
    , auto_reset_resolution_(false)
    , clear_color_(Color::Black)
{
}

void Renderer::SetClearColor(const Color& color)
{
    clear_color_ = color;
}

void Renderer::SetVSyncEnabled(bool enabled)
{
    vsync_ = enabled;
}

void Renderer::ResetResolutionWhenWindowResized(bool enabled)
{
    auto_reset_resolution_ = enabled;
}

TexturePtr Renderer::CreateTexture(const String& file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (TexturePtr ptr = texture_cache_.GetTexture(hash_code))
    {
        return ptr;
    }
    TexturePtr ptr = MakePtr<Texture>();
    if (ptr && ptr->Load(file_path))
    {
        texture_cache_.AddTexture(hash_code, ptr);
    }
    return ptr;
}

TexturePtr Renderer::CreateTexture(const Resource& resource)
{
    size_t hash_code = resource.GetId();
    if (TexturePtr ptr = texture_cache_.GetTexture(hash_code))
    {
        return ptr;
    }
    TexturePtr ptr = MakePtr<Texture>();
    if (ptr && ptr->Load(resource))
    {
        texture_cache_.AddTexture(hash_code, ptr);
    }
    return ptr;
}

GifImagePtr Renderer::CreateGifImage(const String& file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (GifImagePtr ptr = texture_cache_.GetGifImage(hash_code))
    {
        return ptr;
    }
    GifImagePtr ptr = MakePtr<GifImage>();
    if (ptr && ptr->Load(file_path))
    {
        texture_cache_.AddGifImage(hash_code, ptr);
    }
    return ptr;
}

GifImagePtr Renderer::CreateGifImage(const Resource& resource)
{
    size_t hash_code = resource.GetId();
    if (GifImagePtr ptr = texture_cache_.GetGifImage(hash_code))
    {
        return ptr;
    }
    GifImagePtr ptr = MakePtr<GifImage>();
    if (ptr && ptr->Load(resource))
    {
        texture_cache_.AddGifImage(hash_code, ptr);
    }
    return ptr;
}

void Renderer::Destroy()
{
    texture_cache_.Clear();
}

void Renderer::HandleEvent(EventModuleContext& ctx)
{
    if (auto_reset_resolution_)
    {
        if (ctx.evt->IsType<WindowResizedEvent>())
        {
            auto evt = ctx.evt->SafeCast<WindowResizedEvent>();
            Resize(evt->width, evt->height);
        }
    }
}

TextureCache& Renderer::GetTextureCache()
{
    return texture_cache_;
}

}  // namespace kiwano
