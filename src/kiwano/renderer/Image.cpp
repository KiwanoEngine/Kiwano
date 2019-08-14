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
#include "../base/Logger.h"
#include "../platform/modules.h"

namespace kiwano
{
	Image::Image()
		: bitmap_(nullptr)
	{
	}

	Image::Image(ComPtr<ID2D1Bitmap> const & bitmap)
		: Image()
	{
		SetBitmap(bitmap);
	}

	Image::~Image()
	{
	}

	bool Image::IsValid() const
	{
		return !!bitmap_;
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

	ComPtr<ID2D1Bitmap> Image::GetBitmap() const
	{
		return bitmap_;
	}

	void Image::SetBitmap(ComPtr<ID2D1Bitmap> bitmap)
	{
		bitmap_ = bitmap;
	}

}
