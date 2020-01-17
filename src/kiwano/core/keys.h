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
	/// @brief Êó±ê°´¼ü
	enum class MouseButton
	{
		Left,		///< Êó±ê×ó¼ü
		Right,		///< Êó±êÓÒ¼ü
		Middle,		///< Êó±êÖÐ¼ü

		Last
	};


	/// \~chinese
	/// @brief °´¼ü¼üÖµ
	enum class KeyCode
	{
		Unknown,	///< Î´Öª
		Up,			///< ÉÏ¼ü
		Left,		///< ×ó¼ü
		Right,		///< ÓÒ¼ü
		Down,		///< ÏÂ¼ü
		Enter,		///< »Ø³µ¼ü
		Space,		///< ¿Õ¸ñ¼ü
		Esc,		///< ÍË³ö¼ü
		Ctrl,		///< CTRL¼ü
		Shift,		///< SHIFT¼ü
		Alt,		///< ALT¼ü
		Tab,		///< TAB¼ü
		Delete,		///< É¾³ý¼ü
		Back,		///< ÍË¸ñ¼ü
		Super,		///< Cmd|Super|Windows¼ü

		A,			///< A¼ü
		B,			///< B¼ü
		C,			///< C¼ü
		D,			///< D¼ü
		E,			///< E¼ü
		F,			///< F¼ü
		G,			///< G¼ü
		H,			///< H¼ü
		I,			///< I¼ü
		J,			///< J¼ü
		K,			///< K¼ü
		L,			///< L¼ü
		M,			///< M¼ü
		N,			///< N¼ü
		O,			///< O¼ü
		P,			///< P¼ü
		Q,			///< Q¼ü
		R,			///< R¼ü
		S,			///< S¼ü
		T,			///< T¼ü
		U,			///< U¼ü
		V,			///< V¼ü
		W,			///< W¼ü
		X,			///< X¼ü
		Y,			///< Y¼ü
		Z,			///< Z¼ü

		Num0,		///< Êý×Ö0¼ü
		Num1,		///< Êý×Ö1¼ü
		Num2,		///< Êý×Ö2¼ü
		Num3,		///< Êý×Ö3¼ü
		Num4,		///< Êý×Ö4¼ü
		Num5,		///< Êý×Ö5¼ü
		Num6,		///< Êý×Ö6¼ü
		Num7,		///< Êý×Ö7¼ü
		Num8,		///< Êý×Ö8¼ü
		Num9,		///< Êý×Ö9¼ü

		Numpad0,	///< Êý×ÖÐ¡¼üÅÌ0¼ü
		Numpad1,	///< Êý×ÖÐ¡¼üÅÌ1¼ü
		Numpad2,	///< Êý×ÖÐ¡¼üÅÌ2¼ü
		Numpad3,	///< Êý×ÖÐ¡¼üÅÌ3¼ü
		Numpad4,	///< Êý×ÖÐ¡¼üÅÌ4¼ü
		Numpad5,	///< Êý×ÖÐ¡¼üÅÌ5¼ü
		Numpad6,	///< Êý×ÖÐ¡¼üÅÌ6¼ü
		Numpad7,	///< Êý×ÖÐ¡¼üÅÌ7¼ü
		Numpad8,	///< Êý×ÖÐ¡¼üÅÌ8¼ü
		Numpad9,	///< Êý×ÖÐ¡¼üÅÌ9¼ü

		F1,			///< F1¼ü
		F2,			///< F2¼ü
		F3,			///< F3¼ü
		F4,			///< F4¼ü
		F5,			///< F5¼ü
		F6,			///< F6¼ü
		F7,			///< F7¼ü
		F8,			///< F8¼ü
		F9,			///< F9¼ü
		F10,		///< F10¼ü
		F11,		///< F11¼ü
		F12,		///< F12¼ü

		Last
	};
}
