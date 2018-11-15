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

namespace easy2d
{
	Image::Image()
		: bitmap_(nullptr)
		, crop_rect_()
	{
	}

	Image::Image(Resource& res)
		: bitmap_(nullptr)
		, crop_rect_()
	{
		this->Load(res);
	}

	Image::Image(Resource& res, const Rect& crop_rect)
		: bitmap_(nullptr)
		, crop_rect_()
	{
		this->Load(res);
		this->Crop(crop_rect);
	}

	Image::Image(const String & file_name)
		: bitmap_(nullptr)
		, crop_rect_()
	{
		this->Load(file_name);
	}

	Image::Image(const String & file_name, const Rect & crop_rect)
		: bitmap_(nullptr)
		, crop_rect_()
	{
		this->Load(file_name);
		this->Crop(crop_rect);
	}

	Image::~Image()
	{
		SafeRelease(bitmap_);
	}

	bool Image::Load(Resource& res)
	{
		ID2D1Bitmap* bitmap;
		HRESULT hr = devices::Graphics::Instance()->CreateBitmapFromResource(res, &bitmap);
		if (FAILED(hr))
		{
			logs::Trace(L"Load Image from resource failed!", hr);
			return false;
		}
		this->SetBitmap(bitmap);
		return true;
	}

	bool Image::Load(const String & file_name)
	{
		E2D_WARNING_IF(file_name.empty(), "Image Load failed! Invalid file name.");

		File image_file;
		if (!image_file.Open(file_name))
		{
			E2D_WARNING("Image file not found!");
			return false;
		}

		// 用户输入的路径不一定是完整路径，因为用户可能通过 File::AddSearchPath 添加
		// 默认搜索路径，所以需要通过 File::GetPath 获取完整路径
		String image_file_path = image_file.GetPath();

		ID2D1Bitmap* bitmap;
		HRESULT hr = devices::Graphics::Instance()->CreateBitmapFromFile(image_file_path, &bitmap);
		if (FAILED(hr))
		{
			logs::Trace(L"Load Image from file failed!", hr);
			return false;
		}
		this->SetBitmap(bitmap);
		return true;
	}

	void Image::Crop(const Rect& crop_rect)
	{
		if (bitmap_)
		{
			auto bitmap_size = bitmap_->GetSize();
			crop_rect_.origin.x = std::min(std::max(crop_rect.origin.x, 0.f), bitmap_size.width);
			crop_rect_.origin.y = std::min(std::max(crop_rect.origin.y, 0.f), bitmap_size.height);
			crop_rect_.size.width = std::min(std::max(crop_rect.size.width, 0.f), bitmap_size.width - crop_rect.origin.x);
			crop_rect_.size.height = std::min(std::max(crop_rect.size.height, 0.f), bitmap_size.height - crop_rect.origin.y);
		}
	}

	float Image::GetWidth() const
	{
		return crop_rect_.size.width;
	}

	float Image::GetHeight() const
	{
		return crop_rect_.size.height;
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
		else
		{
			return 0;
		}
	}

	float Image::GetSourceHeight() const
	{
		if (bitmap_)
		{
			return bitmap_->GetSize().height;
		}
		else
		{
			return 0;
		}
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

	const Rect & Image::GetCropRect() const
	{
		return crop_rect_;
	}

	ID2D1Bitmap * Image::GetBitmap() const
	{
		return bitmap_;
	}

	void Image::SetBitmap(ID2D1Bitmap * bitmap)
	{
		if (bitmap_ == bitmap)
			return;

		SafeRelease(bitmap_);

		if (bitmap)
		{
			bitmap->AddRef();

			bitmap_ = bitmap;
			crop_rect_.origin.x = crop_rect_.origin.y = 0;
			crop_rect_.size.width = bitmap_->GetSize().width;
			crop_rect_.size.height = bitmap_->GetSize().height;
		}
	}
}