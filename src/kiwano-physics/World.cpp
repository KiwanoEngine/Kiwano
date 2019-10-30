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
		namespace
		{
			const float DefaultGlobalScale = 100.f;		// 100 pixels per meters
		}

		class PhysicWorld::DestructionListener : public b2DestructionListener
		{
			PhysicWorld* world_;

		public:
			DestructionListener(PhysicWorld* world)
				: world_(world)
			{
			}

			void SayGoodbye(b2Joint* joint) override
			{
				if (world_)
				{
					world_->JointRemoved(joint);
				}
			}

			void SayGoodbye(b2Fixture* fixture) override
			{

			}
		};

		PhysicWorld::PhysicWorld()
			: world_(b2Vec2(0, 10.0f))
			, vel_iter_(6)
			, pos_iter_(2)
			, global_scale_(DefaultGlobalScale)
			, destruction_listener_(nullptr)
			, removing_joint_(false)
		{
			destruction_listener_ = new DestructionListener(this);
			world_.SetDestructionListener(destruction_listener_);
		}

		PhysicWorld::~PhysicWorld()
		{
			world_.SetDestructionListener(nullptr);
			if (destruction_listener_)
			{
				delete destruction_listener_;
				destruction_listener_ = nullptr;
			}

			// Make sure b2World was destroyed after b2Body
			RemoveAllChildren();
			RemoveAllBodies();
			RemoveAllJoints();
		}

		void PhysicWorld::RemoveBody(PhysicBody* body)
		{
			if (body)
			{
				if (body->GetB2Body())
				{
					world_.DestroyBody(body->GetB2Body());
				}
			}
		}

		void PhysicWorld::RemoveAllBodies()
		{
			if (world_.GetBodyCount())
			{
				b2Body* body = world_.GetBodyList();
				while (body)
				{
					b2Body* next = body->GetNext();
					world_.DestroyBody(body);
					body = next;
				}
			}
		}

		void PhysicWorld::AddJoint(PhysicJoint* joint)
		{
			if (joint)
			{
				joints_.push_back(joint);
			}
		}

		void PhysicWorld::RemoveJoint(PhysicJoint* joint)
		{
			if (joint)
			{
				auto iter = std::find(joints_.begin(), joints_.end(), joint);
				if (iter != joints_.end())
				{
					joints_.erase(iter);

					if (joint->GetB2Joint())
					{
						removing_joint_ = true;
						world_.DestroyJoint(joint->GetB2Joint());
						removing_joint_ = false;
					}
				}
			}
		}

		void PhysicWorld::RemoveAllJoints()
		{
			if (world_.GetJointCount())
			{
				removing_joint_ = true;
				{
					b2Joint* joint = world_.GetJointList();
					while (joint)
					{
						b2Joint* next = joint->GetNext();
						world_.DestroyJoint(joint);
						joint = next;
					}
				}
				removing_joint_ = false;
			}
			joints_.clear();
		}

		void PhysicWorld::JointRemoved(b2Joint* joint)
		{
			if (!removing_joint_ && joint)
			{
				auto iter = std::find_if(
					joints_.begin(),
					joints_.end(),
					[joint](PhysicJoint* j) -> bool { return j->GetB2Joint() == joint; }
				);

				if (iter != joints_.end())
				{
					joints_.erase(iter);
				}
			}
		}

		b2World* PhysicWorld::GetB2World()
		{
			return &world_;
		}

		const b2World* PhysicWorld::GetB2World() const
		{
			return &world_;
		}

		Vec2 PhysicWorld::GetGravity() const
		{
			b2Vec2 g = world_.GetGravity();
			return Vec2(g.x, g.y);
		}

		void PhysicWorld::SetGravity(Vec2 gravity)
		{
			world_.SetGravity(b2Vec2(gravity.x, gravity.y));
		}

		void PhysicWorld::Update(Duration dt)
		{
			Stage::Update(dt);

			b2Body* b2body = world_.GetBodyList();
			while (b2body)
			{
				PhysicBody* body = static_cast<PhysicBody*>(b2body->GetUserData());
				if (body && body->GetType() != PhysicBody::Type::Static)
				{
					body->UpdateFromActor();
				}

				b2body = b2body->GetNext();
			}

			world_.Step(dt.Seconds(), vel_iter_, pos_iter_);

			b2body = world_.GetBodyList();
			while (b2body)
			{
				PhysicBody* body = static_cast<PhysicBody*>(b2body->GetUserData());
				if (body && body->GetType() != PhysicBody::Type::Static)
				{
					body->UpdateActor();
				}

				b2body = b2body->GetNext();
			}
		}

	}
}
