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

#include <kiwano/platform/Window.h>

namespace kiwano
{

Window::Window()
    : handle_(nullptr)
    , should_close_(false)
    , is_fullscreen_(false)
    , pos_x_(0)
    , pos_y_(0)
    , real_width_(0)
    , real_height_(0)
    , min_width_(0)
    , min_height_(0)
    , max_width_(0)
    , max_height_(0)
{
}

Window::~Window() {}

RefPtr<Event> Window::PollEvent()
{
    RefPtr<Event> evt;
    if (!event_queue_.empty())
    {
        evt = event_queue_.front();
        event_queue_.pop();
    }
    return evt;
}

String Window::GetTitle() const
{
    return title_;
}

int Window::GetPosX() const
{
    return pos_x_;
}

int Window::GetPosY() const
{
    return pos_y_;
}

Size Window::GetLogicalSize() const
{
    return logical_size_;
}

uint32_t Window::GetRealWidth() const
{
    return real_width_;
}

uint32_t Window::GetRealHeight() const
{
    return real_height_;
}

float Window::GetDPI() const
{
    return dpi_;
}

float Window::GetDPIScale() const
{
    return dpi_ / 96.f;
}

Resolution Window::GetCurrentResolution() const
{
    return resolution_;
}

WindowHandle Window::GetHandle() const
{
    return handle_;
}

bool Window::ShouldClose()
{
    return should_close_;
}

void Window::SetShouldClose(bool should)
{
    should_close_ = should;
}

void Window::PushEvent(RefPtr<Event> evt)
{
    event_queue_.push(evt);
}

}  // namespace kiwano
