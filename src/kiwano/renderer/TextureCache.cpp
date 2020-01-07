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

#include <kiwano/renderer/TextureCache.h>
#include <kiwano/renderer/Renderer.h>
#include <kiwano/core/Logger.h>

namespace kiwano
{
	template <typename _Ty, typename _PathTy, typename _CacheTy>
	SmartPtr<_Ty> CreateOrGetCache(_CacheTy& cache, _PathTy const& path, size_t hash)
	{
		auto iter = cache.find(hash);
		if (iter != cache.end())
		{
			return iter->second;
		}

		SmartPtr<_Ty> texture = new _Ty;
		if (texture->Load(path))
		{
			cache.insert(std::make_pair(hash, texture));
		}
		return texture;
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

	TextureCache::TextureCache()
	{
	}

	TextureCache::~TextureCache()
	{
	}

	TexturePtr TextureCache::AddOrGetTexture(String const& file_path)
	{
		return CreateOrGetCache<Texture>(texture_cache_, file_path, file_path.hash());
	}

	TexturePtr TextureCache::AddOrGetTexture(Resource const& res)
	{
		return CreateOrGetCache<Texture>(texture_cache_, res, res.GetId());
	}

	GifImagePtr TextureCache::AddOrGetGifImage(String const& file_path)
	{
		return CreateOrGetCache<GifImage>(gif_texture_cache_, file_path, file_path.hash());
	}

	GifImagePtr TextureCache::AddOrGetGifImage(Resource const& res)
	{
		return CreateOrGetCache<GifImage>(gif_texture_cache_, res, res.GetId());
	}

	void TextureCache::RemoveTexture(String const& file_path)
	{
		RemoveCache(texture_cache_, file_path.hash());
	}

	void TextureCache::RemoveTexture(Resource const& res)
	{
		RemoveCache(texture_cache_, res.GetId());
	}

	void TextureCache::RemoveGifImage(String const& file_path)
	{
		RemoveCache(gif_texture_cache_, file_path.hash());
	}

	void TextureCache::RemoveGifImage(Resource const& res)
	{
		RemoveCache(gif_texture_cache_, res.GetId());
	}

	void TextureCache::Clear()
	{
		texture_cache_.clear();
		gif_texture_cache_.clear();
	}

}
