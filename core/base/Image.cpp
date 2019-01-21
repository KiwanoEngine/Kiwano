// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include "render.h"
#include "logs.h"
#include "../utils/File.h"
#include "../utils/string.h"

namespace easy2d
{
	Image::Image()
		: bitmap_(nullptr)
		, crop_rect_()
	{
	}

	Image::Image(Resource const& res)
		: Image()
	{
		this->Load(res);
	}

	Image::Image(Resource const& res, Rect const& crop_rect)
		: Image()
	{
		this->Load(res);
		this->Crop(crop_rect);
	}

	Image::Image(CpBitmap const & bitmap)
		: Image()
	{
		SetBitmap(bitmap);
	}

	Image::~Image()
	{
	}

	bool Image::Load(Resource const& res)
	{
		HRESULT hr = S_OK;
		CpBitmap bitmap;

		if (res.IsFile())
		{
			File image_file;
			if (!image_file.Open(res.GetFileName()))
			{
				logs::Warningln("Image file '%s' not found!", StringWideCharToMultiByte(res.GetFileName()).c_str());
				return false;
			}

			// 用户输入的路径不一定是完整路径，因为用户可能通过 File::AddSearchPath 添加
			// 默认搜索路径，所以需要通过 File::GetPath 获取完整路径
			String image_file_path = image_file.GetPath();

			hr = Graphics::Instance()->CreateBitmapFromFile(bitmap, image_file_path);
		}
		else
		{
			hr = Graphics::Instance()->CreateBitmapFromResource(bitmap, res);
		}

		if (FAILED(hr))
		{
			logs::Errorln(hr, "Load image file failed!");
			return false;
		}

		SetBitmap(bitmap);
		return true;
	}

	void Image::Crop(Rect const& crop_rect)
	{
		if (bitmap_)
		{
			auto bitmap_size = bitmap_->GetSize();
			crop_rect_.origin.x = std::min(std::max(crop_rect.origin.x, 0.f), bitmap_size.width);
			crop_rect_.origin.y = std::min(std::max(crop_rect.origin.y, 0.f), bitmap_size.height);
			crop_rect_.size.x = std::min(std::max(crop_rect.size.x, 0.f), bitmap_size.width - crop_rect.origin.x);
			crop_rect_.size.y = std::min(std::max(crop_rect.size.y, 0.f), bitmap_size.height - crop_rect.origin.y);
		}
	}

	float Image::GetWidth() const
	{
		return crop_rect_.size.x;
	}

	float Image::GetHeight() const
	{
		return crop_rect_.size.y;
	}

	Size Image::GetSize() const
	{
		return crop_rect_.size;
	}

	float Image::GetSourceWidth() const
	{
		if (bitmap_)
		{
			return bitmap_->GetSize().width;
		}
		return 0;
	}

	float Image::GetSourceHeight() const
	{
		if (bitmap_)
		{
			return bitmap_->GetSize().height;
		}
		return 0;
	}

	Size Image::GetSourceSize() const
	{
		if (bitmap_)
		{
			auto bitmap_size = bitmap_->GetSize();
			return Size{ bitmap_size.width, bitmap_size.height };
		}
		return Size{};
	}

	float Image::GetCropX() const
	{
		return crop_rect_.origin.x;
	}

	float Image::GetCropY() const
	{
		return crop_rect_.origin.y;
	}

	Point Image::GetCropPos() const
	{
		return crop_rect_.origin;
	}

	Rect const& Image::GetCropRect() const
	{
		return crop_rect_;
	}

	CpBitmap const& Image::GetBitmap() const
	{
		return bitmap_;
	}

	void Image::SetBitmap(CpBitmap const & bitmap)
	{
		if (bitmap)
		{
			bitmap_ = bitmap;
			crop_rect_.origin.x = crop_rect_.origin.y = 0;
			crop_rect_.size.x = bitmap_->GetSize().width;
			crop_rect_.size.y = bitmap_->GetSize().height;
		}
	}

}