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
			: body_(nullptr)
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
		}

		Body::~Body()
		{
			if (world_)
			{
				world_->RemoveBody(this);
			}
		}

		b2Fixture* Body::AddShape(ShapePtr shape, Property const& prop)
		{
			KGE_ASSERT(body_ && world_);
			KGE_ASSERT(shape);

			if (shape)
			{
				shape->FitWorld(world_);

				b2FixtureDef fd;
				fd.density = prop.density;
				fd.friction = prop.friction;
				fd.restitution = prop.restitution;
				fd.shape = shape->GetB2Shape();

				return body_->CreateFixture(&fd);
			}
			return nullptr;
		}

		b2Fixture* Body::AddCircleShape(float radius, Property const& prop)
		{
			ShapePtr shape = new CircleShape(radius);
			return AddShape(shape, prop);
		}

		b2Fixture* Body::AddBoxShape(Vec2 const& size, Property const& prop)
		{
			ShapePtr shape = new BoxShape(size);
			return AddShape(shape, prop);
		}

		b2Fixture* Body::AddPolygonShape(Vector<Point> const& vertexs, Property const& prop)
		{
			ShapePtr shape = new PolygonShape(vertexs);
			return AddShape(shape, prop);
		}

		Body::Type Body::GetType() const
		{
			KGE_ASSERT(body_);
			return Type(body_->GetType());
		}

		void Body::SetType(Type type)
		{
			KGE_ASSERT(body_);
			body_->SetType(static_cast<b2BodyType>(type));
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
				actor_->SetRotation(math::Radian2Angle(body_->GetAngle()));
			}
		}

		void Body::UpdateFromActor()
		{
			if (actor_ && body_)
			{
				if (world_)
				{
					body_->SetTransform(
						world_->Stage2World(actor_->GetPosition()),
						math::Angle2Radian(actor_->GetRotation())
					);
				}
				else
				{
					body_->SetTransform(
						Stage2World(actor_->GetPosition()),
						math::Angle2Radian(actor_->GetRotation())
					);
				}
			}
		}

	}
}
