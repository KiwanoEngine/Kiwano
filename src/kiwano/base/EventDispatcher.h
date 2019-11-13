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
#include <kiwano/base/EventListener.h>

namespace kiwano
{
	class KGE_API EventDispatcher
	{
		using Listeners = IntrusiveList<EventListenerPtr>;

	public:
		// Ìí¼Ó¼àÌýÆ÷
		EventListener* AddListener(
			EventListenerPtr listener
		);

		// Ìí¼Ó¼àÌýÆ÷
		EventListener* AddListener(
			EventListener* listener
		);

		// Ìí¼Ó¼àÌýÆ÷
		EventListener* AddListener(
			String const& name,
			EventType type,
			EventListener::Callback callback
		);

		// Ìí¼Ó¼àÌýÆ÷
		EventListener* AddListener(
			EventType type,
			EventListener::Callback callback
		);

		// Æô¶¯¼àÌýÆ÷
		void StartListeners(
			String const& listener_name
		);

		// Í£Ö¹¼àÌýÆ÷
		void StopListeners(
			String const& listener_name
		);

		// ÒÆ³ý¼àÌýÆ÷
		void RemoveListeners(
			String const& listener_name
		);

		// Æô¶¯¼àÌýÆ÷
		void StartListeners(
			const EventType& type
		);

		// Í£Ö¹¼àÌýÆ÷
		void StopListeners(
			const EventType& type
		);

		// ÒÆ³ý¼àÌýÆ÷
		void RemoveListeners(
			const EventType& type
		);

		virtual void Dispatch(Event* evt);

	protected:
		Listeners listeners_;
	};
}
