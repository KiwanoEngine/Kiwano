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
#include <kiwano/render/Renderer.h>
#include <kiwano/render/TextureCache.h>

namespace kiwano
{

GifSprite::GifSprite()
    : animating_(false)
    , next_index_(0)
    , total_loop_count_(1)
    , loop_count_(0)
{
}

GifSprite::GifSprite(const String& file_path)
    : GifSprite()
{
    Load(file_path);
}

GifSprite::GifSprite(const Resource& res)
    : GifSprite()
{
    Load(res);
}

GifSprite::GifSprite(GifImagePtr gif)
    : GifSprite()
{
    SetGifImage(gif);
}

bool GifSprite::Load(const String& file_path)
{
    GifImagePtr image = TextureCache::GetInstance().AddOrGetGifImage(file_path);
    return Load(image);
}

bool GifSprite::Load(const Resource& res)
{
    GifImagePtr image = TextureCache::GetInstance().AddOrGetGifImage(res);
    return Load(image);
}

bool GifSprite::Load(GifImagePtr gif)
{
    if (gif && gif->IsValid())
    {
        gif_ = gif;

        next_index_ = 0;
        loop_count_ = 0;
        frame_      = GifImage::Frame();

        saved_frame_.Reset();
        frame_to_render_.Reset();
        frame_rt_.Reset();

        Size frame_size  = Size(float(gif_->GetWidthInPixels()), float(gif_->GetHeightInPixels()));
        frame_to_render_ = MakePtr<Texture>();
        frame_rt_        = RenderContext::Create(*frame_to_render_, frame_size);

        SetSize(frame_rt_->GetSize());

        if (gif_->GetFramesCount() > 0)
        {
            ComposeNextFrame();
        }
        return true;
    }
    return false;
}

void GifSprite::OnRender(RenderContext& ctx)
{
    if (frame_to_render_ && CheckVisibility(ctx))
    {
        ctx.DrawTexture(*frame_to_render_, nullptr, &GetBounds());
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
    animating_  = true;
    next_index_ = 0;
    loop_count_ = 0;
    frame_      = GifImage::Frame();
}

void GifSprite::ComposeNextFrame()
{
    KGE_ASSERT(frame_rt_);
    KGE_ASSERT(gif_);

    if (frame_rt_)
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
        ClearCurrentFrameArea();
        break;

    case GifImage::DisposalType::Previous:
        RestoreSavedFrame();
        break;
    }
}

void GifSprite::OverlayNextFrame()
{
    KGE_ASSERT(frame_rt_);
    KGE_ASSERT(gif_);

    frame_ = gif_->GetFrame(next_index_);

    if (frame_.disposal_type == GifImage::DisposalType::Previous)
    {
        SaveComposedFrame();
    }

    if (frame_rt_)
    {
        frame_rt_->BeginDraw();

        if (next_index_ == 0)
        {
            frame_rt_->Clear();
            loop_count_++;
        }

        if (frame_.texture)
        {
            frame_rt_->DrawTexture(*frame_.texture, nullptr, &frame_.rect);
        }

        frame_rt_->EndDraw();

        next_index_ = (++next_index_) % gif_->GetFramesCount();
    }

    // Execute callback
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

    if (!saved_frame_)
    {
        saved_frame_ = MakePtr<Texture>();
        frame_rt_->CreateTexture(*saved_frame_, frame_to_render_->GetSizeInPixels());
    }
    saved_frame_->CopyFrom(frame_to_render_);
}

void GifSprite::RestoreSavedFrame()
{
    KGE_ASSERT(frame_rt_);

    if (saved_frame_)
    {
        frame_to_render_->CopyFrom(saved_frame_);
    }
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

}  // namespace kiwano
