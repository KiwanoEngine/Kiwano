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
#include <typeinfo>
#include <typeindex>
#include <kiwano/core/common.h>
#include <kiwano/math/math.h>
#include <kiwano/core/keys.h>

namespace kiwano
{
	class Actor;

	/// \~chinese
	/// @brief 事件类型
	class EventType
		: public std::type_index
	{
		class Dummy { };

	public:
		/// \~chinese
		/// @brief 构建事件类型
		EventType() : std::type_index(typeid(EventType::Dummy)) {}

		/// \~chinese
		/// @brief 构建事件类型
		/// @param info 事件标识符
		EventType(const type_info& info) : std::type_index(info) {}

		/// \~chinese
		/// @brief 构建事件类型
		/// @param index 事件标识符
		EventType(const std::type_index& index) : std::type_index(index) {}
	};

#define KGE_EVENT(EVENT_TYPE)	::kiwano::EventType(typeid(EVENT_TYPE))


	/**
	* \~chinese
	* \defgroup Events 事件
	*/

	/**
	* \addtogroup Events
	* @{
	*/

	/// \~chinese
	/// @brief 事件
	class KGE_API Event
	{
	public:
		/// \~chinese
		/// @brief 构造事件
		Event(EventType const& type);

		virtual ~Event();

		/// \~chinese
		/// @brief 获取类型事件
		inline const EventType& GetType() const
		{
			return type_;
		}

		/// \~chinese
		/// @brief 判断事件类型
		/// @return 是否是指定事件类型
		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline bool IsType() const
		{
			return type_ == KGE_EVENT(_Ty);
		}

		/// \~chinese
		/// @brief 转换为其他类型事件
		/// @throw std::bad_cast 类型无法转换时抛出
		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline const _Ty& Cast() const
		{
			return *dynamic_cast<const _Ty*>(this);
		}

		/// \~chinese
		/// @brief 转换为其他类型事件
		/// @throw std::bad_cast 类型无法转换时抛出
		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline _Ty& Cast()
		{
			return *dynamic_cast<_Ty*>(this);
		}

		/// \~chinese
		/// @brief 安全转换为其他类型事件
		/// @throw std::bad_cast 类型无法转换时抛出
		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline const _Ty& SafeCast() const
		{
			if (!IsType<_Ty>())
				throw std::bad_cast();
			return Cast<_Ty>();
		}

		/// \~chinese
		/// @brief 安全转换为其他类型事件
		/// @throw std::bad_cast 类型无法转换时抛出
		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline _Ty& SafeCast()
		{
			return const_cast<_Ty&>(const_cast<const Event*>(this)->SafeCast<_Ty>());
		}

	private:
		const EventType type_;
	};

	/// \~chinese
	/// @brief 事件特性：判断是否是事件类型
	template <typename _Ty>
	class IsEvent : public std::bool_constant<std::is_base_of<Event, _Ty>::value || std::is_same<Event, _Ty>::value> {};


	/// \~chinese
	/// @brief 鼠标事件
	class KGE_API MouseEvent
		: public Event
	{
	public:
		Point pos;				///< 鼠标位置
		bool left_btn_down;		///< 鼠标左键是否按下
		bool right_btn_down;	///< 鼠标右键是否按下
		Actor* target;			///< 目标

		MouseEvent(EventType const& type);
	};

	/// \~chinese
	/// @brief 鼠标移动事件
	class KGE_API MouseMoveEvent
		: public MouseEvent
	{
	public:
		MouseMoveEvent();
	};

	/// \~chinese
	/// @brief 鼠标按键按下事件
	class KGE_API MouseDownEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;	///< 鼠标键值

		MouseDownEvent();
	};

	/// \~chinese
	/// @brief 鼠标按键抬起事件
	class KGE_API MouseUpEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;	///< 鼠标键值

		MouseUpEvent();
	};

	/// \~chinese
	/// @brief 鼠标点击事件
	class KGE_API MouseClickEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;	///< 鼠标键值

		MouseClickEvent();
	};

	/// \~chinese
	/// @brief 鼠标移入事件
	class KGE_API MouseHoverEvent
		: public MouseEvent
	{
	public:
		MouseHoverEvent();
	};

	/// \~chinese
	/// @brief 鼠标移出事件
	class KGE_API MouseOutEvent
		: public MouseEvent
	{
	public:
		MouseOutEvent();
	};

	/// \~chinese
	/// @brief 鼠标滚轮事件
	class KGE_API MouseWheelEvent
		: public MouseEvent
	{
	public:
		float wheel;			///< 滚轮值

		MouseWheelEvent();
	};

	/// \~chinese
	/// @brief 键盘按下事件
	class KGE_API KeyDownEvent
		: public Event
	{
	public:
		KeyCode::Value code;	///< 键值

		KeyDownEvent();
	};

	/// \~chinese
	/// @brief 键盘抬起事件
	class KGE_API KeyUpEvent
		: public Event
	{
	public:
		KeyCode::Value code;	///< 键值

		KeyUpEvent();
	};

	/// \~chinese
	/// @brief 键盘字符事件
	class KGE_API KeyCharEvent
		: public Event
	{
	public:
		char value;		///< 字符

		KeyCharEvent();
	};

	/// \~chinese
	/// @brief 窗口移动事件
	class KGE_API WindowMovedEvent
		: public Event
	{
	public:
		int x;			///< 窗口左上角 x 坐标
		int y;			///< 窗口左上角 y 坐标

		WindowMovedEvent();
	};

	/// \~chinese
	/// @brief 窗口大小变化事件
	class KGE_API WindowResizedEvent
		: public Event
	{
	public:
		int width;		///< 窗口宽度
		int height;		///< 窗口高度

		WindowResizedEvent();
	};

	/// \~chinese
	/// @brief 窗口焦点变化事件
	class KGE_API WindowFocusChangedEvent
		: public Event
	{
	public:
		bool focus;		///< 是否获取到焦点

		WindowFocusChangedEvent();
	};

	/// \~chinese
	/// @brief 窗口标题更改事件
	class KGE_API WindowTitleChangedEvent
		: public Event
	{
	public:
		String title;	///< 标题

		WindowTitleChangedEvent();
	};

	/// \~chinese
	/// @brief 窗口关闭事件
	class KGE_API WindowClosedEvent
		: public Event
	{
	public:
		WindowClosedEvent();
	};

	/** @} */

}
