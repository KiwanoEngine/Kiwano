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
	template <typename _Ty, typename _PathTy, typename _CacheTy>
	_Ty CreateOrGetCache(_CacheTy& cache, _PathTy const& path, size_t hash)
	{
		auto iter = cache.find(hash);
		if (iter != cache.end())
		{
			return iter->second;
		}

		_Ty image;
		if (image.Load(path))
		{
			cache.insert(std::make_pair(hash, image));
		}
		return image;
	}

	template <typename _CacheTy>
	void RemoveCache(_CacheTy& cache, size_t hash)
	{
		auto iter = cache.find(hash);
		if (iter != cache.end())
		{
			cache.erase(iter);
		}
	}

	ImageCache::ImageCache()
	{
	}

	ImageCache::~ImageCache()
	{
	}

	Image ImageCache::AddOrGetImage(String const& file_path)
	{
		return CreateOrGetCache<Image>(image_cache_, file_path, file_path.hash());
	}

	Image ImageCache::AddOrGetImage(Resource const& res)
	{
		return CreateOrGetCache<Image>(image_cache_, res, res.GetId());
	}

	GifImage ImageCache::AddOrGetGifImage(String const& file_path)
	{
		return CreateOrGetCache<GifImage>(gif_image_cache_, file_path, file_path.hash());
	}

	GifImage ImageCache::AddOrGetGifImage(Resource const& res)
	{
		return CreateOrGetCache<GifImage>(gif_image_cache_, res, res.GetId());
	}

	void ImageCache::RemoveImage(String const& file_path)
	{
		RemoveCache(image_cache_, file_path.hash());
	}

	void ImageCache::RemoveImage(Resource const& res)
	{
		RemoveCache(image_cache_, res.GetId());
	}

	void ImageCache::RemoveGifImage(String const& file_path)
	{
		RemoveCache(gif_image_cache_, file_path.hash());
	}

	void ImageCache::RemoveGifImage(Resource const& res)
	{
		RemoveCache(gif_image_cache_, res.GetId());
	}

	void ImageCache::Clear()
	{
		image_cache_.clear();
		gif_image_cache_.clear();
	}

}
