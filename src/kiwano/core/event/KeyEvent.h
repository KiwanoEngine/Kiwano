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
#include <kiwano/core/event/Event.h>
#include <kiwano/core/Keys.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(KeyEvent);
	KGE_DECLARE_SMART_PTR(KeyDownEvent);
	KGE_DECLARE_SMART_PTR(KeyUpEvent);
	KGE_DECLARE_SMART_PTR(KeyCharEvent);

	/**
	* \addtogroup Events
	* @{
	*/

	/// \~chinese
	/// @brief 键盘事件
	class KGE_API KeyEvent
		: public Event
	{
	public:
		KeyEvent(const EventType& type);
	};

	/// \~chinese
	/// @brief 键盘按下事件
	class KGE_API KeyDownEvent
		: public KeyEvent
	{
	public:
		KeyCode code;	///< 键值

		KeyDownEvent();
	};

	/// \~chinese
	/// @brief 键盘抬起事件
	class KGE_API KeyUpEvent
		: public KeyEvent
	{
	public:
		KeyCode code;	///< 键值

		KeyUpEvent();
	};

	/// \~chinese
	/// @brief 键盘字符事件
	class KGE_API KeyCharEvent
		: public KeyEvent
	{
	public:
		char value;		///< 字符

		KeyCharEvent();
	};

	/** @} */

	template <>
	struct IsEventType<KeyEvent>
	{
		inline bool operator()(const Event* evt) const
		{
			return evt->GetType() == KGE_EVENT(KeyDownEvent)
				|| evt->GetType() == KGE_EVENT(KeyUpEvent)
				|| evt->GetType() == KGE_EVENT(KeyCharEvent);
		}
	};

}
