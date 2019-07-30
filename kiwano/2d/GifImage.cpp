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
#include "../base/logs.h"
#include "../platform/modules.h"

namespace kiwano
{
	GifImage::GifImage()
		: animating_(false)
		, next_index_(0)
		, total_loop_count_(1)
		, loop_count_(0)
		, frames_count_(0)
		, disposal_type_(DisposalType::Unknown)
		, width_in_pixels_(0)
		, height_in_pixels_(0)
		, frame_position_{}
		, bg_color_{}
	{
		factory_ = Renderer::Instance()->GetD2DDeviceResources()->GetWICImagingFactory();
		auto ctx = Renderer::Instance()->GetD2DDeviceResources()->GetDeviceContext();

		ThrowIfFailed(
			ctx->CreateCompatibleRenderTarget(&frame_rt_)
		);
	}

	GifImage::GifImage(Resource const& res)
		: GifImage()
	{
		Load(res);
	}

	bool GifImage::Load(Resource const& res)
	{
		HRESULT hr = S_OK;

		next_index_ = 0;
		loop_count_ = 0;
		frames_count_ = 0;
		disposal_type_ = DisposalType::None;
		
		saved_frame_.Reset();
		decoder_.Reset();

		if (res.IsFileType())
		{
			if (!modules::Shlwapi::Get().PathFileExistsW(res.GetFileName().c_str()))
			{
				KGE_WARNING_LOG(L"Gif file '%s' not found!", res.GetFileName().c_str());
				return false;
			}

			hr = factory_->CreateDecoderFromFilename(
				res.GetFileName().c_str(),
				nullptr,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&decoder_);
		}
		else
		{
			LPVOID buffer;
			DWORD buffer_size;
			HRESULT hr = res.Load(buffer, buffer_size) ? S_OK : E_FAIL;

			ComPtr<IWICStream> stream;

			if (SUCCEEDED(hr))
			{
				hr = factory_->CreateStream(&stream);
			}

			if (SUCCEEDED(hr))
			{
				hr = stream->InitializeFromMemory(
					static_cast<WICInProcPointer>(buffer),
					buffer_size
				);
			}

			if (SUCCEEDED(hr))
			{
				hr = factory_->CreateDecoderFromStream(
					stream.Get(),
					nullptr,
					WICDecodeMetadataCacheOnLoad,
					&decoder_
				);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = GetGlobalMetadata();
		}

		if (SUCCEEDED(hr))
		{
			if (frames_count_ > 0)
			{
				hr = ComposeNextFrame();
			}
		}

		return SUCCEEDED(hr);
	}

	void GifImage::Update(Duration dt)
	{
		VisualNode::Update(dt);

		if (animating_)
		{
			frame_elapsed_ += dt;
			if (frame_delay_ <= frame_elapsed_)
			{
				frame_delay_ -= frame_elapsed_;
				frame_elapsed_ = 0;
				ComposeNextFrame();
			}
		}
	}

	void GifImage::Restart()
	{
		animating_ = true;
		next_index_ = 0;
		loop_count_ = 0;
		disposal_type_ = DisposalType::None;
	}

	void GifImage::OnRender()
	{
		if (frame_rt_)
		{
			ComPtr<ID2D1Bitmap> frame_to_render;
			if (SUCCEEDED(frame_rt_->GetBitmap(&frame_to_render)))
			{
				Rect bounds = GetBounds();
				Renderer::Instance()->DrawBitmap(frame_to_render, bounds, bounds);
			}
		}
	}

	HRESULT GifImage::GetRawFrame(UINT frame_index)
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
			hr = factory_->CreateFormatConverter(&converter);
		}

		if (SUCCEEDED(hr))
		{
			hr = converter->Initialize(
				wic_frame.Get(),
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeCustom);
		}

		if (SUCCEEDED(hr))
		{
			auto ctx = Renderer::Instance()->GetD2DDeviceResources()->GetDeviceContext();

			// Create a D2DBitmap from IWICBitmapSource
			raw_frame_.Reset();
			hr = ctx->CreateBitmapFromWicBitmap(
				converter.Get(),
				nullptr,
				&raw_frame_);
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
					frame_position_.left = static_cast<float>(prop_val.uiVal);
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
					frame_position_.top = static_cast<float>(prop_val.uiVal);
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
					frame_position_.right = static_cast<float>(prop_val.uiVal)
						+ frame_position_.left;
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
					frame_position_.bottom = static_cast<float>(prop_val.uiVal)
						+ frame_position_.top;
				}
				PropVariantClear(&prop_val);
			}
		}

		if (SUCCEEDED(hr))
		{
			unsigned int frame_delay = 0;

			hr = metadata_reader->GetMetadataByName(
				L"/grctlext/Delay",
				&prop_val);

			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
				if (SUCCEEDED(hr))
				{
					hr = UIntMult(prop_val.uiVal, 10, &frame_delay);
				}
				PropVariantClear(&prop_val);
			}
			else
			{
				frame_delay = 0;
			}

			if (SUCCEEDED(hr))
			{
				// 插入一个强制延迟
				if (frame_delay < 90)
				{
					frame_delay = 90;
				}

				frame_delay_.SetMilliseconds(static_cast<long>(frame_delay));
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(
				L"/grctlext/Disposal",
				&prop_val);

			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI1) ? S_OK : E_FAIL;
				if (SUCCEEDED(hr))
				{
					disposal_type_ = DisposalType(prop_val.bVal);
				}
			}
			else
			{
				// 获取 DisposalType 失败，可能图片是只有一帧的图片
				disposal_type_ = DisposalType::Unknown;
			}
		}

		::PropVariantClear(&prop_val);
		return hr;
	}

	HRESULT GifImage::GetGlobalMetadata()
	{
		unsigned int width = 0;
		unsigned int height = 0;

		PROPVARIANT prop_val;
		::PropVariantInit(&prop_val);

		ComPtr<IWICMetadataQueryReader> metadata_reader;

		// 获取帧数量
		HRESULT hr = decoder_->GetFrameCount(&frames_count_);

		if (SUCCEEDED(hr))
		{
			hr = decoder_->GetMetadataQueryReader(
				&metadata_reader);
		}

		if (SUCCEEDED(hr))
		{
			// 获取背景色
			if (FAILED(GetBackgroundColor(metadata_reader.Get())))
			{
				// 如果未能获得颜色，则默认为透明
				bg_color_ = D2D1::ColorF(0, 0.f);
			}
		}

		// 获取全局 frame 大小
		if (SUCCEEDED(hr))
		{
			// 获取宽度
			hr = metadata_reader->GetMetadataByName(
				L"/logscrdesc/Width",
				&prop_val);

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
			hr = metadata_reader->GetMetadataByName(
				L"/logscrdesc/Height",
				&prop_val);

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
			hr = metadata_reader->GetMetadataByName(
				L"/logscrdesc/PixelAspectRatio",
				&prop_val);

			if (SUCCEEDED(hr))
			{
				hr = (prop_val.vt == VT_UI1 ? S_OK : E_FAIL);
				if (SUCCEEDED(hr))
				{
					if (prop_val.bVal != 0)
					{
						// 需要计算比率
						// 最高像素 1：4，最宽像素 4：1，增量为 1/64
						float pixel_asp_ratio = (prop_val.bVal + 15.f) / 64.f;

						// 根据像素长宽比计算像素中的图像宽度和高度，只缩小图像
						if (pixel_asp_ratio > 1.f)
						{
							width_in_pixels_ = width;
							height_in_pixels_ = static_cast<unsigned int>(height / pixel_asp_ratio);
						}
						else
						{
							width_in_pixels_ = static_cast<unsigned int>(width * pixel_asp_ratio);
							height_in_pixels_ = height;
						}
					}
					else
					{
						// 值为 0, 所以像素比为 1
						width_in_pixels_ = width;
						height_in_pixels_ = height;
					}

					SetSize(static_cast<float>(width_in_pixels_), static_cast<float>(height_in_pixels_));
				}
				::PropVariantClear(&prop_val);
			}
		}

		::PropVariantClear(&prop_val);
		return hr;
	}

	HRESULT GifImage::ComposeNextFrame()
	{
		HRESULT hr = E_FAIL;

		if (frame_rt_)
		{
			// 找到延迟大于 0 的帧 (0 延迟帧是不可见的中间帧)
			do
			{
				hr = DisposeCurrentFrame();
				if (SUCCEEDED(hr))
				{
					hr = OverlayNextFrame();
				}
			} while (SUCCEEDED(hr) && frame_delay_.IsZero() && !IsLastFrame());

			animating_ = (SUCCEEDED(hr) && !EndOfAnimation() && frames_count_ > 1);
		}
		return hr;
	}

	HRESULT GifImage::DisposeCurrentFrame()
	{
		HRESULT hr = S_OK;

		switch (disposal_type_)
		{
		case DisposalType::Unknown:
		case DisposalType::None:
			break;
		case DisposalType::Background:
			// 用背景颜色清除当前原始帧覆盖的区域
			hr = ClearCurrentFrameArea();
			break;
		case DisposalType::Previous:
			// 恢复先前构图的帧
			hr = RestoreSavedFrame();
			break;
		default:
			hr = E_FAIL;
		}
		return hr;
	}

	HRESULT GifImage::OverlayNextFrame()
	{
		HRESULT hr = GetRawFrame(next_index_);
		if (SUCCEEDED(hr))
		{
			if (disposal_type_ == DisposalType::Previous)
			{
				hr = SaveComposedFrame();
			}
		}

		if (SUCCEEDED(hr))
		{
			frame_rt_->BeginDraw();

			if (next_index_ == 0)
			{
				// 重新绘制背景
				frame_rt_->Clear(bg_color_);

				loop_count_++;
			}

			frame_rt_->DrawBitmap(raw_frame_.Get(), frame_position_);

			hr = frame_rt_->EndDraw();
		}

		if (SUCCEEDED(hr))
		{
			next_index_ = (++next_index_) % frames_count_;
		}

		if (IsLastFrame() && loop_cb_)
		{
			loop_cb_(loop_count_ - 1);
		}

		if (EndOfAnimation() && done_cb_)
		{
			done_cb_();
		}

		return hr;
	}

	HRESULT GifImage::SaveComposedFrame()
	{
		HRESULT hr = S_OK;

		ComPtr<ID2D1Bitmap> frame_to_be_saved;

		hr = frame_rt_->GetBitmap(&frame_to_be_saved);
		if (SUCCEEDED(hr))
		{
			if (saved_frame_ == nullptr)
			{
				auto size = frame_to_be_saved->GetPixelSize();
				auto prop = D2D1::BitmapProperties(frame_to_be_saved->GetPixelFormat());

				hr = frame_rt_->CreateBitmap(size, prop, &saved_frame_);
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = saved_frame_->CopyFromBitmap(nullptr, frame_to_be_saved.Get(), nullptr);
		}
		return hr;
	}

	HRESULT GifImage::RestoreSavedFrame()
	{
		HRESULT hr = S_OK;

		ComPtr<ID2D1Bitmap> frame_to_copy_to;

		hr = saved_frame_ ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			hr = frame_rt_->GetBitmap(&frame_to_copy_to);
		}

		if (SUCCEEDED(hr))
		{
			hr = frame_to_copy_to->CopyFromBitmap(nullptr, saved_frame_.Get(), nullptr);
		}

		return hr;
	}

	HRESULT GifImage::ClearCurrentFrameArea()
	{
		frame_rt_->BeginDraw();

		frame_rt_->PushAxisAlignedClip(&frame_position_, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		frame_rt_->Clear(bg_color_);
		frame_rt_->PopAxisAlignedClip();

		return frame_rt_->EndDraw();
	}

	HRESULT GifImage::GetBackgroundColor(IWICMetadataQueryReader* metadata_reader)
	{
		DWORD bgcolor = 0;
		BYTE bg_index = 0;
		WICColor bgcolors[256];
		UINT colors_copied = 0;

		PROPVARIANT prop_val;
		PropVariantInit(&prop_val);

		ComPtr<IWICPalette> wic_palette;

		HRESULT hr = metadata_reader->GetMetadataByName(
			L"/logscrdesc/GlobalColorTableFlag",
			&prop_val);

		if (SUCCEEDED(hr))
		{
			hr = (prop_val.vt != VT_BOOL || !prop_val.boolVal) ? E_FAIL : S_OK;
			::PropVariantClear(&prop_val);
		}

		if (SUCCEEDED(hr))
		{
			hr = metadata_reader->GetMetadataByName(
				L"/logscrdesc/BackgroundColorIndex",
				&prop_val);

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
			hr = factory_->CreatePalette(&wic_palette);
		}

		if (SUCCEEDED(hr))
		{
			hr = decoder_->CopyPalette(wic_palette.Get());
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
			// 检查下标
			hr = (bg_index >= colors_copied) ? E_FAIL : S_OK;
		}

		if (SUCCEEDED(hr))
		{
			bgcolor = bgcolors[bg_index];

			// 转换为 ARGB 格式
			float alpha = (bgcolor >> 24) / 255.f;
			bg_color_ = D2D1::ColorF(bgcolor, alpha);
		}
		return hr;
	}

}
