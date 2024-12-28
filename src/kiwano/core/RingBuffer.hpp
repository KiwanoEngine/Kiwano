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
#include <kiwano/core/Common.h>

namespace kiwano
{

/// \~chinese
/// @brief 环形数组
template <typename T>
class RingBuffer
{
public:
    inline RingBuffer(size_t capcity)
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

    inline void PopFront()
    {
        IncreaseFront();
    }

    inline const T& Front() const
    {
        return buffer_[front_];
    }

    inline T& Front()
    {
        return buffer_[front_];
    }

    inline const T& Back() const
    {
        return buffer_[ClampCursor(rear_, 1)];
    }

    inline T& Back()
    {
        return buffer_[ClampCursor(rear_, 1)];
    }

    inline bool IsEmpty() const noexcept
    {
        return front_ == rear_;
    }

    inline bool IsFull() const noexcept
    {
        return front_ == (rear_ + 1) % Capacity();
    }

    inline size_t Size() const
    {
        return ClampCursor(rear_, front_);
    }

    inline size_t Capacity() const
    {
        return buffer_.size();
    }

    inline void Reserve(size_t capacity)
    {
        if (capacity > Capacity())
            buffer_.resize(capacity);
    }

private:
    inline void IncreaseFront()
    {
        front_ = buffer_.empty() ? front_ : (front_ + 1) % Capacity();
    }

    inline void IncreaseRear()
    {
        rear_ = (rear_ + 1) % Capacity();
    }

    inline size_t ClampCursor(size_t cursor, size_t off) const
    {
        return (cursor + Capacity() - off) % Capacity();
    }

private:
    Vector<T> buffer_;
    size_t    front_ = 0;
    size_t    rear_  = 0;
};

}  // namespace kiwano
