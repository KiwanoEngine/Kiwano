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

#include <kiwano/utils/Task.h>

namespace kiwano
{
Task::Task(const Callback& cb, TickerPtr ticker)
    : running_(true)
    , removeable_(false)
    , callback_(cb)
    , ticker_(ticker)
{
}

Task::Task(const String& name, const Callback& cb, TickerPtr ticker)
    : Task(cb, ticker)
{
    SetName(name);
}

Task::Task(const Callback& cb, Duration interval, int times)
    : running_(true)
    , removeable_(false)
    , callback_(cb)
{
    ticker_ = MakePtr<Ticker>(interval, times);
}

Task::Task(const String& name, const Callback& cb, Duration interval, int times)
    : Task(cb, interval, times)
{
    SetName(name);
}

Task::Task()
    : running_(true)
    , removeable_(false)
    , callback_()
{
}

void Task::Start()
{
    if (!running_)
    {
        running_ = true;
        if (ticker_)
            ticker_->Resume();
    }
}

void Task::Stop()
{
    if (running_)
    {
        running_ = false;
        if (ticker_)
            ticker_->Pause();
    }
}

void Task::Update(Duration dt)
{
    if (!running_ || removeable_)
        return;

    if (!ticker_ || ticker_->GetTotalTickCount() == 0)
    {
        Remove();
        return;
    }

    if (ticker_->Tick(dt))
    {
        if (callback_)
            callback_(this, ticker_->GetDeltaTime());

        if (ticker_->GetTickedCount() == ticker_->GetTotalTickCount())
            Remove();
    }
}

void Task::Reset()
{
    if (ticker_)
        ticker_->Reset();
}

}  // namespace kiwano
