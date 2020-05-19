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

#include <kiwano/core/Timer.h>
#include <kiwano/core/Allocator.h>

namespace kiwano
{

TimerPtr Timer::Create()
{
    TimerPtr ptr = memory::New<Timer>();
    return ptr;
}

Timer::Timer()
    : is_paused_(false)
{
    Reset();
}

Timer::~Timer() {}

Duration Timer::GetDeltaTime() const
{
    return delta_time_;
}

Duration Timer::GetTotalTime() const
{
    if (is_paused_)
        return paused_time_ - start_time_ - total_idle_time_;
    return current_time_ - start_time_ - total_idle_time_;
}

bool Timer::IsPausing() const
{
    return is_paused_;
}

void Timer::Tick()
{
    if (is_paused_)
    {
        delta_time_ = 0;
        return;
    }

    current_time_ = Time::Now();

    // compute the time elapsed since the previous frame
    delta_time_ = (current_time_ - previous_time_);

    // set previous time to current time, as in the next tick, this frame will be the previous frame
    previous_time_ = current_time_;

    // delta time can be negative if the processor goes idle for example
    if (delta_time_ < 0)
        delta_time_ = 0;
}

void Timer::Resume()
{
    if (is_paused_)
    {
        const auto now = Time::Now();

        // add the duration of the pause to the total idle time
        total_idle_time_ += (now - paused_time_);

        // set the previous time to the current time
        previous_time_ = now;

        paused_time_ = Time();
        is_paused_   = false;
    }
}

void Timer::Pause()
{
    if (!is_paused_)
    {
        const auto now = Time::Now();

        paused_time_ = now;
        is_paused_   = true;
    }
}

void Timer::Reset()
{
    const auto now = Time::Now();

    start_time_    = now;
    current_time_  = now;
    previous_time_ = now;
    paused_time_   = Time();
    delta_time_    = 0;
    is_paused_     = false;
}

}  // namespace kiwano
