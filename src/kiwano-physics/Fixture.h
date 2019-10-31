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
		class PhysicBody;

		// 夹具
		class PhysicFixture
		{
		public:
			PhysicFixture();
			PhysicFixture(b2Fixture* fixture);

			static PhysicFixture Create(PhysicBody* body, PhysicShape* shape, float density = 0.f, float friction = 0.2f, float restitution = 0.f);

			// 物体
			PhysicBody* GetBody();
			const PhysicBody* GetBody() const;

			// 形状
			PhysicShape GetShape() const;

			// 下一夹具 (同一物体上)
			PhysicFixture GetNext() const;

			bool IsValid() const					{ return !!fixture_; }

			b2Fixture* GetB2Fixture()				{ return fixture_; }
			const b2Fixture* GetB2Fixture() const	{ return fixture_; }
			void SetB2Fixture(b2Fixture* fixture)	{ fixture_ = fixture; }

		protected:
			b2Fixture* fixture_;
		};
	}
}
