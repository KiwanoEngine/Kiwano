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
#include "../base/Logger.h"
#include "../renderer/TextureCache.h"
#include "../renderer/Renderer.h"

namespace kiwano
{
	GifSprite::GifSprite()
		: animating_(false)
		, next_index_(0)
		, total_loop_count_(1)
		, loop_count_(0)
		, disposal_type_(DisposalType::Unknown)
	{
	}

	GifSprite::GifSprite(String const& file_path)
	{
		Load(file_path);
	}

	GifSprite::GifSprite(Resource const& res)
		: GifSprite()
	{
		Load(res);
	}

	GifSprite::GifSprite(GifImage texture)
	{
		Load(texture);
	}

	bool GifSprite::Load(String const& file_path)
	{
		GifImage texture = TextureCache::GetInstance()->AddOrGetGifImage(file_path);
		return Load(texture);
	}

	bool GifSprite::Load(Resource const& res)
	{
		GifImage texture = TextureCache::GetInstance()->AddOrGetGifImage(res);
		return Load(texture);
	}

	bool GifSprite::Load(GifImage texture)
	{
		if (texture.IsValid())
		{
			texture_ = texture;

			next_index_ = 0;
			loop_count_ = 0;
			disposal_type_ = DisposalType::None;

			SetSize(Size{ static_cast<Float32>(texture_.GetWidthInPixels()), static_cast<Float32>(texture_.GetHeightInPixels()) });

			if (!frame_rt_.IsValid())
			{
				Renderer::GetInstance()->CreateTextureRenderTarget(frame_rt_);
			}

			if (texture_.GetFramesCount() > 0)
			{
				ComposeNextFrame();
			}
			return true;
		}
		return false;
	}

	void GifSprite::OnRender(RenderTarget* rt)
	{
		if (frame_.IsValid() && rt->CheckVisibility(GetBounds(), GetTransformMatrix()))
		{
			PrepareRender(rt);

			rt->DrawTexture(frame_);
		}
	}

	void GifSprite::Update(Duration dt)
	{
		Actor::Update(dt);

		if (texture_.IsValid() && animating_)
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

	void GifSprite::RestartAnimation()
	{
		animating_ = true;
		next_index_ = 0;
		loop_count_ = 0;
		disposal_type_ = DisposalType::None;
	}

	void GifSprite::ComposeNextFrame()
	{
		if (frame_rt_.IsValid())
		{
			do
			{
				DisposeCurrentFrame();
				OverlayNextFrame();
			} while (frame_delay_.IsZero() && !IsLastFrame());

			animating_ = (!EndOfAnimation() && texture_.GetFramesCount() > 1);
		}
	}

	void GifSprite::DisposeCurrentFrame()
	{
		switch (disposal_type_)
		{
		case DisposalType::Unknown:
		case DisposalType::None:
			break;

		case DisposalType::Background:
		{
			ClearCurrentFrameArea();
			break;
		}

		case DisposalType::Previous:
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
		Texture raw_texture;

		HRESULT hr = texture_.GetRawFrame(next_index_, raw_texture, frame_rect_, frame_delay_, disposal_type_);
		
		if (SUCCEEDED(hr))
		{
			if (disposal_type_ == DisposalType::Previous)
			{
				SaveComposedFrame();
			}
		}

		if (SUCCEEDED(hr))
		{
			frame_rt_.BeginDraw();

			if (next_index_ == 0)
			{
				// ÖØÐÂ»æÖÆ±³¾°
				frame_rt_.Clear(texture_.GetBackgroundColor());
				loop_count_++;
			}

			frame_rt_.DrawTexture(raw_texture, nullptr, &frame_rect_);
			frame_rt_.EndDraw();
		}

		if (SUCCEEDED(hr))
		{
			Texture frame_to_render = frame_rt_.GetOutput();

			hr = frame_to_render.IsValid() ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				frame_ = frame_to_render;
				next_index_ = (++next_index_) % texture_.GetFramesCount();
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

		ThrowIfFailed(hr);
	}

	void GifSprite::SaveComposedFrame()
	{
		Texture frame_to_be_saved = frame_rt_.GetOutput();

		HRESULT hr = frame_to_be_saved.IsValid() ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			if (!saved_frame_.IsValid())
			{
				auto size = frame_to_be_saved.GetSizeInPixels();
				auto prop = D2D1::BitmapProperties(frame_to_be_saved.GetPixelFormat());

				ComPtr<ID2D1Bitmap> saved_bitmap;
				hr = frame_rt_.GetRenderTarget()->CreateBitmap(D2D1::SizeU(size.x, size.y), prop, &saved_bitmap);

				if (SUCCEEDED(hr))
				{
					saved_frame_.SetBitmap(saved_bitmap);
				}
			}
		}

		if (SUCCEEDED(hr))
		{
			saved_frame_.CopyFrom(frame_to_be_saved);
		}

		ThrowIfFailed(hr);
	}

	void GifSprite::RestoreSavedFrame()
	{
		HRESULT hr = saved_frame_.IsValid() ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			Texture frame_to_copy_to = frame_rt_.GetOutput();

			hr = frame_to_copy_to.IsValid() ? S_OK : E_FAIL;

			if (SUCCEEDED(hr))
			{
				frame_to_copy_to.CopyFrom(saved_frame_);
			}
		}

		ThrowIfFailed(hr);
	}

	void GifSprite::ClearCurrentFrameArea()
	{
		frame_rt_.BeginDraw();

		frame_rt_.PushClipRect(frame_rect_);
		frame_rt_.Clear(texture_.GetBackgroundColor());
		frame_rt_.PopClipRect();

		return frame_rt_.EndDraw();
	}

}
