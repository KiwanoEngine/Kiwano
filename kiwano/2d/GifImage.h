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
#include "include-forwards.h"
#include "../base/Resource.h"
#include "../renderer/render.h"

namespace kiwano
{
	class KGE_API GifImage
		: public Object
	{
	public:
		GifImage();

		GifImage(
			Resource const& res
		);

		bool Load(
			Resource const& res
		);

		inline unsigned int GetWidthInPixels() const	{ return width_in_pixels_; }

		inline unsigned int GetHeightInPixels() const	{ return height_in_pixels_; }

		inline unsigned int GetFrameDelay() const		{ return frame_delay_; }

		inline unsigned int GetFramesCount() const		{ return frames_count_; }

	public:
		enum class DisposalType
		{
			Unknown,
			None,
			Background,
			Previous
		};

		inline DisposalType			GetDisposalType() const				{ return disposal_type_; }

		inline D2D1_COLOR_F			GetBackgroundColor() const			{ return bg_color_; }

		inline D2D1_RECT_F const&	GetFramePosition() const			{ return frame_position_; }

		inline ComPtr<ID2D1Bitmap>	GetRawFrame() const					{ return raw_frame_; }

		inline void					SetDisposalType(DisposalType type)	{ disposal_type_ = type; }

	public:
		HRESULT GetRawFrame(UINT frame_index);
		HRESULT GetGlobalMetadata();
		HRESULT GetBackgroundColor(IWICMetadataQueryReader* metadata_reader);

		HRESULT DisposeCurrentFrame(ComPtr<ID2D1BitmapRenderTarget> frame_rt);
		HRESULT SaveComposedFrame(ComPtr<ID2D1BitmapRenderTarget> frame_rt);
		HRESULT RestoreSavedFrame(ComPtr<ID2D1BitmapRenderTarget> frame_rt);
		HRESULT ClearCurrentFrameArea(ComPtr<ID2D1BitmapRenderTarget> frame_rt);

	protected:
		ComPtr<ID2D1Bitmap>			raw_frame_;
		ComPtr<ID2D1Bitmap>			saved_frame_;
		ComPtr<IWICBitmapDecoder>	decoder_;

		unsigned int	frames_count_;
		unsigned int	frame_delay_;
		unsigned int	width_in_pixels_;
		unsigned int	height_in_pixels_;
		DisposalType	disposal_type_;
		D2D1_RECT_F		frame_position_;
		D2D1_COLOR_F	bg_color_;
	};
}
