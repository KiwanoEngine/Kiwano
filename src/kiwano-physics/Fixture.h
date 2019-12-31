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
		/// @brief ����о�
		class Fixture
		{
		public:
			/// \~chinese
			/// @brief �о߲���
			struct Param
			{
				float density = 0.f;		///< �ܶ�
				float friction = 0.2f;		///< Ħ����
				float restitution = 0.f;	///< ���Իָ�
				bool is_sensor = false;		///< �Ƿ��ǽӴ�������

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
			/// @brief �Ƿ���Ч
			bool IsValid() const;

			/// \~chinese
			/// @brief ��ȡ�о����ڵ�����
			Body* GetBody() const;

			/// \~chinese
			/// @brief ��״
			Shape GetShape() const;

			/// \~chinese
			/// @brief �Ƿ��ǽӴ�������
			bool IsSensor() const;

			/// \~chinese
			/// @brief ���üо��Ƿ��ǽӴ�������
			/// @details �Ӵ�������ֻ���������Ӵ���������Ӱ�������˶�
			void SetSensor(bool sensor);

			/// \~chinese
			/// @brief ��ȡ�оߵ���������
			void GetMassData(float* mass, Point* center, float* inertia) const;

			/// \~chinese
			/// @brief ��ȡ�ܶ�
			float GetDensity() const;

			/// \~chinese
			/// @brief �����ܶ�
			void SetDensity(float density);

			/// \~chinese
			/// @brief ��ȡĦ���� [N]
			float GetFriction() const;

			/// \~chinese
			/// @brief ����Ħ���� [N]
			void SetFriction(float friction);

			/// \~chinese
			/// @brief ��ȡ���Իָ�
			float GetRestitution() const;

			/// \~chinese
			/// @brief ���õ��Իָ�
			void SetRestitution(float restitution);

			/// \~chinese
			/// @brief �����
			bool TestPoint(const Point& p) const;

			b2Fixture* GetB2Fixture() const;
			void SetB2Fixture(b2Fixture* fixture);

		private:
			b2Fixture* fixture_;
		};

		/// \~chinese
		/// @brief ����о��б�
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
