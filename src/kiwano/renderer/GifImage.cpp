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

#include "GifImage.h"
#include "Renderer.h"
#include "../base/Logger.h"

namespace kiwano
{
	GifImage::GifImage()
		: frames_count_(0)
		, width_in_pixels_(0)
		, height_in_pixels_(0)
	{
	}

	GifImage::GifImage(String const& file_path)
	{
		Load(file_path);
	}

	GifImage::GifImage(Resource const& res)
		: GifImage()
	{
		Load(res);
	}

	bool GifImage::Load(String const& file_path)
	{
		Renderer::GetInstance()->CreateGifImage(*this, file_path);

		if (IsValid())
		{
			if (FAILED(GetGlobalMetadata()))
			{
				SetDecoder(nullptr);
				return false;
			}
			return true;
		}
		return false;
	}

	bool GifImage::Load(Resource const& res)
	{
		Renderer::GetInstance()->CreateGifImage(*this, res);

		if (IsValid())
		{
			if (FAILED(GetGlobalMetadata()))
			{
				SetDecoder(nullptr);
				return false;
			}
			return true;
		}
		return false;
	}

	bool GifImage::IsValid() const
	{
		return decoder_ != nullptr;
	}

	HRESULT GifImage::GetGlobalMetadata()
	{
		HRESULT hr = decoder_ ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			hr = decoder_->GetFrameCount(&frames_count_);
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<IWICMetadataQueryReader> metadata_reader;
			hr = decoder_->GetMetadataQueryReader(&metadata_reader);

			if (SUCCEEDED(hr))
			{
				UInt32 width = 0;
				UInt32 height = 0;

				PROPVARIANT prop_val;
				::PropVariantInit(&prop_val);

				// 获取全局 frame 大小
				if (SUCCEEDED(hr))
				{
					// 获取宽度
					hr = metadata_reader->GetMetadataByName(L"/logscrdesc/Width", &prop_val);

					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
						if (SUCCEEDED(hr))
						{
							width = prop_val.uiVal;
						}
						::PropVariantClear(&prop_val);
					}
				}

				if (SUCCEEDED(hr))
				{
					// 获取高度
					hr = metadata_reader->GetMetadataByName(L"/logscrdesc/Height", &prop_val);

					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
						if (SUCCEEDED(hr))
						{
							height = prop_val.uiVal;
						}
						::PropVariantClear(&prop_val);
					}
				}

				if (SUCCEEDED(hr))
				{
					// 获得像素纵横比
					hr = metadata_reader->GetMetadataByName(L"/logscrdesc/PixelAspectRatio", &prop_val);

					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI1 ? S_OK : E_FAIL);
						if (SUCCEEDED(hr))
						{
							if (prop_val.bVal != 0)
							{
								// 需要计算比率
								// 最高像素 1：4，最宽像素 4：1，增量为 1/64
								Float32 pixel_asp_ratio = (prop_val.bVal + 15.f) / 64.f;

								// 根据像素长宽比计算像素中的图像宽度和高度，只缩小图像
								if (pixel_asp_ratio > 1.f)
								{
									width_in_pixels_ = width;
									height_in_pixels_ = static_cast<UInt32>(height / pixel_asp_ratio);
								}
								else
								{
									width_in_pixels_ = static_cast<UInt32>(width * pixel_asp_ratio);
									height_in_pixels_ = height;
								}
							}
							else
							{
								// 值为 0, 所以像素比为 1
								width_in_pixels_ = width;
								height_in_pixels_ = height;
							}
						}
						::PropVariantClear(&prop_val);
					}
				}
				::PropVariantClear(&prop_val);
			}
		}
		return hr;
	}

}
