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
#include "Texture.h"

namespace kiwano
{
	// GIF Í¼Ïñ
	class KGE_API GifImage
	{
	public:
		GifImage();

		GifImage(String const& file_path);

		GifImage(Resource const& res);

		bool Load(String const& file_path);

		bool Load(Resource const& res);

		bool IsValid() const;

		inline UInt32 GetWidthInPixels() const	{ return width_in_pixels_; }

		inline UInt32 GetHeightInPixels() const	{ return height_in_pixels_; }

		inline UInt32 GetFramesCount() const		{ return frames_count_; }

		inline Color GetBackgroundColor() const	{ return bg_color_; }

	public:
		enum class DisposalType
		{
			Unknown,
			None,
			Background,
			Previous
		};

		HRESULT GetRawFrame(
			UInt32 frame_index,
			Texture& raw_frame,
			Rect& frame_rect,
			Duration& delay,
			DisposalType& disposal_type
		);

		inline ComPtr<IWICBitmapDecoder> GetDecoder() const			{ return decoder_; }

		inline void SetDecoder(ComPtr<IWICBitmapDecoder> decoder)	{ decoder_ = decoder; }

	protected:
		HRESULT GetGlobalMetadata();

		HRESULT GetBackgroundColor(
			ComPtr<IWICMetadataQueryReader> metadata_reader
		);

	protected:
		UInt32	frames_count_;
		UInt32	width_in_pixels_;
		UInt32	height_in_pixels_;
		Color	bg_color_;

		ComPtr<IWICBitmapDecoder>	decoder_;
	};
}
