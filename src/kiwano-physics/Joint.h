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

		KGE_DECLARE_SMART_PTR(DistanceJoint);
		class KGE_API DistanceJoint
			: public Joint
		{
		public:
			struct Param : public Joint::ParamBase
			{
				Point local_anchor_a;
				Point local_anchor_b;

				Param(Body* body_a, Body* body_b, Point const& local_anchor_a, Point const& local_anchor_b)
					: ParamBase(body_a, body_b)
					, local_anchor_a(local_anchor_a)
					, local_anchor_b(local_anchor_b)
				{}

				Param(BodyPtr body_a, BodyPtr body_b, Point const& local_anchor_a, Point const& local_anchor_b)
					: Param(body_a.get(), body_b.get(), local_anchor_a, local_anchor_b)
				{}
			};

			DistanceJoint();
			DistanceJoint(World* world, b2DistanceJointDef* def);

			static DistanceJointPtr Create(World* world, Param const& param);
		};
	}
}
