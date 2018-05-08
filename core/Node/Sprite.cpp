#include "..\e2dnode.h"


e2d::Sprite::Sprite()
	: _pImage(nullptr)
{
}

e2d::Sprite::Sprite(Image * image)
	: _pImage(nullptr)
{
	open(image);
}

e2d::Sprite::Sprite(const String& filePath)
	: _pImage(nullptr)
{
	open(filePath);
}

e2d::Sprite::Sprite(int resNameId, const String& resType)
	: _pImage(nullptr)
{
	open(resNameId, resType);
}

e2d::Sprite::Sprite(const String& filePath, double x, double y, double width, double height)
	: _pImage(nullptr)
{
	open(filePath);
	crop(x, y, width, height);
}

e2d::Sprite::Sprite(int resNameId, const String& resType, double x, double y, double width, double height)
	: _pImage(nullptr)
{
	open(resNameId, resType);
	crop(x, y, width, height);
}

e2d::Sprite::~Sprite()
{
}

bool e2d::Sprite::open(Image * image)
{
	if (image)
	{
		SafeRelease(&_pImage);
		_pImage = image;
		_pImage->retain();

		Node::setSize(_pImage->getWidth(), _pImage->getHeight());
		return true;
	}
	return false;
}

bool e2d::Sprite::open(const String& filePath)
{
	if (!_pImage)
	{
		_pImage = new (std::nothrow) Image();
		_pImage->retain();
	}

	if (_pImage->open(filePath))
	{
		Node::setSize(_pImage->getWidth(), _pImage->getHeight());
		return true;
	}
	return false;
}

bool e2d::Sprite::open(int resNameId, const String& resType)
{
	if (!_pImage)
	{
		_pImage = new (std::nothrow) Image();
		_pImage->retain();
	}

	if (_pImage->open(resNameId, resType))
	{
		Node::setSize(_pImage->getWidth(), _pImage->getHeight());
		return true;
	}
	return false;
}

void e2d::Sprite::crop(double x, double y, double width, double height)
{
	_pImage->crop(x, y, width, height);
	Node::setSize(
		min(max(width, 0), _pImage->getSourceWidth() - _pImage->getCropX()),
		min(max(height, 0), _pImage->getSourceHeight() - _pImage->getCropY())
	);
}

e2d::Image * e2d::Sprite::getImage() const
{
	return _pImage;
}

void e2d::Sprite::onRender()
{
	if (_pImage && _pImage->getBitmap())
	{
		// ªÒ»°Õº∆¨≤√ºÙŒª÷√
		float fCropX = static_cast<float>(_pImage->getCropX());
		float fCropY = static_cast<float>(_pImage->getCropY());
		// ‰÷»æÕº∆¨
		Renderer::getRenderTarget()->DrawBitmap(
			_pImage->getBitmap(),
			D2D1::RectF(0, 0, _fWidth, _fHeight),
			_fDisplayOpacity,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(
				fCropX,
				fCropY,
				fCropX + _fWidth,
				fCropY + _fHeight
			)
		);
	}
}

void e2d::Sprite::destroy()
{
	Node::destroy();
	SafeRelease(&_pImage);
}
