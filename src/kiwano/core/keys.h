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

#pragma once
#include <kiwano/macros.h>

namespace kiwano
{
	/// \~chinese
	/// @brief ��갴��
	struct MouseButton
	{
		typedef int Value;

		enum : Value
		{
			Left	= VK_LBUTTON,	///< ������
			Right	= VK_RBUTTON,	///< ����Ҽ�
			Middle	= VK_MBUTTON	///< ����м�
		};
	};


	/// \~chinese
	/// @brief ������ֵ
	struct KeyCode
	{
		typedef int Value;

		enum : Value
		{
			Unknown = 0,			///< δ֪
			Up		= VK_UP,		///< �ϼ�
			Left	= VK_LEFT,		///< ���
			Right	= VK_RIGHT,		///< �Ҽ�
			Down	= VK_DOWN,		///< �¼�
			Enter	= VK_RETURN,	///< �س���
			Space	= VK_SPACE,		///< �ո��
			Esc		= VK_ESCAPE,	///< �˳���
			Ctrl	= VK_CONTROL,	///< CTRL��
			Shift	= VK_SHIFT,		///< SHIFT��
			Alt		= VK_MENU,		///< ALT��
			Tab		= VK_TAB,		///< TAB��
			Delete	= VK_DELETE,	///< ɾ����
			Back	= VK_BACK,		///< �˸��
			Super	= VK_LWIN,		///< Cmd/Super/Windows��

			A = 0x41,				///< A��
			B,						///< B��
			C,						///< C��
			D,						///< D��
			E,						///< E��
			F,						///< F��
			G,						///< G��
			H,						///< H��
			I,						///< I��
			J,						///< J��
			K,						///< K��
			L,						///< L��
			M,						///< M��
			N,						///< N��
			O,						///< O��
			P,						///< P��
			Q,						///< Q��
			R,						///< R��
			S,						///< S��
			T,						///< T��
			U,						///< U��
			V,						///< V��
			W,						///< W��
			X,						///< X��
			Y,						///< Y��
			Z,						///< Z��

			Num0 = 0x30,			///< ����0��
			Num1,					///< ����1��
			Num2,					///< ����2��
			Num3,					///< ����3��
			Num4,					///< ����4��
			Num5,					///< ����5��
			Num6,					///< ����6��
			Num7,					///< ����7��
			Num8,					///< ����8��
			Num9,					///< ����9��

			Numpad0 = VK_NUMPAD0,	///< ����С����0��
			Numpad1,				///< ����С����1��
			Numpad2,				///< ����С����2��
			Numpad3,				///< ����С����3��
			Numpad4,				///< ����С����4��
			Numpad5,				///< ����С����5��
			Numpad6,				///< ����С����6��
			Numpad7,				///< ����С����7��
			Numpad8,				///< ����С����8��
			Numpad9,				///< ����С����9��

			F1 = VK_F1,				///< F1��
			F2,						///< F2��
			F3,						///< F3��
			F4,						///< F4��
			F5,						///< F5��
			F6,						///< F6��
			F7,						///< F7��
			F8,						///< F8��
			F9,						///< F9��
			F10,					///< F10��
			F11,					///< F11��
			F12,					///< F12��
		};
	};
}
