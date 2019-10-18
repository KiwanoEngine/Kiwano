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

namespace kiwano
{
	namespace physics
	{
		class World;

		KGE_DECLARE_SMART_PTR(Shape);
		KGE_DECLARE_SMART_PTR(CircleShape);
		KGE_DECLARE_SMART_PTR(BoxShape);
		KGE_DECLARE_SMART_PTR(PolygonShape);

		// 形状基类
		class KGE_API Shape
			: public ObjectBase
		{
		public:
			Shape();

			Shape(b2Shape* shape);

			b2Shape* GetB2Shape();
			const b2Shape* GetB2Shape() const;
			void SetB2Shape(b2Shape* shape);

			virtual void FitWorld(World* world) = 0;

		protected:
			b2Shape* shape_;
		};

		// 圆形形状
		class KGE_API CircleShape
			: public Shape
		{
		public:
			CircleShape();

			CircleShape(float radius);

			void Set(float radius);

			void FitWorld(World* world) override;

		protected:
			float radius_;
			b2CircleShape circle_;
		};

		// 盒子形状
		class KGE_API BoxShape
			: public Shape
		{
		public:
			BoxShape();

			BoxShape(Vec2 const& size);

			void Set(Vec2 const& size);

			void FitWorld(World* world) override;

		protected:
			Vec2 box_size_;
			b2PolygonShape polygon_;
		};

		// 多边形形状
		class KGE_API PolygonShape
			: public Shape
		{
		public:
			PolygonShape();

			PolygonShape(Vector<Point> const& vertexs);

			void Set(Vector<Point> const& vertexs);

			void FitWorld(World* world) override;

		protected:
			Vector<Point> vertexs_;
			b2PolygonShape polygon_;
		};
	}
}
