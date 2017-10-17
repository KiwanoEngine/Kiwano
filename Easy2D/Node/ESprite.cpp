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

static ID2D1Bitmap * GetBitmapFromFile(const e2d::EString & fileName);
static ID2D1Bitmap * GetBitmapFromResource(const e2d::EString & resourceName, const e2d::EString & resourceType);


e2d::ESprite::ESprite()
{
}

e2d::ESprite::ESprite(const EString & imageFileName)
	: ESprite()
{
	setImage(imageFileName);
}

e2d::ESprite::ESprite(const EString & resourceName, const EString & resourceType)
	: ESprite()
{
	setImage(resourceName, resourceType);
}

void e2d::ESprite::setImage(const EString & fileName)
{
	WARN_IF(fileName.empty(), "ESprite cannot load bitmap from NULL file name.");

	if (fileName.empty() || m_sFileName == fileName)
		return;

	m_sFileName = fileName;

	SafeReleaseInterface(&pBitmap);
	pBitmap = GetBitmapFromFile(m_sFileName);

	ASSERT(pBitmap, "ESprite create device resources failed!");

	this->setSize(pBitmap->GetSize().width, pBitmap->GetSize().height);
}

void e2d::ESprite::setImage(const EString & resourceName, const EString & resourceType)
{
	WARN_IF(resourceName.empty() || resourceType.empty(), "ESprite cannot load bitmap from NULL resource.");

	if (resourceName.empty() || resourceType.empty())
		return;

	if (m_sResourceName == resourceName && m_sResourceType == resourceType)
		return;

	if (!m_sFileName.empty())
		m_sFileName.clear();

	m_sResourceName = resourceName;
	m_sResourceType = resourceType;

	SafeReleaseInterface(&pBitmap);
	pBitmap = GetBitmapFromResource(resourceName, resourceType);

	ASSERT(pBitmap, "ESprite create device resources failed!");

	this->setSize(pBitmap->GetSize().width, pBitmap->GetSize().height);
}

void e2d::ESprite::_onRender()
{
	if (pBitmap)
	{
		// Draw bitmap
		GetRenderTarget()->DrawBitmap(
			pBitmap,
			D2D1::RectF(0, 0, getRealWidth(), getRealHeight()),
			m_fDisplayOpacity
		);
	}
}

bool e2d::ESprite::preloadImage(const EString & fileName)
{
	std::hash<e2d::EString> h;
	size_t hash = h(fileName);

	if (s_mBitmapsFromFile.find(hash) != s_mBitmapsFromFile.end())
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
	hr = GetImagingFactory()->CreateDecoderFromFilename(
		fileName.c_str(),
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
		hr = GetImagingFactory()->CreateFormatConverter(&pConverter);
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
		hr = GetRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);
	}
	if (SUCCEEDED(hr))
	{
		// 保存图片指针和图片的 Hash 名
		std::hash<e2d::EString> h;
		size_t hash = h(fileName);

		s_mBitmapsFromFile.insert(
			std::map<size_t, ID2D1Bitmap*>::value_type(
				hash,
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

bool e2d::ESprite::preloadImage(const EString & resourceName, const EString & resourceType)
{
	std::hash<e2d::EString> h;

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
	IWICBitmapScaler *pScaler = nullptr;
	ID2D1Bitmap *pBitmap = nullptr;

	HRSRC imageResHandle = nullptr;
	HGLOBAL imageResDataHandle = nullptr;
	void *pImageFile = nullptr;
	DWORD imageFileSize = 0;

	// 定位资源
	imageResHandle = ::FindResourceW(HINST_THISCOMPONENT, resourceName.c_str(), resourceType.c_str());

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
		hr = GetImagingFactory()->CreateStream(&pStream);
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
		hr = GetImagingFactory()->CreateDecoderFromStream(
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
		hr = GetImagingFactory()->CreateFormatConverter(&pConverter);
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
		hr = GetRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);
	}

	if (SUCCEEDED(hr))
	{
		std::hash<e2d::EString> h;

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
	SafeReleaseInterface(&pScaler);

	return SUCCEEDED(hr);
}

void e2d::ESprite::clearCache()
{
	for (auto child : s_mBitmapsFromFile)
	{
		SafeReleaseInterface(&child.second);
	}
	for (auto child : s_mBitmapsFromResource)
	{
		SafeReleaseInterface(&child.second);
	}
	s_mBitmapsFromFile.clear();
	s_mBitmapsFromResource.clear();
}




ID2D1Bitmap * GetBitmapFromFile(const e2d::EString & fileName)
{
	if (!e2d::ESprite::preloadImage(fileName))
	{
		return nullptr;
	}

	std::hash<e2d::EString> h;
	size_t hash = h(fileName);

	return s_mBitmapsFromFile.at(hash);
}

ID2D1Bitmap * GetBitmapFromResource(const e2d::EString & resourceName, const e2d::EString & resourceType)
{
	if (!e2d::ESprite::preloadImage(resourceName, resourceType))
	{
		return nullptr;
	}

	ResKey key;
	std::hash<e2d::EString> h;
	key.resNameHash = h(resourceName);
	key.resTypeHash = h(resourceType);

	return s_mBitmapsFromResource.at(key);
}