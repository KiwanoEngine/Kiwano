#include "..\enodes.h"
#include <map>


struct ResKey
{
	ResKey() { resNameHash = 0; resTypeHash = 0; }

	bool operator < (ResKey const& key) const
	{
		if (resNameHash > key.resNameHash)
			return true;
		else if (resNameHash == key.resNameHash)
			return resTypeHash > key.resTypeHash;
		else
			return false;
	}

	size_t resNameHash;
	size_t resTypeHash;
};

static std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;
static std::map<ResKey, ID2D1Bitmap*> s_mBitmapsFromResource;


e2d::EImage::EImage()
	: m_pBitmap(nullptr)
{
}

e2d::EImage::EImage(LPCTSTR fileName)
{
	this->loadFromFile(fileName);
}

e2d::EImage::EImage(LPCTSTR resourceName, LPCTSTR resourceType)
{
	this->loadFromResource(resourceName, resourceType);
}

e2d::EImage::~EImage()
{
}

void e2d::EImage::loadFromFile(const EString & fileName)
{
	WARN_IF(fileName.isEmpty(), "EImage cannot load bitmap from NULL file name.");

	if (fileName.isEmpty())
		return;

	if (!e2d::EImage::preload(fileName))
	{
		WARN_IF(true, "Load EImage from file failed!");
		return;
	}

	m_pBitmap = s_mBitmapsFromFile.at(fileName.hash());
}

void e2d::EImage::loadFromResource(LPCTSTR resourceName, LPCTSTR resourceType)
{
	WARN_IF(!resourceName || !resourceType, "EImage cannot load bitmap from NULL resource.");

	if (!resourceName || !resourceType)
		return;

	if (!e2d::EImage::preload(resourceName, resourceType))
	{
		WARN_IF(true, "Load EImage from resource failed!");
		return;
	}

	ResKey key;
	std::hash<LPCTSTR> h;
	key.resNameHash = h(resourceName);
	key.resTypeHash = h(resourceType);

	m_pBitmap = s_mBitmapsFromResource.at(key);
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

bool e2d::EImage::preload(LPCTSTR resourceName, LPCTSTR resourceType)
{
	std::hash<LPCTSTR> h;

	ResKey key;
	key.resNameHash = h(resourceName);
	key.resTypeHash = h(resourceType);

	if (s_mBitmapsFromResource.find(key) != s_mBitmapsFromResource.end())
	{
		return true;
	}

	HRESULT hr = S_OK;

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
	imageResHandle = ::FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);

	hr = imageResHandle ? S_OK : E_FAIL;
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
		imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// 创建 WIC 流
		hr = ERenderer::getIWICImagingFactory()->CreateStream(&pStream);
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
		hr = ERenderer::getIWICImagingFactory()->CreateDecoderFromStream(
			pStream,
			NULL,
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
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = ERenderer::getIWICImagingFactory()->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		// 图片格式转换成 32bppPBGRA
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
		std::hash<LPCTSTR> h;

		ResKey key;
		key.resNameHash = h(resourceName);
		key.resTypeHash = h(resourceType);

		s_mBitmapsFromResource.insert(
			std::map<ResKey, ID2D1Bitmap*>::value_type(
				key,
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
	for (auto child = s_mBitmapsFromFile.begin(); child != s_mBitmapsFromFile.end(); child++)
	{
		SafeReleaseInterface(&(*child).second);
	}
	for (auto child = s_mBitmapsFromFile.begin(); child != s_mBitmapsFromFile.end(); child++)
	{
		SafeReleaseInterface(&(*child).second);
	}
	s_mBitmapsFromFile.clear();
	s_mBitmapsFromResource.clear();
}

ID2D1Bitmap * e2d::EImage::_getBitmap()
{
	return m_pBitmap;
}
