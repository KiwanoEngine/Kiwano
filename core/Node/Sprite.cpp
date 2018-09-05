#include "..\e2dnode.h"
#include "..\e2dmodule.h"

e2d::Sprite::Sprite()
	: image_(nullptr)
{
}

e2d::Sprite::Sprite(Image * image)
	: image_(nullptr)
{
	Open(image);
}

e2d::Sprite::Sprite(const Resource& res)
	: image_(nullptr)
{
	Open(res);
}

e2d::Sprite::Sprite(const Resource& res, const Rect& crop_rect)
	: image_(nullptr)
{
	Open(res);
	Crop(crop_rect);
}

e2d::Sprite::Sprite(const String & file_name)
	: image_(nullptr)
{
	Open(file_name);
}

e2d::Sprite::Sprite(const String & file_name, const Rect & crop_rect)
	: image_(nullptr)
{
	Open(file_name);
	Crop(crop_rect);
}

e2d::Sprite::~Sprite()
{
	GC::GetInstance()->SafeRelease(image_);
}

bool e2d::Sprite::Open(Image * image)
{
	if (image)
	{
		if (image_) image_->Release();
		image_ = image;
		image_->Retain();

		Node::SetSize(image_->GetWidth(), image_->GetHeight());
		return true;
	}
	return false;
}

bool e2d::Sprite::Open(const Resource& res)
{
	if (!image_)
	{
		image_ = new (e2d::autorelease) Image();
		image_->Retain();
	}

	if (image_->Open(res))
	{
		Node::SetSize(image_->GetWidth(), image_->GetHeight());
		return true;
	}
	return false;
}

bool e2d::Sprite::Open(const String & file_name)
{
	if (!image_)
	{
		image_ = new (e2d::autorelease) Image();
		image_->Retain();
	}

	if (image_->Open(file_name))
	{
		Node::SetSize(image_->GetWidth(), image_->GetHeight());
		return true;
	}
	return false;
}

void e2d::Sprite::Crop(const Rect& crop_rect)
{
	image_->Crop(crop_rect);
	Node::SetSize(
		std::min(std::max(crop_rect.size.width, 0.f), image_->GetSourceWidth() - image_->GetCropX()),
		std::min(std::max(crop_rect.size.height, 0.f), image_->GetSourceHeight() - image_->GetCropY())
	);
}

e2d::Image * e2d::Sprite::GetImage() const
{
	return image_;
}

void e2d::Sprite::Draw() const
{
	if (image_ && image_->GetBitmap())
	{
		// »ñÈ¡Í¼Æ¬²Ã¼ôÎ»ÖÃ
		float fCropX = image_->GetCropX();
		float fCropY = image_->GetCropY();
		// äÖÈ¾Í¼Æ¬
		Renderer::GetInstance()->GetRenderTarget()->DrawBitmap(
			image_->GetBitmap(),
			D2D1::RectF(0, 0, size_.width, size_.height),
			display_opacity_,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				fCropX,
				fCropY,
				fCropX + size_.width,
				fCropY + size_.height
			)
		);
	}
}
