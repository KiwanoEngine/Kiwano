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
	const float dpi = Window::GetInstance()->GetDpi();
	return ((float)(short)LOWORD(l_param_)) * 96.f / dpi;
}

float e2d::MouseEvent::GetY() const
{
	const float dpi = Window::GetInstance()->GetDpi();
	return ((float)(short)HIWORD(l_param_)) * 96.f / dpi;
}

e2d::Point e2d::MouseEvent::GetPos() const
{
	const float dpi = Window::GetInstance()->GetDpi();
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
