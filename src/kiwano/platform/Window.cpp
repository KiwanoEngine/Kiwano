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
    , width_(0)
    , height_(0)
    , min_width_(100)
    , min_height_(50)
{
}

Window::~Window()
{
}

EventPtr Window::PollEvent()
{
    EventPtr evt;
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

Size Window::GetSize() const
{
    return Size(float(width_), float(height_));
}

uint32_t Window::GetWidth() const
{
    return width_;
}

uint32_t Window::GetHeight() const
{
    return height_;
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

void Window::SetFullscreenState(bool is_fullscreen)
{
    is_fullscreen_ = is_fullscreen;
}

void Window::PushEvent(EventPtr evt)
{
    event_queue_.push(evt);
}

}  // namespace kiwano
