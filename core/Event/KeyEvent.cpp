#include "..\e2dcommon.h"


e2d::KeyEvent::KeyEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	: _code(KeyCode(wParam))
	, _type(Type(message))
	, _count(static_cast<int>((DWORD)lParam & 0x0000FFFF))
{
}

e2d::KeyCode e2d::KeyEvent::getCode() const
{
	return _code;
}

int e2d::KeyEvent::getCount() const
{
	return _count;
}

e2d::KeyEvent::Type e2d::KeyEvent::getType() const
{
	return _type;
}

e2d::KeyCode e2d::KeyEvent::convertKeyCode(WPARAM wParam)
{
	switch (wParam)
	{
	case 'A': return KeyCode::A;
	case 'B': return KeyCode::B;
	case 'C': return KeyCode::C;
	case 'D': return KeyCode::D;
	case 'E': return KeyCode::E;
	case 'F': return KeyCode::F;
	case 'G': return KeyCode::G;
	case 'H': return KeyCode::H;
	case 'I': return KeyCode::I;
	case 'J': return KeyCode::J;
	case 'K': return KeyCode::K;
	case 'L': return KeyCode::L;
	case 'M': return KeyCode::M;
	case 'N': return KeyCode::N;
	case 'O': return KeyCode::O;
	case 'P': return KeyCode::P;
	case 'Q': return KeyCode::Q;
	case 'R': return KeyCode::R;
	case 'S': return KeyCode::S;
	case 'T': return KeyCode::T;
	case 'U': return KeyCode::U;
	case 'V': return KeyCode::V;
	case 'W': return KeyCode::W;
	case 'X': return KeyCode::X;
	case 'Y': return KeyCode::Y;
	case 'Z': return KeyCode::Z;
	case '0': return KeyCode::Num0;
	case '1': return KeyCode::Num1;
	case '2': return KeyCode::Num2;
	case '3': return KeyCode::Num3;
	case '4': return KeyCode::Num4;
	case '5': return KeyCode::Num5;
	case '6': return KeyCode::Num6;
	case '7': return KeyCode::Num7;
	case '8': return KeyCode::Num8;
	case '9': return KeyCode::Num9;
	case VK_NUMPAD0: return KeyCode::Numpad0;
	case VK_NUMPAD1: return KeyCode::Numpad1;
	case VK_NUMPAD2: return KeyCode::Numpad2;
	case VK_NUMPAD3: return KeyCode::Numpad3;
	case VK_NUMPAD4: return KeyCode::Numpad4;
	case VK_NUMPAD5: return KeyCode::Numpad5;
	case VK_NUMPAD6: return KeyCode::Numpad6;
	case VK_NUMPAD7: return KeyCode::Numpad7;
	case VK_NUMPAD8: return KeyCode::Numpad8;
	case VK_NUMPAD9: return KeyCode::Numpad9;
	case VK_UP: return KeyCode::Up;
	case VK_DOWN: return KeyCode::Down;
	case VK_LEFT: return KeyCode::Left;
	case VK_RIGHT: return KeyCode::Right;
	case VK_RETURN: return KeyCode::Enter;
	case VK_SPACE: return KeyCode::Space;
	case VK_ESCAPE: return KeyCode::Esc;
	default: return KeyCode::Unknown;
	}
}
