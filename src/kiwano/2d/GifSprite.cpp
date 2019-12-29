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

#include <kiwano/2d/GifSprite.h>
#include <kiwano/core/win32/helper.h>
#include <kiwano/renderer/TextureCache.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	GifSprite::GifSprite()
		: animating_(false)
		, next_index_(0)
		, total_loop_count_(1)
		, loop_count_(0)
	{
	}

	bool GifSprite::Load(String const& file_path)
	{
		GifImagePtr image = TextureCache::instance().AddOrGetGifImage(file_path);
		return Load(image);
	}

	bool GifSprite::Load(Resource const& res)
	{
		GifImagePtr image = TextureCache::instance().AddOrGetGifImage(res);
		return Load(image);
	}

	bool GifSprite::Load(GifImagePtr gif)
	{
		if (gif && gif->IsValid())
		{
			gif_ = gif;

			next_index_ = 0;
			loop_count_ = 0;
			frame_.disposal_type = GifImage::DisposalType::None;

			SetSize(Size{ static_cast<float>(gif_->GetWidthInPixels()), static_cast<float>(gif_->GetHeightInPixels()) });

			if (!frame_rt_)
			{
				Renderer::instance().CreateTextureRenderTarget(frame_rt_);
			}

			if (gif_->GetFramesCount() > 0)
			{
				ComposeNextFrame();
			}
			return true;
		}
		return false;
	}

	void GifSprite::OnRender(RenderTarget* rt)
	{
		if (frame_to_render_ && CheckVisibilty(rt))
		{
			PrepareToRender(rt);

			rt->DrawTexture(*frame_to_render_, &frame_.rect, nullptr);
		}
	}

	void GifSprite::Update(Duration dt)
	{
		Actor::Update(dt);

		if (gif_ && gif_->IsValid() && animating_)
		{
			frame_elapsed_ += dt;
			if (frame_.delay <= frame_elapsed_)
			{
				frame_.delay -= frame_elapsed_;
				frame_elapsed_ = 0;
				ComposeNextFrame();
			}
		}
	}

	void GifSprite::SetGifImage(GifImagePtr gif)
	{
		gif_ = gif;
		RestartAnimation();
	}

	void GifSprite::RestartAnimation()
	{
		animating_ = true;
		next_index_ = 0;
		loop_count_ = 0;
		frame_.disposal_type = GifImage::DisposalType::None;
	}

	void GifSprite::ComposeNextFrame()
	{
		KGE_ASSERT(frame_rt_);
		KGE_ASSERT(gif_);

		if (frame_rt_->IsValid())
		{
			do
			{
				DisposeCurrentFrame();
				OverlayNextFrame();
			} while (frame_.delay.IsZero() && !IsLastFrame());

			animating_ = (!EndOfAnimation() && gif_->GetFramesCount() > 1);
		}
	}

	void GifSprite::DisposeCurrentFrame()
	{
		switch (frame_.disposal_type)
		{
		case GifImage::DisposalType::Unknown:
		case GifImage::DisposalType::None:
			break;

		case GifImage::DisposalType::Background:
		{
			ClearCurrentFrameArea();
			break;
		}

		case GifImage::DisposalType::Previous:
		{
			RestoreSavedFrame();
			break;
		}

		default:
			ThrowIfFailed(E_FAIL);
		}
	}

	void GifSprite::OverlayNextFrame()
	{
		KGE_ASSERT(frame_rt_);
		KGE_ASSERT(gif_ && gif_->IsValid());

		frame_ = gif_->GetFrame(next_index_);
		
		if (frame_.disposal_type == GifImage::DisposalType::Previous)
		{
			SaveComposedFrame();
		}

		if (frame_rt_->IsValid())
		{
			frame_rt_->BeginDraw();

			if (next_index_ == 0)
			{
				loop_count_++;
			}

			if (frame_.texture)
			{
				frame_rt_->DrawTexture(*frame_.texture, nullptr, &frame_.rect);
			}

			frame_rt_->EndDraw();

			if (!frame_to_render_)
			{
				frame_to_render_ = new Texture;
			}

			if (frame_rt_->GetOutput(*frame_to_render_))
			{
				next_index_ = (++next_index_) % gif_->GetFramesCount();
			}
		}

		if (IsLastFrame() && loop_cb_)
		{
			loop_cb_(loop_count_ - 1);
		}

		if (EndOfAnimation() && done_cb_)
		{
			done_cb_();
		}
	}

	void GifSprite::SaveComposedFrame()
	{
		KGE_ASSERT(frame_rt_);

		TexturePtr frame_to_be_saved = new Texture;

		HRESULT hr = frame_rt_->GetOutput(*frame_to_be_saved) ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			if (!saved_frame_)
			{
				saved_frame_ = new Texture;
				frame_rt_->CreateTexture(*saved_frame_, frame_to_be_saved->GetSizeInPixels(), frame_to_be_saved->GetPixelFormat());
			}

			hr = saved_frame_ ? S_OK : E_FAIL;
		}

		if (SUCCEEDED(hr))
		{
			saved_frame_->CopyFrom(frame_to_be_saved);
		}

		ThrowIfFailed(hr);
	}

	void GifSprite::RestoreSavedFrame()
	{
		KGE_ASSERT(frame_rt_);
		HRESULT hr = saved_frame_ ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			TexturePtr frame_to_copy_to = new Texture;

			hr = frame_rt_->GetOutput(*frame_to_copy_to) ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				frame_to_copy_to->CopyFrom(saved_frame_);
			}
		}

		ThrowIfFailed(hr);
	}

	void GifSprite::ClearCurrentFrameArea()
	{
		KGE_ASSERT(frame_rt_);
		frame_rt_->BeginDraw();

		frame_rt_->PushClipRect(frame_.rect);
		frame_rt_->Clear();
		frame_rt_->PopClipRect();

		return frame_rt_->EndDraw();
	}

}
