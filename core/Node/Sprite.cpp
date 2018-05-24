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

e2d::Sprite::Sprite(const String& filePath)
	: _image(nullptr)
{
	open(filePath);
}

e2d::Sprite::Sprite(int resNameId, const String& resType)
	: _image(nullptr)
{
	open(resNameId, resType);
}

e2d::Sprite::Sprite(const String& filePath, const Rect& cropRect)
	: _image(nullptr)
{
	open(filePath);
	crop(cropRect);
}

e2d::Sprite::Sprite(int resNameId, const String& resType, const Rect& cropRect)
	: _image(nullptr)
{
	open(resNameId, resType);
	crop(cropRect);
}

e2d::Sprite::~Sprite()
{
}

bool e2d::Sprite::open(Image * image)
{
	if (image)
	{
		GC::release(_image);
		_image = image;
		_image->retain();

		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

bool e2d::Sprite::open(const String& filePath)
{
	if (!_image)
	{
		_image = Create<Image>();
		_image->retain();
	}

	if (_image->open(filePath))
	{
		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

bool e2d::Sprite::open(int resNameId, const String& resType)
{
	if (!_image)
	{
		_image = Create<Image>();
		_image->retain();
	}

	if (_image->open(resNameId, resType))
	{
		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

void e2d::Sprite::crop(const Rect& cropRect)
{
	_image->crop(cropRect);
	Node::setSize(
		min(max(cropRect.size.width, 0), _image->getSourceWidth() - _image->getCropX()),
		min(max(cropRect.size.height, 0), _image->getSourceHeight() - _image->getCropY())
	);
}

e2d::Image * e2d::Sprite::getImage() const
{
	return _image;
}

void e2d::Sprite::onRender()
{
	if (_image && _image->getBitmap())
	{
		// »ñÈ¡Í¼Æ¬²Ã¼ôÎ»ÖÃ
		float fCropX = float(_image->getCropX());
		float fCropY = float(_image->getCropY());
		// äÖÈ¾Í¼Æ¬
		Renderer::getRenderTarget()->DrawBitmap(
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

void e2d::Sprite::onDestroy()
{
	Node::onDestroy();
	GC::release(_image);
}
