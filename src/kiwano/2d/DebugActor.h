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
#include <kiwano/2d/Actor.h>
#include <kiwano/render/TextLayout.h>

namespace kiwano
{

template <typename T>
class SimpleRingBuffer
{
public:
    SimpleRingBuffer(size_t capcity)
    {
        Reserve(capcity);
    }

    void PushBack(const T& val)
    {
        if (IsFull())
            Reserve(Capacity() * 2);
        buffer_[rear_] = val;
        IncreaseRear();
    }

    void PopFront()
    {
        IncreaseFront();
    }

    const T& Front() const
    {
        return buffer_[front_];
    }

    T& Front()
    {
        return buffer_[front_];
    }

    const T& Back() const
    {
        return buffer_[ClampCursor(rear_, 1)];
    }

    T& Back()
    {
        return buffer_[ClampCursor(rear_, 1)];
    }

    bool IsEmpty() const noexcept
    {
        return front_ = rear_;
    }

    bool IsFull() const noexcept
    {
        return front_ == (rear_ + 1) % Capacity();
    }

    size_t Size() const
    {
        return ClampCursor(rear_, front_);
    }

    size_t Capacity() const
    {
        return buffer_.size();
    }

    void Reserve(size_t capacity)
    {
        buffer_.resize(capacity);
    }

private:
    void IncreaseFront()
    {
        if (buffer_.empty())
            return;
        front_ = (front_ + 1) % Capacity();
    }

    void IncreaseRear()
    {
        rear_ = (rear_ + 1) % Capacity();
    }

    size_t ClampCursor(size_t cursor, size_t off) const
    {
        return (cursor + Capacity() - off) % Capacity();
    }

private:
    Vector<T> buffer_;
    size_t    front_ = 0;
    size_t    rear_  = 0;
};


/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief µ÷ÊÔ½Úµã
 */
class KGE_API DebugActor : public Actor
{
public:
    DebugActor();

    virtual ~DebugActor();

    void OnRender(RenderContext& ctx) override;

    void OnUpdate(Duration dt) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    std::locale comma_locale_;
    BrushPtr    background_brush_;
    TextStyle   debug_text_style_;
    TextLayout  debug_text_;

    SimpleRingBuffer<Time> frame_buffer_;
};

/** @} */
}  // namespace kiwano
