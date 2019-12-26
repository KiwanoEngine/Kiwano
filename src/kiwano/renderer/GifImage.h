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

#pragma once
#include <kiwano/renderer/Texture.h>
#include <kiwano/core/time.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(GifImage);

	// GIF Í¼Ïñ
	class KGE_API GifImage
		: public ObjectBase
	{
	public:
		GifImage();

		GifImage(String const& file_path);

		GifImage(Resource const& res);

		bool Load(String const& file_path);

		bool Load(Resource const& res);

		bool IsValid() const;

		uint32_t GetWidthInPixels() const;

		uint32_t GetHeightInPixels() const;

		uint32_t GetFramesCount() const;

	public:
		enum class DisposalType
		{
			Unknown,
			None,
			Background,
			Previous
		};

		struct Frame
		{
			Duration delay;
			TexturePtr raw;
			Rect rect;
			DisposalType disposal_type;

			Frame();
		};

		Frame GetFrame(uint32_t index);

		ComPtr<IWICBitmapDecoder> GetDecoder() const;

		void SetDecoder(ComPtr<IWICBitmapDecoder> decoder);

	private:
		HRESULT GetGlobalMetadata();

	private:
		uint32_t	frames_count_;
		uint32_t	width_in_pixels_;
		uint32_t	height_in_pixels_;

		ComPtr<IWICBitmapDecoder> decoder_;
	};

	inline GifImage::Frame::Frame()
		: disposal_type(DisposalType::Unknown)
	{
	}

	inline uint32_t GifImage::GetWidthInPixels() const
	{
		return width_in_pixels_;
	}

	inline uint32_t GifImage::GetHeightInPixels() const
	{
		return height_in_pixels_;
	}

	inline uint32_t GifImage::GetFramesCount() const
	{
		return frames_count_;
	}

	inline ComPtr<IWICBitmapDecoder> GifImage::GetDecoder() const
	{
		return decoder_;
	}

	inline void GifImage::SetDecoder(ComPtr<IWICBitmapDecoder> decoder)
	{
		decoder_ = decoder;
	}
}
