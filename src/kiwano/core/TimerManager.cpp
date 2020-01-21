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

#include <kiwano/core/Logger.h>
#include <kiwano/core/TimerManager.h>

namespace kiwano
{
void TimerManager::UpdateTimers(Duration dt)
{
    if (timers_.empty())
        return;

    TimerPtr next;
    for (auto timer = timers_.first_item(); timer; timer = next)
    {
        next = timer->next_item();

        timer->Update(dt);

        if (timer->IsRemoveable())
            timers_.remove(timer);
    }
}

Timer* TimerManager::AddTimer(Timer::Callback const& cb, Duration interval, int times)
{
    return AddTimer(String(), cb, interval, times);
}

Timer* TimerManager::AddTimer(String const& name, Timer::Callback const& cb, Duration interval, int times)
{
    TimerPtr timer = new Timer(name, cb, interval, times);
    return AddTimer(timer);
}

Timer* TimerManager::AddTimer(TimerPtr timer)
{
    KGE_ASSERT(timer && "AddTimer failed, NULL pointer exception");

    if (timer)
    {
        timer->Reset();
        timers_.push_back(timer);
    }

    return timer.get();
}

void TimerManager::StopTimers(String const& name)
{
    if (timers_.empty())
        return;

    for (auto& timer : timers_)
    {
        if (timer.IsName(name))
        {
            timer.Stop();
        }
    }
}

void TimerManager::StartTimers(String const& name)
{
    if (timers_.empty())
        return;

    for (auto& timer : timers_)
    {
        if (timer.IsName(name))
        {
            timer.Start();
        }
    }
}

void TimerManager::RemoveTimers(String const& name)
{
    if (timers_.empty())
        return;

    for (auto& timer : timers_)
    {
        if (timer.IsName(name))
        {
            timer.Remove();
        }
    }
}

void TimerManager::StopAllTimers()
{
    if (timers_.empty())
        return;

    for (auto& timer : timers_)
    {
        timer.Stop();
    }
}

void TimerManager::StartAllTimers()
{
    if (timers_.empty())
        return;

    for (auto& timer : timers_)
    {
        timer.Start();
    }
}

void TimerManager::RemoveAllTimers()
{
    timers_.clear();
}

const TimerManager::Timers& TimerManager::GetAllTimers() const
{
    return timers_;
}
}  // namespace kiwano
