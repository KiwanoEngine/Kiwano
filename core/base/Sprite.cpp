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

#include "Sprite.h"
#include "render.h"

namespace easy2d
{
	Sprite::Sprite()
		: image_(nullptr)
	{
	}

	Sprite::Sprite(SpImage const& image)
		: image_(nullptr)
	{
		Load(image);
	}

	Sprite::Sprite(Resource const& res)
		: image_(nullptr)
	{
		Load(res);
	}

	Sprite::Sprite(Resource const& res, const Rect& crop_rect)
		: image_(nullptr)
	{
		Load(res);
		Crop(crop_rect);
	}

	Sprite::Sprite(std::wstring const& file_name)
		: image_(nullptr)
	{
		Load(file_name);
	}

	Sprite::Sprite(std::wstring const& file_name, const Rect & crop_rect)
		: image_(nullptr)
	{
		Load(file_name);
		Crop(crop_rect);
	}

	Sprite::~Sprite()
	{
	}

	bool Sprite::Load(SpImage const& image)
	{
		if (image)
		{
			image_ = image;

			Node::SetSize(image_->GetWidth(), image_->GetHeight());
			return true;
		}
		return false;
	}

	bool Sprite::Load(Resource const& res)
	{
		if (!image_)
		{
			image_ = new (std::nothrow) Image;
		}

		if (image_)
		{
			if (image_->Load(res))
			{
				Node::SetSize(image_->GetWidth(), image_->GetHeight());
				return true;
			}
		}
		return false;
	}

	bool Sprite::Load(std::wstring const& file_name)
	{
		if (!image_)
		{
			image_ = new (std::nothrow) Image;
		}

		if (image_)
		{
			if (image_->Load(file_name))
			{
				Node::SetSize(image_->GetWidth(), image_->GetHeight());
				return true;
			}
		}
		return false;
	}

	void Sprite::Crop(const Rect& crop_rect)
	{
		image_->Crop(crop_rect);
		Node::SetSize(
			std::min(std::max(crop_rect.size.x, 0.f), image_->GetSourceWidth() - image_->GetCropX()),
			std::min(std::max(crop_rect.size.y, 0.f), image_->GetSourceHeight() - image_->GetCropY())
		);
	}

	SpImage const& Sprite::GetImage() const
	{
		return image_;
	}

	void Sprite::OnRender()
	{
		if (image_)
		{
			Graphics::Instance()->DrawImage(image_);
		}
	}
}