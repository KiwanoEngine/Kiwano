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

#include "GifSprite.h"
#include "GifImage.h"
#include "../base/logs.h"
#include "../platform/modules.h"

namespace kiwano
{
	GifSprite::GifSprite()
		: animating_(false)
		, next_index_(0)
		, total_loop_count_(1)
		, loop_count_(0)
	{
	}

	GifSprite::GifSprite(Resource const& res)
		: GifSprite()
	{
		Load(res);
	}

	GifSprite::GifSprite(GifImagePtr image)
	{
		Load(image);
	}

	bool GifSprite::Load(Resource const& res)
	{
		GifImagePtr image = new (std::nothrow) GifImage;
		if (image->Load(res))
		{
			return Load(image);
		}
		return false;
	}

	bool GifSprite::Load(GifImagePtr image)
	{
		if (image && image_ != image)
		{
			image_ = image;

			next_index_ = 0;
			loop_count_ = 0;

			SetSize(
				static_cast<float>(image_->GetWidthInPixels()),
				static_cast<float>(image_->GetHeightInPixels())
			);

			if (!frame_rt_)
			{
				auto ctx = Renderer::Instance()->GetD2DDeviceResources()->GetDeviceContext();
				ThrowIfFailed(
					ctx->CreateCompatibleRenderTarget(&frame_rt_)
				);
			}

			if (image_->GetFramesCount() > 0)
			{
				ComposeNextFrame();
			}
			return true;
		}
		return false;
	}

	void GifSprite::Update(Duration dt)
	{
		VisualNode::Update(dt);

		if (image_ && animating_)
		{
			frame_elapsed_ += dt;
			if (frame_delay_ <= frame_elapsed_)
			{
				frame_delay_ -= frame_elapsed_;
				frame_elapsed_ = 0;
				ComposeNextFrame();
			}
		}
	}

	void GifSprite::OnRender()
	{
		if (frame_to_render_)
		{
			Rect bounds = GetBounds();
			Renderer::Instance()->DrawBitmap(frame_to_render_, bounds, bounds);
		}
	}

	void GifSprite::RestartAnimation()
	{
		animating_ = true;
		next_index_ = 0;
		loop_count_ = 0;
		image_->SetDisposalType(GifImage::DisposalType::None);
	}

	void GifSprite::ComposeNextFrame()
	{
		if (frame_rt_)
		{
			// 找到延迟大于 0 的帧 (0 延迟帧是不可见的中间帧)
			HRESULT hr = E_FAIL;
			do
			{
				hr = image_->DisposeCurrentFrame(frame_rt_);
				if (SUCCEEDED(hr))
				{
					hr = OverlayNextFrame();
				}
				
				if (SUCCEEDED(hr))
				{
					frame_delay_.SetMilliseconds(static_cast<long>(image_->GetFrameDelay()));
				}
			} while (SUCCEEDED(hr) && frame_delay_.IsZero() && !IsLastFrame());

			animating_ = (SUCCEEDED(hr) && !EndOfAnimation() && image_->GetFramesCount() > 1);
		}
	}

	HRESULT GifSprite::OverlayNextFrame()
	{
		HRESULT hr = image_->GetRawFrame(next_index_);
		if (SUCCEEDED(hr))
		{
			if (image_->GetDisposalType() == GifImage::DisposalType::Previous)
			{
				hr = image_->SaveComposedFrame(frame_rt_);
			}
		}

		if (SUCCEEDED(hr))
		{
			frame_rt_->BeginDraw();

			if (next_index_ == 0)
			{
				// 重新绘制背景
				frame_rt_->Clear(image_->GetBackgroundColor());
				loop_count_++;
			}

			frame_rt_->DrawBitmap(image_->GetRawFrame().Get(), image_->GetFramePosition());
			hr = frame_rt_->EndDraw();
		}

		if (SUCCEEDED(hr))
		{
			frame_to_render_ = nullptr;
			hr = frame_rt_->GetBitmap(&frame_to_render_);
		}

		if (SUCCEEDED(hr))
		{
			next_index_ = (++next_index_) % image_->GetFramesCount();
		}

		if (IsLastFrame() && loop_cb_)
		{
			loop_cb_(loop_count_ - 1);
		}

		if (EndOfAnimation() && done_cb_)
		{
			done_cb_();
		}
		return hr;
	}

}
