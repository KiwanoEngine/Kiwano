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

		KGE_DECLARE_SMART_PTR(Body);

		/**
		* \addtogroup Physics
		* @{
		*/

		/// \~chinese
		/// @brief 物体
		class KGE_API Body
			: public virtual RefCounter
		{
		public:
			/// \~chinese
			/// @brief 物体类型
			enum class Type
			{
				Static = 0,	///< 静态物体
				Kinematic,	///< 动力学物体
				Dynamic,	///< 动态物体
			};

			Body();
			Body(b2Body* body, Actor* actor);
			Body(World* world, Actor* actor);
			Body(World* world, ActorPtr actor);
			virtual ~Body();

			/// \~chinese
			/// @brief 初始化
			/// @param[in] world 物理世界
			/// @param[in] actor 绑定的角色
			void Init(World* world, Actor* actor);

			/// \~chinese
			/// @brief 添加夹具
			/// @param shape 物体形状
			/// @param density 物体密度
			Fixture AddFixture(Shape* shape, const Fixture::Param& param);

			/// \~chinese
			/// @brief 添加圆形夹具
			/// @param radius 圆形半径
			/// @param density 物体密度
			Fixture AddCircleShape(float radius, float density = 0.f);

			/// \~chinese
			/// @brief 添加盒形夹具
			/// @param size 盒大小
			/// @param density 物体密度
			Fixture AddBoxShape(Vec2 const& size, float density = 0.f);

			/// \~chinese
			/// @brief 添加多边形夹具
			/// @param vertexs 多边形端点
			/// @param density 物体密度
			Fixture AddPolygonShape(Vector<Point> const& vertexs, float density = 0.f);

			/// \~chinese
			/// @brief 添加线段形夹具
			/// @param p1 线段起点
			/// @param p2 线段终点
			/// @param density 物体密度
			Fixture AddEdgeShape(Point const& p1, Point const& p2, float density = 0.f);

			/// \~chinese
			/// @brief 添加链条形夹具
			/// @param vertexs 链条端点
			/// @param loop 是否闭合
			/// @param density 物体密度
			Fixture AddChainShape(Vector<Point> const& vertexs, bool loop, float density = 0.f);

			/// \~chinese
			/// @brief 获取夹具列表
			FixtureList GetFixtureList() const;

			/// \~chinese
			/// @brief 移除夹具
			void RemoveFixture(Fixture const& fixture);

			/// \~chinese
			/// @brief 获取接触边列表
			ContactEdgeList GetContactList() const;

			/// \~chinese
			/// @brief 获取类别码
			uint16_t GetCategoryBits() const;

			/// \~chinese
			/// @brief 设置类别码
			void SetCategoryBits(uint16_t category_bits);

			/// \~chinese
			/// @brief 获取碰撞掩码
			uint16_t GetMaskBits() const;

			/// \~chinese
			/// @brief 设置碰撞掩码
			void SetMaskBits(uint16_t mask_bits);

			/// \~chinese
			/// @brief 获取组索引
			int16_t GetGroupIndex() const;

			/// \~chinese
			/// @brief 设置组索引
			void SetGroupIndex(int16_t index);

			/// \~chinese
			/// @brief 获取旋转角度
			float GetBodyRotation() const;

			/// \~chinese
			/// @brief 设置旋转角度
			void SetBodyRotation(float angle);

			/// \~chinese
			/// @brief 获取物体位置
			Point GetBodyPosition() const;

			/// \~chinese
			/// @brief 设置物体位置
			void SetBodyPosition(Point const& pos);

			/// \~chinese
			/// @brief 位置和旋转变换
			void SetBodyTransform(Point const& pos, float angle);

			/// \~chinese
			/// @brief 获取质量 [kg]
			float GetMass() const;

			/// \~chinese
			/// @brief 获取惯性
			float GetInertia() const;

			/// \~chinese
			/// @brief 获取质量数据
			/// @param[out] mass 物体质量 [kg]
			/// @param[out] center 质心位置
			/// @param[out] inertia 惯性
			void GetMassData(float* mass, Point* center, float* inertia) const;

			/// \~chinese
			/// @brief 设置质量数据
			/// @param mass 物体质量 [kg]
			/// @param center 质心位置
			/// @param inertia 惯性
			void SetMassData(float mass, Point const& center, float inertia);

			/// \~chinese
			/// @brief 重置质量数据
			void ResetMassData();

			/// \~chinese
			/// @brief 获取世界坐标系上的点在物体上的位置
			Point GetLocalPoint(Point const& world) const;

			/// \~chinese
			/// @brief 获取物体上的点在世界坐标系的位置
			Point GetWorldPoint(Point const& local) const;

			/// \~chinese
			/// @brief 获取物体质心相对于物体的位置
			Point GetLocalCenter() const;

			/// \~chinese
			/// @brief 获取物体质心位置
			Point GetWorldCenter() const;

			/// \~chinese
			/// @brief 获取物体类型
			Type GetType() const;

			/// \~chinese
			/// @brief 设置物体类型
			void SetType(Type type);

			/// \~chinese
			/// @brief 获取物体受重力的比例
			float GetGravityScale() const;

			/// \~chinese
			/// @brief 设置物体受重力的比例
			void SetGravityScale(float scale);

			/// \~chinese
			/// @brief 施力
			/// @param force 力的大小和方向
			/// @param point 施力点
			/// @param wake 是否唤醒物体
			void ApplyForce(Vec2 const& force, Point const& point, bool wake = true);

			/// \~chinese
			/// @brief 给物体中心施力
			/// @param force 力的大小和方向
			/// @param wake 是否唤醒物体
			void ApplyForceToCenter(Vec2 const& force, bool wake = true);

			/// \~chinese
			/// @brief 施加扭矩
			/// @param torque 扭矩
			/// @param wake 是否唤醒物体
			void ApplyTorque(float torque, bool wake = false);

			/// \~chinese
			/// @brief 旋转角度是否固定
			bool IsIgnoreRotation() const;

			/// \~chinese
			/// @brief 设置是否固定旋转角度
			void SetIgnoreRotation(bool flag);

			/// \~chinese
			/// @brief 是否是子弹物体
			bool IsBullet() const;

			/// \~chinese
			/// @brief 设置物体是否是子弹物体
			void SetBullet(bool flag);

			/// \~chinese
			/// @brief 是否处于唤醒状态
			bool IsAwake() const;

			/// \~chinese
			/// @brief 设置唤醒状态
			void SetAwake(bool flag);

			/// \~chinese
			/// @brief 是否启用休眠
			bool IsSleepingAllowed() const;

			/// \~chinese
			/// @brief 设置是否允许休眠
			void SetSleepingAllowed(bool flag);

			/// \~chinese
			/// @brief 是否启用
			bool IsActive() const;

			/// \~chinese
			/// @brief 设置启用状态
			void SetActive(bool flag);

			/// \~chinese
			/// @brief 获取物体所在物理世界
			World* GetWorld() const;

			/// \~chinese
			/// @brief 获取物体绑定的角色
			Actor* GetActor() const;

			/// \~chinese
			/// @brief 设置物体绑定的角色
			void SetActor(Actor* actor);

			/// \~chinese
			/// @brief 将物体信息更新到角色
			void UpdateActor();

			/// \~chinese
			/// @brief 将角色信息更新到物体
			void UpdateFromActor();

			b2Body* GetB2Body() const;
			void SetB2Body(b2Body* body);

		private:
			/// \~chinese
			/// @brief 销毁物体
			void UpdateFixtureFilter(b2Fixture* fixture);

			/// \~chinese
			/// @brief 销毁物体
			void Destroy();

		private:
			Actor* actor_;
			World* world_;
			b2Body* body_;

			uint16_t category_bits_;
			uint16_t mask_bits_;
			int16_t group_index_;
		};

		/** @} */

		inline Body::Body(World* world, ActorPtr actor)			: Body(world, actor.get()) {}

		inline FixtureList Body::GetFixtureList() const			{ KGE_ASSERT(body_); return FixtureList(Fixture(body_->GetFixtureList())); }

		inline ContactEdgeList Body::GetContactList() const		{ KGE_ASSERT(body_); return ContactEdgeList(ContactEdge(body_->GetContactList())); }

		inline uint16_t Body::GetCategoryBits() const			{ return category_bits_; }

		inline uint16_t Body::GetMaskBits() const				{ return mask_bits_; }

		inline int16_t Body::GetGroupIndex() const				{ return group_index_; }

		inline float Body::GetBodyRotation() const				{ KGE_ASSERT(body_); return math::Radian2Degree(body_->GetAngle()); }

		inline void Body::SetBodyRotation(float angle)			{ SetBodyTransform(GetBodyPosition(), angle); }

		inline void Body::SetBodyPosition(Point const& pos)		{ SetBodyTransform(pos, GetBodyRotation()); }

		inline float Body::GetMass() const						{ KGE_ASSERT(body_); return body_->GetMass(); }

		inline float Body::GetInertia() const					{ KGE_ASSERT(body_); return body_->GetInertia(); }

		inline Body::Type Body::GetType() const					{ KGE_ASSERT(body_); return Type(body_->GetType()); }

		inline void Body::SetType(Type type)					{ KGE_ASSERT(body_); body_->SetType(static_cast<b2BodyType>(type)); }

		inline float Body::GetGravityScale() const				{ KGE_ASSERT(body_); return body_->GetGravityScale(); }

		inline void Body::SetGravityScale(float scale)			{ KGE_ASSERT(body_); body_->SetGravityScale(scale); }

		inline bool Body::IsIgnoreRotation() const				{ KGE_ASSERT(body_); return body_->IsFixedRotation(); }

		inline void Body::SetIgnoreRotation(bool flag)			{ KGE_ASSERT(body_); body_->SetFixedRotation(flag); }

		inline bool Body::IsBullet() const						{ KGE_ASSERT(body_); return body_->IsBullet(); }

		inline void Body::SetBullet(bool flag)					{ KGE_ASSERT(body_); body_->SetBullet(flag); }

		inline bool Body::IsAwake() const						{ KGE_ASSERT(body_); return body_->IsAwake(); }

		inline void Body::SetAwake(bool flag)					{ KGE_ASSERT(body_); body_->SetAwake(flag); }

		inline bool Body::IsSleepingAllowed() const				{ KGE_ASSERT(body_); return body_->IsSleepingAllowed(); }

		inline void Body::SetSleepingAllowed(bool flag)			{ KGE_ASSERT(body_); body_->SetSleepingAllowed(flag); }

		inline bool Body::IsActive() const						{ KGE_ASSERT(body_); return body_->IsActive(); }

		inline void Body::SetActive(bool flag)					{ KGE_ASSERT(body_); body_->SetActive(flag); }

		inline Actor* Body::GetActor() const					{ return actor_; }

		inline void Body::SetActor(Actor* actor)				{ actor_ = actor; }

		inline b2Body* Body::GetB2Body() const					{ return body_; }

		inline World* Body::GetWorld() const					{ return world_; }
	}
}
