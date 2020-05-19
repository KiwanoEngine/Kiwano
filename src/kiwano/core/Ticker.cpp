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

#include <kiwano/core/Ticker.h>

namespace kiwano
{

TickerPtr Ticker::Create(Duration interval, int times)
{
    TickerPtr ptr = memory::New<Ticker>();
    if (ptr)
    {
        ptr->SetInterval(interval);
        ptr->SetTotalTickTimes(times);
    }
    return ptr;
}

Ticker::Ticker()
    : ticked_times_(0)
    , total_times_(0)
{
}

bool Ticker::Tick()
{
    if (!timer_)
        timer_ = Timer::Create();

    if (timer_->IsPausing())
        return false;

    timer_->Tick();

    return this->Tick(timer_->GetDeltaTime());
}

bool Ticker::Tick(Duration dt)
{
    if (ticked_times_ == total_times_)
        return false;

    elapsed_time_ += dt;

    if (elapsed_time_ + error_time_ > interval_)
    {
        delta_time_   = elapsed_time_;
        error_time_   = (elapsed_time_ + error_time_) - interval_;
        elapsed_time_ = 0;
        ++ticked_times_;
        return true;
    }
    return false;
}

Duration Ticker::GetDeltaTime()
{
    return delta_time_;
}

TimerPtr Ticker::GetTimer()
{
    return timer_;
}

void Ticker::SetTimer(TimerPtr timer)
{
    timer_ = timer;
}

void Ticker::Reset()
{
    if (timer_)
        timer_->Reset();

    elapsed_time_ = 0;
    delta_time_   = 0;
    error_time_   = 0;
    ticked_times_ = 0;
}

}  // namespace kiwano
