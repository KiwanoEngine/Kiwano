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
#include <kiwano-physics/helper.h>
#include <kiwano-physics/Fixture.h>

namespace kiwano
{
	namespace physics
	{
		class Body;

		/**
		* \addtogroup Physics
		* @{
		*/

		/// \~chinese
		/// @brief ����Ӵ�
		class KGE_API Contact
		{
		public:
			Contact();
			Contact(b2Contact* contact);

			/// \~chinese
			/// @brief �Ƿ���Ч
			bool IsValid() const;

			/// \~chinese
			/// @brief �Ƿ��ǽӴ�
			bool IsTouching() const;

			/// \~chinese
			/// @brief ���û���� (��������һ��ʱ�䲽)
			void SetEnabled(bool flag);

			/// \~chinese
			/// @brief �Ƿ�����
			bool IsEnabled() const;

			/// \~chinese
			/// @brief ��ȡ����A�ļо�
			Fixture GetFixtureA() const;

			/// \~chinese
			/// @brief ��ȡ����B�ļо�
			Fixture GetFixtureB() const;

			/// \~chinese
			/// @brief ��ȡ����A
			Body* GetBodyA() const;

			/// \~chinese
			/// @brief ��ȡ����B
			Body* GetBodyB() const;

			/// \~chinese
			/// @brief ����Ħ����
			void SetFriction(float friction);

			/// \~chinese
			/// @brief ��ȡĦ����
			float GetFriction() const;

			/// \~chinese
			/// @brief ����Ħ����
			void ResetFriction();

			/// \~chinese
			/// @brief ���õ��Իָ�
			void SetRestitution(float restitution);

			/// \~chinese
			/// @brief ��ȡ���Իָ�
			float GetRestitution() const;

			/// \~chinese
			/// @brief ���õ��Իָ�
			void ResetRestitution();

			/// \~chinese
			/// @brief ���������ٶ�
			void SetTangentSpeed(float speed);

			/// \~chinese
			/// @brief ��ȡ�����ٶ�
			float GetTangentSpeed() const;

			b2Contact* GetB2Contact() const;
			void SetB2Contact(b2Contact* contact);

		private:
			b2Contact* contact_;
		};


		/// \~chinese
		/// @brief �Ӵ���
		class KGE_API ContactEdge
		{
		public:
			ContactEdge();
			ContactEdge(b2ContactEdge* edge);

			/// \~chinese
			/// @brief �Ƿ���Ч
			bool IsValid() const;

			/// \~chinese
			/// @brief ��ȡ�Ӵ�����
			Body* GetOtherBody() const;

			/// \~chinese
			/// @brief ��ȡ�Ӵ�
			Contact GetContact() const;

			b2ContactEdge* GetB2ContactEdge() const;
			void SetB2ContactEdge(b2ContactEdge* edge);

		private:
			b2ContactEdge* edge_;
		};


		/// \~chinese
		/// @brief ����Ӵ��б�
		class ContactList
			: public List<Contact>
		{
		public:
			ContactList()
			{
			}

			ContactList(const Contact& first)
			{
				Contact current = first;
				while (current.GetB2Contact())
				{
					push_back(current);
					current = current.GetB2Contact()->GetNext();
				}
			}
		};


		/// \~chinese
		/// @brief ����Ӵ����б�
		class ContactEdgeList
			: public List<ContactEdge>
		{
		public:
			ContactEdgeList()
			{
			}

			ContactEdgeList(const ContactEdge& first)
			{
				ContactEdge current = first;
				while (current.GetB2ContactEdge())
				{
					push_back(current);
					current = current.GetB2ContactEdge()->next;
				}
			}
		};

		/** @} */


		inline bool Contact::IsValid() const						{ return contact_ != nullptr;}
		inline bool Contact::IsTouching() const						{ KGE_ASSERT(contact_); return contact_->IsTouching(); }
		inline void Contact::SetEnabled(bool flag)					{ KGE_ASSERT(contact_); contact_->SetEnabled(flag); }
		inline bool Contact::IsEnabled() const						{ KGE_ASSERT(contact_); return contact_->IsEnabled(); }
		inline void Contact::SetFriction(float friction)			{ KGE_ASSERT(contact_); contact_->SetFriction(friction); }
		inline float Contact::GetFriction() const					{ KGE_ASSERT(contact_); return contact_->GetFriction(); }
		inline void Contact::ResetFriction()						{ KGE_ASSERT(contact_); contact_->ResetFriction(); }
		inline void Contact::SetRestitution(float restitution)		{ KGE_ASSERT(contact_); contact_->SetRestitution(restitution); }
		inline float Contact::GetRestitution() const				{ KGE_ASSERT(contact_); return contact_->GetRestitution(); }
		inline void Contact::ResetRestitution()						{ KGE_ASSERT(contact_); contact_->ResetRestitution(); }
		inline b2Contact* Contact::GetB2Contact() const				{ return contact_; }
		inline void Contact::SetB2Contact(b2Contact* contact)		{ contact_ = contact; }

		inline bool ContactEdge::IsValid() const							{ return edge_ != nullptr; }
		inline Body* ContactEdge::GetOtherBody() const						{ KGE_ASSERT(edge_); return static_cast<Body*>(edge_->other->GetUserData()); }
		inline Contact ContactEdge::GetContact() const						{ KGE_ASSERT(edge_); return Contact(edge_->contact); }
		inline b2ContactEdge* ContactEdge::GetB2ContactEdge() const			{ return edge_; }
		inline void ContactEdge::SetB2ContactEdge(b2ContactEdge* edge)		{ edge_ = edge; }

	}
}
