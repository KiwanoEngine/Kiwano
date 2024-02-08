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

RefPtr<Texture> TextureCache::Preload(StringView file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (RefPtr<Texture> ptr = this->GetTexture(hash_code))
    {
        return ptr;
    }
    RefPtr<Texture> ptr =  MakePtr<Texture>();
    if (ptr && ptr->Load(file_path))
    {
        this->AddTexture(hash_code, ptr);
    }
    return ptr;
}

RefPtr<Texture> TextureCache::Preload(const Resource& res)
{
    size_t hash_code = res.GetId();
    if (RefPtr<Texture> ptr = this->GetTexture(hash_code))
    {
        return ptr;
    }
    RefPtr<Texture> ptr =  MakePtr<Texture>();
    if (ptr && ptr->Load(res))
    {
        this->AddTexture(hash_code, ptr);
    }
    return ptr;
}

RefPtr<GifImage> TextureCache::PreloadGif(StringView file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (RefPtr<GifImage> ptr = this->GetGifImage(hash_code))
    {
        return ptr;
    }
    RefPtr<GifImage> ptr =  MakePtr<GifImage>();
    if (ptr && ptr->Load(file_path))
    {
        this->AddGifImage(hash_code, ptr);
    }
    return ptr;
}

RefPtr<GifImage> TextureCache::PreloadGif(const Resource& res)
{
    size_t hash_code = res.GetId();
    if (RefPtr<GifImage> ptr = this->GetGifImage(hash_code))
    {
        return ptr;
    }
    RefPtr<GifImage> ptr =  MakePtr<GifImage>();
    if (ptr && ptr->Load(res))
    {
        this->AddGifImage(hash_code, ptr);
    }
    return ptr;
}

void TextureCache::AddTexture(size_t key, RefPtr<Texture> texture)
{
    texture_cache_[key] = texture;
}

void TextureCache::AddGifImage(size_t key, RefPtr<GifImage> gif)
{
    gif_texture_cache_[key] = gif;
}

RefPtr<Texture> TextureCache::GetTexture(size_t key) const
{
    if (texture_cache_.count(key))
    {
        return texture_cache_.at(key);
    }
    return RefPtr<Texture>();
}

RefPtr<GifImage> TextureCache::GetGifImage(size_t key) const
{
    if (gif_texture_cache_.count(key))
    {
        return gif_texture_cache_.at(key);
    }
    return RefPtr<GifImage>();
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
