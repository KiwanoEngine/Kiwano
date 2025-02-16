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

#include <kiwano/render/BitmapCache.h>

namespace kiwano
{

BitmapCache::BitmapCache() {}

BitmapCache::~BitmapCache()
{
    Clear();
}

RefPtr<Bitmap> BitmapCache::Preload(StringView file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (RefPtr<Bitmap> ptr = this->GetBitmap(hash_code))
    {
        return ptr;
    }
    RefPtr<Bitmap> ptr = MakePtr<Bitmap>();
    if (ptr && ptr->Load(file_path))
    {
        this->AddBitmap(hash_code, ptr);
    }
    return ptr;
}

RefPtr<Bitmap> BitmapCache::Preload(const Resource& res)
{
    size_t hash_code = res.GetId();
    if (RefPtr<Bitmap> ptr = this->GetBitmap(hash_code))
    {
        return ptr;
    }
    RefPtr<Bitmap> ptr = MakePtr<Bitmap>();
    if (ptr && ptr->Load(res))
    {
        this->AddBitmap(hash_code, ptr);
    }
    return ptr;
}

RefPtr<GifImage> BitmapCache::PreloadGif(StringView file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (RefPtr<GifImage> ptr = this->GetGifImage(hash_code))
    {
        return ptr;
    }
    RefPtr<GifImage> ptr = MakePtr<GifImage>();
    if (ptr && ptr->Load(file_path))
    {
        this->AddGifImage(hash_code, ptr);
    }
    return ptr;
}

RefPtr<GifImage> BitmapCache::PreloadGif(const Resource& res)
{
    size_t hash_code = res.GetId();
    if (RefPtr<GifImage> ptr = this->GetGifImage(hash_code))
    {
        return ptr;
    }
    RefPtr<GifImage> ptr = MakePtr<GifImage>();
    if (ptr && ptr->Load(res))
    {
        this->AddGifImage(hash_code, ptr);
    }
    return ptr;
}

void BitmapCache::AddBitmap(size_t key, RefPtr<Bitmap> Bitmap)
{
    Bitmap_cache_[key] = Bitmap;
}

void BitmapCache::AddGifImage(size_t key, RefPtr<GifImage> gif)
{
    gif_Bitmap_cache_[key] = gif;
}

RefPtr<Bitmap> BitmapCache::GetBitmap(size_t key) const
{
    if (Bitmap_cache_.count(key))
    {
        return Bitmap_cache_.at(key);
    }
    return RefPtr<Bitmap>();
}

RefPtr<GifImage> BitmapCache::GetGifImage(size_t key) const
{
    if (gif_Bitmap_cache_.count(key))
    {
        return gif_Bitmap_cache_.at(key);
    }
    return RefPtr<GifImage>();
}

void BitmapCache::RemoveBitmap(size_t key)
{
    Bitmap_cache_.erase(key);
}

void BitmapCache::RemoveGifImage(size_t key)
{
    gif_Bitmap_cache_.erase(key);
}

void BitmapCache::Clear()
{
    Bitmap_cache_.clear();
    gif_Bitmap_cache_.clear();
}

}  // namespace kiwano
