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

#include <kiwano-physics/Shape.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
	namespace physics
	{
		Shape::Shape()
			: shape_(nullptr)
		{
		}

		Shape::Shape(b2Shape* shape)
			: shape_(shape)
		{
		}

		b2Shape* Shape::GetB2Shape()
		{
			return shape_;
		}

		const b2Shape* Shape::GetB2Shape() const
		{
			return shape_;
		}

		void Shape::SetB2Shape(b2Shape* shape)
		{
			shape_ = shape;
		}

		//
		// CircleShape
		//

		CircleShape::CircleShape()
			: Shape(&circle_)
			, circle_()
			, radius_(0.f)
		{
		}

		CircleShape::CircleShape(float radius)
			: CircleShape()
		{
			Set(radius);
		}

		void CircleShape::Set(float radius)
		{
			radius_ = radius;
		}

		void CircleShape::FitWorld(World* world)
		{
			KGE_ASSERT(world);
			circle_.m_radius = world->Stage2World(radius_);
		}

		//
		// BoxShape
		//

		BoxShape::BoxShape()
			: Shape(&polygon_)
			, polygon_()
		{
		}

		BoxShape::BoxShape(Vec2 const& size)
			: BoxShape()
		{
			Set(size);
		}

		void BoxShape::Set(Vec2 const& size)
		{
			box_size_ = size;
		}

		void BoxShape::FitWorld(World* world)
		{
			KGE_ASSERT(world);

			b2Vec2 box = world->Stage2World(box_size_);
			polygon_.SetAsBox(box.x / 2, box.y / 2);
		}

		//
		// PolygonShape
		//

		PolygonShape::PolygonShape()
			: Shape(&polygon_)
			, polygon_()
		{
		}

		PolygonShape::PolygonShape(Vector<Point> const& vertexs)
			: PolygonShape()
		{
			Set(vertexs);
		}

		void PolygonShape::Set(Vector<Point> const& vertexs)
		{
			vertexs_ = vertexs;
		}

		void PolygonShape::FitWorld(World* world)
		{
			KGE_ASSERT(world);

			Vector<b2Vec2> b2vertexs;
			b2vertexs.reserve(vertexs_.size());
			for (const auto& v : vertexs_)
			{
				b2vertexs.push_back(world->Stage2World(v));
			}

			polygon_.Set(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));
		}

	}
}
