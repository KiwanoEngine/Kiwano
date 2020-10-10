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

#include <kiwano/2d/animation/FrameSequence.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

FrameSequence::FrameSequence() {}

FrameSequence::~FrameSequence() {}

FrameSequence::FrameSequence(const Vector<KeyFramePtr>& frames)
{
    AddFrames(frames);
}

void FrameSequence::AddFrame(KeyFramePtr frame)
{
    if (frame)
    {
        frames_.push_back(frame);
    }
    else
    {
        Fail("FrameSequence::Add failed, NULL pointer exception");
    }
}

void FrameSequence::AddFrames(const Vector<KeyFramePtr>& frames)
{
    if (frames_.empty())
        frames_ = frames;
    else
    {
        frames_.reserve(frames_.size() + frames.size());
        for (const auto& texture : frames)
            AddFrame(texture);
    }
}

KeyFramePtr FrameSequence::GetFrame(size_t index) const
{
    KGE_ASSERT(index < frames_.size());
    return frames_[index];
}

const Vector<KeyFramePtr>& FrameSequence::GetFrames() const
{
    return frames_;
}

size_t FrameSequence::GetFramesCount() const
{
    return frames_.size();
}

FrameSequencePtr FrameSequence::Clone() const
{
    auto frame_seq = MakePtr<FrameSequence>();
    if (frame_seq)
    {
        frame_seq->AddFrames(frames_);
    }
    return frame_seq;
}

FrameSequencePtr FrameSequence::Reverse() const
{
    auto frame_seq = MakePtr<FrameSequence>();
    if (!frames_.empty())
    {
        for (auto iter = frames_.crbegin(), crend = frames_.crend(); iter != crend; ++iter)
        {
            frame_seq->AddFrame(*iter);
        }
    }
    return frame_seq;
}

KeyFrameSpliter::KeyFrameSpliter(TexturePtr texture)
    : texture(texture)
{
    if (texture)
    {
        crop_rect = Rect(Point(), texture->GetSize());
    }
}

KeyFrameSpliter::KeyFrameSpliter(TexturePtr texture, const Rect& crop_rect)
    : texture(texture)
    , crop_rect(crop_rect)
{
}

Vector<KeyFramePtr> KeyFrameSpliter::Split(int cols, int rows, int max_num, float padding_x, float padding_y)
{
    if (cols <= 0 || rows <= 0 || max_num == 0)
        return {};

    if (!texture)
        return {};

    float raw_width  = crop_rect.GetWidth();
    float raw_height = crop_rect.GetHeight();
    float width      = (raw_width - (cols - 1) * padding_x) / cols;
    float height     = (raw_height - (rows - 1) * padding_y) / rows;

    Vector<KeyFramePtr> frames;
    frames.reserve((max_num > 0) ? max_num : (rows * cols));

    int current_num = 0;

    float dty = crop_rect.GetTop();
    for (int i = 0; i < rows; i++)
    {
        float dtx = crop_rect.GetLeft();

        for (int j = 0; j < cols; j++)
        {
            KeyFramePtr ptr = MakePtr<KeyFrame>();
            if (ptr)
            {
                ptr->SetTexture(texture);
                ptr->SetCropRect(Rect{ dtx, dty, dtx + width, dty + height });
                frames.push_back(ptr);
                ++current_num;
            }
            dtx += (width + padding_x);
        }
        dty += (height + padding_y);

        if (max_num > 0 && current_num == max_num)
            break;
    }
    return frames;
}

}  // namespace kiwano
