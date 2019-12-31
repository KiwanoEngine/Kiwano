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
		KGE_DECLARE_SMART_PTR(MouseJoint);
		KGE_DECLARE_SMART_PTR(PrismaticJoint);
		KGE_DECLARE_SMART_PTR(PulleyJoint);
		KGE_DECLARE_SMART_PTR(RevoluteJoint);
		KGE_DECLARE_SMART_PTR(RopeJoint);
		KGE_DECLARE_SMART_PTR(WeldJoint);
		KGE_DECLARE_SMART_PTR(WheelJoint);

		/**
		* \addtogroup Physics
		* @{
		*/

		/// \~chinese
		/// @brief 关节
		class KGE_API Joint
			: public virtual RefCounter
		{
		public:
			/// \~chinese
			/// @brief 关节类型
			enum class Type
			{
				Unknown = 0,	///< 未知
				Revolute,		///< 旋转关节
				Prismatic,		///< 平移关节
				Distance,		///< 固定距离关节
				Pulley,			///< 滑轮关节
				Mouse,			///< 鼠标关节
				Gear,			///< 齿轮关节
				Wheel,			///< 轮关节
				Weld,			///< 焊接关节
				Friction,		///< 摩擦关节
				Rope,			///< 绳关节
				Motor			///< 马达关节
			};

			/// \~chinese
			/// @brief 关节基础参数
			struct ParamBase
			{
				Body* body_a;	///< 关节连接的物体A
				Body* body_b;	///< 关节连接的物体B

				ParamBase(Body* body_a, Body* body_b)
					: body_a(body_a), body_b(body_b)
				{
				}

				ParamBase(BodyPtr body_a, BodyPtr body_b)
					: body_a(body_a.get()), body_b(body_b.get())
				{
				}
			};

			Joint();
			Joint(b2Joint* joint);
			Joint(World* world, b2JointDef* joint_def);
			virtual ~Joint();

			void Init(World* world, b2JointDef* joint_def);

			/// \~chinese
			/// @brief 获取关节连接的物体A
			BodyPtr GetBodyA() const;

			/// \~chinese
			/// @brief 获取关节连接的物体B
			BodyPtr GetBodyB() const;

			/// \~chinese
			/// @brief 获取物理世界
			World* GetWorld() const;

			b2Joint* GetB2Joint() const;
			void SetB2Joint(b2Joint* joint);

		private:
			b2Joint* joint_;
			World* world_;
			Type type_;
		};


		/// \~chinese
		/// @brief 固定距离关节
		class KGE_API DistanceJoint
			: public Joint
		{
		public:
			/// \~chinese
			/// @brief 固定距离关节参数
			struct Param : public Joint::ParamBase
			{
				Point anchor_a;			///< 物体A的锚点位置
				Point anchor_b;			///< 物体B的锚点位置
				float frequency_hz;		///< 弹簧阻尼器频率
				float damping_ratio;	///< 阻尼比

				Param(
					Body* body_a,
					Body* body_b,
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
					BodyPtr body_a,
					BodyPtr body_b,
					Point const& anchor_a,
					Point const& anchor_b,
					float frequency_hz = 0.f,
					float damping_ratio = 0.f
				)
					: Param(body_a.get(), body_b.get(), anchor_a, anchor_b, frequency_hz, damping_ratio)
				{}
			};

			DistanceJoint();
			DistanceJoint(World* world, b2DistanceJointDef* def);
			DistanceJoint(World* world, Param const& param);

			/// \~chinese
			/// @brief 设置关节长度
			void SetLength(float length);
			float GetLength() const;

			/// \~chinese
			/// @brief 设置弹簧阻尼器频率 [赫兹]
			void SetFrequency(float hz);
			float GetFrequency() const;

			/// \~chinese
			/// @brief 设置阻尼比
			void SetDampingRatio(float ratio);
			float GetDampingRatio() const;

		private:
			b2DistanceJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 摩擦关节
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
			FrictionJoint(World* world, Param const& param);

			/// \~chinese
			/// @brief 设定最大摩擦力
			void SetMaxForce(float force);
			float GetMaxForce() const;

			/// \~chinese
			/// @brief 设定最大转矩
			void SetMaxTorque(float torque);
			float GetMaxTorque() const;

		private:
			b2FrictionJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 齿轮关节
		class KGE_API GearJoint
			: public Joint
		{
		public:
			/// \~chinese
			/// @brief 齿轮关节参数
			struct Param : public Joint::ParamBase
			{
				JointPtr joint_a;	///< 关节A
				JointPtr joint_b;	///< 关节B
				float ratio;		///< 齿轮传动比

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
			GearJoint(World* world, Param param);

			/// \~chinese
			/// @brief 设定齿轮传动比
			void SetRatio(float ratio);
			float GetRatio() const;

		private:
			b2GearJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 马达关节
		class KGE_API MotorJoint
			: public Joint
		{
		public:
			/// \~chinese
			/// @brief 马达关节参数
			struct Param : public Joint::ParamBase
			{
				float max_force;			///< 最大摩擦力
				float max_torque;			///< 最大转矩
				float correction_factor;	///< 位置矫正因子（范围 0-1）

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
			MotorJoint(World* world, Param const& param);

			/// \~chinese
			/// @brief 设定最大摩擦力
			void SetMaxForce(float force);
			float GetMaxForce() const;

			/// \~chinese
			/// @brief 设定最大转矩
			void SetMaxTorque(float torque);
			float GetMaxTorque() const;

		private:
			b2MotorJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 平移关节
		class KGE_API PrismaticJoint
			: public Joint
		{
		public:
			/// \~chinese
			/// @brief 平移关节参数
			struct Param : public Joint::ParamBase
			{
				Point anchor;				///< 锚点位置
				Vec2 axis;					///< 平移轴向量
				bool enable_limit;			///< 关节限制开关
				float lower_translation;	///< 关节下限
				float upper_translation;	///< 关节上限
				bool enable_motor;			///< 马达开关
				float max_motor_force;		///< 最大马达力 [N]
				float motor_speed;			///< 马达转速 [degree/s]

				Param(
					Body* body_a,
					Body* body_b,
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
					BodyPtr body_a,
					BodyPtr body_b,
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
			PrismaticJoint(World* world, b2PrismaticJointDef* def);
			PrismaticJoint(World* world, Param const& param);

			/// \~chinese
			/// @brief 获取参考角
			float GetReferenceAngle() const;

			/// \~chinese
			/// @brief 获取关节转换
			float GetJointTranslation() const;

			/// \~chinese
			/// @brief 获取关节速度
			float GetJointSpeed() const;

			/// \~chinese
			/// @brief 是否启用关节限制
			bool IsLimitEnabled() const;
			void EnableLimit(bool flag);

			/// \~chinese
			/// @brief 设置关节限制
			float GetLowerLimit() const;
			float GetUpperLimit() const;
			void SetLimits(float lower, float upper);

			/// \~chinese
			/// @brief 是否启用马达
			bool IsMotorEnabled() const;
			void EnableMotor(bool flag);

			/// \~chinese
			/// @brief 设置马达转速 [degree/s]
			void SetMotorSpeed(float speed);
			float GetMotorSpeed() const;

			/// \~chinese
			/// @brief 设定最大马达力 [N]
			void SetMaxMotorForce(float force);
			float GetMaxMotorForce() const;

		private:
			b2PrismaticJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 滑轮关节
		class KGE_API PulleyJoint
			: public Joint
		{
		public:
			/// \~chinese
			/// @brief 滑轮关节参数
			struct Param : public Joint::ParamBase
			{
				Point anchor_a;			///< 
				Point anchor_b;			///< 
				Point ground_anchor_a;	///< 
				Point ground_anchor_b;	///< 
				float ratio;			///< 

				Param(
					Body* body_a,
					Body* body_b,
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
					BodyPtr body_a,
					BodyPtr body_b,
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
			PulleyJoint(World* world, b2PulleyJointDef* def);
			PulleyJoint(World* world, Param const& param);

			Point GetGroundAnchorA() const;
			Point GetGroundAnchorB() const;

			float GetRatio() const;

			float GetLengthA() const;
			float GetLengthB() const;

			float GetCurrentLengthA() const;
			float GetCurrentLengthB() const;

		private:
			b2PulleyJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 旋转关节
		class KGE_API RevoluteJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				Point anchor;
				bool enable_limit;
				float lower_angle;
				float upper_angle;
				bool enable_motor;
				float max_motor_torque;
				float motor_speed;

				Param(
					Body* body_a,
					Body* body_b,
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
					BodyPtr body_a,
					BodyPtr body_b,
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
			RevoluteJoint(World* world, b2RevoluteJointDef* def);
			RevoluteJoint(World* world, Param const& param);

			float GetReferenceAngle() const;
			float GetJointAngle() const;
			float GetJointSpeed() const;

			bool IsLimitEnabled() const;
			void EnableLimit(bool flag);

			float GetLowerLimit() const;
			float GetUpperLimit() const;
			void SetLimits(float lower, float upper);

			bool IsMotorEnabled() const;
			void EnableMotor(bool flag);

			/// \~chinese
			/// @brief 设置马达转速 [degree/s]
			void SetMotorSpeed(float speed);
			float GetMotorSpeed() const;

			/// \~chinese
			/// @brief 设定最大马达转矩 [N/m]
			void SetMaxMotorTorque(float torque);
			float GetMaxMotorTorque() const;

		private:
			b2RevoluteJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 绳关节
		class KGE_API RopeJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				Point local_anchor_a;
				Point local_anchor_b;
				float max_length;

				Param(
					Body* body_a,
					Body* body_b,
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
					BodyPtr body_a,
					BodyPtr body_b,
					Point const& local_anchor_a,
					Point const& local_anchor_b,
					float max_length = 0.f
				)
					: Param(body_a.get(), body_b.get(), local_anchor_a, local_anchor_b, max_length)
				{}
			};

			RopeJoint();
			RopeJoint(World* world, b2RopeJointDef* def);
			RopeJoint(World* world, Param const& param);

			void SetMaxLength(float length);
			float GetMaxLength() const;

		private:
			b2RopeJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 焊接关节
		class KGE_API WeldJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				Point anchor;
				float frequency_hz;
				float damping_ratio;

				Param(
					Body* body_a,
					Body* body_b,
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
					BodyPtr body_a,
					BodyPtr body_b,
					Point const& anchor,
					float frequency_hz = 0.f,
					float damping_ratio = 0.f
				)
					: Param(body_a.get(), body_b.get(), anchor, frequency_hz, damping_ratio)
				{}
			};

			WeldJoint();
			WeldJoint(World* world, b2WeldJointDef* def);
			WeldJoint(World* world, Param const& param);

			/// \~chinese
			/// @brief 设置弹簧阻尼器频率 [赫兹]
			void SetFrequency(float hz);
			float GetFrequency() const;

			/// \~chinese
			/// @brief 设置阻尼比
			void SetDampingRatio(float ratio);
			float GetDampingRatio() const;

		private:
			b2WeldJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 轮关节
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
			WheelJoint(World* world, Param const& param);

			float GetJointTranslation() const;
			float GetJointLinearSpeed() const;
			float GetJointAngle() const;
			float GetJointAngularSpeed() const;

			bool IsMotorEnabled() const;
			void EnableMotor(bool flag);

			/// \~chinese
			/// @brief 设置马达转速 [degree/s]
			void SetMotorSpeed(float speed);
			float GetMotorSpeed() const;

			/// \~chinese
			/// @brief 设定最大马达转矩 [N/m]
			void SetMaxMotorTorque(float torque);
			float GetMaxMotorTorque() const;

			void SetSpringFrequencyHz(float hz);
			float GetSpringFrequencyHz() const;

			void SetSpringDampingRatio(float ratio);
			float GetSpringDampingRatio() const;

		private:
			b2WheelJoint* raw_joint_;
		};


		/// \~chinese
		/// @brief 鼠标关节
		/// @details 用于使身体的某个点追踪世界上的指定点，例如让物体追踪鼠标位置
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

				Param(BodyPtr body_a, BodyPtr body_b, Point const& target, float max_force, float frequency_hz = 5.0f, float damping_ratio = 0.7f)
					: Param(body_a.get(), body_b.get(), target, max_force, frequency_hz, damping_ratio)
				{}
			};

			MouseJoint();
			MouseJoint(World* world, b2MouseJointDef* def);
			MouseJoint(World* world, Param const& param);

			/// \~chinese
			/// @brief 设定最大摩擦力 [N]
			void SetMaxForce(float force);
			float GetMaxForce() const;

			/// \~chinese
			/// @brief 设置响应速度 [hz]
			void SetFrequency(float hz);
			float GetFrequency() const;

			/// \~chinese
			/// @brief 设置阻尼比
			void SetDampingRatio(float ratio);
			float GetDampingRatio() const;

		private:
			b2MouseJoint* raw_joint_;
		};

		/** @} */


		inline b2Joint* Joint::GetB2Joint() const					{ return joint_; }
		inline World* Joint::GetWorld() const						{ return world_; }

		inline void DistanceJoint::SetFrequency(float hz)			{ KGE_ASSERT(raw_joint_); raw_joint_->SetFrequency(hz); }
		inline float DistanceJoint::GetFrequency() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->GetFrequency(); }
		inline void DistanceJoint::SetDampingRatio(float ratio)		{ KGE_ASSERT(raw_joint_); raw_joint_->SetDampingRatio(ratio); }
		inline float DistanceJoint::GetDampingRatio() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->GetDampingRatio(); }

		inline float PrismaticJoint::GetReferenceAngle() const		{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetReferenceAngle()); }
		inline bool PrismaticJoint::IsLimitEnabled() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->IsLimitEnabled(); }
		inline void PrismaticJoint::EnableLimit(bool flag)			{ KGE_ASSERT(raw_joint_); raw_joint_->EnableLimit(flag); }
		inline bool PrismaticJoint::IsMotorEnabled() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->IsMotorEnabled(); }
		inline void PrismaticJoint::EnableMotor(bool flag)			{ KGE_ASSERT(raw_joint_); raw_joint_->EnableMotor(flag); }
		inline void PrismaticJoint::SetMotorSpeed(float speed)		{ KGE_ASSERT(raw_joint_); raw_joint_->SetMotorSpeed(math::Degree2Radian(speed)); }
		inline float PrismaticJoint::GetMotorSpeed() const			{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetMotorSpeed()); }
		inline void PrismaticJoint::SetMaxMotorForce(float force)	{ KGE_ASSERT(raw_joint_); raw_joint_->SetMaxMotorForce(force); }
		inline float PrismaticJoint::GetMaxMotorForce() const		{ KGE_ASSERT(raw_joint_); return raw_joint_->GetMaxMotorForce(); }

		inline float RevoluteJoint::GetReferenceAngle() const		{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetReferenceAngle()); }
		inline bool RevoluteJoint::IsLimitEnabled() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->IsLimitEnabled(); }
		inline void RevoluteJoint::EnableLimit(bool flag)			{ KGE_ASSERT(raw_joint_); raw_joint_->EnableLimit(flag); }
		inline bool RevoluteJoint::IsMotorEnabled() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->IsMotorEnabled(); }
		inline void RevoluteJoint::EnableMotor(bool flag)			{ KGE_ASSERT(raw_joint_); raw_joint_->EnableMotor(flag); }
		inline void RevoluteJoint::SetMotorSpeed(float speed)		{ KGE_ASSERT(raw_joint_); raw_joint_->SetMotorSpeed(math::Degree2Radian(speed)); }
		inline float RevoluteJoint::GetMotorSpeed() const			{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetMotorSpeed()); }

		inline void WeldJoint::SetFrequency(float hz)				{ KGE_ASSERT(raw_joint_); raw_joint_->SetFrequency(hz); }
		inline float WeldJoint::GetFrequency() const				{ KGE_ASSERT(raw_joint_); return raw_joint_->GetFrequency(); }
		inline void WeldJoint::SetDampingRatio(float ratio)			{ KGE_ASSERT(raw_joint_); raw_joint_->SetDampingRatio(ratio); }
		inline float WeldJoint::GetDampingRatio() const				{ KGE_ASSERT(raw_joint_); return raw_joint_->GetDampingRatio(); }
		inline float WheelJoint::GetJointAngle() const				{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetJointAngle()); }
		inline float WheelJoint::GetJointAngularSpeed() const		{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetJointAngularSpeed()); }
		inline bool WheelJoint::IsMotorEnabled() const				{ KGE_ASSERT(raw_joint_); return raw_joint_->IsMotorEnabled(); }
		inline void WheelJoint::EnableMotor(bool flag)				{ KGE_ASSERT(raw_joint_); raw_joint_->EnableMotor(flag); }
		inline void WheelJoint::SetMotorSpeed(float speed)			{ KGE_ASSERT(raw_joint_); raw_joint_->SetMotorSpeed(math::Degree2Radian(speed)); }
		inline float WheelJoint::GetMotorSpeed() const				{ KGE_ASSERT(raw_joint_); return math::Radian2Degree(raw_joint_->GetMotorSpeed()); }
		inline void WheelJoint::SetSpringFrequencyHz(float hz)		{ KGE_ASSERT(raw_joint_); raw_joint_->SetSpringFrequencyHz(hz); }
		inline float WheelJoint::GetSpringFrequencyHz() const		{ KGE_ASSERT(raw_joint_); return raw_joint_->GetSpringFrequencyHz(); }
		inline void WheelJoint::SetSpringDampingRatio(float ratio)	{ KGE_ASSERT(raw_joint_); raw_joint_->SetSpringDampingRatio(ratio); }
		inline float WheelJoint::GetSpringDampingRatio() const		{ KGE_ASSERT(raw_joint_); return raw_joint_->GetSpringDampingRatio(); }

		inline void MouseJoint::SetFrequency(float hz)				{ KGE_ASSERT(raw_joint_); raw_joint_->SetFrequency(hz); }
		inline float MouseJoint::GetFrequency() const				{ KGE_ASSERT(raw_joint_); return raw_joint_->GetFrequency(); }
		inline void MouseJoint::SetDampingRatio(float ratio)		{ KGE_ASSERT(raw_joint_); raw_joint_->SetDampingRatio(ratio); }
		inline float MouseJoint::GetDampingRatio() const			{ KGE_ASSERT(raw_joint_); return raw_joint_->GetDampingRatio(); }
	}
}
