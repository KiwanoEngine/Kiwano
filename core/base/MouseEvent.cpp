// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "MouseEvent.h"
#include "window.h"

namespace easy2d
{
	MouseEvent::MouseEvent(UINT message, WPARAM w_param, LPARAM l_param)
		: message_(message)
		, w_param_(w_param)
		, l_param_(l_param)
	{
	}

	float MouseEvent::GetX() const
	{
		return ((float)(short)LOWORD(l_param_)) * Window::Instance().GetContentScaleX();
	}

	float MouseEvent::GetY() const
	{
		return ((float)(short)HIWORD(l_param_)) * Window::Instance().GetContentScaleY();
	}

	Point MouseEvent::GetPosition() const
	{
		return Point(
			((float)(short)LOWORD(l_param_)) * Window::Instance().GetContentScaleX(),
			((float)(short)HIWORD(l_param_)) * Window::Instance().GetContentScaleY()
		);
	}

	bool MouseEvent::IsShiftDown() const
	{
		return GET_KEYSTATE_WPARAM(w_param_) == MK_SHIFT;
	}

	bool MouseEvent::IsCtrlDown() const
	{
		return GET_KEYSTATE_WPARAM(w_param_) == MK_CONTROL;
	}

	float MouseEvent::GetWheelDelta() const
	{
		return static_cast<float>(GET_WHEEL_DELTA_WPARAM(w_param_));
	}

	bool MouseEvent::IsLButtonDown() const
	{
		return GET_KEYSTATE_WPARAM(w_param_) == MK_LBUTTON;
	}

	bool MouseEvent::IsRButtonDown() const
	{
		return GET_KEYSTATE_WPARAM(w_param_) == MK_RBUTTON;
	}

	bool MouseEvent::IsMButtonDown() const
	{
		return GET_KEYSTATE_WPARAM(w_param_) == MK_MBUTTON;
	}

	MouseEvent::Type MouseEvent::GetType() const
	{
		return Type(message_);
	}
}