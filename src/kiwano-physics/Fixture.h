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

		// 夹具
		class Fixture
		{
		public:
			struct Param
			{
				float density = 0.f;
				float friction = 0.2f;
				float restitution = 0.f;
				bool is_sensor = false;

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

			// 物体
			Body* GetBody();
			const Body* GetBody() const;

			// 形状
			Shape GetShape() const;

			// 下一夹具 (同一物体上)
			Fixture GetNext() const;

			// 接触传感器
			bool IsSensor() const					{ KGE_ASSERT(fixture_); return fixture_->IsSensor(); }
			void SetSensor(bool sensor)				{ KGE_ASSERT(fixture_); fixture_->SetSensor(sensor); }

			// 质量数据
			void GetMassData(float* mass, Point* center, float* inertia) const;

			// 密度
			float GetDensity() const				{ KGE_ASSERT(fixture_); return fixture_->GetDensity(); }
			void SetDensity(float density)			{ KGE_ASSERT(fixture_); fixture_->SetDensity(density); }

			// 摩擦力
			float GetFriction() const				{ KGE_ASSERT(fixture_); return fixture_->GetFriction(); }
			void SetFriction(float friction)		{ KGE_ASSERT(fixture_); fixture_->SetFriction(friction); }

			// 弹性恢复
			float GetRestitution() const			{ KGE_ASSERT(fixture_); return fixture_->GetRestitution(); }
			void SetRestitution(float restitution)	{ KGE_ASSERT(fixture_); fixture_->SetRestitution(restitution); }

			// 点测试
			bool TestPoint(const Point& p) const;

			bool IsValid() const					{ return !!fixture_; }

			b2Fixture* GetB2Fixture()				{ return fixture_; }
			const b2Fixture* GetB2Fixture() const	{ return fixture_; }
			void SetB2Fixture(b2Fixture* fixture)	{ fixture_ = fixture; }

		private:
			b2Fixture* fixture_;
		};
	}
}
