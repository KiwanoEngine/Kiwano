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
		/// @brief 物理接触
		class KGE_API Contact
		{
		public:
			Contact();
			Contact(b2Contact* contact);

			/// \~chinese
			/// @brief 是否有效
			bool IsValid() const;

			/// \~chinese
			/// @brief 是否是接触
			bool IsTouching() const;

			/// \~chinese
			/// @brief 启用或禁用 (仅作用于一个时间步)
			void SetEnabled(bool flag);

			/// \~chinese
			/// @brief 是否启用
			bool IsEnabled() const;

			/// \~chinese
			/// @brief 获取物体A的夹具
			Fixture GetFixtureA() const;

			/// \~chinese
			/// @brief 获取物体B的夹具
			Fixture GetFixtureB() const;

			/// \~chinese
			/// @brief 获取物体A
			Body* GetBodyA() const;

			/// \~chinese
			/// @brief 获取物体B
			Body* GetBodyB() const;

			/// \~chinese
			/// @brief 设置摩擦力
			void SetFriction(float friction);

			/// \~chinese
			/// @brief 获取摩擦力
			float GetFriction() const;

			/// \~chinese
			/// @brief 重置摩擦力
			void ResetFriction();

			/// \~chinese
			/// @brief 设置弹性恢复
			void SetRestitution(float restitution);

			/// \~chinese
			/// @brief 获取弹性恢复
			float GetRestitution() const;

			/// \~chinese
			/// @brief 重置弹性恢复
			void ResetRestitution();

			/// \~chinese
			/// @brief 设置切线速度
			void SetTangentSpeed(float speed);

			/// \~chinese
			/// @brief 获取切线速度
			float GetTangentSpeed() const;

			b2Contact* GetB2Contact() const;
			void SetB2Contact(b2Contact* contact);

		private:
			b2Contact* contact_;
		};


		/// \~chinese
		/// @brief 接触边
		class KGE_API ContactEdge
		{
		public:
			ContactEdge();
			ContactEdge(b2ContactEdge* edge);

			/// \~chinese
			/// @brief 是否有效
			bool IsValid() const;

			/// \~chinese
			/// @brief 获取接触物体
			Body* GetOtherBody() const;

			/// \~chinese
			/// @brief 获取接触
			Contact GetContact() const;

			b2ContactEdge* GetB2ContactEdge() const;
			void SetB2ContactEdge(b2ContactEdge* edge);

		private:
			b2ContactEdge* edge_;
		};


		/// \~chinese
		/// @brief 物理接触列表
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
		/// @brief 物理接触边列表
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
