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
#include "../base/logs.h"

namespace kiwano
{

	ImageCache::ImageCache()
	{
	}

	ImageCache::~ImageCache()
	{
	}

	ImagePtr ImageCache::AddImage(Resource const& res)
	{
		size_t hash_code = res.GetHashCode();
		auto iter = image_cache_.find(hash_code);
		if (iter != image_cache_.end())
		{
			return iter->second;
		}

		HRESULT hr = S_OK;
		ComPtr<ID2D1Bitmap> bitmap;

		if (res.IsFileType())
		{
			hr = Renderer::GetInstance()->GetD2DDeviceResources()->CreateBitmapFromFile(bitmap, res.GetFileName());
		}
		else
		{
			hr = Renderer::GetInstance()->GetD2DDeviceResources()->CreateBitmapFromResource(bitmap, res);
		}

		if (SUCCEEDED(hr))
		{
			ImagePtr ptr = new Image(bitmap);
			image_cache_.insert(std::make_pair(hash_code, ptr));
			return ptr;
		}
		else
		{
			KGE_ERROR_LOG(L"Load image file failed with HRESULT of %08X", hr);
		}
		return nullptr;
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

	void ImageCache::Clear()
	{
		image_cache_.clear();
	}

}
