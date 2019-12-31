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
		/// @brief ����
		class KGE_API Body
			: public virtual RefCounter
		{
		public:
			/// \~chinese
			/// @brief ��������
			enum class Type
			{
				Static = 0,	///< ��̬����
				Kinematic,	///< ����ѧ����
				Dynamic,	///< ��̬����
			};

			Body();
			Body(b2Body* body, Actor* actor);
			Body(World* world, Actor* actor);
			Body(World* world, ActorPtr actor);
			virtual ~Body();

			/// \~chinese
			/// @brief ��ʼ��
			/// @param[in] world ��������
			/// @param[in] actor �󶨵Ľ�ɫ
			void Init(World* world, Actor* actor);

			/// \~chinese
			/// @brief ��Ӽо�
			/// @param shape ������״
			/// @param density �����ܶ�
			Fixture AddFixture(Shape* shape, const Fixture::Param& param);

			/// \~chinese
			/// @brief ���Բ�μо�
			/// @param radius Բ�ΰ뾶
			/// @param density �����ܶ�
			Fixture AddCircleShape(float radius, float density = 0.f);

			/// \~chinese
			/// @brief ��Ӻ��μо�
			/// @param size �д�С
			/// @param density �����ܶ�
			Fixture AddBoxShape(Vec2 const& size, float density = 0.f);

			/// \~chinese
			/// @brief ��Ӷ���μо�
			/// @param vertexs ����ζ˵�
			/// @param density �����ܶ�
			Fixture AddPolygonShape(Vector<Point> const& vertexs, float density = 0.f);

			/// \~chinese
			/// @brief ����߶��μо�
			/// @param p1 �߶����
			/// @param p2 �߶��յ�
			/// @param density �����ܶ�
			Fixture AddEdgeShape(Point const& p1, Point const& p2, float density = 0.f);

			/// \~chinese
			/// @brief ��������μо�
			/// @param vertexs �����˵�
			/// @param loop �Ƿ�պ�
			/// @param density �����ܶ�
			Fixture AddChainShape(Vector<Point> const& vertexs, bool loop, float density = 0.f);

			/// \~chinese
			/// @brief ��ȡ�о��б�
			FixtureList GetFixtureList() const;

			/// \~chinese
			/// @brief �Ƴ��о�
			void RemoveFixture(Fixture const& fixture);

			/// \~chinese
			/// @brief ��ȡ�Ӵ����б�
			ContactEdgeList GetContactList() const;

			/// \~chinese
			/// @brief ��ȡ�����
			uint16_t GetCategoryBits() const;

			/// \~chinese
			/// @brief ���������
			void SetCategoryBits(uint16_t category_bits);

			/// \~chinese
			/// @brief ��ȡ��ײ����
			uint16_t GetMaskBits() const;

			/// \~chinese
			/// @brief ������ײ����
			void SetMaskBits(uint16_t mask_bits);

			/// \~chinese
			/// @brief ��ȡ������
			int16_t GetGroupIndex() const;

			/// \~chinese
			/// @brief ����������
			void SetGroupIndex(int16_t index);

			/// \~chinese
			/// @brief ��ȡ��ת�Ƕ�
			float GetBodyRotation() const;

			/// \~chinese
			/// @brief ������ת�Ƕ�
			void SetBodyRotation(float angle);

			/// \~chinese
			/// @brief ��ȡ����λ��
			Point GetBodyPosition() const;

			/// \~chinese
			/// @brief ��������λ��
			void SetBodyPosition(Point const& pos);

			/// \~chinese
			/// @brief λ�ú���ת�任
			void SetBodyTransform(Point const& pos, float angle);

			/// \~chinese
			/// @brief ��ȡ���� [kg]
			float GetMass() const;

			/// \~chinese
			/// @brief ��ȡ����
			float GetInertia() const;

			/// \~chinese
			/// @brief ��ȡ��������
			/// @param[out] mass �������� [kg]
			/// @param[out] center ����λ��
			/// @param[out] inertia ����
			void GetMassData(float* mass, Point* center, float* inertia) const;

			/// \~chinese
			/// @brief ������������
			/// @param mass �������� [kg]
			/// @param center ����λ��
			/// @param inertia ����
			void SetMassData(float mass, Point const& center, float inertia);

			/// \~chinese
			/// @brief ������������
			void ResetMassData();

			/// \~chinese
			/// @brief ��ȡ��������ϵ�ϵĵ��������ϵ�λ��
			Point GetLocalPoint(Point const& world) const;

			/// \~chinese
			/// @brief ��ȡ�����ϵĵ�����������ϵ��λ��
			Point GetWorldPoint(Point const& local) const;

			/// \~chinese
			/// @brief ��ȡ������������������λ��
			Point GetLocalCenter() const;

			/// \~chinese
			/// @brief ��ȡ��������λ��
			Point GetWorldCenter() const;

			/// \~chinese
			/// @brief ��ȡ��������
			Type GetType() const;

			/// \~chinese
			/// @brief ������������
			void SetType(Type type);

			/// \~chinese
			/// @brief ��ȡ�����������ı���
			float GetGravityScale() const;

			/// \~chinese
			/// @brief ���������������ı���
			void SetGravityScale(float scale);

			/// \~chinese
			/// @brief ʩ��
			/// @param force ���Ĵ�С�ͷ���
			/// @param point ʩ����
			/// @param wake �Ƿ�������
			void ApplyForce(Vec2 const& force, Point const& point, bool wake = true);

			/// \~chinese
			/// @brief ����������ʩ��
			/// @param force ���Ĵ�С�ͷ���
			/// @param wake �Ƿ�������
			void ApplyForceToCenter(Vec2 const& force, bool wake = true);

			/// \~chinese
			/// @brief ʩ��Ť��
			/// @param torque Ť��
			/// @param wake �Ƿ�������
			void ApplyTorque(float torque, bool wake = false);

			/// \~chinese
			/// @brief ��ת�Ƕ��Ƿ�̶�
			bool IsIgnoreRotation() const;

			/// \~chinese
			/// @brief �����Ƿ�̶���ת�Ƕ�
			void SetIgnoreRotation(bool flag);

			/// \~chinese
			/// @brief �Ƿ����ӵ�����
			bool IsBullet() const;

			/// \~chinese
			/// @brief ���������Ƿ����ӵ�����
			void SetBullet(bool flag);

			/// \~chinese
			/// @brief �Ƿ��ڻ���״̬
			bool IsAwake() const;

			/// \~chinese
			/// @brief ���û���״̬
			void SetAwake(bool flag);

			/// \~chinese
			/// @brief �Ƿ���������
			bool IsSleepingAllowed() const;

			/// \~chinese
			/// @brief �����Ƿ���������
			void SetSleepingAllowed(bool flag);

			/// \~chinese
			/// @brief �Ƿ�����
			bool IsActive() const;

			/// \~chinese
			/// @brief ��������״̬
			void SetActive(bool flag);

			/// \~chinese
			/// @brief ��ȡ����������������
			World* GetWorld() const;

			/// \~chinese
			/// @brief ��ȡ����󶨵Ľ�ɫ
			Actor* GetActor() const;

			/// \~chinese
			/// @brief ��������󶨵Ľ�ɫ
			void SetActor(Actor* actor);

			/// \~chinese
			/// @brief ��������Ϣ���µ���ɫ
			void UpdateActor();

			/// \~chinese
			/// @brief ����ɫ��Ϣ���µ�����
			void UpdateFromActor();

			b2Body* GetB2Body() const;
			void SetB2Body(b2Body* body);

		private:
			/// \~chinese
			/// @brief ��������
			void UpdateFixtureFilter(b2Fixture* fixture);

			/// \~chinese
			/// @brief ��������
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
