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
	/// @brief �¼�����
	class EventType
		: public std::type_index
	{
		class Dummy { };

	public:
		/// \~chinese
		/// @brief �����¼�����
		EventType() : std::type_index(typeid(EventType::Dummy)) {}

		/// \~chinese
		/// @brief �����¼�����
		/// @param info �¼���ʶ��
		EventType(const type_info& info) : std::type_index(info) {}

		/// \~chinese
		/// @brief �����¼�����
		/// @param index �¼���ʶ��
		EventType(const std::type_index& index) : std::type_index(index) {}
	};

#define KGE_EVENT(EVENT_TYPE)	::kiwano::EventType(typeid(EVENT_TYPE))


	/**
	* \~chinese
	* \defgroup Events �¼�
	*/

	/**
	* \addtogroup Events
	* @{
	*/

	/// \~chinese
	/// @brief �¼�
	class KGE_API Event
	{
	public:
		/// \~chinese
		/// @brief �����¼�
		Event(EventType const& type);

		virtual ~Event();

		/// \~chinese
		/// @brief ��ȡ�����¼�
		inline const EventType& GetType() const
		{
			return type_;
		}

		/// \~chinese
		/// @brief �ж��¼�����
		/// @return �Ƿ���ָ���¼�����
		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline bool IsType() const
		{
			return type_ == KGE_EVENT(_Ty);
		}

		/// \~chinese
		/// @brief ת��Ϊ���������¼�
		/// @throw std::bad_cast �����޷�ת��ʱ�׳�
		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline const _Ty& Cast() const
		{
			return *dynamic_cast<const _Ty*>(this);
		}

		/// \~chinese
		/// @brief ת��Ϊ���������¼�
		/// @throw std::bad_cast �����޷�ת��ʱ�׳�
		template <
			typename _Ty,
			typename = typename std::enable_if<std::is_base_of<Event, _Ty>::value, int>::type
		>
		inline _Ty& Cast()
		{
			return *dynamic_cast<_Ty*>(this);
		}

		/// \~chinese
		/// @brief ��ȫת��Ϊ���������¼�
		/// @throw std::bad_cast �����޷�ת��ʱ�׳�
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
		/// @brief ��ȫת��Ϊ���������¼�
		/// @throw std::bad_cast �����޷�ת��ʱ�׳�
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
	/// @brief �¼����ԣ��ж��Ƿ����¼�����
	template <typename _Ty>
	class IsEvent : public std::bool_constant<std::is_base_of<Event, _Ty>::value || std::is_same<Event, _Ty>::value> {};


	/// \~chinese
	/// @brief ����¼�
	class KGE_API MouseEvent
		: public Event
	{
	public:
		Point pos;				///< ���λ��
		bool left_btn_down;		///< �������Ƿ���
		bool right_btn_down;	///< ����Ҽ��Ƿ���
		Actor* target;			///< Ŀ��

		MouseEvent(EventType const& type);
	};

	/// \~chinese
	/// @brief ����ƶ��¼�
	class KGE_API MouseMoveEvent
		: public MouseEvent
	{
	public:
		MouseMoveEvent();
	};

	/// \~chinese
	/// @brief ��갴�������¼�
	class KGE_API MouseDownEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;	///< ����ֵ

		MouseDownEvent();
	};

	/// \~chinese
	/// @brief ��갴��̧���¼�
	class KGE_API MouseUpEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;	///< ����ֵ

		MouseUpEvent();
	};

	/// \~chinese
	/// @brief ������¼�
	class KGE_API MouseClickEvent
		: public MouseEvent
	{
	public:
		MouseButton::Value button;	///< ����ֵ

		MouseClickEvent();
	};

	/// \~chinese
	/// @brief ��������¼�
	class KGE_API MouseHoverEvent
		: public MouseEvent
	{
	public:
		MouseHoverEvent();
	};

	/// \~chinese
	/// @brief ����Ƴ��¼�
	class KGE_API MouseOutEvent
		: public MouseEvent
	{
	public:
		MouseOutEvent();
	};

	/// \~chinese
	/// @brief �������¼�
	class KGE_API MouseWheelEvent
		: public MouseEvent
	{
	public:
		float wheel;			///< ����ֵ

		MouseWheelEvent();
	};

	/// \~chinese
	/// @brief ���̰����¼�
	class KGE_API KeyDownEvent
		: public Event
	{
	public:
		KeyCode::Value code;	///< ��ֵ

		KeyDownEvent();
	};

	/// \~chinese
	/// @brief ����̧���¼�
	class KGE_API KeyUpEvent
		: public Event
	{
	public:
		KeyCode::Value code;	///< ��ֵ

		KeyUpEvent();
	};

	/// \~chinese
	/// @brief �����ַ��¼�
	class KGE_API KeyCharEvent
		: public Event
	{
	public:
		char value;		///< �ַ�

		KeyCharEvent();
	};

	/// \~chinese
	/// @brief �����ƶ��¼�
	class KGE_API WindowMovedEvent
		: public Event
	{
	public:
		int x;			///< �������Ͻ� x ����
		int y;			///< �������Ͻ� y ����

		WindowMovedEvent();
	};

	/// \~chinese
	/// @brief ���ڴ�С�仯�¼�
	class KGE_API WindowResizedEvent
		: public Event
	{
	public:
		int width;		///< ���ڿ��
		int height;		///< ���ڸ߶�

		WindowResizedEvent();
	};

	/// \~chinese
	/// @brief ���ڽ���仯�¼�
	class KGE_API WindowFocusChangedEvent
		: public Event
	{
	public:
		bool focus;		///< �Ƿ��ȡ������

		WindowFocusChangedEvent();
	};

	/// \~chinese
	/// @brief ���ڱ�������¼�
	class KGE_API WindowTitleChangedEvent
		: public Event
	{
	public:
		String title;	///< ����

		WindowTitleChangedEvent();
	};

	/// \~chinese
	/// @brief ���ڹر��¼�
	class KGE_API WindowClosedEvent
		: public Event
	{
	public:
		WindowClosedEvent();
	};

	/** @} */

}
