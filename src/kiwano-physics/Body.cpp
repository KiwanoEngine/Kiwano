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

#include <kiwano-physics/Body.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
	namespace physics
	{

		PhysicBody::PhysicBody()
			: body_(nullptr)
			, actor_(nullptr)
			, world_(nullptr)
			, category_bits_(0x0001)
			, mask_bits_(0xFFFF)
			, group_index_(0)
		{
		}

		PhysicBody::PhysicBody(b2Body* body, Actor* actor)
			: PhysicBody()
		{
			SetB2Body(body);
			SetActor(actor);
		}

		PhysicBody::PhysicBody(PhysicWorld* world, Actor* actor)
			: PhysicBody()
		{
			Init(world, actor);
		}

		PhysicBody::~PhysicBody()
		{
			Destroy();
		}

		void PhysicBody::Init(PhysicWorld* world, Actor* actor)
		{
			KGE_ASSERT(world);

			Destroy();

			world_ = world;
			b2BodyDef def;
			b2Body* b2body = world->GetB2World()->CreateBody(&def);

			SetB2Body(b2body);
			SetActor(actor);
			UpdateFromActor();
		}

		PhysicFixture PhysicBody::AddFixture(PhysicShape* shape, const PhysicFixture::Param& param)
		{
			KGE_ASSERT(body_ && world_);
			return PhysicFixture(this, shape, param);
		}

		PhysicFixture PhysicBody::AddCircleShape(float radius, float density)
		{
			return AddFixture(&PhysicCircleShape(radius), PhysicFixture::Param(density));
		}

		PhysicFixture PhysicBody::AddBoxShape(Vec2 const& size, float density)
		{
			return AddFixture(&PhysicBoxShape(size), PhysicFixture::Param(density));
		}

		PhysicFixture PhysicBody::AddPolygonShape(Vector<Point> const& vertexs, float density)
		{
			return AddFixture(&PhysicPolygonShape(vertexs), PhysicFixture::Param(density));
		}

		PhysicFixture PhysicBody::AddEdgeShape(Point const& p1, Point const& p2, float density)
		{
			return AddFixture(&PhysicEdgeShape(p1, p2), PhysicFixture::Param(density));
		}

		PhysicFixture PhysicBody::AddChainShape(Vector<Point> const& vertexs, bool loop, float density)
		{
			return AddFixture(&PhysicChainShape(vertexs, loop), PhysicFixture::Param(density));
		}

		void PhysicBody::RemoveFixture(PhysicFixture const& fixture)
		{
			if (fixture.GetB2Fixture())
			{
				b2Fixture* ptr = const_cast<b2Fixture*>(fixture.GetB2Fixture());
				body_->DestroyFixture(ptr);
			}
		}

		void PhysicBody::SetCategoryBits(uint16_t category_bits)
		{
			KGE_ASSERT(body_);

			if (category_bits != category_bits_)
			{
				category_bits_ = category_bits;

				b2Fixture* fixture = body_->GetFixtureList();
				while (fixture)
				{
					UpdateFixtureFilter(fixture);
					fixture = fixture->GetNext();
				}
			}
		}

		void PhysicBody::SetMaskBits(uint16_t mask_bits)
		{
			KGE_ASSERT(body_);

			if (mask_bits != mask_bits_)
			{
				mask_bits_ = mask_bits;

				b2Fixture* fixture = body_->GetFixtureList();
				while (fixture)
				{
					UpdateFixtureFilter(fixture);
					fixture = fixture->GetNext();
				}
			}
		}

		void PhysicBody::SetGroupIndex(int16_t index)
		{
			KGE_ASSERT(body_);

			if (index != group_index_)
			{
				group_index_ = index;

				b2Fixture* fixture = body_->GetFixtureList();
				while (fixture)
				{
					UpdateFixtureFilter(fixture);
					fixture = fixture->GetNext();
				}
			}
		}

		void PhysicBody::GetMassData(float* mass, Point* center, float* inertia) const
		{
			KGE_ASSERT(body_ && world_);

			b2MassData data;
			body_->GetMassData(&data);

			if (mass) *mass = data.mass;
			if (center) *center = world_->World2Stage(data.center);
			if (inertia) *inertia = data.I;
		}

		void PhysicBody::SetMassData(float mass, Point const& center, float inertia)
		{
			KGE_ASSERT(body_ && world_);

			b2MassData data;
			data.mass = mass;
			data.center = world_->Stage2World(center);
			data.I = inertia;
			body_->SetMassData(&data);
		}

		void PhysicBody::ResetMassData()
		{
			KGE_ASSERT(body_);
			body_->ResetMassData();
		}

		Point PhysicBody::GetBodyPosition() const
		{
			KGE_ASSERT(body_ && world_);
			return world_->World2Stage(body_->GetPosition());
		}

		void PhysicBody::SetBodyTransform(Point const& pos, float angle)
		{
			KGE_ASSERT(body_ && world_);
			body_->SetTransform(world_->Stage2World(pos), math::Degree2Radian(angle));
		}

		Point PhysicBody::GetLocalPoint(Point const& world) const
		{
			KGE_ASSERT(body_ && world_);
			return world_->World2Stage(body_->GetLocalPoint(world_->Stage2World(world)));
		}

		Point PhysicBody::GetWorldPoint(Point const& local) const
		{
			KGE_ASSERT(body_ && world_);
			return world_->World2Stage(body_->GetWorldPoint(world_->Stage2World(local)));
		}

		Point PhysicBody::GetLocalCenter() const
		{
			KGE_ASSERT(body_ && world_);
			return world_->World2Stage(body_->GetLocalCenter());
		}

		Point PhysicBody::GetWorldCenter() const
		{
			KGE_ASSERT(body_ && world_);
			return world_->World2Stage(body_->GetWorldCenter());
		}

		void PhysicBody::ApplyForce(Vec2 const& force, Point const& point, bool wake)
		{
			KGE_ASSERT(body_ && world_);
			body_->ApplyForce(b2Vec2(force.x, force.y), world_->Stage2World(point), wake);
		}

		void PhysicBody::ApplyForceToCenter(Vec2 const& force, bool wake)
		{
			KGE_ASSERT(body_ && world_);
			body_->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
		}

		void PhysicBody::ApplyTorque(float torque, bool wake)
		{
			KGE_ASSERT(body_ && world_);
			body_->ApplyTorque(torque, wake);
		}

		void PhysicBody::SetB2Body(b2Body* body)
		{
			body_ = body;
			if (body_)
			{
				body_->SetUserData(this);
			}
		}

		void PhysicBody::Destroy()
		{
			if (world_)
			{
				world_->RemoveBody(this);
			}

			body_ = nullptr;
			world_ = nullptr;
			actor_ = nullptr;
		}

		void PhysicBody::UpdateActor()
		{
			if (actor_ && body_)
			{
				if (world_)
				{
					actor_->SetPosition(world_->World2Stage(body_->GetPosition()));
				}
				else
				{
					actor_->SetPosition(World2Stage(body_->GetPosition()));
				}
				actor_->SetRotation(math::Radian2Degree(body_->GetAngle()));
			}
		}

		void PhysicBody::UpdateFromActor()
		{
			if (actor_ && body_)
			{
				if (world_)
				{
					body_->SetTransform(
						world_->Stage2World(actor_->GetPosition()),
						math::Degree2Radian(actor_->GetRotation())
					);
				}
				else
				{
					body_->SetTransform(
						Stage2World(actor_->GetPosition()),
						math::Degree2Radian(actor_->GetRotation())
					);
				}
			}
		}

		void PhysicBody::UpdateFixtureFilter(b2Fixture* fixture)
		{
			b2Filter filter;
			filter.categoryBits = category_bits_;
			filter.maskBits = mask_bits_;
			filter.groupIndex = group_index_;
			fixture->SetFilterData(filter);
		}

}
}
