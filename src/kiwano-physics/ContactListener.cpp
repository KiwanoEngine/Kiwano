// Copyright (c) 2018-2019 Kiwano - Nomango
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

#include <kiwano-physics/ContactListener.h>
#include <kiwano-physics/Body.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
	namespace physics
	{
		ContactListener::ContactListener()
			: running_(true)
		{
		}

		ContactListener::~ContactListener()
		{
		}


		ContactCallbackListener::ContactCallbackListener()
		{
		}

		ContactCallbackListener::~ContactCallbackListener()
		{
		}


		ContactListener* ContactDispatcher::AddContactListener(ContactListenerPtr listener)
		{
			return AddContactListener(listener.get());
		}

		ContactListener* ContactDispatcher::AddContactListener(ContactListener* listener)
		{
			KGE_ASSERT(listener && "AddListener failed, NULL pointer exception");

			if (listener)
			{
				listeners_.push_back(listener);
			}
			return listener;
		}

		void ContactDispatcher::StartContactListeners(String const& listener_name)
		{
			for (auto listener : listeners_)
			{
				if (listener->IsName(listener_name))
				{
					listener->Start();
				}
			}
		}

		void ContactDispatcher::StopContactListeners(String const& listener_name)
		{
			for (auto listener : listeners_)
			{
				if (listener->IsName(listener_name))
				{
					listener->Stop();
				}
			}
		}

		void ContactDispatcher::RemoveContactListeners(String const& listener_name)
		{
			ContactListenerPtr next;
			for (auto listener = listeners_.first_item(); listener; listener = next)
			{
				next = listener->next_item();

				if (listener->IsName(listener_name))
				{
					listeners_.remove(listener);
				}
			}
		}

		void ContactDispatcher::StartAllContactListeners()
		{
			for (auto listener : listeners_)
			{
				listener->Start();
			}
		}

		void ContactDispatcher::StopAllContactListeners()
		{
			for (auto listener : listeners_)
			{
				listener->Stop();
			}
		}

		void ContactDispatcher::RemoveAllContactListeners()
		{
			listeners_.clear();
		}

		void ContactDispatcher::OnContactBegin(b2Contact* b2contact)
		{
			if (listeners_.empty())
				return;

			Contact contact(b2contact);
			ContactListenerPtr next;
			for (auto listener = listeners_.first_item(); listener; listener = next)
			{
				next = listener->next_item();

				if (listener->IsRunning())
				{
					listener->OnContactBegin(contact);
				}
			}
		}

		void ContactDispatcher::OnContactEnd(b2Contact* b2contact)
		{
			if (listeners_.empty())
				return;

			Contact contact(b2contact);
			ContactListenerPtr next;
			for (auto listener = listeners_.first_item(); listener; listener = next)
			{
				next = listener->next_item();

				if (listener->IsRunning())
				{
					listener->OnContactEnd(contact);
				}
			}
		}

	}
}
