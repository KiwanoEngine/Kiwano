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
#include <kiwano/common/common.h>
#include <kiwano/core/SmartPtr.hpp>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Event.h>

namespace kiwano
{
	class EventDispatcher;

	KGE_DECLARE_SMART_PTR(EventListener);

	// ÊÂ¼þ¼àÌýÆ÷
	class KGE_API EventListener
		: public ObjectBase
		, protected IntrusiveListItem<EventListenerPtr>
	{
		friend class EventDispatcher;
		friend IntrusiveList<EventListenerPtr>;

	public:
		using Callback = Function<void(Event&)>;

		EventListener();

		EventListener(
			EventType type,
			Callback const& callback
		);

		EventListener(
			String const& name,
			EventType type,
			Callback const& callback
		);

		template <
			typename _EventTy,
			typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type
		>
		EventListener(Callback const& callback)
			: EventListener(KGE_EVENT(_EventTy), callback)
		{
		}

		template <
			typename _EventTy,
			typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type
		>
		EventListener(String const& name, Callback const& callback)
			: EventListener(name, KGE_EVENT(_EventTy), callback)
		{
		}

		virtual ~EventListener();

		void Start();

		void Stop();

		bool IsRunning() const;

		Callback GetCallback() const;

		void SetCallback(Callback const& cb);

		EventType const& GetEventType() const;

		void SetEventType(EventType const& type);

	protected:
		bool		running_;
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

	inline bool EventListener::IsRunning() const
	{
		return running_;
	}

	inline EventListener::Callback EventListener::GetCallback() const
	{
		return callback_;
	}

	inline void EventListener::SetCallback(Callback const& cb)
	{
		callback_ = cb;
	}

	inline EventType const& EventListener::GetEventType() const
	{
		return type_;
	}

	inline void EventListener::SetEventType(EventType const& type)
	{
		type_ = type;
	}

}
