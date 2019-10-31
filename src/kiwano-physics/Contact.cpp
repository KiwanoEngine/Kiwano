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

#include <kiwano-physics/Contact.h>
#include <kiwano-physics/Body.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
	namespace physics
	{

		PhysicContact::PhysicContact()
			: contact_(nullptr)
		{
		}

		PhysicContact::PhysicContact(b2Contact* contact)
			: PhysicContact()
		{
			SetB2Contact(contact);
		}

		PhysicContact PhysicContact::GetNext()
		{
			KGE_ASSERT(contact_);
			return PhysicContact(contact_->GetNext());
		}

		const PhysicContact PhysicContact::GetNext() const
		{
			KGE_ASSERT(contact_);
			return PhysicContact(contact_->GetNext());
		}

		PhysicFixture PhysicContact::GetFixtureA()
		{
			KGE_ASSERT(contact_);
			return PhysicFixture(contact_->GetFixtureA());
		}

		const PhysicFixture PhysicContact::GetFixtureA() const
		{
			KGE_ASSERT(contact_);
			return PhysicFixture(contact_->GetFixtureA());
		}

		PhysicFixture PhysicContact::GetFixtureB()
		{
			KGE_ASSERT(contact_);
			return PhysicFixture(contact_->GetFixtureB());
		}

		const PhysicFixture PhysicContact::GetFixtureB() const
		{
			KGE_ASSERT(contact_);
			return PhysicFixture(contact_->GetFixtureB());
		}

		void PhysicContact::SetTangentSpeed(float speed)
		{
			KGE_ASSERT(contact_);

			PhysicBody* body = GetFixtureA().GetBody();
			KGE_ASSERT(body);

			PhysicWorld* world = body->GetWorld();
			KGE_ASSERT(world);

			contact_->SetTangentSpeed(world->Stage2World(speed));
		}

		float PhysicContact::GetTangentSpeed() const
		{
			KGE_ASSERT(contact_);

			const PhysicBody* body = GetFixtureA().GetBody();
			KGE_ASSERT(body);

			const PhysicWorld* world = body->GetWorld();
			KGE_ASSERT(world);

			return world->World2Stage(contact_->GetTangentSpeed());
		}


		PhysicContactEdge::PhysicContactEdge()
			: edge_(nullptr)
		{
		}

		PhysicContactEdge::PhysicContactEdge(b2ContactEdge* edge)
			: PhysicContactEdge()
		{
			SetB2ContactEdge(edge);
		}

	}
}
