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
#include <kiwano-physics/Shape.h>
#include <kiwano-physics/Fixture.h>
#include <kiwano-physics/Contact.h>

namespace kiwano
{
	namespace physics
	{
		class World;

		// 物体
		KGE_DECLARE_SMART_PTR(Body);
		class KGE_API Body
			: public virtual RefCounter
		{
		public:
			enum class Type
			{
				Static = 0,
				Kinematic,
				Dynamic,
			};

			Body();
			Body(b2Body* body, Actor* actor);
			Body(World* world, Actor* actor);
			Body(World* world, ActorPtr actor) : Body(world, actor.get()) {}
			virtual ~Body();

			// 初始化
			void Init(World* world, Actor* actor);

			// 添加夹具
			Fixture AddFixture(Shape* shape, const Fixture::Param& param);

			// 添加形状
			Fixture AddCircleShape(float radius, float density = 0.f);
			Fixture AddBoxShape(Vec2 const& size, float density = 0.f);
			Fixture AddPolygonShape(Vector<Point> const& vertexs, float density = 0.f);
			Fixture AddEdgeShape(Point const& p1, Point const& p2, float density = 0.f);
			Fixture AddChainShape(Vector<Point> const& vertexs, bool loop, float density = 0.f);

			// 获取夹具
			Fixture GetFixtureList() const			{ KGE_ASSERT(body_); return Fixture(body_->GetFixtureList()); }

			// 移除夹具
			void RemoveFixture(Fixture const& fixture);

			// 获取接触边
			ContactEdge GetContactList() const		{ KGE_ASSERT(body_); ContactEdge(body_->GetContactList()); }

			// 类别码
			uint16_t GetCategoryBits() const				{ return category_bits_; }
			void SetCategoryBits(uint16_t category_bits);

			// 碰撞掩码
			uint16_t GetMaskBits() const					{ return mask_bits_; }
			void SetMaskBits(uint16_t mask_bits);

			// 组索引
			int16_t GetGroupIndex() const					{ return group_index_; }
			void SetGroupIndex(int16_t index);

			// 旋转角度
			float GetBodyRotation() const					{ KGE_ASSERT(body_); return math::Radian2Degree(body_->GetAngle()); }
			void SetBodyRotation(float angle)				{ SetBodyTransform(GetBodyPosition(), angle); }

			// 位置
			Point GetBodyPosition() const;
			void SetBodyPosition(Point const& pos)			{ SetBodyTransform(pos, GetBodyRotation()); }

			// 位置和旋转变换
			void SetBodyTransform(Point const& pos, float angle);

			// 质量
			float GetMass() const							{ KGE_ASSERT(body_); return body_->GetMass(); }

			// 惯性
			float GetInertia() const						{ KGE_ASSERT(body_); return body_->GetInertia(); }

			// 质量数据
			void GetMassData(float* mass, Point* center, float* inertia) const;
			void SetMassData(float mass, Point const& center, float inertia);
			void ResetMassData();

			// 坐标转换
			Point GetLocalPoint(Point const& world) const;
			Point GetWorldPoint(Point const& local) const;

			// 质心坐标
			Point GetLocalCenter() const;
			Point GetWorldCenter() const;

			// 物体类型
			Type GetType() const							{ KGE_ASSERT(body_); return Type(body_->GetType()); }
			void SetType(Type type)							{ KGE_ASSERT(body_); body_->SetType(static_cast<b2BodyType>(type)); }

			// 重力因子
			float GetGravityScale() const					{ KGE_ASSERT(body_); return body_->GetGravityScale(); }
			void SetGravityScale(float scale)				{ KGE_ASSERT(body_); body_->SetGravityScale(scale); }

			// 施力
			void ApplyForce(Vec2 const& force, Point const& point, bool wake = true);
			void ApplyForceToCenter(Vec2 const& force, bool wake = true);

			// 施加扭矩
			void ApplyTorque(float torque, bool wake = true);

			// 固定旋转
			bool IsIgnoreRotation() const					{ KGE_ASSERT(body_); return body_->IsFixedRotation(); }
			void SetIgnoreRotation(bool flag)				{ KGE_ASSERT(body_); body_->SetFixedRotation(flag); }

			// 子弹
			bool IsBullet() const							{ KGE_ASSERT(body_); return body_->IsBullet(); }
			void SetBullet(bool flag)						{ KGE_ASSERT(body_); body_->SetBullet(flag); }

			// 休眠
			bool IsAwake() const							{ KGE_ASSERT(body_); return body_->IsAwake(); }
			void SetAwake(bool flag)						{ KGE_ASSERT(body_); body_->SetAwake(flag); }
			bool IsSleepingAllowed() const					{ KGE_ASSERT(body_); return body_->IsSleepingAllowed(); }
			void SetSleepingAllowed(bool flag)				{ KGE_ASSERT(body_); body_->SetSleepingAllowed(flag); }

			// 活动状态
			bool IsActive() const							{ KGE_ASSERT(body_); return body_->IsActive(); }
			void SetActive(bool flag)						{ KGE_ASSERT(body_); body_->SetActive(flag); }

			Actor* GetActor() const							{ return actor_; }
			void SetActor(Actor* actor)						{ actor_ = actor; }

			b2Body* GetB2Body()								{ return body_; }
			const b2Body* GetB2Body() const					{ return body_; }
			void SetB2Body(b2Body* body);

			World* GetWorld()							{ return world_; }
			const World* GetWorld() const				{ return world_; }

			void Destroy();

			void UpdateActor();
			void UpdateFromActor();

		protected:
			void UpdateFixtureFilter(b2Fixture* fixture);

		protected:
			Actor* actor_;
			World* world_;
			b2Body* body_;

			uint16_t category_bits_;
			uint16_t mask_bits_;
			int16_t group_index_;
		};
	}
}
