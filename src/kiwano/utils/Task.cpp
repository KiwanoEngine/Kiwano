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
TaskPtr Task::Create(const Callback& cb, TickerPtr ticker)
{
    TaskPtr ptr = memory::New<Task>();
    if (ptr)
    {
        ptr->SetCallback(cb);
        ptr->SetTicker(ticker);
    }
    return ptr;
}

TaskPtr Task::Create(const String& name, const Callback& cb, TickerPtr ticker)
{
    TaskPtr ptr = Task::Create(cb, ticker);
    if (ptr)
    {
        ptr->SetName(name);
    }
    return ptr;
}

TaskPtr Task::Create(const Callback& cb, Duration interval, int times)
{
    TickerPtr ticker = Ticker::Create(interval, times);
    return Task::Create(cb, ticker);
}

TaskPtr Task::Create(const String& name, const Callback& cb, Duration interval, int times)
{
    TickerPtr ticker = Ticker::Create(interval, times);
    return Task::Create(name, cb, ticker);
}

Task::Task()
    : running_(true)
    , removeable_(false)
    , callback_()
{
}

void Task::Update(Duration dt)
{
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
