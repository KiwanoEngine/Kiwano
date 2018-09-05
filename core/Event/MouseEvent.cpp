#include "..\e2devent.h"
#include "..\e2dmodule.h"

e2d::MouseEvent::MouseEvent(HWND hWnd, UINT message, WPARAM w_param, LPARAM l_param, float dpi)
	: message_(message)
	, w_param_(w_param)
	, l_param_(l_param)
	, type_(Type(message))
{
	pos_.x = ((float)(short)LOWORD(l_param)) * 96.f / dpi;
	pos_.y = ((float)(short)HIWORD(l_param)) * 96.f / dpi;
}

float e2d::MouseEvent::GetX() const
{
	return pos_.x;
}

float e2d::MouseEvent::GetY() const
{
	return pos_.y;
}

e2d::Point e2d::MouseEvent::GetPos() const
{
	return pos_;
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
	return type_;
}
