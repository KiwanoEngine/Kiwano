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

#include <kiwano/2d/animation/FrameAnimation.h>
#include <kiwano/2d/Sprite.h>

namespace kiwano
{

SpriteFrame::SpriteFrame(RefPtr<Image> image, const Rect& src_rect, bool reset_sprite_size)
    : src_rect(src_rect)
    , reset_sprite_size(reset_sprite_size)
{
    auto bitmap = RefPtr<Bitmap>(dynamic_cast<Bitmap*>(image.Get()));
    if (bitmap)
    {
        this->bitmap = bitmap;
    }
    else
    {
        this->image = image;
    }
}

Vector<SpriteFrame> SpriteFrame::SplitBitmap(RefPtr<Bitmap> bitmap, const Rect& src_rect, int cols, int rows,
                                             int max_num, float padding_x, float padding_y)
{
    if (cols <= 0 || rows <= 0 || max_num == 0)
        return {};

    if (!bitmap || !bitmap->IsValid())
        return {};

    float raw_width  = src_rect.GetWidth();
    float raw_height = src_rect.GetHeight();
    float width      = (raw_width - (cols - 1) * padding_x) / cols;
    float height     = (raw_height - (rows - 1) * padding_y) / rows;

    Vector<SpriteFrame> frames;
    frames.reserve((max_num > 0) ? max_num : (rows * cols));

    int current_num = 0;

    float dty = src_rect.GetTop();
    for (int i = 0; i < rows; i++)
    {
        float dtx = src_rect.GetLeft();

        for (int j = 0; j < cols; j++)
        {
            frames.emplace_back(bitmap, Rect{ dtx, dty, dtx + width, dty + height });
            ++current_num;

            dtx += (width + padding_x);

            if (max_num > 0 && current_num >= max_num)
                break;
        }
        dty += (height + padding_y);

        if (max_num > 0 && current_num >= max_num)
            break;
    }
    return frames;
}

FrameSequence FrameSequence::Reverse() const
{
    FrameSequence frame_seq;
    frame_seq.duration = duration;
    if (!frames.empty())
    {
        frame_seq.frames.reserve(frames.size());
        for (auto iter = frames.crbegin(), crend = frames.crend(); iter != crend; ++iter)
        {
            frame_seq.frames.push_back(*iter);
        }
    }
    return frame_seq;
}

FrameAnimation::FrameAnimation()
    : current_index_(0)
{
}

FrameAnimation::FrameAnimation(const Vector<FrameSequence>& frame_seq_list)
    : current_index_(0)
{
    SetFrameSequences(frame_seq_list);
}

void FrameAnimation::SetFrameSequences(const Vector<FrameSequence>& frame_seq_list)
{
    frame_seq_list_ = frame_seq_list;
    progress_.clear();
    progress_.reserve(frame_seq_list.size());

    Duration total_dur = 0;
    for (const auto& frame_seq : frame_seq_list_)
    {
        total_dur += frame_seq.duration;
    }

    for (const auto& frame_seq : frame_seq_list_)
    {
        if (total_dur > 0)
        {
            progress_.push_back(frame_seq.duration.GetSeconds() / total_dur.GetSeconds());
        }
        else
        {
            progress_.push_back(0.f);
        }
    }
    SetDuration(total_dur);
}

void FrameAnimation::Init(Actor* target)
{
    if (frame_seq_list_.empty())
    {
        Done();
        return;
    }

    if (!frame_seq_list_[0].frames.empty())
    {
        SetTargetFrame(target, frame_seq_list_[0].frames[0]);
        current_index_ = 0;
    }
}

void FrameAnimation::UpdateTween(Actor* target, float percent)
{
    while (current_index_ < progress_.size())
    {
        if (percent < progress_[current_index_])
            break;
        current_index_++;
    }

    if (current_index_ >= progress_.size())
    {
        Done();
        return;
    }

    float start         = current_index_ == 0 ? 0.f : progress_[current_index_];
    float end           = current_index_ + 1 >= progress_.size() ? 1.f : progress_[current_index_ + 1];
    float inner_percent = (percent - start) / (end - start);

    const auto& frame_seq = GetCurrentFrameSequence();
    auto        size      = frame_seq.frames.size();
    auto        index     = std::min(static_cast<size_t>(math::Floor(size * inner_percent)), size - 1);
    SetTargetFrame(target, frame_seq.frames[index]);
}

void FrameAnimation::SetTargetFrame(Actor* target, const SpriteFrame& frame)
{
    auto sprite_target = dynamic_cast<Sprite*>(target);
    KGE_ASSERT(sprite_target && "FrameAnimation only supports Sprites!");

    if (frame.bitmap)
    {
        sprite_target->SetBitmap(frame.bitmap, frame.src_rect, frame.reset_sprite_size);
    }
    else if (frame.image)
    {
        sprite_target->SetImage(frame.bitmap, frame.src_rect, frame.reset_sprite_size);
    }
}

FrameAnimation* FrameAnimation::Clone() const
{
    FrameAnimation* ptr = new FrameAnimation(frame_seq_list_);
    DoClone(ptr);
    return ptr;
}

FrameAnimation* FrameAnimation::Reverse() const
{
    FrameAnimation* ptr = new FrameAnimation;
    DoClone(ptr);

    if (frame_seq_list_.size())
    {
        Vector<FrameSequence> reversed_frame_seq_list;
        reversed_frame_seq_list.reserve(frame_seq_list_.size());
        for (auto iter = frame_seq_list_.rbegin(); iter != frame_seq_list_.rend(); ++iter)
        {
            reversed_frame_seq_list.push_back(*iter);
        }
        ptr->SetFrameSequences(reversed_frame_seq_list);
    }
    return ptr;
}

}  // namespace kiwano
