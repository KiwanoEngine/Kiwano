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

#include <kiwano-physics/Body.h>
#include <kiwano-physics/Fixture.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
namespace physics
{

namespace
{

FixturePtr CreateFixture(Body* body, b2Shape* shape, const Fixture::Param& param)
{
    KGE_ASSERT(body);

    b2Body* b2body = body->GetB2Body();
    KGE_ASSERT(b2body);

    b2FixtureDef fd;
    fd.density     = param.density;
    fd.friction    = param.friction;
    fd.restitution = param.restitution;
    fd.shape       = shape;

    FixturePtr ptr = new (std::nothrow) Fixture;
    if (ptr)
    {
        b2Fixture* fixture = b2body->CreateFixture(&fd);
        if (fixture)
        {
            fixture->SetUserData(ptr.Get());
            ptr->SetB2Fixture(fixture);
            return ptr;
        }
    }
    return nullptr;
}

}  // namespace

FixturePtr Fixture::CreateCircle(Body* body, Param const& param, float radius, Point const& offset)
{
    KGE_ASSERT(body);
    World* world = body->GetWorld();

    b2CircleShape shape;
    shape.m_radius = world->Stage2World(radius);
    shape.m_p      = world->Stage2World(offset);

    return CreateFixture(body, &shape, param);
}

FixturePtr Fixture::CreateRect(Body* body, Param const& param, Size const& size, Point const& offset, float rotation)
{
    KGE_ASSERT(body);
    World* world = body->GetWorld();

    b2Vec2 b2size   = world->Stage2World(size);
    b2Vec2 b2offset = world->Stage2World(offset);

    b2PolygonShape shape;
    shape.SetAsBox(b2size.x / 2, b2size.y / 2, b2offset, rotation);

    return CreateFixture(body, &shape, param);
}

FixturePtr Fixture::CreatePolygon(Body* body, Param const& param, Vector<Point> const& vertexs)
{
    KGE_ASSERT(body);
    World* world = body->GetWorld();

    Vector<b2Vec2> b2vertexs;
    b2vertexs.reserve(vertexs.size());
    for (const auto& v : vertexs)
    {
        b2vertexs.push_back(world->Stage2World(v));
    }

    b2PolygonShape shape;
    shape.Set(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));

    return CreateFixture(body, &shape, param);
}

FixturePtr Fixture::CreateEdge(Body* body, Param const& param, Point const& p1, Point const& p2)
{
    KGE_ASSERT(body);
    World* world = body->GetWorld();

    b2Vec2 start = world->Stage2World(p1);
    b2Vec2 end   = world->Stage2World(p2);

    b2EdgeShape shape;
    shape.Set(start, end);

    return CreateFixture(body, &shape, param);
}

FixturePtr Fixture::CreateChain(Body* body, Param const& param, Vector<Point> const& vertexs, bool loop)
{
    KGE_ASSERT(body);
    World* world = body->GetWorld();

    Vector<b2Vec2> b2vertexs;
    b2vertexs.reserve(vertexs.size());
    for (const auto& v : vertexs)
    {
        b2vertexs.push_back(world->Stage2World(v));
    }

    b2ChainShape shape;
    if (loop)
    {
        shape.CreateLoop(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));
    }
    else
    {
        shape.CreateChain(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));
    }
    return CreateFixture(body, &shape, param);
}

Fixture::Fixture()
    : fixture_(nullptr)
{
}

Fixture::~Fixture()
{
    if (fixture_)
    {
        b2Body* body = fixture_->GetBody();
        if (body)
        {
            body->DestroyFixture(fixture_);
        }
    }
}

Body* Fixture::GetBody() const
{
    fixture_->GetShape();
    KGE_ASSERT(fixture_);
    return static_cast<Body*>(fixture_->GetBody()->GetUserData());
}

void Fixture::GetMassData(float* mass, Point* center, float* inertia) const
{
    KGE_ASSERT(fixture_);

    const Body* body = GetBody();
    KGE_ASSERT(body);

    const World* world = body->GetWorld();
    KGE_ASSERT(world);

    b2MassData data;
    fixture_->GetMassData(&data);

    if (mass)
        *mass = data.mass;
    if (center)
        *center = world->World2Stage(data.center);
    if (inertia)
        *inertia = data.I;
}

bool Fixture::TestPoint(const Point& p) const
{
    KGE_ASSERT(fixture_);

    const Body* body = GetBody();
    KGE_ASSERT(body);

    const World* world = body->GetWorld();
    KGE_ASSERT(world);

    return fixture_->TestPoint(world->Stage2World(p));
}

}  // namespace physics
}  // namespace kiwano
