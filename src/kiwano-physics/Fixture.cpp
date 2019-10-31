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

#include <kiwano-physics/Fixture.h>
#include <kiwano-physics/Body.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
	namespace physics
	{

		PhysicFixture::PhysicFixture()
			: fixture_(nullptr)
		{
		}

		PhysicFixture::PhysicFixture(b2Fixture* fixture)
			: PhysicFixture()
		{
			SetB2Fixture(fixture);
		}

		PhysicFixture PhysicFixture::Create(PhysicBody* body, PhysicShape* shape, float density, float friction, float restitution)
		{
			KGE_ASSERT(body);

			if (shape)
			{
				shape->FitWorld(body->GetWorld());

				b2Body* b2body = body->GetB2Body();
				b2FixtureDef fd;
				fd.density = density;
				fd.friction = friction;
				fd.restitution = restitution;
				fd.shape = shape->GetB2Shape();
				auto fixture = b2body->CreateFixture(&fd);
				return PhysicFixture(fixture);
			}
			return PhysicFixture();
		}

		PhysicBody* PhysicFixture::GetBody()
		{
			KGE_ASSERT(fixture_);
			return static_cast<PhysicBody*>(fixture_->GetBody()->GetUserData());
		}

		const PhysicBody* PhysicFixture::GetBody() const
		{
			KGE_ASSERT(fixture_);
			return static_cast<const PhysicBody*>(fixture_->GetBody()->GetUserData());
		}

		PhysicShape PhysicFixture::GetShape() const
		{
			KGE_ASSERT(fixture_);
			return PhysicShape(fixture_->GetShape());
		}

		PhysicFixture PhysicFixture::GetNext() const
		{
			KGE_ASSERT(fixture_);
			return PhysicFixture(fixture_->GetNext());
		}

	}
}
