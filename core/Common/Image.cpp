#include "..\e2dutil.h"
#include "..\e2dmodule.h"
#include "..\e2dtool.h"

std::map<size_t, ID2D1Bitmap*> e2d::Image::bitmap_cache_;

e2d::Image::Image()
	: bitmap_(nullptr)
	, crop_rect_()
{
}

e2d::Image::Image(const Resource& res)
	: bitmap_(nullptr)
	, crop_rect_()
{
	this->Open(res);
}

e2d::Image::Image(const Resource& res, const Rect& crop_rect)
	: bitmap_(nullptr)
	, crop_rect_()
{
	this->Open(res);
	this->Crop(crop_rect);
}

e2d::Image::Image(const String & file_name)
	: bitmap_(nullptr)
	, crop_rect_()
{
	this->Open(file_name);
}

e2d::Image::Image(const String & file_name, const Rect & crop_rect)
	: bitmap_(nullptr)
	, crop_rect_()
{
	this->Open(file_name);
	this->Crop(crop_rect);
}

e2d::Image::~Image()
{
}

bool e2d::Image::Open(const Resource& res)
{
	if (!Image::Preload(res))
	{
		WARN("Load Image from file failed!");
		return false;
	}

	this->SetBitmap(bitmap_cache_.at(res.name));
	return true;
}

bool e2d::Image::Open(const String & file_name)
{
	WARN_IF(file_name.IsEmpty(), "Image Open failed! Invalid file name.");

	if (file_name.IsEmpty())
		return false;

	if (!Image::Preload(file_name))
	{
		WARN("Load Image from file failed!");
		return false;
	}

	this->SetBitmap(bitmap_cache_.at(file_name.GetHash()));
	return true;
}

void e2d::Image::Crop(const Rect& crop_rect)
{
	if (bitmap_)
	{
		crop_rect_.origin.x = std::min(std::max(crop_rect.origin.x, 0.f), this->GetSourceWidth());
		crop_rect_.origin.y = std::min(std::max(crop_rect.origin.y, 0.f), this->GetSourceHeight());
		crop_rect_.size.width = std::min(std::max(crop_rect.size.width, 0.f), this->GetSourceWidth() - crop_rect.origin.x);
		crop_rect_.size.height = std::min(std::max(crop_rect.size.height, 0.f), this->GetSourceHeight() - crop_rect.origin.y);
	}
}

float e2d::Image::GetWidth() const
{
	return crop_rect_.size.width;
}

float e2d::Image::GetHeight() const
{
	return crop_rect_.size.height;
}

e2d::Size e2d::Image::GetSize() const
{
	return crop_rect_.size;
}

float e2d::Image::GetSourceWidth() const
{
	if (bitmap_)
	{
		return bitmap_->GetSize().width;
	}
	else
	{
		return 0;
	}
}

float e2d::Image::GetSourceHeight() const
{
	if (bitmap_)
	{
		return bitmap_->GetSize().height;
	}
	else
	{
		return 0;
	}
}

e2d::Size e2d::Image::GetSourceSize() const
{
	if (bitmap_)
	{
		return Size(GetSourceWidth(), GetSourceHeight());
	}
	else
	{
		return Size();
	}
}

float e2d::Image::GetCropX() const
{
	return crop_rect_.origin.x;
}

float e2d::Image::GetCropY() const
{
	return crop_rect_.origin.y;
}

e2d::Point e2d::Image::GetCropPos() const
{
	return crop_rect_.origin;
}

bool e2d::Image::Preload(const Resource& res)
{
	if (bitmap_cache_.find(res.name) != bitmap_cache_.end())
	{
		return true;
	}

	IWICImagingFactory *pImagingFactory = Renderer::GetImagingFactory();
	ID2D1HwndRenderTarget* pRenderTarget = Renderer::GetInstance()->GetRenderTarget();
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
		MAKEINTRESOURCE(res.name), 
		(LPCWSTR)res.type
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
		bitmap_cache_.insert(std::make_pair(res.name, pBitmap));
	}

	// 释放相关资源
	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pStream);
	SafeRelease(pConverter);

	return SUCCEEDED(hr);
}

bool e2d::Image::Preload(const String & file_name)
{
	String actualFilePath = File(file_name).GetPath();
	if (actualFilePath.IsEmpty())
		return false;

	size_t hash = actualFilePath.GetHash();
	if (bitmap_cache_.find(hash) != bitmap_cache_.end())
		return true;

	IWICImagingFactory *pImagingFactory = Renderer::GetImagingFactory();
	ID2D1HwndRenderTarget* pRenderTarget = Renderer::GetInstance()->GetRenderTarget();
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
		bitmap_cache_.insert(std::make_pair(hash, pBitmap));
	}

	// 释放相关资源
	SafeRelease(pDecoder);
	SafeRelease(pSource);
	SafeRelease(pStream);
	SafeRelease(pConverter);

	return SUCCEEDED(hr);
}

void e2d::Image::ClearCache()
{
	if (bitmap_cache_.empty())
		return;

	for (const auto& bitmap : bitmap_cache_)
	{
		bitmap.second->Release();
	}
	bitmap_cache_.clear();
}

void e2d::Image::SetBitmap(ID2D1Bitmap * bitmap)
{
	if (bitmap)
	{
		bitmap_ = bitmap;
		crop_rect_.origin.x = crop_rect_.origin.y = 0;
		crop_rect_.size.width = bitmap_->GetSize().width;
		crop_rect_.size.height = bitmap_->GetSize().height;
	}
}

ID2D1Bitmap * e2d::Image::GetBitmap()
{
	return bitmap_;
}
