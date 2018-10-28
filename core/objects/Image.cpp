// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "..\e2dobject.h"
#include "..\e2dmodule.h"
#include "..\e2dtool.h"

std::map<size_t, ID2D1Bitmap*> easy2d::Image::bitmap_cache_;

easy2d::Image::Image()
	: bitmap_(nullptr)
	, crop_rect_()
{
}

easy2d::Image::Image(Resource& res)
	: bitmap_(nullptr)
	, crop_rect_()
{
	this->Load(res);
}

easy2d::Image::Image(Resource& res, const Rect& crop_rect)
	: bitmap_(nullptr)
	, crop_rect_()
{
	this->Load(res);
	this->Crop(crop_rect);
}

easy2d::Image::Image(const std::wstring & file_name)
	: bitmap_(nullptr)
	, crop_rect_()
{
	this->Load(file_name);
}

easy2d::Image::Image(const std::wstring & file_name, const Rect & crop_rect)
	: bitmap_(nullptr)
	, crop_rect_()
{
	this->Load(file_name);
	this->Crop(crop_rect);
}

easy2d::Image::~Image()
{
	SafeRelease(bitmap_);
}

bool easy2d::Image::Load(Resource& res)
{
	if (!Image::CacheBitmap(res))
	{
		E2D_WARNING("Load Image from file failed!");
		return false;
	}

	this->SetBitmap(bitmap_cache_.at(res.GetHashCode()));
	return true;
}

bool easy2d::Image::Load(const std::wstring & file_name)
{
	E2D_WARNING_IF(file_name.empty(), "Image Load failed! Invalid file name.");

	if (file_name.empty())
		return false;

	if (!Image::CacheBitmap(file_name))
	{
		E2D_WARNING("Load Image from file failed!");
		return false;
	}

	this->SetBitmap(bitmap_cache_.at(std::hash<std::wstring>{}(file_name)));
	return true;
}

void easy2d::Image::Crop(const Rect& crop_rect)
{
	if (bitmap_)
	{
		auto bitmap_size = bitmap_->GetSize();
		crop_rect_.origin.x = std::min(std::max(crop_rect.origin.x, 0.f), bitmap_size.width);
		crop_rect_.origin.y = std::min(std::max(crop_rect.origin.y, 0.f), bitmap_size.height);
		crop_rect_.size.width = std::min(std::max(crop_rect.size.width, 0.f), bitmap_size.width - crop_rect.origin.x);
		crop_rect_.size.height = std::min(std::max(crop_rect.size.height, 0.f), bitmap_size.height - crop_rect.origin.y);
	}
}

float easy2d::Image::GetWidth() const
{
	return crop_rect_.size.width;
}

float easy2d::Image::GetHeight() const
{
	return crop_rect_.size.height;
}

easy2d::Size easy2d::Image::GetSize() const
{
	return crop_rect_.size;
}

float easy2d::Image::GetSourceWidth() const
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

float easy2d::Image::GetSourceHeight() const
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

easy2d::Size easy2d::Image::GetSourceSize() const
{
	Size source_size;
	if (bitmap_)
	{
		auto bitmap_size = bitmap_->GetSize();
		source_size.width = bitmap_size.width;
		source_size.height = bitmap_size.height;
	}
	return std::move(source_size);
}

float easy2d::Image::GetCropX() const
{
	return crop_rect_.origin.x;
}

float easy2d::Image::GetCropY() const
{
	return crop_rect_.origin.y;
}

easy2d::Point easy2d::Image::GetCropPos() const
{
	return crop_rect_.origin;
}

const easy2d::Rect & easy2d::Image::GetCropRect() const
{
	return crop_rect_;
}

ID2D1Bitmap * easy2d::Image::GetBitmap() const
{
	return bitmap_;
}

bool easy2d::Image::CacheBitmap(Resource& res)
{
	size_t hash_code = res.GetHashCode();
	if (bitmap_cache_.find(hash_code) != bitmap_cache_.end())
	{
		return true;
	}

	HRESULT hr;

	HINSTANCE				hinstance = GetModuleHandle(nullptr);
	IWICImagingFactory*		imaging_factory = Device::GetGraphics()->GetImagingFactory();
	ID2D1HwndRenderTarget*	render_target = Device::GetGraphics()->GetRenderTarget();
	IWICBitmapDecoder*		decoder = nullptr;
	IWICBitmapFrameDecode*	source = nullptr;
	IWICStream*				stream = nullptr;
	IWICFormatConverter*	converter = nullptr;
	ID2D1Bitmap*			bitmap = nullptr;

	// 加载资源
	hr = res.Load() ? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		// 创建 WIC 流
		hr = imaging_factory->CreateStream(&stream);
	}

	if (SUCCEEDED(hr))
	{
		// 初始化流
		hr = stream->InitializeFromMemory(
			static_cast<WICInProcPointer>(res.GetData()),
			res.GetDataSize()
		);
	}

	if (SUCCEEDED(hr))
	{
		// 创建流的解码器
		hr = imaging_factory->CreateDecoderFromStream(
			stream,
			nullptr,
			WICDecodeMetadataCacheOnLoad,
			&decoder
		);
	}

	if (SUCCEEDED(hr))
	{
		// 创建初始化框架
		hr = decoder->GetFrame(0, &source);
	}

	if (SUCCEEDED(hr))
	{
		// 创建图片格式转换器
		hr = imaging_factory->CreateFormatConverter(&converter);
	}

	if (SUCCEEDED(hr))
	{
		// 图片格式转换成 32bppPBGRA
		hr = converter->Initialize(
			source,
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
		hr = render_target->CreateBitmapFromWicBitmap(
			converter,
			nullptr,
			&bitmap
		);
	}

	if (SUCCEEDED(hr))
	{
		bitmap_cache_.insert(std::make_pair(hash_code, bitmap));
	}

	// 释放相关资源
	SafeRelease(decoder);
	SafeRelease(source);
	SafeRelease(stream);
	SafeRelease(converter);

	return SUCCEEDED(hr);
}

bool easy2d::Image::CacheBitmap(const std::wstring & file_name)
{
	size_t hash_code = std::hash<std::wstring>{}(file_name);
	if (bitmap_cache_.find(hash_code) != bitmap_cache_.end())
		return true;

	File image_file;
	if (!image_file.Open(file_name))
		return false;

	// 用户输入的路径不一定是完整路径，因为用户可能通过 File::AddSearchPath 添加
	// 默认搜索路径，所以需要通过 File::GetPath 获取完整路径
	std::wstring image_file_path = image_file.GetPath();

	Graphics*				graphics_device = Device::GetGraphics();
	IWICImagingFactory*		imaging_factory = graphics_device->GetImagingFactory();
	ID2D1HwndRenderTarget*	render_target = graphics_device->GetRenderTarget();
	IWICBitmapDecoder*		decoder = nullptr;
	IWICBitmapFrameDecode*	source = nullptr;
	IWICStream*				stream = nullptr;
	IWICFormatConverter*	converter = nullptr;
	ID2D1Bitmap*			bitmap = nullptr;

	// 创建解码器
	HRESULT hr = imaging_factory->CreateDecoderFromFilename(
		image_file_path.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder
	);

	if (SUCCEEDED(hr))
	{
		// 创建初始化框架
		hr = decoder->GetFrame(0, &source);
	}

	if (SUCCEEDED(hr))
	{
		// 创建图片格式转换器
		hr = imaging_factory->CreateFormatConverter(&converter);
	}

	if (SUCCEEDED(hr))
	{
		// 图片格式转换成 32bppPBGRA
		hr = converter->Initialize(
			source,
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
		hr = render_target->CreateBitmapFromWicBitmap(
			converter,
			nullptr,
			&bitmap
		);
	}

	if (SUCCEEDED(hr))
	{
		bitmap_cache_.insert(std::make_pair(hash_code, bitmap));
	}

	// 释放相关资源
	SafeRelease(decoder);
	SafeRelease(source);
	SafeRelease(stream);
	SafeRelease(converter);

	return SUCCEEDED(hr);
}

void easy2d::Image::ClearCache()
{
	if (bitmap_cache_.empty())
		return;

	for (const auto& bitmap : bitmap_cache_)
	{
		bitmap.second->Release();
	}
	bitmap_cache_.clear();
}

void easy2d::Image::SetBitmap(ID2D1Bitmap * bitmap)
{
	if (bitmap_ == bitmap)
		return;

	if (bitmap_)
	{
		bitmap_->Release();
	}

	if (bitmap)
	{
		bitmap->AddRef();

		bitmap_ = bitmap;
		crop_rect_.origin.x = crop_rect_.origin.y = 0;
		crop_rect_.size.width = bitmap_->GetSize().width;
		crop_rect_.size.height = bitmap_->GetSize().height;
	}
}
