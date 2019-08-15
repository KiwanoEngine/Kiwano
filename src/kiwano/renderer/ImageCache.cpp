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

#include "ImageCache.h"
#include "Renderer.h"
#include "../base/Logger.h"

namespace kiwano
{

	ImageCache::ImageCache()
	{
	}

	ImageCache::~ImageCache()
	{
	}

	Image ImageCache::AddImage(Resource const& res)
	{
		size_t hash_code = res.GetHashCode();

		auto iter = image_cache_.find(hash_code);
		if (iter != image_cache_.end())
		{
			return iter->second;
		}

		Image image;
		if (image.Load(res))
		{
			image_cache_.insert(std::make_pair(hash_code, image));
		}
		return image;
	}

	void ImageCache::RemoveImage(Resource const& res)
	{
		size_t hash_code = res.GetHashCode();

		auto iter = image_cache_.find(hash_code);
		if (iter != image_cache_.end())
		{
			image_cache_.erase(iter);
		}
	}

	GifImagePtr ImageCache::AddGifImage(Resource const& res)
	{
		size_t hash_code = res.GetHashCode();

		auto iter = gif_image_cache_.find(hash_code);
		if (iter != gif_image_cache_.end())
		{
			return iter->second;
		}

		GifImagePtr ptr = new GifImage;
		if (ptr->Load(res))
		{
			gif_image_cache_.insert(std::make_pair(hash_code, ptr));
		}
		return ptr;
	}

	void ImageCache::RemoveGifImage(Resource const& res)
	{
		size_t hash_code = res.GetHashCode();

		auto iter = gif_image_cache_.find(hash_code);
		if (iter != gif_image_cache_.end())
		{
			gif_image_cache_.erase(iter);
		}
	}

	void ImageCache::Clear()
	{
		image_cache_.clear();
		gif_image_cache_.clear();
	}

}
