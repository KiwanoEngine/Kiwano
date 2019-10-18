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

			struct Property
			{
				float density;		// 密度 kg/m^2
				float friction;		// 摩擦系数 [0,1]
				float restitution;	// 弹性 [0,1]

				Property()
					: density(0.f)
					, friction(0.2f)
					, restitution(0.f)
				{
				}

				Property(float density, float friction, float restitution)
					: density(density)
					, friction(friction)
					, restitution(restitution)
				{
				}
			};

			Body();
			Body(b2Body* body, Actor* actor);
			Body(World* world, Actor* actor);
			virtual ~Body();

			b2Fixture* AddShape(ShapePtr shape, Property const& prop);
			b2Fixture* AddCircleShape(float radius, Property const& prop = Property());
			b2Fixture* AddBoxShape(Vec2 const& size, Property const& prop = Property());
			b2Fixture* AddPolygonShape(Vector<Point> const& vertexs, Property const& prop = Property());

			Type GetType() const;
			void SetType(Type type);

			Actor* GetActor() const				{ return actor_; }
			void SetActor(Actor* actor)			{ actor_ = actor; }

			b2Body* GetB2Body()					{ return body_; }
			const b2Body* GetB2Body() const		{ return body_; }
			void SetB2Body(b2Body* body);

			World* GetWorld()					{ return world_; }
			const World* GetWorld() const		{ return world_; }

			void UpdateActor();
			void UpdateFromActor();

		protected:
			Actor* actor_;
			World* world_;
			b2Body* body_;
		};

		KGE_DECLARE_SMART_PTR(Body);
	}
}
