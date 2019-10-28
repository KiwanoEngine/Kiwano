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
#include <kiwano-physics/Body.h>
#include <kiwano-physics/Joint.h>

namespace kiwano
{
	namespace physics
	{
		// 物理世界
		class KGE_API World
			: public Stage
		{
			friend class Body;
			friend class Joint;

		public:
			World();

			virtual ~World();

			// 创建刚体
			BodyPtr CreateBody(ActorPtr actor);
			BodyPtr CreateBody(Actor* actor);

			// 获取重力
			Vec2 GetGravity() const;

			// 设置重力
			void SetGravity(Vec2 gravity);

			// 获取全局缩放比例
			inline float	GetGlobalScale() const			{ return global_scale_; }

			// 设置全局缩放比例
			inline void		SetGlobalScale(float scale)		{ global_scale_ = scale; }

			// 游戏世界单位转换为物理世界单位
			inline float	World2Stage(float value)		{ return value * GetGlobalScale(); }
			inline Point	World2Stage(const b2Vec2& pos)	{ return Point(World2Stage(pos.x), World2Stage(pos.y)); }

			// 物理世界单位转换为游戏世界单位
			inline float	Stage2World(float value)		{ return value / GetGlobalScale(); }
			inline b2Vec2	Stage2World(const Point& pos)	{ return b2Vec2(Stage2World(pos.x), Stage2World(pos.y)); }

			// 设置速度迭代次数, 默认为 6
			inline void SetVelocityIterations(int vel_iter)	{ vel_iter_ = vel_iter; }

			// 设置位置迭代次数, 默认为 2
			inline void SetPositionIterations(int pos_iter)	{ pos_iter_ = pos_iter; }

			// 获取 Box2D 世界
			b2World* GetB2World();

			// 获取 Box2D 世界
			const b2World* GetB2World() const;

		protected:
			// 移除刚体
			void RemoveBody(Body* body);

			// 移除所有刚体
			void RemoveAllBodies();

			// 添加关节
			void AddJoint(Joint* joint);

			// 移除关节
			void RemoveJoint(Joint* joint);

			// 移除所有关节
			void RemoveAllJoints();

			// 关节被移除
			void JointRemoved(b2Joint* joint);

		protected:
			void Update(Duration dt) override;

			class DestructionListener;
			friend DestructionListener;

		protected:
			b2World world_;
			int vel_iter_;
			int pos_iter_;
			float global_scale_;
			DestructionListener* destruction_listener_;

			bool removing_joint_;
			Vector<Joint*> joints_;
		};

		KGE_DECLARE_SMART_PTR(World);
	}
}
