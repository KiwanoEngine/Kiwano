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
		// ��������
		class KGE_API World
			: public Stage
		{
			friend class Body;
			friend class Joint;

		public:
			World();

			virtual ~World();

			// ��ȡ����
			Vec2 GetGravity() const;

			// ��������
			void SetGravity(Vec2 gravity);

			// ��ȡȫ�����ű���
			inline float	GetGlobalScale() const					{ return global_scale_; }

			// ����ȫ�����ű���
			inline void		SetGlobalScale(float scale)				{ global_scale_ = scale; }

			// ��Ϸ���絥λת��Ϊ�������絥λ
			inline float	World2Stage(float value) const			{ return value * GetGlobalScale(); }
			inline Point	World2Stage(const b2Vec2& pos) const	{ return Point(World2Stage(pos.x), World2Stage(pos.y)); }

			// �������絥λת��Ϊ��Ϸ���絥λ
			inline float	Stage2World(float value) const			{ return value / GetGlobalScale(); }
			inline b2Vec2	Stage2World(const Point& pos) const		{ return b2Vec2(Stage2World(pos.x), Stage2World(pos.y)); }

			// �����ٶȵ�������, Ĭ��Ϊ 6
			inline void SetVelocityIterations(int vel_iter)			{ vel_iter_ = vel_iter; }

			// ����λ�õ�������, Ĭ��Ϊ 2
			inline void SetPositionIterations(int pos_iter)			{ pos_iter_ = pos_iter; }

			b2World* GetB2World();
			const b2World* GetB2World() const;

		private:
			// �Ƴ�����
			void RemoveBody(Body* body);

			// �Ƴ���������
			void RemoveAllBodies();

			// ��ӹؽ�
			void AddJoint(Joint* joint);

			// �Ƴ��ؽ�
			void RemoveJoint(Joint* joint);

			// �Ƴ����йؽ�
			void RemoveAllJoints();

			// �ؽڱ��Ƴ�
			void JointRemoved(b2Joint* joint);

			void Update(Duration dt) override;

		private:
			b2World world_;
			int vel_iter_;
			int pos_iter_;
			float global_scale_;

			class DestructionListener;
			friend DestructionListener;
			DestructionListener* destruction_listener_;

			class ContactListener;
			friend ContactListener;
			ContactListener* contact_listener_;

			bool removing_joint_;
			Vector<Joint*> joints_;
		};

		KGE_DECLARE_SMART_PTR(World);
	}
}
