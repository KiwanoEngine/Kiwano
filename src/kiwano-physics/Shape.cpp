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
		PhysicShape::PhysicShape()
			: shape_(nullptr)
		{
		}

		PhysicShape::PhysicShape(b2Shape* shape)
			: shape_(shape)
		{
		}

		b2Shape* PhysicShape::GetB2Shape()
		{
			return shape_;
		}

		const b2Shape* PhysicShape::GetB2Shape() const
		{
			return shape_;
		}

		void PhysicShape::SetB2Shape(b2Shape* shape)
		{
			shape_ = shape;
		}

		//
		// PhysicCircleShape
		//

		PhysicCircleShape::PhysicCircleShape()
			: PhysicShape(&circle_)
			, circle_()
			, radius_(0.f)
		{
		}

		PhysicCircleShape::PhysicCircleShape(float radius, Point const& offset)
			: PhysicCircleShape()
		{
			Set(radius, offset);
		}

		void PhysicCircleShape::Set(float radius, Point const& offset)
		{
			radius_ = radius;
			offset_ = offset;
		}

		void PhysicCircleShape::FitWorld(PhysicWorld* world)
		{
			KGE_ASSERT(world);
			circle_.m_radius = world->Stage2World(radius_);
			circle_.m_p = world->Stage2World(offset_);
		}

		//
		// PhysicBoxShape
		//

		PhysicBoxShape::PhysicBoxShape()
			: PhysicShape(&polygon_)
			, polygon_()
			, rotation_(0.f)
		{
		}

		PhysicBoxShape::PhysicBoxShape(Vec2 const& size, Point const& offset, float rotation)
			: PhysicBoxShape()
		{
			Set(size, offset, rotation);
		}

		void PhysicBoxShape::Set(Vec2 const& size, Point const& offset, float rotation)
		{
			box_size_ = size;
			offset_ = offset;
			rotation_ = rotation;
		}

		void PhysicBoxShape::FitWorld(PhysicWorld* world)
		{
			KGE_ASSERT(world);

			b2Vec2 box = world->Stage2World(box_size_);
			b2Vec2 offset = world->Stage2World(offset_);
			polygon_.SetAsBox(box.x / 2, box.y / 2, offset, rotation_);
		}

		//
		// PhysicPolygonShape
		//

		PhysicPolygonShape::PhysicPolygonShape()
			: PhysicShape(&polygon_)
			, polygon_()
		{
		}

		PhysicPolygonShape::PhysicPolygonShape(Vector<Point> const& vertexs)
			: PhysicPolygonShape()
		{
			Set(vertexs);
		}

		void PhysicPolygonShape::Set(Vector<Point> const& vertexs)
		{
			vertexs_ = vertexs;
		}

		void PhysicPolygonShape::FitWorld(PhysicWorld* world)
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
		// PhysicEdgeShape
		//

		PhysicEdgeShape::PhysicEdgeShape()
			: PhysicShape(&edge_)
			, edge_()
		{
		}

		PhysicEdgeShape::PhysicEdgeShape(Point const& p1, Point const& p2)
			: PhysicEdgeShape()
		{
			Set(p1, p2);
		}

		void PhysicEdgeShape::Set(Point const& p1, Point const& p2)
		{
			p_[0] = p1;
			p_[1] = p2;
		}

		void PhysicEdgeShape::FitWorld(PhysicWorld* world)
		{
			KGE_ASSERT(world);

			b2Vec2 p1 = world->Stage2World(p_[0]);
			b2Vec2 p2 = world->Stage2World(p_[1]);
			edge_.Set(p1, p2);
		}

		//
		// PhysicChainShape
		//

		PhysicChainShape::PhysicChainShape()
			: PhysicShape(&chain_)
			, chain_()
			, loop_(false)
		{
		}

		PhysicChainShape::PhysicChainShape(Vector<Point> const& vertexs, bool loop)
			: PhysicChainShape()
		{
			Set(vertexs, loop);
		}

		void PhysicChainShape::Set(Vector<Point> const& vertexs, bool loop)
		{
			vertexs_ = vertexs;
			loop_ = loop;
		}

		void PhysicChainShape::FitWorld(PhysicWorld* world)
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
