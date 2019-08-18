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

#include "Image.h"
#include "Renderer.h"
#include "../base/Logger.h"

namespace kiwano
{
	Image::Image()
	{
	}

	Image::Image(String const& file_path)
	{
		Load(file_path);
	}

	Image::Image(Resource const& res)
	{
		Load(res);
	}

	Image::Image(ComPtr<ID2D1Bitmap> const & bitmap)
	{
		SetBitmap(bitmap);
	}

	Image::~Image()
	{
	}

	bool Image::Load(String const& file_path)
	{
		Renderer::GetInstance()->CreateImage(*this, file_path);
		return IsValid();
	}

	bool Image::Load(Resource const& res)
	{
		Renderer::GetInstance()->CreateImage(*this, res);
		return IsValid();
	}

	bool Image::IsValid() const
	{
		return bitmap_ != nullptr;
	}

	float Image::GetWidth() const
	{
		if (bitmap_)
		{
			return bitmap_->GetSize().width;
		}
		return 0;
	}

	float Image::GetHeight() const
	{
		if (bitmap_)
		{
			return bitmap_->GetSize().height;
		}
		return 0;
	}

	Size Image::GetSize() const
	{
		if (bitmap_)
		{
			auto bitmap_size = bitmap_->GetSize();
			return Size{ bitmap_size.width, bitmap_size.height };
		}
		return Size{};
	}

	UINT32 Image::GetWidthInPixels() const
	{
		if (bitmap_)
		{
			return bitmap_->GetPixelSize().width;
		}
		return 0;
	}

	UINT32 Image::GetHeightInPixels() const
	{
		if (bitmap_)
		{
			return bitmap_->GetPixelSize().height;
		}
		return 0;
	}

	math::Vec2T<UINT32> Image::GetSizeInPixels() const
	{
		if (bitmap_)
		{
			auto bitmap_size = bitmap_->GetPixelSize();
			return math::Vec2T<UINT32>{ bitmap_size.width, bitmap_size.height };
		}
		return math::Vec2T<UINT32>{};
	}

	void Image::CopyFrom(Image const& copy_from)
	{
		if (IsValid() && copy_from.IsValid())
		{
			HRESULT hr = bitmap_->CopyFromBitmap(nullptr, copy_from.GetBitmap().get(), nullptr);

			ThrowIfFailed(hr);
		}
	}

	void Image::CopyFrom(Image const& copy_from, Rect const& src_rect, Point const& dest_point)
	{
		if (IsValid() && copy_from.IsValid())
		{
			HRESULT hr = bitmap_->CopyFromBitmap(
				&D2D1::Point2U(UINT(dest_point.x), UINT(dest_point.y)),
				copy_from.GetBitmap().get(),
				&D2D1::RectU(
					UINT(src_rect.GetLeft()),
					UINT(src_rect.GetTop()),
					UINT(src_rect.GetRight()),
					UINT(src_rect.GetBottom()))
			);

			ThrowIfFailed(hr);
		}
	}

	D2D1_PIXEL_FORMAT Image::GetPixelFormat() const
	{
		if (bitmap_)
		{
			return bitmap_->GetPixelFormat();
		}
		return D2D1_PIXEL_FORMAT();
	}

	ComPtr<ID2D1Bitmap> Image::GetBitmap() const
	{
		return bitmap_;
	}

	void Image::SetBitmap(ComPtr<ID2D1Bitmap> bitmap)
	{
		bitmap_ = bitmap;
	}

}
