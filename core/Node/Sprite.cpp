#include "..\e2dnode.h"


e2d::Sprite::Sprite()
	: _image(nullptr)
{
}

e2d::Sprite::Sprite(Image * image)
	: _image(nullptr)
{
	open(image);
}

e2d::Sprite::Sprite(const Resource& res)
	: _image(nullptr)
{
	open(res);
}

e2d::Sprite::Sprite(const Resource& res, const Rect& cropRect)
	: _image(nullptr)
{
	open(res);
	crop(cropRect);
}

e2d::Sprite::Sprite(const String & fileName)
	: _image(nullptr)
{
	open(fileName);
}

e2d::Sprite::Sprite(const String & fileName, const Rect & cropRect)
	: _image(nullptr)
{
	open(fileName);
	crop(cropRect);
}

e2d::Sprite::~Sprite()
{
	GC::getInstance()->safeRelease(_image);
}

bool e2d::Sprite::open(Image * image)
{
	if (image)
	{
		if (_image) _image->release();
		_image = image;
		_image->retain();

		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

bool e2d::Sprite::open(const Resource& res)
{
	if (!_image)
	{
		_image = new (e2d::autorelease) Image();
		_image->retain();
	}

	if (_image->open(res))
	{
		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

bool e2d::Sprite::open(const String & fileName)
{
	return open(Resource(fileName));
}

void e2d::Sprite::crop(const Rect& cropRect)
{
	_image->crop(cropRect);
	Node::setSize(
		std::min(std::max(cropRect.size.width, 0.f), _image->getSourceWidth() - _image->getCropX()),
		std::min(std::max(cropRect.size.height, 0.f), _image->getSourceHeight() - _image->getCropY())
	);
}

e2d::Image * e2d::Sprite::getImage() const
{
	return _image;
}

void e2d::Sprite::draw(Renderer * renderer) const
{
	if (_image && _image->getBitmap())
	{
		// »ñÈ¡Í¼Æ¬²Ã¼ôÎ»ÖÃ
		float fCropX = _image->getCropX();
		float fCropY = _image->getCropY();
		// äÖÈ¾Í¼Æ¬
		renderer->getRenderTarget()->DrawBitmap(
			_image->getBitmap(),
			D2D1::RectF(0, 0, _width, _height),
			_displayOpacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				fCropX,
				fCropY,
				fCropX + _width,
				fCropY + _height
			)
		);
	}
}
