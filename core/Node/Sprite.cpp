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

e2d::Sprite::Sprite(const String& filePath, double x, double y, double width, double height)
	: _image(nullptr)
{
	open(filePath);
	crop(x, y, width, height);
}

e2d::Sprite::Sprite(int resNameId, const String& resType, double x, double y, double width, double height)
	: _image(nullptr)
{
	open(resNameId, resType);
	crop(x, y, width, height);
}

e2d::Sprite * e2d::Sprite::create()
{
	return GC::create<Sprite>();
}

e2d::Sprite * e2d::Sprite::create(Image * image)
{
	return GC::create<Sprite>(image);
}

e2d::Sprite * e2d::Sprite::create(const String & filePath)
{
	return GC::create<Sprite>(filePath);
}

e2d::Sprite * e2d::Sprite::create(int resNameId, const String & resType)
{
	return GC::create<Sprite>(resNameId, resType);
}

e2d::Sprite * e2d::Sprite::create(const String & filePath, double x, double y, double width, double height)
{
	return GC::create<Sprite>(filePath, x, y, width, height);
}

e2d::Sprite * e2d::Sprite::create(int resNameId, const String & resType, double x, double y, double width, double height)
{
	return GC::create<Sprite>(resNameId, resType, x, y, width, height);
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
		_image = GC::create<Image>();
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
		_image = GC::create<Image>();
		_image->retain();
	}

	if (_image->open(resNameId, resType))
	{
		Node::setSize(_image->getWidth(), _image->getHeight());
		return true;
	}
	return false;
}

void e2d::Sprite::crop(double x, double y, double width, double height)
{
	_image->crop(x, y, width, height);
	Node::setSize(
		min(max(width, 0), _image->getSourceWidth() - _image->getCropX()),
		min(max(height, 0), _image->getSourceHeight() - _image->getCropY())
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
