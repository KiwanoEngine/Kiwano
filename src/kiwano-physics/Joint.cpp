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

#include <kiwano-physics/Joint.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
	namespace physics
	{
		//
		// Joint
		//

		Joint::Joint()
			: joint_(nullptr)
			, world_(nullptr)
			, type_(Type::Unknown)
		{
		}

		Joint::Joint(b2Joint* joint)
			: Joint()
		{
			SetB2Joint(joint);
		}

		Joint::Joint(World* world, b2JointDef* joint_def)
			: Joint()
		{
			world_ = world;
			if (world_)
			{
				b2Joint* joint = world_->GetB2World()->CreateJoint(joint_def);
				SetB2Joint(joint);
			}
		}

		Joint::~Joint()
		{
			if (world_)
			{
				world_->RemoveJoint(this);
			}
		}

		BodyPtr Joint::GetBodyA() const
		{
			KGE_ASSERT(joint_);

			b2Body* body = joint_->GetBodyA();
			return BodyPtr(static_cast<Body*>(body->GetUserData()));
		}

		BodyPtr Joint::GetBodyB() const
		{
			KGE_ASSERT(joint_);

			b2Body* body = joint_->GetBodyB();
			return BodyPtr(static_cast<Body*>(body->GetUserData()));
		}

		void Joint::SetB2Joint(b2Joint* joint)
		{
			joint_ = joint;
			if (joint_)
			{
				type_ = Joint::Type(joint_->GetType());
			}
		}

		//
		// DistanceJoint
		//

		DistanceJoint::DistanceJoint()
			: Joint()
		{
		}

		DistanceJoint::DistanceJoint(World* world, b2DistanceJointDef* def)
			: Joint(world, def)
		{
		}

		DistanceJointPtr DistanceJoint::Create(World* world, Param const& param)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2DistanceJointDef def;
			def.bodyA = param.body_a->GetB2Body();
			def.bodyB = param.body_b->GetB2Body();
			def.localAnchorA = world->Stage2World(param.local_anchor_a);
			def.localAnchorB = world->Stage2World(param.local_anchor_b);
			def.length = world->Stage2World((param.body_a->GetWorldPoint(param.local_anchor_a) - param.body_b->GetWorldPoint(param.local_anchor_b)).Length());

			DistanceJointPtr joint = new DistanceJoint(world, &def);
			return joint;
		}
}
}
