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
		KGE_DECLARE_SMART_PTR(PhysicJoint);
		KGE_DECLARE_SMART_PTR(DistanceJoint);
		KGE_DECLARE_SMART_PTR(FrictionJoint);
		KGE_DECLARE_SMART_PTR(GearJoint);
		KGE_DECLARE_SMART_PTR(MotorJoint);
		KGE_DECLARE_SMART_PTR(MouseJoint);
		KGE_DECLARE_SMART_PTR(PrismaticJoint);
		KGE_DECLARE_SMART_PTR(PulleyJoint);
		KGE_DECLARE_SMART_PTR(RevoluteJoint);
		KGE_DECLARE_SMART_PTR(RopeJoint);
		KGE_DECLARE_SMART_PTR(WeldJoint);
		KGE_DECLARE_SMART_PTR(WheelJoint);

		// 关节
		class KGE_API PhysicJoint
			: public virtual RefCounter
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
				PhysicBody* body_a;
				PhysicBody* body_b;

				ParamBase(PhysicBody* body_a, PhysicBody* body_b) : body_a(body_a), body_b(body_b) {}
				ParamBase(PhysicBodyPtr body_a, PhysicBodyPtr body_b) : body_a(body_a.get()), body_b(body_b.get()) {}
			};

			PhysicJoint();
			PhysicJoint(b2Joint* joint);
			PhysicJoint(PhysicWorld* world, b2JointDef* joint_def);
			virtual ~PhysicJoint();

			void Init(PhysicWorld* world, b2JointDef* joint_def);

			PhysicBodyPtr GetBodyA() const;
			PhysicBodyPtr GetBodyB() const;

			b2Joint* GetB2Joint()				{ return joint_; }
			const b2Joint* GetB2Joint() const	{ return joint_; }
			void SetB2Joint(b2Joint* joint);

			PhysicWorld* GetWorld()					{ return world_; }
			const PhysicWorld* GetWorld() const		{ return world_; }

		protected:
			b2Joint* joint_;
			PhysicWorld* world_;
			Type type_;
		};


		// 固定距离关节
		class KGE_API DistanceJoint
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point anchor_a;
				Point anchor_b;
				float frequency_hz;
				float damping_ratio;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
					Point const& anchor_a,
					Point const& anchor_b,
					float frequency_hz = 0.f,
					float damping_ratio = 0.f
				)
					: ParamBase(body_a, body_b)
					, anchor_a(anchor_a)
					, anchor_b(anchor_b)
					, frequency_hz(frequency_hz)
					, damping_ratio(damping_ratio)
				{}

				Param(
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					Point const& anchor_a,
					Point const& anchor_b,
					float frequency_hz = 0.f,
					float damping_ratio = 0.f
				)
					: Param(body_a.get(), body_b.get(), anchor_a, anchor_b, frequency_hz, damping_ratio)
				{}
			};

			DistanceJoint();
			DistanceJoint(PhysicWorld* world, b2DistanceJointDef* def);
			DistanceJoint(PhysicWorld* world, Param const& param);

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
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point anchor;
				float max_force;
				float max_torque;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
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
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					Point const& anchor,
					float max_force = 0.f,
					float max_torque = 0.f
				)
					: Param(body_a.get(), body_b.get(), anchor, max_force, max_torque)
				{}
			};

			FrictionJoint();
			FrictionJoint(PhysicWorld* world, b2FrictionJointDef* def);
			FrictionJoint(PhysicWorld* world, Param const& param);

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
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				PhysicJointPtr joint_a;
				PhysicJointPtr joint_b;
				float ratio;

				Param(
					PhysicJoint* joint_a,
					PhysicJoint* joint_b,
					float ratio = 1.f
				)
					: ParamBase(nullptr, nullptr)
					, joint_a(joint_a)
					, joint_b(joint_b)
					, ratio(ratio)
				{}

				Param(
					PhysicJointPtr joint_a,
					PhysicJointPtr joint_b,
					float ratio = 1.f
				)
					: Param(joint_a.get(), joint_b.get(), ratio)
				{}
			};

			GearJoint();
			GearJoint(PhysicWorld* world, b2GearJointDef* def);
			GearJoint(PhysicWorld* world, Param const& param);

			// 设定齿轮传动比
			void SetRatio(float ratio);
			float GetRatio() const;

		protected:
			b2GearJoint* raw_joint_;
		};


		// 马达关节
		class KGE_API MotorJoint
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				float max_force;
				float max_torque;
				float correction_factor;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
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
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					float max_force = 0.f,
					float max_torque = 0.f,
					float correction_factor = 0.3f
				)
					: Param(body_a.get(), body_b.get(), max_force, max_torque, correction_factor)
				{}
			};

			MotorJoint();
			MotorJoint(PhysicWorld* world, b2MotorJointDef* def);
			MotorJoint(PhysicWorld* world, Param const& param);

			// 设定最大摩擦力
			void SetMaxForce(float force);
			float GetMaxForce() const;

			// 设定最大转矩
			void SetMaxTorque(float torque);
			float GetMaxTorque() const;

		protected:
			b2MotorJoint* raw_joint_;
		};


		// 平移关节
		class KGE_API PrismaticJoint
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point anchor;
				Vec2 axis;
				bool enable_limit;
				float lower_translation;
				float upper_translation;
				bool enable_motor;
				float max_motor_force;
				float motor_speed;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
					Point const& anchor,
					Vec2 const& axis,
					bool enable_limit = false,
					float lower_translation = 0.0f,
					float upper_translation = 0.0f,
					bool enable_motor = false,
					float max_motor_force = 0.0f,
					float motor_speed = 0.0f
				)
					: ParamBase(body_a, body_b)
					, anchor(anchor)
					, axis(axis)
					, enable_limit(enable_limit)
					, lower_translation(lower_translation)
					, upper_translation(upper_translation)
					, enable_motor(enable_motor)
					, max_motor_force(max_motor_force)
					, motor_speed(motor_speed)
				{}

				Param(
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					Point const& anchor,
					Vec2 const& axis,
					bool enable_limit = false,
					float lower_translation = 0.0f,
					float upper_translation = 0.0f,
					bool enable_motor = false,
					float max_motor_force = 0.0f,
					float motor_speed = 0.0f
				)
					: Param(body_a.get(), body_b.get(), anchor, axis, enable_limit, lower_translation, upper_translation, enable_motor, max_motor_force, motor_speed)
				{}
			};

			PrismaticJoint();
			PrismaticJoint(PhysicWorld* world, b2PrismaticJointDef* def);
			PrismaticJoint(PhysicWorld* world, Param const& param);

			float GetReferenceAngle() const				{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetReferenceAngle()); }
			float GetJointTranslation() const;
			float GetJointSpeed() const;

			bool IsLimitEnabled() const					{ KGE_ASSERT(raw_joint_); return raw_joint_->IsLimitEnabled(); }
			void EnableLimit(bool flag)					{ KGE_ASSERT(raw_joint_); raw_joint_->EnableLimit(flag); }

			float GetLowerLimit() const;
			float GetUpperLimit() const;
			void SetLimits(float lower, float upper);

			bool IsMotorEnabled() const					{ KGE_ASSERT(raw_joint_); return raw_joint_->IsMotorEnabled(); }
			void EnableMotor(bool flag)					{ KGE_ASSERT(raw_joint_); raw_joint_->EnableMotor(flag); }

			// 设置马达转速 [degree/s]
			void SetMotorSpeed(float speed)				{ KGE_ASSERT(raw_joint_); raw_joint_->SetMotorSpeed(math::Degree2Radian(speed)); }
			float GetMotorSpeed() const					{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetMotorSpeed()); }

			// 设定最大马达力 [N]
			void SetMaxMotorForce(float force)			{ KGE_ASSERT(raw_joint_); raw_joint_->SetMaxMotorForce(force); }
			float GetMaxMotorForce() const				{ KGE_ASSERT(raw_joint_); return raw_joint_->GetMaxMotorForce(); }

		protected:
			b2PrismaticJoint* raw_joint_;
		};


		// 滑轮关节
		class KGE_API PulleyJoint
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point anchor_a;
				Point anchor_b;
				Point ground_anchor_a;
				Point ground_anchor_b;
				float ratio;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
					Point const& anchor_a,
					Point const& anchor_b,
					Point const& ground_anchor_a,
					Point const& ground_anchor_b,
					float ratio = 1.0f
				)
					: ParamBase(body_a, body_b)
					, anchor_a(anchor_a)
					, anchor_b(anchor_b)
					, ground_anchor_a(ground_anchor_a)
					, ground_anchor_b(ground_anchor_b)
					, ratio(ratio)
				{}

				Param(
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					Point const& anchor_a,
					Point const& anchor_b,
					Point const& ground_anchor_a,
					Point const& ground_anchor_b,
					float ratio = 1.0f
				)
					: Param(body_a.get(), body_b.get(), anchor_a, anchor_b, ground_anchor_a, ground_anchor_b, ratio)
				{}
			};

			PulleyJoint();
			PulleyJoint(PhysicWorld* world, b2PulleyJointDef* def);
			PulleyJoint(PhysicWorld* world, Param const& param);

			Point GetGroundAnchorA() const;
			Point GetGroundAnchorB() const;

			float GetRatio() const;

			float GetLengthA() const;
			float GetLengthB() const;

			float GetCurrentLengthA() const;
			float GetCurrentLengthB() const;

		protected:
			b2PulleyJoint* raw_joint_;
		};


		// 旋转关节
		class KGE_API RevoluteJoint
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point anchor;
				bool enable_limit;
				float lower_angle;
				float upper_angle;
				bool enable_motor;
				float max_motor_torque;
				float motor_speed;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
					Point const& anchor,
					bool enable_limit = false,
					float lower_angle = 0.0f,
					float upper_angle = 0.0f,
					bool enable_motor = false,
					float max_motor_torque = 0.0f,
					float motor_speed = 0.0f
				)
					: ParamBase(body_a, body_b)
					, anchor(anchor)
					, enable_limit(enable_limit)
					, lower_angle(lower_angle)
					, upper_angle(upper_angle)
					, enable_motor(enable_motor)
					, max_motor_torque(max_motor_torque)
					, motor_speed(motor_speed)
				{}

				Param(
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					Point const& anchor,
					bool enable_limit = false,
					float lower_angle = 0.0f,
					float upper_angle = 0.0f,
					bool enable_motor = false,
					float max_motor_torque = 0.0f,
					float motor_speed = 0.0f
				)
					: Param(body_a.get(), body_b.get(), anchor, enable_limit, lower_angle, upper_angle, enable_motor, max_motor_torque, motor_speed)
				{}
			};

			RevoluteJoint();
			RevoluteJoint(PhysicWorld* world, b2RevoluteJointDef* def);
			RevoluteJoint(PhysicWorld* world, Param const& param);

			float GetReferenceAngle() const				{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetReferenceAngle()); }
			float GetJointAngle() const;
			float GetJointSpeed() const;

			bool IsLimitEnabled() const					{ KGE_ASSERT(raw_joint_); return raw_joint_->IsLimitEnabled(); }
			void EnableLimit(bool flag)					{ KGE_ASSERT(raw_joint_); raw_joint_->EnableLimit(flag); }

			float GetLowerLimit() const;
			float GetUpperLimit() const;
			void SetLimits(float lower, float upper);

			bool IsMotorEnabled() const					{ KGE_ASSERT(raw_joint_); return raw_joint_->IsMotorEnabled(); }
			void EnableMotor(bool flag)					{ KGE_ASSERT(raw_joint_); raw_joint_->EnableMotor(flag); }

			// 设置马达转速 [degree/s]
			void SetMotorSpeed(float speed)				{ KGE_ASSERT(raw_joint_); raw_joint_->SetMotorSpeed(math::Degree2Radian(speed)); }
			float GetMotorSpeed() const					{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetMotorSpeed()); }

			// 设定最大马达转矩 [N/m]
			void SetMaxMotorTorque(float torque);
			float GetMaxMotorTorque() const;

		protected:
			b2RevoluteJoint* raw_joint_;
		};


		// 绳关节
		class KGE_API RopeJoint
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point local_anchor_a;
				Point local_anchor_b;
				float max_length;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
					Point const& local_anchor_a,
					Point const& local_anchor_b,
					float max_length = 0.f
				)
					: ParamBase(body_a, body_b)
					, local_anchor_a(local_anchor_a)
					, local_anchor_b(local_anchor_b)
					, max_length(max_length)
				{}

				Param(
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					Point const& local_anchor_a,
					Point const& local_anchor_b,
					float max_length = 0.f
				)
					: Param(body_a.get(), body_b.get(), local_anchor_a, local_anchor_b, max_length)
				{}
			};

			RopeJoint();
			RopeJoint(PhysicWorld* world, b2RopeJointDef* def);
			RopeJoint(PhysicWorld* world, Param const& param);

			void SetMaxLength(float length);
			float GetMaxLength() const;

		protected:
			b2RopeJoint* raw_joint_;
		};


		// 焊接关节
		class KGE_API WeldJoint
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point anchor;
				float frequency_hz;
				float damping_ratio;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
					Point const& anchor,
					float frequency_hz = 0.f,
					float damping_ratio = 0.f
				)
					: ParamBase(body_a, body_b)
					, anchor(anchor)
					, frequency_hz(frequency_hz)
					, damping_ratio(damping_ratio)
				{}

				Param(
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					Point const& anchor,
					float frequency_hz = 0.f,
					float damping_ratio = 0.f
				)
					: Param(body_a.get(), body_b.get(), anchor, frequency_hz, damping_ratio)
				{}
			};

			WeldJoint();
			WeldJoint(PhysicWorld* world, b2WeldJointDef* def);
			WeldJoint(PhysicWorld* world, Param const& param);

			// 设置弹簧阻尼器频率 [赫兹]
			void SetFrequency(float hz)				{ KGE_ASSERT(raw_joint_); raw_joint_->SetFrequency(hz); }
			float GetFrequency() const				{ KGE_ASSERT(raw_joint_); return raw_joint_->GetFrequency(); }

			// 设置阻尼比
			void SetDampingRatio(float ratio)		{ KGE_ASSERT(raw_joint_); raw_joint_->SetDampingRatio(ratio); }
			float GetDampingRatio() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->GetDampingRatio(); }

		protected:
			b2WeldJoint* raw_joint_;
		};


		// 轮关节
		class KGE_API WheelJoint
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point anchor;
				Vec2 axis;
				bool enable_motor;
				float max_motor_torque;
				float motor_speed;
				float frequency_hz;
				float damping_ratio;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
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
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
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
			WheelJoint(PhysicWorld* world, b2WheelJointDef* def);
			WheelJoint(PhysicWorld* world, Param const& param);

			float GetJointTranslation() const;
			float GetJointLinearSpeed() const;
			float GetJointAngle() const					{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetJointAngle()); }
			float GetJointAngularSpeed() const			{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetJointAngularSpeed()); }

			bool IsMotorEnabled() const					{ KGE_ASSERT(raw_joint_); return raw_joint_->IsMotorEnabled(); }
			void EnableMotor(bool flag)					{ KGE_ASSERT(raw_joint_); raw_joint_->EnableMotor(flag); }

			// 设置马达转速 [degree/s]
			void SetMotorSpeed(float speed)				{ KGE_ASSERT(raw_joint_); raw_joint_->SetMotorSpeed(math::Degree2Radian(speed)); }
			float GetMotorSpeed() const					{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetMotorSpeed()); }

			// 设定最大马达转矩 [N/m]
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
			: public PhysicJoint
		{
		public:
			struct Param : public PhysicJoint::ParamBase
			{
				Point target;
				float max_force;
				float frequency_hz;
				float damping_ratio;

				Param(
					PhysicBody* body_a,
					PhysicBody* body_b,
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
					PhysicBodyPtr body_a,
					PhysicBodyPtr body_b,
					Point const& target,
					float max_force,
					float frequency_hz = 5.0f,
					float damping_ratio = 0.7f
				)
					: Param(body_a.get(), body_b.get(), target, max_force, frequency_hz, damping_ratio)
				{}
			};

			MouseJoint();
			MouseJoint(PhysicWorld* world, b2MouseJointDef* def);
			MouseJoint(PhysicWorld* world, Param const& param);

			// 设定最大摩擦力 [N]
			void SetMaxForce(float force);
			float GetMaxForce() const;

			// 设置响应速度 [hz]
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
