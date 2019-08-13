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

#include "Frame.h"
#include "../renderer/ImageCache.h"

namespace kiwano
{
	Frame::Frame()
	{
	}

	Frame::Frame(Resource const& res)
	{
		Load(res);
	}

	Frame::Frame(ImagePtr image)
		: image_(image)
	{
	}

	bool Frame::Load(Resource const& res)
	{
		ImagePtr image = ImageCache::GetInstance()->AddImage(res);
		if (image && image->IsValid())
		{
			SetImage(image);
			return true;
		}
		return false;
	}

	void Frame::Crop(Rect const& crop_rect)
	{
		if (image_)
		{
			auto bitmap_size = image_->GetSize();
			crop_rect_.origin.x = std::min(std::max(crop_rect.origin.x, 0.f), bitmap_size.x);
			crop_rect_.origin.y = std::min(std::max(crop_rect.origin.y, 0.f), bitmap_size.y);
			crop_rect_.size.x = std::min(std::max(crop_rect.size.x, 0.f), bitmap_size.x - crop_rect.origin.x);
			crop_rect_.size.y = std::min(std::max(crop_rect.size.y, 0.f), bitmap_size.y - crop_rect.origin.y);
		}
	}

	void Frame::SetImage(ImagePtr image)
	{
		image_ = image;
		if (image_)
		{
			crop_rect_.origin.x = crop_rect_.origin.y = 0;
			crop_rect_.size.x = image_->GetWidth();
			crop_rect_.size.y = image_->GetHeight();
		}
	}
}
