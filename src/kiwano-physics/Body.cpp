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

		Body::Body()
			: ignore_rotation_(false)
			, body_(nullptr)
			, actor_(nullptr)
			, world_(nullptr)
		{
		}

		Body::Body(b2Body* body, Actor* actor)
			: Body()
		{
			SetB2Body(body);
			SetActor(actor);
		}

		Body::Body(World* world, Actor* actor)
			: Body()
		{
			world_ = world;
			if (world_)
			{
				b2BodyDef def;
				b2Body* body = world_->GetB2World()->CreateBody(&def);
				SetB2Body(body);
			}
			SetActor(actor);
			UpdateFromActor();
		}

		Body::~Body()
		{
			if (world_)
			{
				world_->RemoveBody(this);
			}
		}

		Fixture Body::AddShape(Shape* shape, Fixture::Property const& prop)
		{
			KGE_ASSERT(body_ && world_);

			if (shape)
			{
				return Fixture(this, shape, prop);
			}
			return Fixture();
		}

		Fixture Body::AddCircleShape(float radius, Fixture::Property const& prop)
		{
			return AddShape(&CircleShape(radius), prop);
		}

		Fixture Body::AddBoxShape(Vec2 const& size, Fixture::Property const& prop)
		{
			return AddShape(&BoxShape(size), prop);
		}

		Fixture Body::AddPolygonShape(Vector<Point> const& vertexs, Fixture::Property const& prop)
		{
			return AddShape(&PolygonShape(vertexs), prop);
		}

		Fixture Body::AddEdgeShape(Point const& p1, Point const& p2, Fixture::Property const& prop)
		{
			return AddShape(&EdgeShape(p1, p2), prop);
		}

		Fixture Body::AddChainShape(Vector<Point> const& vertexs, bool loop, Fixture::Property const& prop)
		{
			return AddShape(&ChainShape(vertexs, loop), prop);
		}

		void Body::RemoveFixture(Fixture const& fixture)
		{
			if (fixture.GetB2Fixture())
			{
				b2Fixture* ptr = const_cast<b2Fixture*>(fixture.GetB2Fixture());
				body_->DestroyFixture(ptr);
			}
		}

		Point Body::GetLocalPoint(Point const& world) const
		{
			KGE_ASSERT(body_ && world_);
			return world_->World2Stage(body_->GetLocalPoint(world_->Stage2World(world)));
		}

		Point Body::GetWorldPoint(Point const& local) const
		{
			KGE_ASSERT(body_ && world_);
			return world_->World2Stage(body_->GetWorldPoint(world_->Stage2World(local)));
		}

		void Body::SetB2Body(b2Body* body)
		{
			body_ = body;
			if (body_)
			{
				body_->SetUserData(this);
			}
		}

		void Body::UpdateActor()
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

				if (!ignore_rotation_)
				{
					actor_->SetRotation(math::Radian2Angle(body_->GetAngle()));
				}
			}
		}

		void Body::UpdateFromActor()
		{
			if (actor_ && body_)
			{
				float rotation = ignore_rotation_ ? body_->GetAngle() : math::Angle2Radian(actor_->GetRotation());

				if (world_)
				{
					body_->SetTransform(
						world_->Stage2World(actor_->GetPosition()),
						rotation
					);
				}
				else
				{
					body_->SetTransform(
						Stage2World(actor_->GetPosition()),
						rotation
					);
				}
			}
		}

}
}
