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

#pragma once
#include <kiwano-physics/Contact.h>

namespace kiwano
{
	namespace physics
	{
		class ContactDispatcher;

		KGE_DECLARE_SMART_PTR(ContactListener);

		// �Ӵ�������
		class KGE_API ContactListener
			: public ObjectBase
			, protected IntrusiveListItem<ContactListenerPtr>
		{
			friend IntrusiveList<ContactListenerPtr>;
			friend class ContactDispatcher;

		public:
			ContactListener();
			virtual ~ContactListener();

			// �Ӵ���ʼ
			virtual void OnContactBegin(Contact contact)	{ KGE_NOT_USED(contact); }

			// �Ӵ�����
			virtual void OnContactEnd(Contact contact)	{ KGE_NOT_USED(contact); }

			inline void Start()									{ running_ = true; }
			inline void Stop()									{ running_ = true; }
			inline bool IsRunning() const						{ return running_; }

		protected:
			bool running_;
		};


		KGE_DECLARE_SMART_PTR(ContactCallbackListener);

		// �Ӵ��ص�������
		class KGE_API ContactCallbackListener
			: public ContactListener
		{
			friend IntrusiveList<ContactListenerPtr>;
			friend class ContactDispatcher;

		public:
			using ContactBeginCallback	= Function<void(Contact)>;
			using ContactEndCallback	= Function<void(Contact)>;

			ContactCallbackListener();
			virtual ~ContactCallbackListener();

			// �Ӵ���ʼ�ص�
			void SetCallbackOnContactBegin(ContactBeginCallback const& cb)	{ begin_ = cb; }
			ContactBeginCallback GetCallbackOnContactBegin() const			{ return begin_; }

			// �Ӵ������ص�
			void SetCallbackOnContactEnd(ContactEndCallback const& cb)		{ end_ = cb; }
			ContactEndCallback GetCallbackOnContactEnd() const				{ return end_; }

			void OnContactBegin(Contact contact) override				{ if (begin_) begin_(contact); }
			void OnContactEnd(Contact contact) override				{ if (end_) end_(contact); }

		protected:
			ContactBeginCallback begin_;
			ContactEndCallback end_;
		};


		// �Ӵ��ַ���
		class KGE_API ContactDispatcher
		{
		public:
			using Listeners = IntrusiveList<ContactListenerPtr>;

			// ��Ӽ�����
			ContactListener* AddContactListener(
				ContactListenerPtr listener
			);

			// ��Ӽ�����
			ContactListener* AddContactListener(
				ContactListener* listener
			);

			// ����������
			void StartContactListeners(
				String const& listener_name
			);

			// ֹͣ������
			void StopContactListeners(
				String const& listener_name
			);

			// �Ƴ�������
			void RemoveContactListeners(
				String const& listener_name
			);

			// �������м�����
			void StartAllContactListeners();

			// ֹͣ���м�����
			void StopAllContactListeners();

			// �Ƴ����м�����
			void RemoveAllContactListeners();

		protected:
			void OnContactBegin(b2Contact* contact);

			void OnContactEnd(b2Contact* contact);

		private:
			Listeners listeners_;
		};
	}
}
