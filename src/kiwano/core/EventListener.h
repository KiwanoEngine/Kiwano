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
#include <kiwano/core/common.h>
#include <kiwano/core/SmartPtr.hpp>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/event/Event.h>
#include <kiwano/core/event/MouseEvent.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/WindowEvent.h>

namespace kiwano
{
	class EventDispatcher;

	KGE_DECLARE_SMART_PTR(EventListener);

	/**
	* \~chinese
	* @brief 事件监听器
	*/
	class KGE_API EventListener
		: public virtual ObjectBase
		, protected IntrusiveListItem<EventListenerPtr>
	{
		friend class EventDispatcher;
		friend IntrusiveList<EventListenerPtr>;

	public:
		/// \~chinese
		/// @brief 监听器回调函数
		using Callback = Function<void(Event&)>;

		/// \~chinese
		/// @brief 构造空监听器
		EventListener();

		/// \~chinese
		/// @brief 构造监听器
		/// @param type 监听的事件类型
		/// @param callback 回调函数
		EventListener(EventType type, Callback const& callback);

		/// \~chinese
		/// @brief 构造监听器
		/// @param name 监听器名称
		/// @param type 监听的事件类型
		/// @param callback 回调函数
		EventListener(String const& name, EventType type, Callback const& callback);

		/// \~chinese
		/// @brief 构造监听器
		/// @tparam _EventTy 事件类型
		/// @param callback 回调函数
		template <
			typename _EventTy,
			typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type
		>
		inline EventListener(Callback const& callback)
			: EventListener(KGE_EVENT(_EventTy), callback)
		{
		}

		/// \~chinese
		/// @brief 构造监听器
		/// @tparam _EventTy 事件类型
		/// @param name 监听器名称
		/// @param callback 回调函数
		template <
			typename _EventTy,
			typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type
		>
		inline EventListener(String const& name, Callback const& callback)
			: EventListener(name, KGE_EVENT(_EventTy), callback)
		{
		}

		virtual ~EventListener();

		/// \~chinese
		/// @brief 启动监听器
		void Start();

		/// \~chinese
		/// @brief 停止监听器
		void Stop();

		/// \~chinese
		/// @brief 移除监听器
		void Remove();

		/// \~chinese
		/// @brief 是否正在运行
		bool IsRunning() const;

		/// \~chinese
		/// @brief 是否可移除
		bool IsRemoveable() const;

		/// \~chinese
		/// @brief 获取回调函数
		const Callback& GetCallback() const;

		/// \~chinese
		/// @brief 设置回调函数
		void SetCallback(Callback const& cb);

		/// \~chinese
		/// @brief 获取监听的事件类型
		EventType GetEventType() const;

		/// \~chinese
		/// @brief 设置监听的事件类型
		void SetEventType(EventType const& type);

		/// \~chinese
		/// @brief 设置监听的事件类型
		/// @tparam _EventTy 事件类型
		template <
			typename _EventTy,
			typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type
		>
		inline void SetEventType()
		{
			SetEventType(KGE_EVENT(_EventTy));
		}

		/// \~chinese
		/// @brief 接收消息
		void Receive(Event& evt);

	private:
		bool		running_;
		bool		removeable_;
		EventType	type_;
		Callback	callback_;
	};


	inline void EventListener::Start()
	{
		running_ = true;
	}

	inline void EventListener::Stop()
	{
		running_ = false;
	}

	inline void EventListener::Remove()
	{
		removeable_ = true;
	}

	inline bool EventListener::IsRunning() const
	{
		return running_;
	}

	inline bool EventListener::IsRemoveable() const
	{
		return removeable_;
	}

	inline const EventListener::Callback& EventListener::GetCallback() const
	{
		return callback_;
	}

	inline void EventListener::SetCallback(Callback const& cb)
	{
		callback_ = cb;
	}

	inline EventType EventListener::GetEventType() const
	{
		return type_;
	}

	inline void EventListener::SetEventType(EventType const& type)
	{
		type_ = type;
	}

	inline void EventListener::Receive(Event& evt)
	{
		if (type_ == evt.GetType() && callback_)
		{
			callback_(evt);
		}
	}

}
