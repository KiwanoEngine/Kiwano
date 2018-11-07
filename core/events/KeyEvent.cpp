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


namespace easy2d
{
	KeyEvent::KeyEvent(UINT message, WPARAM w_param, LPARAM l_param)
		: message_(message)
		, w_param_(w_param)
		, l_param_(l_param)
	{
	}

	KeyCode KeyEvent::GetCode() const
	{
		switch (w_param_)
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

	int KeyEvent::GetCount() const
	{
		return static_cast<int>((DWORD)l_param_ & 0x0000FFFF);
	}

	KeyEvent::Type KeyEvent::GetType() const
	{
		return Type(message_);
	}
}