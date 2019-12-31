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

namespace kiwano
{
	namespace physics
	{
		class Body;

		/**
		* \addtogroup Physics
		* @{
		*/

		/// \~chinese
		/// @brief 物理夹具
		class Fixture
		{
		public:
			/// \~chinese
			/// @brief 夹具参数
			struct Param
			{
				float density = 0.f;		///< 密度
				float friction = 0.2f;		///< 摩擦力
				float restitution = 0.f;	///< 弹性恢复
				bool is_sensor = false;		///< 是否是接触传感器

				Param() {}

				Param(float density, float friction = 0.2f, float restitution = 0.f, bool is_sensor = false)
					: density(density)
					, friction(friction)
					, restitution(restitution)
					, is_sensor(is_sensor)
				{}
			};

			Fixture();
			Fixture(b2Fixture* fixture);
			Fixture(Body* body, Shape* shape, const Param& param);

			/// \~chinese
			/// @brief 是否有效
			bool IsValid() const;

			/// \~chinese
			/// @brief 获取夹具所在的物体
			Body* GetBody() const;

			/// \~chinese
			/// @brief 形状
			Shape GetShape() const;

			/// \~chinese
			/// @brief 是否是接触传感器
			bool IsSensor() const;

			/// \~chinese
			/// @brief 设置夹具是否是接触传感器
			/// @details 接触传感器只会产生物理接触，而不会影响物体运动
			void SetSensor(bool sensor);

			/// \~chinese
			/// @brief 获取夹具的质量数据
			void GetMassData(float* mass, Point* center, float* inertia) const;

			/// \~chinese
			/// @brief 获取密度
			float GetDensity() const;

			/// \~chinese
			/// @brief 设置密度
			void SetDensity(float density);

			/// \~chinese
			/// @brief 获取摩擦力 [N]
			float GetFriction() const;

			/// \~chinese
			/// @brief 设置摩擦力 [N]
			void SetFriction(float friction);

			/// \~chinese
			/// @brief 获取弹性恢复
			float GetRestitution() const;

			/// \~chinese
			/// @brief 设置弹性恢复
			void SetRestitution(float restitution);

			/// \~chinese
			/// @brief 点测试
			bool TestPoint(const Point& p) const;

			b2Fixture* GetB2Fixture() const;
			void SetB2Fixture(b2Fixture* fixture);

		private:
			b2Fixture* fixture_;
		};

		/// \~chinese
		/// @brief 物理夹具列表
		class FixtureList
			: public List<Fixture>
		{
		public:
			FixtureList()
			{
			}

			FixtureList(const Fixture& first)
			{
				Fixture current = first;
				while (current.GetB2Fixture())
				{
					push_back(current);
					current = current.GetB2Fixture()->GetNext();
				}
			}
		};

		/** @} */

		inline bool Fixture::IsSensor() const					{ KGE_ASSERT(fixture_); return fixture_->IsSensor(); }
		inline void Fixture::SetSensor(bool sensor)				{ KGE_ASSERT(fixture_); fixture_->SetSensor(sensor); }
		inline float Fixture::GetDensity() const				{ KGE_ASSERT(fixture_); return fixture_->GetDensity(); }
		inline void Fixture::SetDensity(float density)			{ KGE_ASSERT(fixture_); fixture_->SetDensity(density); }
		inline float Fixture::GetFriction() const				{ KGE_ASSERT(fixture_); return fixture_->GetFriction(); }
		inline void Fixture::SetFriction(float friction)		{ KGE_ASSERT(fixture_); fixture_->SetFriction(friction); }
		inline float Fixture::GetRestitution() const			{ KGE_ASSERT(fixture_); return fixture_->GetRestitution(); }
		inline void Fixture::SetRestitution(float restitution)	{ KGE_ASSERT(fixture_); fixture_->SetRestitution(restitution); }
		inline bool Fixture::IsValid() const					{ return fixture_ != nullptr; }
		inline b2Fixture* Fixture::GetB2Fixture() const			{ return fixture_; }
		inline void Fixture::SetB2Fixture(b2Fixture* fixture)	{ fixture_ = fixture; }
	}
}
