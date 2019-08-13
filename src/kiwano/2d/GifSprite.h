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
#include "Actor.h"
#include "../base/Resource.h"
#include "../renderer/render.h"
#include "../renderer/GifImage.h"

namespace kiwano
{
	class KGE_API GifSprite
		: public VisualActor
	{
	public:
		using LoopDoneCallback = Closure<void(int)>;
		using DoneCallback = Closure<void()>;

		GifSprite();

		GifSprite(
			Resource const& res
		);

		GifSprite(
			GifImagePtr image
		);

		bool Load(
			Resource const& res
		);

		bool Load(
			GifImagePtr image
		);

		// 设置 GIF 动画循环次数
		inline void SetLoopCount(int loops)							{ total_loop_count_ = loops; }

		// 设置 GIF 动画每次循环结束回调函数
		inline void SetLoopDoneCallback(LoopDoneCallback const& cb)	{ loop_cb_ = cb; }

		// 设置 GIF 动画结束回调函数
		inline void SetDoneCallback(DoneCallback const& cb)			{ done_cb_ = cb; }

		// 重新播放动画
		void RestartAnimation();

		inline LoopDoneCallback GetLoopDoneCallback() const			{ return loop_cb_; }

		inline DoneCallback GetDoneCallback() const					{ return done_cb_; }

		void OnRender() override;

	protected:
		void Update(Duration dt) override;

		void ComposeNextFrame();

		HRESULT OverlayNextFrame();

		inline bool IsLastFrame() const								{ return (next_index_ == 0); }

		inline bool EndOfAnimation() const							{ return IsLastFrame() && loop_count_ == total_loop_count_ + 1; }

	protected:
		bool			animating_;
		int				total_loop_count_;
		int				loop_count_;
		unsigned int	next_index_;
		Duration		frame_delay_;
		Duration		frame_elapsed_;

		LoopDoneCallback	loop_cb_;
		DoneCallback		done_cb_;

		GifImagePtr						image_;
		ComPtr<ID2D1Bitmap>				frame_to_render_;
		ComPtr<ID2D1BitmapRenderTarget>	frame_rt_;
	};
}
