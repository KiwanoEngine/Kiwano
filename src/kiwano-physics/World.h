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
		* \defgroup Physics 物理引擎
		*/

		/**
		* \addtogroup Physics
		* @{
		*/

		/**
		* \~chinese
		* @brief 物理世界
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
			/// @brief 获取重力 [N]
			Vec2 GetGravity() const;

			/// \~chinese
			/// @brief 设置重力 [N]
			void SetGravity(Vec2 gravity);

			/// \~chinese
			/// @brief 获取全局缩放比例
			/// @details 缩放比例是指由物理世界的单位米转换到屏幕像素的比例，默认比例为1:100
			float GetGlobalScale() const;

			/// \~chinese
			/// @brief 设置全局缩放比例
			/// @details 缩放比例是指由物理世界的单位米转换到屏幕像素的比例，默认比例为1:100
			void SetGlobalScale(float scale);

			/// \~chinese
			/// @brief 游戏世界单位转换为物理世界单位
			/// @details 根据全局缩放比例将物理世界的单位米转换为像素单位
			float World2Stage(float value) const;

			/// \~chinese
			/// @brief 游戏世界单位转换为物理世界单位
			/// @details 根据全局缩放比例将物理世界的单位米转换为像素单位
			Vec2 World2Stage(const b2Vec2& pos) const;

			/// \~chinese
			/// @brief 物理世界单位转换为游戏世界单位
			/// @details 根据全局缩放比例将像素单位转换为物理世界的单位米
			float Stage2World(float value) const;

			/// \~chinese
			/// @brief 物理世界单位转换为游戏世界单位
			/// @details 根据全局缩放比例将像素单位转换为物理世界的单位米
			b2Vec2 Stage2World(const Vec2& pos) const;

			/// \~chinese
			/// @brief 设置速度迭代次数, 默认为 6
			void SetVelocityIterations(int vel_iter);

			/// \~chinese
			/// @brief 设置位置迭代次数, 默认为 2
			void SetPositionIterations(int pos_iter);

			b2World* GetB2World();

			const b2World* GetB2World() const;

		private:
			/// \~chinese
			/// @brief 移除物体
			void RemoveBody(Body* body);

			/// \~chinese
			/// @brief 移除所有物体
			void RemoveAllBodies();

			/// \~chinese
			/// @brief 添加关节
			void AddJoint(Joint* joint);

			/// \~chinese
			/// @brief 移除关节
			void RemoveJoint(Joint* joint);

			/// \~chinese
			/// @brief 移除所有关节
			void RemoveAllJoints();

			/// \~chinese
			/// @brief 关节被移除
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
