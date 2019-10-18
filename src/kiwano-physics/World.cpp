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

#include "World.h"

namespace kiwano
{
	namespace physics
	{
		World::World()
			: world_(b2Vec2(0, 10))
			, vel_iter_(6)
			, pos_iter_(2)
			, global_scale_(100.f)
		{
		}

		World::World(Vec2 gravity)
			: world_(b2Vec2(gravity.x, gravity.y))
			, vel_iter_(6)
			, pos_iter_(2)
			, global_scale_(100.f)
		{
		}

		World::~World()
		{
			// Make sure destroyed b2World after b2Body
			RemoveAllChildren();
		}

		BodyPtr World::CreateBody(Actor* actor)
		{
			BodyPtr body = new Body(this, actor);
			body->UpdateFromActor();
			return body;
		}

		JointPtr World::CreateJoint(b2JointDef* joint_def)
		{
			JointPtr joint = new Joint(this, joint_def);
			return joint;
		}

		void World::RemoveBody(Body* body)
		{
			if (body)
			{
				auto iter = std::find(bodies_.begin(), bodies_.end(), body);
				if (iter != bodies_.end())
				{
					bodies_.erase(iter);
				}

				if (body->GetB2Body())
				{
					world_.DestroyBody(body->GetB2Body());
				}
			}
		}

		void World::RemoveAllBodies()
		{
			bodies_.clear();
		}

		void World::RemoveJoint(Joint* joint)
		{
			if (joint)
			{
				auto iter = std::find(joints_.begin(), joints_.end(), joint);
				if (iter != joints_.end())
				{
					joints_.erase(iter);
				}

				if (joint->GetB2Joint())
				{
					world_.DestroyJoint(joint->GetB2Joint());
				}
			}
		}

		void World::RemoveAllJoints()
		{
			joints_.clear();
		}

		b2World* World::GetB2World()
		{
			return &world_;
		}

		const b2World* World::GetB2World() const
		{
			return &world_;
		}

		Vec2 World::GetGravity() const
		{
			b2Vec2 g = world_.GetGravity();
			return Vec2(g.x, g.y);
		}

		void World::SetGravity(Vec2 gravity)
		{
			world_.SetGravity(b2Vec2(gravity.x, gravity.y));
		}

		void World::Update(Duration dt)
		{
			Stage::Update(dt);

			world_.Step(dt.Seconds(), vel_iter_, pos_iter_);

			b2Body* b2body = world_.GetBodyList();
			while (b2body)
			{
				Body* body = static_cast<Body*>(b2body->GetUserData());
				if (body)
				{
					body->UpdateActor();
				}

				b2body = b2body->GetNext();
			}
		}

	}
}
