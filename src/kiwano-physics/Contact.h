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

		// �Ӵ�
		class KGE_API Contact
		{
		public:
			Contact();
			Contact(b2Contact* contact);
			
			// �Ƿ��ǽӴ�
			bool IsTouching() const						{ KGE_ASSERT(contact_); return contact_->IsTouching(); }

			// ���û���� (��������һ��ʱ�䲽)
			void SetEnabled(bool flag)					{ KGE_ASSERT(contact_); contact_->SetEnabled(flag); }
			bool IsEnabled() const						{ KGE_ASSERT(contact_); return contact_->IsEnabled(); }

			// ��ȡ��һ�Ӵ�
			Contact GetNext();
			const Contact GetNext() const;

			// �о� A
			Fixture GetFixtureA();
			const Fixture GetFixtureA() const;

			// �о� B
			Fixture GetFixtureB();
			const Fixture GetFixtureB() const;

			// Ħ��
			void SetFriction(float friction)			{ KGE_ASSERT(contact_); contact_->SetFriction(friction); }
			float GetFriction() const					{ KGE_ASSERT(contact_); return contact_->GetFriction(); }
			void ResetFriction()						{ KGE_ASSERT(contact_); contact_->ResetFriction(); }

			// ���Իָ�
			void SetRestitution(float restitution)		{ KGE_ASSERT(contact_); contact_->SetRestitution(restitution); }
			float GetRestitution() const				{ KGE_ASSERT(contact_); return contact_->GetRestitution(); }
			void ResetRestitution()						{ KGE_ASSERT(contact_); contact_->ResetRestitution(); }

			// �����ٶ�
			void SetTangentSpeed(float speed);
			float GetTangentSpeed() const;

			b2Contact* GetB2Contact()					{ return contact_; }
			const b2Contact* GetB2Contact() const		{ return contact_; }
			void SetB2Contact(b2Contact* contact)		{ contact_ = contact; }

		protected:
			b2Contact* contact_;
		};


		// �Ӵ���
		class KGE_API ContactEdge
		{
		public:
			ContactEdge();
			ContactEdge(b2ContactEdge* edge);

			// ��ȡ�Ӵ�����
			Body* GetOtherBody()						{ KGE_ASSERT(edge_); return static_cast<Body*>(edge_->other->GetUserData()); }
			const Body* GetOtherBody() const			{ KGE_ASSERT(edge_); return static_cast<Body*>(edge_->other->GetUserData()); }

			// ��ȡ�Ӵ�
			Contact GetContact()						{ KGE_ASSERT(edge_); return Contact(edge_->contact); }
			const Contact GetContact() const			{ KGE_ASSERT(edge_); return Contact(edge_->contact); }

			// ��ȡ��һ�Ӵ���
			ContactEdge GetPrev()						{ KGE_ASSERT(edge_); return ContactEdge(edge_->prev); }
			const ContactEdge GetPrev() const			{ KGE_ASSERT(edge_); return ContactEdge(edge_->prev); }

			// ��ȡ��һ�Ӵ���
			ContactEdge GetNext()						{ KGE_ASSERT(edge_); return ContactEdge(edge_->next); }
			const ContactEdge GetNext() const			{ KGE_ASSERT(edge_); return ContactEdge(edge_->next); }

			b2ContactEdge* GetB2ContactEdge()				{ return edge_; }
			const b2ContactEdge* GetB2ContactEdge() const	{ return edge_; }
			void SetB2ContactEdge(b2ContactEdge* edge)		{ edge_ = edge; }

		protected:
			b2ContactEdge* edge_;
		};

	}
}
