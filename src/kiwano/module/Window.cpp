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

#include <kiwano/module/Window.h>
#include <kiwano/platform/Application.h>

namespace kiwano
{

Window::Window()
    : handle_(nullptr)
    , should_close_(false)
    , is_fullscreen_(false)
    , pos_x_(0)
    , pos_y_(0)
    , width_(0)
    , height_(0)
    , min_width_(0)
    , min_height_(0)
    , max_width_(0)
    , max_height_(0)
    , event_buffer_(24)
{
}

Window::~Window() {}

void Window::OnUpdate(UpdateModuleContext& ctx)
{
    // Poll events
    PumpEvents();

    auto& app = Application::GetInstance();
    while (!event_buffer_.IsEmpty())
    {
        RefPtr<Event> evt = event_buffer_.Front();
        event_buffer_.PopFront();

        auto ctx = EventModuleContext(app.GetModules(), evt);
        ctx.Next();
    }
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

Resolution Window::GetCurrentResolution() const
{
    return resolution_;
}

WindowHandle Window::GetHandle() const
{
    return handle_;
}

bool Window::ShouldClose() const
{
    return should_close_;
}

void Window::SetShouldClose(bool should)
{
    should_close_ = should;
}

void Window::PushEvent(RefPtr<Event> evt)
{
    event_buffer_.PushBack(evt);
}

}  // namespace kiwano
