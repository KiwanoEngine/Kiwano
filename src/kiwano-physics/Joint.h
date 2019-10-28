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

#pragma once
#include <kiwano-physics/helper.h>
#include <kiwano-physics/Body.h>

namespace kiwano
{
	namespace physics
	{
		KGE_DECLARE_SMART_PTR(Joint);
		KGE_DECLARE_SMART_PTR(DistanceJoint);
		KGE_DECLARE_SMART_PTR(FrictionJoint);
		KGE_DECLARE_SMART_PTR(GearJoint);
		KGE_DECLARE_SMART_PTR(MotorJoint);
		KGE_DECLARE_SMART_PTR(WheelJoint);
		KGE_DECLARE_SMART_PTR(MouseJoint);

		// 关节
		class KGE_API Joint
			: public ObjectBase
		{
		public:
			enum class Type
			{
				Unknown = 0,
				Revolute,
				Prismatic,
				Distance,
				Pulley,
				Mouse,
				Gear,
				Wheel,
				Weld,
				Friction,
				Rope,
				Motor
			};

			struct ParamBase
			{
				Body* body_a;
				Body* body_b;

				ParamBase(Body* body_a, Body* body_b) : body_a(body_a), body_b(body_b) {}
				ParamBase(BodyPtr body_a, BodyPtr body_b) : body_a(body_a.get()), body_b(body_b.get()) {}
			};

			Joint();
			Joint(b2Joint* joint);
			Joint(World* world, b2JointDef* joint_def);
			virtual ~Joint();

			BodyPtr GetBodyA() const;
			BodyPtr GetBodyB() const;

			b2Joint* GetB2Joint()				{ return joint_; }
			const b2Joint* GetB2Joint() const	{ return joint_; }
			void SetB2Joint(b2Joint* joint);

			World* GetWorld()					{ return world_; }
			const World* GetWorld() const		{ return world_; }

		protected:
			b2Joint* joint_;
			World* world_;
			Type type_;
		};


		// 固定距离关节
		class KGE_API DistanceJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				Point local_anchor_a;
				Point local_anchor_b;
				float frequency_hz;
				float damping_ratio;

				Param(
					Body* body_a,
					Body* body_b,
					Point const& local_anchor_a,
					Point const& local_anchor_b,
					float frequency_hz = 0.f,
					float damping_ratio = 0.f
				)
					: ParamBase(body_a, body_b)
					, local_anchor_a(local_anchor_a)
					, local_anchor_b(local_anchor_b)
					, frequency_hz(frequency_hz)
					, damping_ratio(damping_ratio)
				{}

				Param(
					BodyPtr body_a,
					BodyPtr body_b,
					Point const& local_anchor_a,
					Point const& local_anchor_b,
					float frequency_hz = 0.f,
					float damping_ratio = 0.f
				)
					: Param(body_a.get(), body_b.get(), local_anchor_a, local_anchor_b, frequency_hz, damping_ratio)
				{}
			};

			DistanceJoint();
			DistanceJoint(World* world, b2DistanceJointDef* def);

			static DistanceJointPtr Create(World* world, Param const& param);

			void SetLength(float length);
			float GetLength() const;

			// 设置弹簧阻尼器频率 [赫兹]
			void SetFrequency(float hz)			{ KGE_ASSERT(raw_joint_); raw_joint_->SetFrequency(hz); }
			float GetFrequency() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->GetFrequency(); }

			// 设置阻尼比
			void SetDampingRatio(float ratio)	{ KGE_ASSERT(raw_joint_); raw_joint_->SetDampingRatio(ratio); }
			float GetDampingRatio() const		{ KGE_ASSERT(raw_joint_); return raw_joint_->GetDampingRatio(); }

		protected:
			b2DistanceJoint* raw_joint_;
		};


		// 摩擦关节
		class KGE_API FrictionJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				Point anchor;
				float max_force;
				float max_torque;

				Param(
					Body* body_a,
					Body* body_b,
					Point const& anchor,
					float max_force = 0.f,
					float max_torque = 0.f
				)
					: ParamBase(body_a, body_b)
					, anchor(anchor)
					, max_force(max_force)
					, max_torque(max_torque)
				{}

				Param(
					BodyPtr body_a,
					BodyPtr body_b,
					Point const& anchor,
					float max_force = 0.f,
					float max_torque = 0.f
				)
					: Param(body_a.get(), body_b.get(), anchor, max_force, max_torque)
				{}
			};

			FrictionJoint();
			FrictionJoint(World* world, b2FrictionJointDef* def);

			static FrictionJointPtr Create(World* world, Param const& param);

			// 设定最大摩擦力
			void SetMaxForce(float force);
			float GetMaxForce() const;

			// 设定最大转矩
			void SetMaxTorque(float torque);
			float GetMaxTorque() const;

		protected:
			b2FrictionJoint* raw_joint_;
		};


		// 齿轮关节
		class KGE_API GearJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				JointPtr joint_a;
				JointPtr joint_b;
				float ratio;

				Param(
					Joint* joint_a,
					Joint* joint_b,
					float ratio = 1.f
				)
					: ParamBase(nullptr, nullptr)
					, joint_a(joint_a)
					, joint_b(joint_b)
					, ratio(ratio)
				{}

				Param(
					JointPtr joint_a,
					JointPtr joint_b,
					float ratio = 1.f
				)
					: Param(joint_a.get(), joint_b.get(), ratio)
				{}
			};

			GearJoint();
			GearJoint(World* world, b2GearJointDef* def);

			static GearJointPtr Create(World* world, Param const& param);

			// 设定齿轮传动比
			void SetRatio(float ratio);
			float GetRatio() const;

		protected:
			b2GearJoint* raw_joint_;
		};


		// 马达关节
		class KGE_API MotorJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				float max_force;
				float max_torque;
				float correction_factor;

				Param(
					Body* body_a,
					Body* body_b,
					float max_force = 1.f,
					float max_torque = 100.f,
					float correction_factor = 0.3f
				)
					: ParamBase(body_a, body_b)
					, max_force(max_force)
					, max_torque(max_torque)
					, correction_factor(correction_factor)
				{}

				Param(
					BodyPtr body_a,
					BodyPtr body_b,
					float max_force = 0.f,
					float max_torque = 0.f,
					float correction_factor = 0.3f
				)
					: Param(body_a.get(), body_b.get(), max_force, max_torque, correction_factor)
				{}
			};

			MotorJoint();
			MotorJoint(World* world, b2MotorJointDef* def);

			static MotorJointPtr Create(World* world, Param const& param);

			// 设定最大摩擦力
			void SetMaxForce(float force);
			float GetMaxForce() const;

			// 设定最大转矩
			void SetMaxTorque(float torque);
			float GetMaxTorque() const;

		protected:
			b2MotorJoint* raw_joint_;
		};


		// 轮关节
		class KGE_API WheelJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				Point anchor;
				Vec2 axis;
				bool enable_motor;
				float max_motor_torque;
				float motor_speed;
				float frequency_hz;
				float damping_ratio;

				Param(
					Body* body_a,
					Body* body_b,
					Point const& anchor,
					Vec2 const& axis,
					float frequency_hz = 2.0f,
					float damping_ratio = 0.7f,
					bool enable_motor = false,
					float max_motor_torque = 0.0f,
					float motor_speed = 0.0f
				)
					: ParamBase(body_a, body_b)
					, anchor(anchor)
					, axis(axis)
					, enable_motor(enable_motor)
					, max_motor_torque(max_motor_torque)
					, motor_speed(motor_speed)
					, frequency_hz(frequency_hz)
					, damping_ratio(damping_ratio)
				{}

				Param(
					BodyPtr body_a,
					BodyPtr body_b,
					Point const& anchor,
					Vec2 const& axis,
					float frequency_hz = 2.0f,
					float damping_ratio = 0.7f,
					bool enable_motor = false,
					float max_motor_torque = 0.0f,
					float motor_speed = 0.0f
				)
					: Param(body_a.get(), body_b.get(), anchor, axis, frequency_hz, damping_ratio, enable_motor, max_motor_torque, motor_speed)
				{}
			};

			WheelJoint();
			WheelJoint(World* world, b2WheelJointDef* def);

			static WheelJointPtr Create(World* world, Param const& param);

			float GetJointTranslation() const;
			float GetJointLinearSpeed() const;
			float GetJointAngle() const					{ KGE_ASSERT(raw_joint_); return math::Radian2Angle(raw_joint_->GetJointAngle()); }
			float GetJointAngularSpeed() const			{ KGE_ASSERT(raw_joint_); return math::Radian2Angle(raw_joint_->GetJointAngularSpeed()); }

			bool IsMotorEnabled() const					{ KGE_ASSERT(raw_joint_); return raw_joint_->IsMotorEnabled(); }
			void EnableMotor(bool flag)					{ KGE_ASSERT(raw_joint_); raw_joint_->EnableMotor(flag); }

			// 设置马达转速 [度/秒]
			void SetMotorSpeed(float speed)				{ KGE_ASSERT(raw_joint_); raw_joint_->SetMotorSpeed(math::Angle2Radian(speed)); }
			float GetMotorSpeed() const					{ KGE_ASSERT(raw_joint_); return math::Radian2Angle(raw_joint_->GetMotorSpeed()); }

			// 设定最大马达转矩
			void SetMaxMotorTorque(float torque);
			float GetMaxMotorTorque() const;

			void SetSpringFrequencyHz(float hz)			{ KGE_ASSERT(raw_joint_); raw_joint_->SetSpringFrequencyHz(hz); }
			float GetSpringFrequencyHz() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->GetSpringFrequencyHz(); }

			void SetSpringDampingRatio(float ratio)		{ KGE_ASSERT(raw_joint_); raw_joint_->SetSpringDampingRatio(ratio); }
			float GetSpringDampingRatio() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->GetSpringDampingRatio(); }

		protected:
			b2WheelJoint* raw_joint_;
		};


		// 鼠标关节
		// 用于使身体的某个点追踪世界上的指定点，例如让物体追踪鼠标位置
		class KGE_API MouseJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				Point target;
				float max_force;
				float frequency_hz;
				float damping_ratio;

				Param(
					Body* body_a,
					Body* body_b,
					Point const& target,
					float max_force,
					float frequency_hz = 5.0f,
					float damping_ratio = 0.7f
				)
					: ParamBase(body_a, body_b)
					, target(target)
					, max_force(max_force)
					, frequency_hz(frequency_hz)
					, damping_ratio(damping_ratio)
				{}

				Param(
					BodyPtr body_a,
					BodyPtr body_b,
					Point const& target,
					float max_force,
					float frequency_hz = 5.0f,
					float damping_ratio = 0.7f
				)
					: Param(body_a.get(), body_b.get(), target, max_force, frequency_hz, damping_ratio)
				{}
			};

			MouseJoint();
			MouseJoint(World* world, b2MouseJointDef* def);

			static MouseJointPtr Create(World* world, Param const& param);

			// 设定最大摩擦力
			void SetMaxForce(float force);
			float GetMaxForce() const;

			// 设置响应速度 [赫兹]
			void SetFrequency(float hz)			{ KGE_ASSERT(raw_joint_); raw_joint_->SetFrequency(hz); }
			float GetFrequency() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->GetFrequency(); }

			// 设置阻尼比
			void SetDampingRatio(float ratio)	{ KGE_ASSERT(raw_joint_); raw_joint_->SetDampingRatio(ratio); }
			float GetDampingRatio() const		{ KGE_ASSERT(raw_joint_); return raw_joint_->GetDampingRatio(); }

		protected:
			b2MouseJoint* raw_joint_;
		};
	}
}
