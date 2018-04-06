#include "..\enode.h"
#include <map>

static std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;


e2d::Image::Image()
	: m_pBitmap(nullptr)
	, m_fSourceCropX(0)
	, m_fSourceCropY(0)
	, m_fSourceCropWidth(0)
	, m_fSourceCropHeight(0)
{
}

e2d::Image::Image(String strFileName)
{
	this->open(strFileName);
}

e2d::Image::Image(String strFileName, double nCropX, double nCropY, double nCropWidth, double nCropHeight)
{
	this->open(strFileName);
	this->crop(nCropX, nCropY, nCropWidth, nCropHeight);
}

e2d::Image::~Image()
{
}

void e2d::Image::open(String strFilePath)
{
	WARN_IF(strFilePath.isEmpty(), "Image cannot load bitmap from NULL file name.");

	if (strFilePath.isEmpty())
		return;

	if (!Image::preload(strFilePath))
	{
		WARN_IF(true, "Load Image from file failed!");
		return;
	}

	m_pBitmap = s_mBitmapsFromFile.at(strFilePath.getHashCode());
	m_fSourceCropX = m_fSourceCropY = 0;
	m_fSourceCropWidth = m_pBitmap->GetSize().width;
	m_fSourceCropHeight = m_pBitmap->GetSize().height;
}

void e2d::Image::crop(double x, double y, double width, double height)
{
	if (m_pBitmap)
	{
		m_fSourceCropX = min(max(x, 0), this->getSourceWidth());
		m_fSourceCropY = min(max(y, 0), this->getSourceHeight());
		m_fSourceCropWidth = min(max(width, 0), this->getSourceWidth() - m_fSourceCropX);
		m_fSourceCropHeight = min(max(height, 0), this->getSourceHeight() - m_fSourceCropY);
	}
}

double e2d::Image::getWidth() const
{
	return m_fSourceCropWidth;
}

double e2d::Image::getHeight() const
{
	return m_fSourceCropHeight;
}

e2d::Size e2d::Image::getSize() const
{
	return Size(m_fSourceCropWidth, m_fSourceCropHeight);
}

double e2d::Image::getSourceWidth() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->GetSize().width;
	}
	else
	{
		return 0;
	}
}

double e2d::Image::getSourceHeight() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->GetSize().height;
	}
	else
	{
		return 0;
	}
}

e2d::Size e2d::Image::getSourceSize() const
{
	if (m_pBitmap)
	{
		return Size(getSourceWidth(), getSourceHeight());
	}
	else
	{
		return Size();
	}
}

double e2d::Image::getCropX() const
{
	return m_fSourceCropX;
}

double e2d::Image::getCropY() const
{
	return m_fSourceCropY;
}

e2d::Point e2d::Image::getCropPos() const
{
	return Point(m_fSourceCropX, m_fSourceCropY);
}

bool e2d::Image::preload(String fileName)
{
	if (s_mBitmapsFromFile.find(fileName.getHashCode()) != s_mBitmapsFromFile.end())
	{
		return true;
	}

	HRESULT hr = S_OK;

	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	ID2D1Bitmap *pBitmap = nullptr;

	// 创建解码器
	hr = Renderer::getIWICImagingFactory()->CreateDecoderFromFilename(
		fileName,
		NULL,
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
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = Renderer::getIWICImagingFactory()->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		// 图片格式转换成 32bbpPBGRA
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}
	if (SUCCEEDED(hr))
	{
		// 从 WIC 位图创建一个 Direct2D 位图
		hr = Renderer::getRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);
	}
	if (SUCCEEDED(hr))
	{
		// 保存图片指针和图片的 Hash 名
		s_mBitmapsFromFile.insert(
			std::map<size_t, ID2D1Bitmap*>::value_type(
				fileName.getHashCode(),
				pBitmap)
		);
	}

	// 释放相关资源
	SafeReleaseInterface(&pDecoder);
	SafeReleaseInterface(&pSource);
	SafeReleaseInterface(&pStream);
	SafeReleaseInterface(&pConverter);

	return SUCCEEDED(hr);
}

void e2d::Image::clearCache()
{
	for (auto child = s_mBitmapsFromFile.begin(); child != s_mBitmapsFromFile.end(); child++)
	{
		SafeReleaseInterface(&(*child).second);
	}
	s_mBitmapsFromFile.clear();
}

ID2D1Bitmap * e2d::Image::getBitmap()
{
	return m_pBitmap;
}
