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

		b2Shape* Shape::GetB2Shape() const
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

		CircleShape::CircleShape(float radius, Point const& offset)
			: CircleShape()
		{
			Set(radius, offset);
		}

		void CircleShape::Set(float radius, Point const& offset)
		{
			radius_ = radius;
			offset_ = offset;
		}

		void CircleShape::FitWorld(World* world)
		{
			KGE_ASSERT(world);
			circle_.m_radius = world->Stage2World(radius_);
			circle_.m_p = world->Stage2World(offset_);
		}

		//
		// BoxShape
		//

		BoxShape::BoxShape()
			: Shape(&polygon_)
			, polygon_()
			, rotation_(0.f)
		{
		}

		BoxShape::BoxShape(Vec2 const& size, Point const& offset, float rotation)
			: BoxShape()
		{
			Set(size, offset, rotation);
		}

		void BoxShape::Set(Vec2 const& size, Point const& offset, float rotation)
		{
			box_size_ = size;
			offset_ = offset;
			rotation_ = rotation;
		}

		void BoxShape::FitWorld(World* world)
		{
			KGE_ASSERT(world);

			b2Vec2 box = world->Stage2World(box_size_);
			b2Vec2 offset = world->Stage2World(offset_);
			polygon_.SetAsBox(box.x / 2, box.y / 2, offset, rotation_);
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

		//
		// EdgeShape
		//

		EdgeShape::EdgeShape()
			: Shape(&edge_)
			, edge_()
		{
		}

		EdgeShape::EdgeShape(Point const& p1, Point const& p2)
			: EdgeShape()
		{
			Set(p1, p2);
		}

		void EdgeShape::Set(Point const& p1, Point const& p2)
		{
			p_[0] = p1;
			p_[1] = p2;
		}

		void EdgeShape::FitWorld(World* world)
		{
			KGE_ASSERT(world);

			b2Vec2 p1 = world->Stage2World(p_[0]);
			b2Vec2 p2 = world->Stage2World(p_[1]);
			edge_.Set(p1, p2);
		}

		//
		// ChainShape
		//

		ChainShape::ChainShape()
			: Shape(&chain_)
			, chain_()
			, loop_(false)
		{
		}

		ChainShape::ChainShape(Vector<Point> const& vertexs, bool loop)
			: ChainShape()
		{
			Set(vertexs, loop);
		}

		void ChainShape::Set(Vector<Point> const& vertexs, bool loop)
		{
			vertexs_ = vertexs;
			loop_ = loop;
		}

		void ChainShape::FitWorld(World* world)
		{
			KGE_ASSERT(world);

			Vector<b2Vec2> b2vertexs;
			b2vertexs.reserve(vertexs_.size());
			for (const auto& v : vertexs_)
			{
				b2vertexs.push_back(world->Stage2World(v));
			}

			if (loop_)
			{
				chain_.CreateLoop(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));
			}
			else
			{
				chain_.CreateChain(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));
			}
		}

}
}
