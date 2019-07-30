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
#include "Node.h"
#include "../base/Resource.h"
#include "../renderer/render.h"

namespace kiwano
{
	class KGE_API GifImage
		: public VisualNode
	{
	public:
		typedef Closure<void(int)> LoopDoneCallback;
		typedef Closure<void()> DoneCallback;

		GifImage();

		GifImage(
			Resource const& res
		);

		bool Load(
			Resource const& res
		);

		// 设置 GIF 动画循环次数
		inline void SetLoopCount(int loops)								{ total_loop_count_ = loops; }

		// 设置 GIF 动画每次循环结束回调函数
		inline void SetLoopDoneCallback(LoopDoneCallback const& cb)		{ loop_cb_ = cb; }

		// 设置 GIF 动画结束回调函数
		inline void SetDoneCallback(DoneCallback const& cb)				{ done_cb_ = cb; }

		// 重新播放动画
		void Restart();

		inline int GetFramesCount() const								{ return static_cast<int>(frames_count_); }
		inline int GetLoopCount() const									{ return total_loop_count_; }
		inline LoopDoneCallback GetLoopDoneCallback() const				{ return loop_cb_; }
		inline DoneCallback GetDoneCallback() const						{ return done_cb_; }

		void OnRender() override;

	protected:
		void Update(Duration dt) override;

		HRESULT GetRawFrame(UINT frame_index);
		HRESULT GetGlobalMetadata();
		HRESULT GetBackgroundColor(IWICMetadataQueryReader* metadata_reader);

		HRESULT ComposeNextFrame();
		HRESULT DisposeCurrentFrame();
		HRESULT OverlayNextFrame();

		HRESULT SaveComposedFrame();
		HRESULT RestoreSavedFrame();
		HRESULT ClearCurrentFrameArea();

		inline bool IsLastFrame() const		{ return (next_index_ == 0); }
		inline bool EndOfAnimation() const	{ return IsLastFrame() && loop_count_ == total_loop_count_ + 1; }

	protected:
		Duration frame_delay_;
		Duration frame_elapsed_;

		ComPtr<IWICImagingFactory>		factory_;
		ComPtr<ID2D1BitmapRenderTarget>	frame_rt_;
		ComPtr<ID2D1Bitmap>				raw_frame_;
		ComPtr<ID2D1Bitmap>				saved_frame_;
		ComPtr<IWICBitmapDecoder>		decoder_;

		enum class DisposalType
		{
			Unknown,
			None,
			Background,
			Previous
		};

		bool			animating_;
		int				total_loop_count_;
		int				loop_count_;
		unsigned int	next_index_;
		unsigned int	frames_count_;
		unsigned int	width_in_pixels_;
		unsigned int	height_in_pixels_;
		DisposalType	disposal_type_;
		D2D1_RECT_F		frame_position_;
		D2D1_COLOR_F	bg_color_;

		LoopDoneCallback	loop_cb_;
		DoneCallback		done_cb_;
	};
}
