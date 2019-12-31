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
		KGE_DECLARE_SMART_PTR(World);

		/**
		* \~chinese
		* \defgroup Physics ��������
		*/

		/**
		* \addtogroup Physics
		* @{
		*/

		/**
		* \~chinese
		* @brief ��������
		*/
		class KGE_API World
			: public Stage
		{
			friend class Body;
			friend class Joint;

		public:
			World();

			virtual ~World();

			/// \~chinese
			/// @brief ��ȡ���� [N]
			Vec2 GetGravity() const;

			/// \~chinese
			/// @brief �������� [N]
			void SetGravity(Vec2 gravity);

			/// \~chinese
			/// @brief ��ȡȫ�����ű���
			/// @details ���ű�����ָ����������ĵ�λ��ת������Ļ���صı�����Ĭ�ϱ���Ϊ1:100
			float GetGlobalScale() const;

			/// \~chinese
			/// @brief ����ȫ�����ű���
			/// @details ���ű�����ָ����������ĵ�λ��ת������Ļ���صı�����Ĭ�ϱ���Ϊ1:100
			void SetGlobalScale(float scale);

			/// \~chinese
			/// @brief ��Ϸ���絥λת��Ϊ�������絥λ
			/// @details ����ȫ�����ű�������������ĵ�λ��ת��Ϊ���ص�λ
			float World2Stage(float value) const;

			/// \~chinese
			/// @brief ��Ϸ���絥λת��Ϊ�������絥λ
			/// @details ����ȫ�����ű�������������ĵ�λ��ת��Ϊ���ص�λ
			Vec2 World2Stage(const b2Vec2& pos) const;

			/// \~chinese
			/// @brief �������絥λת��Ϊ��Ϸ���絥λ
			/// @details ����ȫ�����ű��������ص�λת��Ϊ��������ĵ�λ��
			float Stage2World(float value) const;

			/// \~chinese
			/// @brief �������絥λת��Ϊ��Ϸ���絥λ
			/// @details ����ȫ�����ű��������ص�λת��Ϊ��������ĵ�λ��
			b2Vec2 Stage2World(const Vec2& pos) const;

			/// \~chinese
			/// @brief �����ٶȵ�������, Ĭ��Ϊ 6
			void SetVelocityIterations(int vel_iter);

			/// \~chinese
			/// @brief ����λ�õ�������, Ĭ��Ϊ 2
			void SetPositionIterations(int pos_iter);

			b2World* GetB2World();

			const b2World* GetB2World() const;

		private:
			/// \~chinese
			/// @brief �Ƴ�����
			void RemoveBody(Body* body);

			/// \~chinese
			/// @brief �Ƴ���������
			void RemoveAllBodies();

			/// \~chinese
			/// @brief ��ӹؽ�
			void AddJoint(Joint* joint);

			/// \~chinese
			/// @brief �Ƴ��ؽ�
			void RemoveJoint(Joint* joint);

			/// \~chinese
			/// @brief �Ƴ����йؽ�
			void RemoveAllJoints();

			/// \~chinese
			/// @brief �ؽڱ��Ƴ�
			void JointRemoved(b2Joint* joint);

		protected:
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


		/** @} */

		inline float World::GetGlobalScale() const
		{
			return global_scale_;
		}

		inline void World::SetGlobalScale(float scale)
		{
			global_scale_ = scale;
		}

		inline float World::World2Stage(float value) const
		{
			return value * GetGlobalScale();
		}

		inline Vec2 World::World2Stage(const b2Vec2& pos) const
		{
			return Point(World2Stage(pos.x), World2Stage(pos.y));
		}

		inline float World::Stage2World(float value) const
		{
			return value / GetGlobalScale();
		}

		inline b2Vec2 World::Stage2World(const Vec2& pos) const
		{
			return b2Vec2(Stage2World(pos.x), Stage2World(pos.y));
		}

		inline void World::SetVelocityIterations(int vel_iter)
		{
			vel_iter_ = vel_iter;
		}

		inline void World::SetPositionIterations(int pos_iter)
		{
			pos_iter_ = pos_iter;
		}
	}
}
