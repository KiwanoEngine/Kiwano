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

#include "..\e2dobject.h"
#include "..\e2dmodule.h"

easy2d::Sprite::Sprite()
	: image_(nullptr)
{
}

easy2d::Sprite::Sprite(Image * image)
	: image_(nullptr)
{
	Load(image);
}

easy2d::Sprite::Sprite(const Resource& res)
	: image_(nullptr)
{
	Load(res);
}

easy2d::Sprite::Sprite(const Resource& res, const Rect& crop_rect)
	: image_(nullptr)
{
	Load(res);
	Crop(crop_rect);
}

easy2d::Sprite::Sprite(const String & file_name)
	: image_(nullptr)
{
	Load(file_name);
}

easy2d::Sprite::Sprite(const String & file_name, const Rect & crop_rect)
	: image_(nullptr)
{
	Load(file_name);
	Crop(crop_rect);
}

easy2d::Sprite::~Sprite()
{
	SafeRelease(image_);
}

bool easy2d::Sprite::Load(Image * image)
{
	if (image)
	{
		if (image_)
		{
			image_->Release();
		}

		image_ = image;
		image_->Retain();

		Node::SetSize(image_->GetWidth(), image_->GetHeight());
		return true;
	}
	return false;
}

bool easy2d::Sprite::Load(const Resource& res)
{
	if (!image_)
	{
		image_ = new Image();
		image_->Retain();
	}

	if (image_->Load(res))
	{
		Node::SetSize(image_->GetWidth(), image_->GetHeight());
		return true;
	}
	return false;
}

bool easy2d::Sprite::Load(const String & file_name)
{
	if (!image_)
	{
		image_ = new Image();
		image_->Retain();
	}

	if (image_->Load(file_name))
	{
		Node::SetSize(image_->GetWidth(), image_->GetHeight());
		return true;
	}
	return false;
}

void easy2d::Sprite::Crop(const Rect& crop_rect)
{
	image_->Crop(crop_rect);
	Node::SetSize(
		std::min(std::max(crop_rect.size.width, 0.f), image_->GetSourceWidth() - image_->GetCropX()),
		std::min(std::max(crop_rect.size.height, 0.f), image_->GetSourceHeight() - image_->GetCropY())
	);
}

easy2d::Image * easy2d::Sprite::GetImage() const
{
	return image_;
}

void easy2d::Sprite::OnDraw() const
{
	if (image_ && image_->GetBitmap())
	{
		auto crop_pos = image_->GetCropPos();
		Device::GetGraphics()->GetRenderTarget()->DrawBitmap(
			image_->GetBitmap(),
			D2D1::RectF(0, 0, GetTransform().size.width, GetTransform().size.height),
			GetDisplayOpacity(),
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				crop_pos.x,
				crop_pos.y,
				crop_pos.y + GetTransform().size.width,
				crop_pos.y + GetTransform().size.height
			)
		);
	}
}
