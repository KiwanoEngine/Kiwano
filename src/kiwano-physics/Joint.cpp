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
				world_->AddJoint(this);

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
			, raw_joint_(nullptr)
		{
		}

		DistanceJoint::DistanceJoint(World* world, b2DistanceJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		DistanceJointPtr DistanceJoint::Create(World* world, Param const& param)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2DistanceJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.local_anchor_a), world->Stage2World(param.local_anchor_b));
			def.frequencyHz = param.frequency_hz;
			def.dampingRatio = param.damping_ratio;

			DistanceJointPtr joint = new DistanceJoint(world, &def);
			joint->raw_joint_ = static_cast<b2DistanceJoint*>(joint->GetB2Joint());
			return joint;
		}

		void DistanceJoint::SetLength(float length)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetLength(world_->Stage2World(length));
		}

		float DistanceJoint::GetLength() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetLength());
		}

		//
		// FrictionJoint
		//

		FrictionJoint::FrictionJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		FrictionJoint::FrictionJoint(World* world, b2FrictionJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		FrictionJointPtr FrictionJoint::Create(World* world, Param const& param)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2FrictionJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor));
			def.maxForce = param.max_force;
			def.maxTorque = world->Stage2World(param.max_torque);

			FrictionJointPtr joint = new FrictionJoint(world, &def);
			joint->raw_joint_ = static_cast<b2FrictionJoint*>(joint->GetB2Joint());
			return joint;
		}

		void FrictionJoint::SetMaxForce(float length)
		{
			KGE_ASSERT(raw_joint_);
			raw_joint_->SetMaxForce(length);
		}

		float FrictionJoint::GetMaxForce() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetMaxForce();
		}

		void FrictionJoint::SetMaxTorque(float length)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetMaxTorque(world_->Stage2World(length));
		}

		float FrictionJoint::GetMaxTorque() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetMaxTorque());
		}

		//
		// GearJoint
		//

		GearJoint::GearJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		GearJoint::GearJoint(World* world, b2GearJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		GearJointPtr GearJoint::Create(World* world, Param const& param)
		{
			KGE_ASSERT(param.joint_a && param.joint_b);

			b2GearJointDef def;
			def.joint1 = param.joint_a->GetB2Joint();
			def.joint2 = param.joint_b->GetB2Joint();
			def.ratio = param.ratio;

			GearJointPtr joint = new GearJoint(world, &def);
			joint->raw_joint_ = static_cast<b2GearJoint*>(joint->GetB2Joint());
			return joint;
		}

		void GearJoint::SetRatio(float ratio)
		{
			KGE_ASSERT(raw_joint_);
			raw_joint_->SetRatio(ratio);
		}

		float GearJoint::GetRatio() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetRatio();
		}

		//
		// MotorJoint
		//

		MotorJoint::MotorJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		MotorJoint::MotorJoint(World* world, b2MotorJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		MotorJointPtr MotorJoint::Create(World* world, Param const& param)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2MotorJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body());
			def.maxForce = param.max_force;
			def.maxTorque = world->Stage2World(param.max_torque);

			MotorJointPtr joint = new MotorJoint(world, &def);
			joint->raw_joint_ = static_cast<b2MotorJoint*>(joint->GetB2Joint());
			return joint;
		}

		void MotorJoint::SetMaxForce(float length)
		{
			KGE_ASSERT(raw_joint_);
			raw_joint_->SetMaxForce(length);
		}

		float MotorJoint::GetMaxForce() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetMaxForce();
		}

		void MotorJoint::SetMaxTorque(float length)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetMaxTorque(world_->Stage2World(length));
		}

		float MotorJoint::GetMaxTorque() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetMaxTorque());
		}

		//
		// WheelJoint
		//

		WheelJoint::WheelJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		WheelJoint::WheelJoint(World* world, b2WheelJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		WheelJointPtr WheelJoint::Create(World* world, Param const& param)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2WheelJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor), world->Stage2World(param.axis));
			def.enableMotor = param.enable_motor;
			def.maxMotorTorque = world->Stage2World(param.max_motor_torque);
			def.motorSpeed = world->Stage2World(param.motor_speed);
			def.frequencyHz = param.frequency_hz;
			def.dampingRatio = param.damping_ratio;

			WheelJointPtr joint = new WheelJoint(world, &def);
			joint->raw_joint_ = static_cast<b2WheelJoint*>(joint->GetB2Joint());
			return joint;
		}

		float WheelJoint::GetJointTranslation() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetJointTranslation());
		}

		float WheelJoint::GetJointLinearSpeed() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetJointLinearSpeed());
		}

		void WheelJoint::SetMaxMotorTorque(float torque)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetMaxMotorTorque(world_->Stage2World(torque));
		}

		float WheelJoint::GetMaxMotorTorque() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetMaxMotorTorque());
		}

		//
		// MouseJoint
		//

		MouseJoint::MouseJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		MouseJoint::MouseJoint(World* world, b2MouseJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		MouseJointPtr MouseJoint::Create(World* world, Param const& param)
		{
			KGE_ASSERT(param.body_a && param.body_b);
			KGE_ASSERT(param.body_a->GetWorld() && param.body_a->GetWorld() == param.body_b->GetWorld());

			b2MouseJointDef def;
			def.bodyA = param.body_a->GetB2Body();
			def.bodyB = param.body_b->GetB2Body();
			def.target = param.body_a->GetWorld()->Stage2World(param.target);
			def.maxForce = param.max_force;
			def.frequencyHz = param.frequency_hz;
			def.dampingRatio = param.damping_ratio;

			MouseJointPtr joint = new MouseJoint(world, &def);
			joint->raw_joint_ = static_cast<b2MouseJoint*>(joint->GetB2Joint());
			return joint;
		}

		void MouseJoint::SetMaxForce(float length)
		{
			KGE_ASSERT(raw_joint_);
			raw_joint_->SetMaxForce(length);
		}

		float MouseJoint::GetMaxForce() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetMaxForce();
		}

}
}
