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

#include "..\e2devent.h"
#include "..\e2dmodule.h"

e2d::MouseEvent::MouseEvent(UINT message, WPARAM w_param, LPARAM l_param)
	: message_(message)
	, w_param_(w_param)
	, l_param_(l_param)
{
}

float e2d::MouseEvent::GetX() const
{
	float dpi = Graphics::GetDpi();
	return ((float)(short)LOWORD(l_param_)) * 96.f / dpi;
}

float e2d::MouseEvent::GetY() const
{
	float dpi = Graphics::GetDpi();
	return ((float)(short)HIWORD(l_param_)) * 96.f / dpi;
}

e2d::Point e2d::MouseEvent::GetPosition() const
{
	float dpi = Graphics::GetDpi();
	return Point(
		((float)(short)LOWORD(l_param_)) * 96.f / dpi,
		((float)(short)HIWORD(l_param_)) * 96.f / dpi
	);
}

bool e2d::MouseEvent::IsShiftDown() const
{
	return GET_KEYSTATE_WPARAM(w_param_) == MK_SHIFT;
}

bool e2d::MouseEvent::IsCtrlDown() const
{
	return GET_KEYSTATE_WPARAM(w_param_) == MK_CONTROL;
}

float e2d::MouseEvent::GetWheelDelta() const
{
	return static_cast<float>(GET_WHEEL_DELTA_WPARAM(w_param_));
}

bool e2d::MouseEvent::IsLButtonDown() const
{
	return GET_KEYSTATE_WPARAM(w_param_) == MK_LBUTTON;
}

bool e2d::MouseEvent::IsRButtonDown() const
{
	return GET_KEYSTATE_WPARAM(w_param_) == MK_RBUTTON;
}

bool e2d::MouseEvent::IsMButtonDown() const
{
	return GET_KEYSTATE_WPARAM(w_param_) == MK_MBUTTON;
}

e2d::MouseEvent::Type e2d::MouseEvent::GetType() const
{
	return Type(message_);
}
