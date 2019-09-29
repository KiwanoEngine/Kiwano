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
#include "../core/core.h"
#include "../base/SmartPtr.hpp"
#include "ObjectBase.h"
#include "Event.hpp"

namespace kiwano
{
	typedef Function<void(Event const&)> EventCallback;

	class EventDispatcher;

	KGE_DECLARE_SMART_PTR(EventListener);

	// ÊÂ¼þ¼àÌýÆ÷
	class KGE_API EventListener
		: public ObjectBase
		, protected intrusive_list_item<EventListenerPtr>
	{
		friend class EventDispatcher;
		friend class intrusive_list<EventListenerPtr>;

	public:
		EventListener(
			std::uint32_t type,
			EventCallback const& callback,
			String const& name = L""
		);

		virtual ~EventListener();

		inline void Start()				{ running_ = true; }

		inline void Stop()				{ running_ = true; }

		inline bool IsRunning() const	{ return running_; }

	protected:
		bool			running_;
		std::uint32_t	type_;
		EventCallback	callback_;
	};
}
