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
		class PhysicContactDispatcher;

		KGE_DECLARE_SMART_PTR(PhysicContactListener);

		// �Ӵ�������
		class KGE_API PhysicContactListener
			: public ObjectBase
			, protected IntrusiveListItem<PhysicContactListenerPtr>
		{
			friend IntrusiveList<PhysicContactListenerPtr>;
			friend class PhysicContactDispatcher;

		public:
			PhysicContactListener();
			virtual ~PhysicContactListener();

			// �Ӵ���ʼ
			virtual void OnContactBegin(PhysicContact contact)	{ KGE_NOT_USED(contact); }

			// �Ӵ�����
			virtual void OnContactEnd(PhysicContact contact)	{ KGE_NOT_USED(contact); }

			inline void Start()									{ running_ = true; }
			inline void Stop()									{ running_ = true; }
			inline bool IsRunning() const						{ return running_; }

		protected:
			bool running_;
		};


		KGE_DECLARE_SMART_PTR(PhysicContactCallbackListener);

		// �Ӵ��ص�������
		class KGE_API PhysicContactCallbackListener
			: public PhysicContactListener
		{
			friend IntrusiveList<PhysicContactListenerPtr>;
			friend class PhysicContactDispatcher;

		public:
			using ContactBeginCallback	= Function<void(PhysicContact)>;
			using ContactEndCallback	= Function<void(PhysicContact)>;

			PhysicContactCallbackListener();
			virtual ~PhysicContactCallbackListener();

			// �Ӵ���ʼ�ص�
			void SetCallbackOnContactBegin(ContactBeginCallback const& cb)	{ begin_ = cb; }
			ContactBeginCallback GetCallbackOnContactBegin() const			{ return begin_; }

			// �Ӵ������ص�
			void SetCallbackOnContactEnd(ContactEndCallback const& cb)		{ end_ = cb; }
			ContactEndCallback GetCallbackOnContactEnd() const				{ return end_; }

			void OnContactBegin(PhysicContact contact) override				{ if (begin_) begin_(contact); }
			void OnContactEnd(PhysicContact contact) override				{ if (end_) end_(contact); }

		protected:
			ContactBeginCallback begin_;
			ContactEndCallback end_;
		};


		// �Ӵ��ַ���
		class KGE_API PhysicContactDispatcher
		{
		public:
			using Listeners = IntrusiveList<PhysicContactListenerPtr>;

			// ���Ӽ�����
			PhysicContactListener* AddContactListener(
				PhysicContactListenerPtr listener
			);

			// ���Ӽ�����
			PhysicContactListener* AddContactListener(
				PhysicContactListener* listener
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

		protected:
			void OnContactBegin(b2Contact* contact);

			void OnContactEnd(b2Contact* contact);

		private:
			Listeners listeners_;
		};
	}
}
