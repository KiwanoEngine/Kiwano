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

namespace kiwano
{

TimerPtr Timer::Create(const Callback& cb, Duration interval, int times)
{
    TimerPtr ptr = new (std::nothrow) Timer;
    if (ptr)
    {
        ptr->SetCallback(cb);
        ptr->SetInterval(interval);
        ptr->SetTotalRunTimes(times);
    }
    return ptr;
}

TimerPtr Timer::Create(const String& name, const Callback& cb, Duration interval, int times)
{
    TimerPtr ptr = new (std::nothrow) Timer;
    if (ptr)
    {
        ptr->SetName(name);
        ptr->SetCallback(cb);
        ptr->SetInterval(interval);
        ptr->SetTotalRunTimes(times);
    }
    return ptr;
}

Timer::Timer()
    : running_(true)
    , removeable_(false)
    , run_times_(0)
    , total_times_(0)
    , interval_(0)
    , elapsed_(0)
    , callback_()
{
}

void Timer::Update(Duration dt)
{
    if (total_times_ == 0)
    {
        Remove();
        return;
    }

    if (IsRunning())
    {
        if (!interval_.IsZero())
        {
            elapsed_ += dt;
            if (elapsed_ < interval_)
                return;
        }

        if (callback_)
            callback_(this, elapsed_);

        ++run_times_;
        elapsed_ = 0;

        if (run_times_ == total_times_)
            Remove();
    }
}

void Timer::Reset()
{
    elapsed_   = 0;
    run_times_ = 0;
}

}  // namespace kiwano
