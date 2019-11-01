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

		PhysicFixture::PhysicFixture(PhysicBody* body, PhysicShape* shape, const Param& param)
			: PhysicFixture()
		{
			KGE_ASSERT(body);

			if (shape)
			{
				shape->FitWorld(body->GetWorld());

				b2Body* b2body = body->GetB2Body();
				b2FixtureDef fd;
				fd.density = param.density;
				fd.friction = param.friction;
				fd.restitution = param.restitution;
				fd.shape = shape->GetB2Shape();
				auto fixture = b2body->CreateFixture(&fd);
				SetB2Fixture(fixture);
			}
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

		void PhysicFixture::GetMassData(float* mass, Point* center, float* inertia) const
		{
			KGE_ASSERT(fixture_);

			const PhysicBody* body = GetBody();
			KGE_ASSERT(body);

			const PhysicWorld* world = body->GetWorld();
			KGE_ASSERT(world);

			b2MassData data;
			fixture_->GetMassData(&data);

			if (mass) *mass = data.mass;
			if (center) *center = world->World2Stage(data.center);
			if (inertia) *inertia = data.I;
		}

		bool PhysicFixture::TestPoint(const Point& p) const
		{
			KGE_ASSERT(fixture_);

			const PhysicBody* body = GetBody();
			KGE_ASSERT(body);

			const PhysicWorld* world = body->GetWorld();
			KGE_ASSERT(world);

			return fixture_->TestPoint(world->Stage2World(p));
		}

	}
}
