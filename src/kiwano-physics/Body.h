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
		class PhysicWorld;

		// ����
		KGE_DECLARE_SMART_PTR(PhysicBody);
		class KGE_API PhysicBody
			: public virtual RefCounter
		{
		public:
			enum class Type
			{
				Static = 0,
				Kinematic,
				Dynamic,
			};

			PhysicBody();
			PhysicBody(b2Body* body, Actor* actor);
			PhysicBody(PhysicWorld* world, Actor* actor);
			PhysicBody(PhysicWorld* world, ActorPtr actor) : PhysicBody(world, actor.get()) {}
			virtual ~PhysicBody();

			// ��ʼ��
			void Init(PhysicWorld* world, Actor* actor);

			// �����״
			PhysicFixture AddShape(PhysicShape* shape, float density = 0.f, float friction = 0.2f, float restitution = 0.f);
			PhysicFixture AddCircleShape(float radius, float density = 0.f);
			PhysicFixture AddBoxShape(Vec2 const& size, float density = 0.f);
			PhysicFixture AddPolygonShape(Vector<Point> const& vertexs, float density = 0.f);
			PhysicFixture AddEdgeShape(Point const& p1, Point const& p2, float density = 0.f);
			PhysicFixture AddChainShape(Vector<Point> const& vertexs, bool loop, float density = 0.f);

			// ��ȡ�о�
			PhysicFixture GetFixtureList() const			{ KGE_ASSERT(body_); PhysicFixture(body_->GetFixtureList()); }

			// �Ƴ��о�
			void RemoveFixture(PhysicFixture const& fixture);

			// ��ȡ�Ӵ���
			PhysicContactEdge GetContactList() const		{ KGE_ASSERT(body_); PhysicContactEdge(body_->GetContactList()); }

			// �����
			uint16_t GetCategoryBits() const				{ return category_bits_; }
			void SetCategoryBits(uint16_t category_bits);

			// ��ײ����
			uint16_t GetMaskBits() const					{ return mask_bits_; }
			void SetMaskBits(uint16_t mask_bits);

			// ������
			int16_t GetGroupIndex() const					{ return group_index_; }
			void SetGroupIndex(int16_t index);

			// ��ת�Ƕ�
			float GetBodyRotation() const					{ KGE_ASSERT(body_); return math::Radian2Degree(body_->GetAngle()); }
			void SetBodyRotation(float angle)				{ SetBodyTransform(GetBodyPosition(), angle); }

			// λ��
			Point GetBodyPosition() const;
			void SetBodyPosition(Point const& pos)			{ SetBodyTransform(pos, GetBodyRotation()); }

			// λ�ú���ת�任
			void SetBodyTransform(Point const& pos, float angle);

			// ����
			float GetMass() const							{ KGE_ASSERT(body_); return body_->GetMass(); }

			// ����
			float GetInertia() const						{ KGE_ASSERT(body_); return body_->GetInertia(); }

			// ��������
			void GetMassData(float* mass, Point* center, float* inertia);
			void SetMassData(float mass, Point const& center, float inertia);
			void ResetMassData();

			// ����ת��
			Point GetLocalPoint(Point const& world) const;
			Point GetWorldPoint(Point const& local) const;

			// ��������
			Point GetLocalCenter() const;
			Point GetWorldCenter() const;

			// ��������
			Type GetType() const							{ KGE_ASSERT(body_); return Type(body_->GetType()); }
			void SetType(Type type)							{ KGE_ASSERT(body_); body_->SetType(static_cast<b2BodyType>(type)); }

			// ��������
			float GetGravityScale() const					{ KGE_ASSERT(body_); return body_->GetGravityScale(); }
			void SetGravityScale(float scale)				{ KGE_ASSERT(body_); body_->SetGravityScale(scale); }

			// ʩ��
			void ApplyForce(Vec2 const& force, Point const& point, bool wake = true);
			void ApplyForceToCenter(Vec2 const& force, bool wake = true);

			// ʩ��Ť��
			void ApplyTorque(float torque, bool wake = true);

			// �̶���ת
			bool IsIgnoreRotation() const					{ KGE_ASSERT(body_); return body_->IsFixedRotation(); }
			void SetIgnoreRotation(bool flag)				{ KGE_ASSERT(body_); body_->SetFixedRotation(flag); }

			// �ӵ�
			bool IsBullet() const							{ KGE_ASSERT(body_); return body_->IsBullet(); }
			void SetBullet(bool flag)						{ KGE_ASSERT(body_); body_->SetBullet(flag); }

			// ����
			bool IsAwake() const							{ KGE_ASSERT(body_); return body_->IsAwake(); }
			void SetAwake(bool flag)						{ KGE_ASSERT(body_); body_->SetAwake(flag); }
			bool IsSleepingAllowed() const					{ KGE_ASSERT(body_); return body_->IsSleepingAllowed(); }
			void SetSleepingAllowed(bool flag)				{ KGE_ASSERT(body_); body_->SetSleepingAllowed(flag); }

			// �״̬
			bool IsActive() const							{ KGE_ASSERT(body_); return body_->IsActive(); }
			void SetActive(bool flag)						{ KGE_ASSERT(body_); body_->SetActive(flag); }

			Actor* GetActor() const							{ return actor_; }
			void SetActor(Actor* actor)						{ actor_ = actor; }

			b2Body* GetB2Body()								{ return body_; }
			const b2Body* GetB2Body() const					{ return body_; }
			void SetB2Body(b2Body* body);

			PhysicWorld* GetWorld()							{ return world_; }
			const PhysicWorld* GetWorld() const				{ return world_; }

			void Destroy();

			void UpdateActor();
			void UpdateFromActor();

		protected:
			void UpdateFixtureFilter(b2Fixture* fixture);

		protected:
			Actor* actor_;
			PhysicWorld* world_;
			b2Body* body_;

			uint16_t category_bits_;
			uint16_t mask_bits_;
			int16_t group_index_;
		};
	}
}
