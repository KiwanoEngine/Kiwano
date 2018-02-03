#include "..\enodes.h"
#include <map>

static std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;


e2d::EImage::EImage()
	: m_pBitmap(nullptr)
	, m_fSourceClipX(0)
	, m_fSourceClipY(0)
	, m_fSourceClipWidth(0)
	, m_fSourceClipHeight(0)
{
}

e2d::EImage::EImage(LPCTSTR strFileName)
{
	this->loadFrom(strFileName);
}

e2d::EImage::EImage(LPCTSTR strFileName, float nClipX, float nClipY, float nClipWidth, float nClipHeight)
{
	this->loadFrom(strFileName);
	this->clip(nClipX, nClipY, nClipWidth, nClipHeight);
}

e2d::EImage::~EImage()
{
}

void e2d::EImage::loadFrom(const EString & strFilePath)
{
	WARN_IF(strFilePath.isEmpty(), "EImage cannot load bitmap from NULL file name.");

	if (strFilePath.isEmpty())
		return;

	if (!EImage::preload(strFilePath))
	{
		WARN_IF(true, "Load EImage from file failed!");
		return;
	}

	m_pBitmap = s_mBitmapsFromFile.at(strFilePath.hash());
	m_fSourceClipX = m_fSourceClipY = 0;
	m_fSourceClipWidth = m_pBitmap->GetSize().width;
	m_fSourceClipHeight = m_pBitmap->GetSize().height;
}

void e2d::EImage::loadFrom(const EString & strFilePath, float x, float y, float width, float height)
{
	loadFrom(strFilePath);
	clip(x, y, width, height);
}

void e2d::EImage::clip(float x, float y, float width, float height)
{
	if (m_pBitmap)
	{
		m_fSourceClipX = min(max(x, 0), this->getSourceWidth());
		m_fSourceClipY = min(max(y, 0), this->getSourceHeight());
		m_fSourceClipWidth = min(max(width, 0), this->getSourceWidth() - m_fSourceClipX);
		m_fSourceClipHeight = min(max(height, 0), this->getSourceHeight() - m_fSourceClipY);
	}
}

float e2d::EImage::getWidth() const
{
	return m_fSourceClipWidth;
}

float e2d::EImage::getHeight() const
{
	return m_fSourceClipHeight;
}

e2d::ESize e2d::EImage::getSize() const
{
	return ESize(m_fSourceClipWidth, m_fSourceClipHeight);
}

float e2d::EImage::getSourceWidth() const
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

float e2d::EImage::getSourceHeight() const
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

e2d::ESize e2d::EImage::getSourceSize() const
{
	if (m_pBitmap)
	{
		return ESize(getSourceWidth(), getSourceHeight());
	}
	else
	{
		return ESize();
	}
}

float e2d::EImage::getClipX() const
{
	return m_fSourceClipX;
}

float e2d::EImage::getClipY() const
{
	return m_fSourceClipY;
}

e2d::EPoint e2d::EImage::getClipPos() const
{
	return EPoint(m_fSourceClipX, m_fSourceClipY);
}

bool e2d::EImage::preload(const EString & fileName)
{
	if (s_mBitmapsFromFile.find(fileName.hash()) != s_mBitmapsFromFile.end())
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
	hr = ERenderer::getIWICImagingFactory()->CreateDecoderFromFilename(
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
		hr = ERenderer::getIWICImagingFactory()->CreateFormatConverter(&pConverter);
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
		hr = ERenderer::getRenderTarget()->CreateBitmapFromWicBitmap(
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
				fileName.hash(),
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

void e2d::EImage::clearCache()
{
	for (auto child : s_mBitmapsFromFile)
	{
		SafeReleaseInterface(&child.second);
	}
	s_mBitmapsFromFile.clear();
}

ID2D1Bitmap * e2d::EImage::getBitmap()
{
	return m_pBitmap;
}
