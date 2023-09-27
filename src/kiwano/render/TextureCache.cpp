// Copyright (c) 2016-2019 Kiwano - Nomango
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

#include <kiwano/render/TextureCache.h>

namespace kiwano
{

TextureCache::TextureCache() {}

TextureCache::~TextureCache()
{
    Clear();
}

TexturePtr TextureCache::Preload(const String& file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (TexturePtr ptr = this->GetTexture(hash_code))
    {
        return ptr;
    }
    TexturePtr ptr = MakePtr<Texture>();
    if (ptr && ptr->Load(file_path))
    {
        this->AddTexture(hash_code, ptr);
    }
    return ptr;
}

TexturePtr TextureCache::Preload(const Resource& res)
{
    size_t hash_code = res.GetId();
    if (TexturePtr ptr = this->GetTexture(hash_code))
    {
        return ptr;
    }
    TexturePtr ptr = MakePtr<Texture>();
    if (ptr && ptr->Load(res))
    {
        this->AddTexture(hash_code, ptr);
    }
    return ptr;
}

GifImagePtr TextureCache::PreloadGif(const String& file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (GifImagePtr ptr = this->GetGifImage(hash_code))
    {
        return ptr;
    }
    GifImagePtr ptr = MakePtr<GifImage>();
    if (ptr && ptr->Load(file_path))
    {
        this->AddGifImage(hash_code, ptr);
    }
    return ptr;
}

GifImagePtr TextureCache::PreloadGif(const Resource& res)
{
    size_t hash_code = res.GetId();
    if (GifImagePtr ptr = this->GetGifImage(hash_code))
    {
        return ptr;
    }
    GifImagePtr ptr = MakePtr<GifImage>();
    if (ptr && ptr->Load(res))
    {
        this->AddGifImage(hash_code, ptr);
    }
    return ptr;
}

void TextureCache::AddTexture(size_t key, TexturePtr texture)
{
    texture_cache_[key] = texture;
}

void TextureCache::AddGifImage(size_t key, GifImagePtr gif)
{
    gif_texture_cache_[key] = gif;
}

TexturePtr TextureCache::GetTexture(size_t key) const
{
    if (texture_cache_.count(key))
    {
        return texture_cache_.at(key);
    }
    return TexturePtr();
}

GifImagePtr TextureCache::GetGifImage(size_t key) const
{
    if (gif_texture_cache_.count(key))
    {
        return gif_texture_cache_.at(key);
    }
    return GifImagePtr();
}

void TextureCache::RemoveTexture(size_t key)
{
    texture_cache_.erase(key);
}

void TextureCache::RemoveGifImage(size_t key)
{
    gif_texture_cache_.erase(key);
}

void TextureCache::Clear()
{
    texture_cache_.clear();
    gif_texture_cache_.clear();
}

}  // namespace kiwano
