#include "..\enodes.h"
#include <map>

static std::map<size_t, ID2D1Bitmap*> s_mBitmapsFromFile;
//static std::map<size_t, size_t, ID2D1Bitmap*> s_mBitmapsFromResource;

static ID2D1Bitmap * GetBitmapFromFile(e2d::EString fileName);
static ID2D1Bitmap * GetBitmapFromResource(e2d::EString resourceName, e2d::EString resourceType);

static bool _loadBitmapFromFile(e2d::EString fileName);
static bool _loadBitmapFromResource(e2d::EString resourceName, e2d::EString resourceType);


e2d::ESprite::ESprite()
{
}

e2d::ESprite::ESprite(EString imageFileName)
	: ESprite()
{
	setImage(imageFileName);
}

e2d::ESprite::ESprite(EString resourceName, EString resourceType)
	: ESprite()
{
	setImage(resourceName, resourceType);
}

void e2d::ESprite::setImage(EString fileName)
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

void e2d::ESprite::setImage(EString resourceName, EString resourceType)
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
			D2D1::RectF(0, 0, m_fWidth, m_fHeight),
			m_fDisplayOpacity
		);
	}
}




bool _loadBitmapFromFile(e2d::EString fileName)
{
	HRESULT hr = S_OK;

	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	ID2D1Bitmap *pBitmap = nullptr;

	hr = GetImagingFactory()->CreateDecoderFromFilename(
		fileName.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = GetImagingFactory()->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
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
		// Create a Direct2D bitmap from the WIC bitmap.
		hr = GetRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);
	}
	if (SUCCEEDED(hr))
	{
		std::hash<e2d::EString> h;
		size_t hash = h(fileName);

		s_mBitmapsFromFile.insert(
			std::map<size_t, ID2D1Bitmap*>::value_type(
				hash,
				pBitmap)
		);
	}

	SafeReleaseInterface(&pDecoder);
	SafeReleaseInterface(&pSource);
	SafeReleaseInterface(&pStream);
	SafeReleaseInterface(&pConverter);

	return SUCCEEDED(hr);
}

bool _loadBitmapFromResource(e2d::EString resourceName, e2d::EString resourceType)
{
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

	// Locate the resource.
	imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName.c_str(), resourceType.c_str());

	hr = imageResHandle ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		// Load the resource.
		imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageResDataHandle ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// Lock it to get a system memory pointer.
		pImageFile = LockResource(imageResDataHandle);

		hr = pImageFile ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// Calculate the size.
		imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
		// Create a WIC stream to map onto the memory.
		hr = GetImagingFactory()->CreateStream(&pStream);
	}

	if (SUCCEEDED(hr))
	{
		// Initialize the stream with the memory pointer and size.
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create a decoder for the stream.
		hr = GetImagingFactory()->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr))
	{
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = GetImagingFactory()->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
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
		//create a Direct2D bitmap from the WIC bitmap.
		hr = GetRenderTarget()->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			&pBitmap
		);
	}

	SafeReleaseInterface(&pDecoder);
	SafeReleaseInterface(&pSource);
	SafeReleaseInterface(&pStream);
	SafeReleaseInterface(&pConverter);
	SafeReleaseInterface(&pScaler);

	return SUCCEEDED(hr);
}

ID2D1Bitmap * GetBitmapFromFile(e2d::EString fileName)
{
	std::hash<e2d::EString> h;
	size_t hash = h(fileName);

	if (s_mBitmapsFromFile.find(hash) == s_mBitmapsFromFile.end())
	{
		if (!_loadBitmapFromFile(fileName))
		{
			return nullptr;
		}
	}
	return s_mBitmapsFromFile.at(hash);
}

ID2D1Bitmap * GetBitmapFromResource(e2d::EString resourceName, e2d::EString resourceType)
{
	return nullptr;
}