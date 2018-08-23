#include "..\e2dcommon.h"
#include "..\e2dbase.h"
#include "..\e2dtool.h"

std::map<size_t, ID2D1Bitmap*> e2d::Image::_bitmapCache;

e2d::Image::Image()
	: _bitmap(nullptr)
	, _cropRect()
{
}

e2d::Image::Image(const Resource& res)
	: _bitmap(nullptr)
	, _cropRect()
{
	this->open(res);
}

e2d::Image::Image(const Resource& res, const Rect& cropRect)
	: _bitmap(nullptr)
	, _cropRect()
{
	this->open(res);
	this->crop(cropRect);
}

e2d::Image::Image(const String & fileName)
	: _bitmap(nullptr)
	, _cropRect()
{
	this->open(fileName);
}

e2d::Image::Image(const String & fileName, const Rect & cropRect)
	: _bitmap(nullptr)
	, _cropRect()
{
	this->open(fileName);
	this->crop(cropRect);
}

e2d::Image::~Image()
{
}

bool e2d::Image::open(const Resource& res)
{
	if (!Image::preload(res))
	{
		WARN("Load Image from file failed!");
		return false;
	}

	this->_setBitmap(_bitmapCache.at(res.resNameId));
	return true;
}

bool e2d::Image::open(const String & fileName)
{
	WARN_IF(fileName.isEmpty(), "Image open failed! Invalid file name.");

	if (fileName.isEmpty())
		return false;

	if (!Image::preload(fileName))
	{
		WARN("Load Image from file failed!");
		return false;
	}

	this->_setBitmap(_bitmapCache.at(fileName.hash()));
	return true;
}

void e2d::Image::crop(const Rect& cropRect)
{
	if (_bitmap)
	{
		_cropRect.origin.x = std::min(std::max(cropRect.origin.x, 0.f), this->getSourceWidth());
		_cropRect.origin.y = std::min(std::max(cropRect.origin.y, 0.f), this->getSourceHeight());
		_cropRect.size.width = std::min(std::max(cropRect.size.width, 0.f), this->getSourceWidth() - cropRect.origin.x);
		_cropRect.size.height = std::min(std::max(cropRect.size.height, 0.f), this->getSourceHeight() - cropRect.origin.y);
	}
}

float e2d::Image::getWidth() const
{
	return _cropRect.size.width;
}

float e2d::Image::getHeight() const
{
	return _cropRect.size.height;
}

e2d::Size e2d::Image::getSize() const
{
	return _cropRect.size;
}

float e2d::Image::getSourceWidth() const
{
	if (_bitmap)
	{
		return _bitmap->GetSize().width;
	}
	else
	{
		return 0;
	}
}

float e2d::Image::getSourceHeight() const
{
	if (_bitmap)
	{
		return _bitmap->GetSize().height;
	}
	else
	{
		return 0;
	}
}

e2d::Size e2d::Image::getSourceSize() const
{
	if (_bitmap)
	{
		return Size(getSourceWidth(), getSourceHeight());
	}
	else
	{
		return Size();
	}
}

float e2d::Image::getCropX() const
{
	return _cropRect.origin.x;
}

float e2d::Image::getCropY() const
{
	return _cropRect.origin.y;
}

e2d::Point e2d::Image::getCropPos() const
{
	return _cropRect.origin;
}

bool e2d::Image::preload(const Resource& res)
{
	if (_bitmapCache.find(res.resNameId) != _bitmapCache.end())
	{
		return true;
	}

	Renderer* renderer = Game::getInstance()->getRenderer();
	ID2D1HwndRenderTarget* pRenderTarget = renderer->getRenderTarget();
	IWICImagingFactory *pImagingFactory = renderer->getImagingFactory();
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	ID2D1Bitmap *pBitmap = nullptr;
	HRSRC imageResHandle = nullptr;
	HGLOBAL imageResDataHandle = nullptr;
	void *pImageFile = nullptr;
	DWORD imageFileSize = 0;

	// 定位资源
	imageResHandle = ::FindResourceW(
		HINST_THISCOMPONENT, 
		MAKEINTRESOURCE(res.resNameId), 
		(LPCWSTR)res.resType
	);

	HRESULT hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// 加载资源
		imageResDataHandle = ::LoadResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageResDataHandle ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// 获取文件指针，并锁定资源
		pImageFile = ::LockResource(imageResDataHandle);

		hr = pImageFile ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// 计算大小
		imageFileSize = ::SizeofResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// 创建 WIC 流
		hr = pImagingFactory->CreateStream(&pStream);
	}

	if (SUCCEEDED(hr))
	{
		// 初始化流
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}

	if (SUCCEEDED(hr))
	{
		// 创建流的解码器
		hr = pImagingFactory->CreateDecoderFromStream(
			pStream,
			nullptr,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}

	if (SUCCEEDED(hr))
	{
		// 创建初始化框架
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// 创建图片格式转换器
		hr = pImagingFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		// 图片格式转换成 32bppPBGRA
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		// 从 WIC 位图创建一个 Direct2D 位图
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			&pBitmap
		);
	}

	if (SUCCEEDED(hr))
	{
		_bitmapCache.insert(std::make_pair(res.resNameId, pBitmap));
	}

	// 释放相关资源
	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pStream);
	SafeRelease(pConverter);

	return SUCCEEDED(hr);
}

bool e2d::Image::preload(const String & fileName)
{
	String actualFilePath = File(fileName).getFilePath();
	if (actualFilePath.isEmpty())
		return false;

	size_t hash = actualFilePath.hash();
	if (_bitmapCache.find(hash) != _bitmapCache.end())
		return true;

	Renderer* renderer = Game::getInstance()->getRenderer();
	ID2D1HwndRenderTarget* pRenderTarget = renderer->getRenderTarget();
	IWICImagingFactory *pImagingFactory = renderer->getImagingFactory();
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	ID2D1Bitmap *pBitmap = nullptr;

	// 创建解码器
	HRESULT hr = pImagingFactory->CreateDecoderFromFilename(
		(LPCWSTR)actualFilePath,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		// 创建初始化框架
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// 创建图片格式转换器
		hr = pImagingFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		// 图片格式转换成 32bppPBGRA
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr))
	{
		// 从 WIC 位图创建一个 Direct2D 位图
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			&pBitmap
		);
	}

	if (SUCCEEDED(hr))
	{
		_bitmapCache.insert(std::make_pair(hash, pBitmap));
	}

	// 释放相关资源
	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pStream);
	SafeRelease(pConverter);

	return SUCCEEDED(hr);
}

void e2d::Image::clearCache()
{
	if (_bitmapCache.empty())
		return;

	for (const auto& bitmap : _bitmapCache)
	{
		bitmap.second->Release();
	}
	_bitmapCache.clear();
}

void e2d::Image::_setBitmap(ID2D1Bitmap * bitmap)
{
	if (bitmap)
	{
		_bitmap = bitmap;
		_cropRect.origin.x = _cropRect.origin.y = 0;
		_cropRect.size.width = _bitmap->GetSize().width;
		_cropRect.size.height = _bitmap->GetSize().height;
	}
}

ID2D1Bitmap * e2d::Image::getBitmap()
{
	return _bitmap;
}
