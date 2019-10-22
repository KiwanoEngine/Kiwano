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
#include <kiwano-physics/Fixture.h>

namespace kiwano
{
	namespace physics
	{
		class World;

		// 刚体
		class KGE_API Body
			: public ObjectBase
		{
		public:
			enum class Type
			{
				Static = 0,
				Kinematic,
				Dynamic,
			};

			Body();
			Body(b2Body* body, Actor* actor);
			Body(World* world, Actor* actor);
			virtual ~Body();

			// 添加形状
			Fixture AddShape(Shape* shape, Fixture::Property const& prop = Fixture::Property());
			Fixture AddCircleShape(float radius, Fixture::Property const& prop = Fixture::Property());
			Fixture AddBoxShape(Vec2 const& size, Fixture::Property const& prop = Fixture::Property());
			Fixture AddPolygonShape(Vector<Point> const& vertexs, Fixture::Property const& prop = Fixture::Property());
			Fixture AddEdgeShape(Point const& p1, Point const& p2, Fixture::Property const& prop = Fixture::Property());
			Fixture AddChainShape(Vector<Point> const& vertexs, bool loop = false, Fixture::Property const& prop = Fixture::Property());

			// 获取夹具列表
			Fixture GetFixture() const						{ KGE_ASSERT(body_); Fixture(body_->GetFixtureList()); }

			// 移除夹具
			void RemoveFixture(Fixture const& fixture);

			// 获取质量
			float GetMass() const							{ KGE_ASSERT(body_); return body_->GetMass(); }

			Point GetLocalPoint(Point const& world) const;
			Point GetWorldPoint(Point const& local) const;

			Type GetType() const							{ KGE_ASSERT(body_); return Type(body_->GetType()); }
			void SetType(Type type)							{ KGE_ASSERT(body_); body_->SetType(static_cast<b2BodyType>(type)); }

			bool IsIgnoreRotation() const					{ return ignore_rotation_; }
			void SetIgnoreRotation(bool ignore_rotation)	{ ignore_rotation_ = ignore_rotation; }

			Actor* GetActor() const							{ return actor_; }
			void SetActor(Actor* actor)						{ actor_ = actor; }

			b2Body* GetB2Body()								{ return body_; }
			const b2Body* GetB2Body() const					{ return body_; }
			void SetB2Body(b2Body* body);

			World* GetWorld()								{ return world_; }
			const World* GetWorld() const					{ return world_; }

			void UpdateActor();
			void UpdateFromActor();

		protected:
			bool ignore_rotation_;
			Actor* actor_;
			World* world_;
			b2Body* body_;
		};

		KGE_DECLARE_SMART_PTR(Body);
	}
}
