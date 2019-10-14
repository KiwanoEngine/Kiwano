// Copyright (c) 2016-2018 Kiwano - Nomango
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

#include <kiwano/renderer/Texture.h>
#include <kiwano/renderer/Renderer.h>
#include <kiwano/base/win32/helper.h>

namespace kiwano
{
	InterpolationMode Texture::default_interpolation_mode_ = InterpolationMode::Linear;

	Texture::Texture()
		: interpolation_mode_(default_interpolation_mode_)
	{
	}

	Texture::Texture(String const& file_path)
		: Texture()
	{
		Load(file_path);
	}

	Texture::Texture(Resource const& res)
		: Texture()
	{
		Load(res);
	}

	Texture::Texture(ComPtr<ID2D1Bitmap> const & bitmap)
		: Texture()
	{
		SetBitmap(bitmap);
	}

	Texture::~Texture()
	{
	}

	bool Texture::Load(String const& file_path)
	{
		Renderer::GetInstance()->CreateTexture(*this, file_path);
		return IsValid();
	}

	bool Texture::Load(Resource const& res)
	{
		Renderer::GetInstance()->CreateTexture(*this, res);
		return IsValid();
	}

	bool Texture::IsValid() const
	{
		return bitmap_ != nullptr;
	}

	float Texture::GetWidth() const
	{
		if (bitmap_)
		{
			return bitmap_->GetSize().width;
		}
		return 0;
	}

	float Texture::GetHeight() const
	{
		if (bitmap_)
		{
			return bitmap_->GetSize().height;
		}
		return 0;
	}

	Size Texture::GetSize() const
	{
		if (bitmap_)
		{
			auto bitmap_size = bitmap_->GetSize();
			return Size{ bitmap_size.width, bitmap_size.height };
		}
		return Size{};
	}

	uint32_t Texture::GetWidthInPixels() const
	{
		if (bitmap_)
		{
			return bitmap_->GetPixelSize().width;
		}
		return 0;
	}

	uint32_t Texture::GetHeightInPixels() const
	{
		if (bitmap_)
		{
			return bitmap_->GetPixelSize().height;
		}
		return 0;
	}

	math::Vec2T<uint32_t> Texture::GetSizeInPixels() const
	{
		if (bitmap_)
		{
			auto bitmap_size = bitmap_->GetPixelSize();
			return math::Vec2T<uint32_t>{ bitmap_size.width, bitmap_size.height };
		}
		return math::Vec2T<uint32_t>{};
	}

	InterpolationMode Texture::GetBitmapInterpolationMode() const
	{
		return interpolation_mode_;
	}

	void Texture::CopyFrom(Texture const& copy_from)
	{
		if (IsValid() && copy_from.IsValid())
		{
			HRESULT hr = bitmap_->CopyFromBitmap(nullptr, copy_from.GetBitmap().get(), nullptr);

			ThrowIfFailed(hr);
		}
	}

	void Texture::CopyFrom(Texture const& copy_from, Rect const& src_rect, Point const& dest_point)
	{
		if (IsValid() && copy_from.IsValid())
		{
			HRESULT hr = bitmap_->CopyFromBitmap(
				&D2D1::Point2U(uint32_t(dest_point.x), uint32_t(dest_point.y)),
				copy_from.GetBitmap().get(),
				&D2D1::RectU(
					uint32_t(src_rect.GetLeft()),
					uint32_t(src_rect.GetTop()),
					uint32_t(src_rect.GetRight()),
					uint32_t(src_rect.GetBottom()))
			);

			ThrowIfFailed(hr);
		}
	}

	void Texture::SetInterpolationMode(InterpolationMode mode)
	{
		interpolation_mode_ = mode;
		switch (mode)
		{
		case InterpolationMode::Linear:
			break;
		case InterpolationMode::Nearest:
			break;
		default:
			break;
		}
	}

	D2D1_PIXEL_FORMAT Texture::GetPixelFormat() const
	{
		if (bitmap_)
		{
			return bitmap_->GetPixelFormat();
		}
		return D2D1_PIXEL_FORMAT();
	}

	ComPtr<ID2D1Bitmap> Texture::GetBitmap() const
	{
		return bitmap_;
	}

	void Texture::SetBitmap(ComPtr<ID2D1Bitmap> bitmap)
	{
		bitmap_ = bitmap;
	}

	void Texture::SetDefaultInterpolationMode(InterpolationMode mode)
	{
	}

}
