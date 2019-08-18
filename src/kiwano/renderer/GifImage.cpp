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
		, bg_color_{}
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
		UInt32 width = 0;
		UInt32 height = 0;

		PROPVARIANT prop_val;
		::PropVariantInit(&prop_val);

		ComPtr<IWICMetadataQueryReader> metadata_reader;

		// 获取帧数量
		HRESULT hr = decoder_ ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			hr = decoder_->GetFrameCount(&frames_count_);
		}

		if (SUCCEEDED(hr))
		{
			hr = decoder_->GetMetadataQueryReader(&metadata_reader);
		}

		if (SUCCEEDED(hr))
		{
			// 获取背景色
			if (FAILED(GetBackgroundColor(metadata_reader.get())))
			{
				// 如果未能获得颜色，则默认为透明
				bg_color_ = Color(0, 0.f);
			}
		}

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
		return hr;
	}

	HRESULT GifImage::GetBackgroundColor(ComPtr<IWICMetadataQueryReader> metadata_reader)
	{
		BYTE bg_index = 0;
		WICColor bgcolors[256];
		UInt32 colors_copied = 0;
		ComPtr<IWICPalette> wic_palette;

		PROPVARIANT prop_val;
		PropVariantInit(&prop_val);

		HRESULT hr = metadata_reader->GetMetadataByName(L"/logscrdesc/GlobalColorTableFlag", &prop_val);

		if (SUCCEEDED(hr))
		{
			hr = (prop_val.vt != VT_BOOL || !prop_val.boolVal) ? E_FAIL : S_OK;
			::PropVariantClear(&prop_val);
		}

		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(L"/logscrdesc/BackgroundColorIndex", &prop_val);

			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt != VT_UI1) ? E_FAIL : S_OK;
				if (SUCCEEDED(hr))
				{
					bg_index = prop_val.bVal;
				}
				::PropVariantClear(&prop_val);
			}
		}

		if (SUCCEEDED(hr))
		{
			auto factory = Renderer::GetInstance()->GetD2DDeviceResources()->GetWICImagingFactory();
			hr = factory->CreatePalette(&wic_palette);
		}

		if (SUCCEEDED(hr))
		{
			hr = decoder_->CopyPalette(wic_palette.get());
		}

		if (SUCCEEDED(hr))
		{
			hr = wic_palette->GetColors(
				ARRAYSIZE(bgcolors),
				bgcolors,
				&colors_copied);
		}

		if (SUCCEEDED(hr))
		{
			hr = (bg_index >= colors_copied) ? E_FAIL : S_OK;
		}

		if (SUCCEEDED(hr))
		{
			// 转换为 ARGB 格式
			Float32 alpha = (bgcolors[bg_index] >> 24) / 255.f;
			bg_color_ = Color(bgcolors[bg_index], alpha);
		}
		return hr;
	}

	HRESULT GifImage::GetRawFrame(UInt32 frame_index, Image& raw_frame, Rect& frame_rect, Duration& delay, DisposalType& disposal_type)
	{
		ComPtr<IWICFormatConverter> converter;
		ComPtr<IWICBitmapFrameDecode> wic_frame;
		ComPtr<IWICMetadataQueryReader> metadata_reader;

		PROPVARIANT prop_val;
		PropVariantInit(&prop_val);

		// Retrieve the current frame
		HRESULT hr = decoder_->GetFrame(frame_index, &wic_frame);
		if (SUCCEEDED(hr))
		{
			// Format convert to 32bppPBGRA which D2D expects
			auto factory = Renderer::GetInstance()->GetD2DDeviceResources()->GetWICImagingFactory();
			hr = factory->CreateFormatConverter(&converter);
		}

		if (SUCCEEDED(hr))
		{
			hr = converter->Initialize(
				wic_frame.get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeCustom);
		}

		if (SUCCEEDED(hr))
		{
			auto ctx = Renderer::GetInstance()->GetD2DDeviceResources()->GetDeviceContext();

			// Create a D2DBitmap from IWICBitmapSource
			ComPtr<ID2D1Bitmap> raw_bitmap;
			hr = ctx->CreateBitmapFromWicBitmap(
				converter.get(),
				nullptr,
				&raw_bitmap
			);

			if (SUCCEEDED(hr))
			{
				raw_frame.SetBitmap(raw_bitmap);
			}
		}

		if (SUCCEEDED(hr))
		{
			// Get Metadata Query Reader from the frame
			hr = wic_frame->GetMetadataQueryReader(&metadata_reader);
		}

		// Get the Metadata for the current frame
		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(L"/imgdesc/Left", &prop_val);
			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
				if (SUCCEEDED(hr))
				{
					frame_rect.origin.x = static_cast<Float32>(prop_val.uiVal);
				}
				PropVariantClear(&prop_val);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(L"/imgdesc/Top", &prop_val);
			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
				if (SUCCEEDED(hr))
				{
					frame_rect.origin.y = static_cast<Float32>(prop_val.uiVal);
				}
				PropVariantClear(&prop_val);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(L"/imgdesc/Width", &prop_val);
			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
				if (SUCCEEDED(hr))
				{
					frame_rect.size.x = static_cast<Float32>(prop_val.uiVal);
				}
				PropVariantClear(&prop_val);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(L"/imgdesc/Height", &prop_val);
			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
				if (SUCCEEDED(hr))
				{
					frame_rect.size.y = static_cast<Float32>(prop_val.uiVal);
				}
				PropVariantClear(&prop_val);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(L"/grctlext/Delay", &prop_val);

			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);

				if (SUCCEEDED(hr))
				{
					UInt32 udelay = 0;
					hr = UIntMult(prop_val.uiVal, 10, &udelay);
					if (SUCCEEDED(hr))
					{
						delay.SetMilliseconds(static_cast<long>(udelay));
					}
				}
				PropVariantClear(&prop_val);
			}
			else
			{
				delay = 0;
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(L"/grctlext/Disposal", &prop_val);

			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI1) ? S_OK : E_FAIL;
				if (SUCCEEDED(hr))
				{
					disposal_type = DisposalType(prop_val.bVal);
				}
				::PropVariantClear(&prop_val);
			}
			else
			{
				// 获取 DisposalType 失败，可能图片是只有一帧的图片
				disposal_type = DisposalType::Unknown;
			}
		}

		::PropVariantClear(&prop_val);
		return hr;
	}

}
