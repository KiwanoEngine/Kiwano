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

#include <kiwano-physics/PhysicBody.h>
#include <kiwano-physics/Fixture.h>
#include <kiwano-physics/PhysicWorld.h>

namespace kiwano
{
namespace physics
{

FixturePtr Fixture::CreateCircle(Param const& param, float radius, Point const& offset)
{
    FixturePtr ptr = new (std::nothrow) Fixture;
    if (ptr)
    {
        auto shape      = std::make_unique<b2CircleShape>();
        shape->m_radius = global::ToMeters(radius);
        shape->m_p      = global::ToMeters(offset);

        ptr->shape_ = std::move(shape);
        ptr->param_ = param;
    }
    return ptr;
}

FixturePtr Fixture::CreateRect(Param const& param, Size const& size, Point const& offset, float rotation)
{
    FixturePtr ptr = new (std::nothrow) Fixture;
    if (ptr)
    {
        b2Vec2 b2size   = global::ToMeters(size);
        b2Vec2 b2offset = global::ToMeters(offset);

        auto shape = std::make_unique<b2PolygonShape>();
        shape->SetAsBox(b2size.x / 2, b2size.y / 2, b2offset, math::Degree2Radian(rotation));

        ptr->shape_ = std::move(shape);
        ptr->param_ = param;
    }
    return ptr;
}

FixturePtr Fixture::CreatePolygon(Param const& param, Vector<Point> const& vertexs)
{
    FixturePtr ptr = new (std::nothrow) Fixture;
    if (ptr)
    {
        Vector<b2Vec2> b2vertexs;
        b2vertexs.reserve(vertexs.size());
        for (const auto& v : vertexs)
        {
            b2vertexs.push_back(global::ToMeters(v));
        }

        auto shape = std::make_unique<b2PolygonShape>();
        shape->Set(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));

        ptr->shape_ = std::move(shape);
        ptr->param_ = param;
    }
    return ptr;
}

FixturePtr Fixture::CreateEdge(Param const& param, Point const& p1, Point const& p2)
{
    FixturePtr ptr = new (std::nothrow) Fixture;
    if (ptr)
    {
        b2Vec2 start = global::ToMeters(p1);
        b2Vec2 end   = global::ToMeters(p2);

        auto shape = std::make_unique<b2EdgeShape>();
        shape->Set(start, end);

        ptr->shape_ = std::move(shape);
        ptr->param_ = param;
    }
    return ptr;
}

FixturePtr Fixture::CreateChain(Param const& param, Vector<Point> const& vertexs, bool loop)
{
    FixturePtr ptr = new (std::nothrow) Fixture;
    if (ptr)
    {
        Vector<b2Vec2> b2vertexs;
        b2vertexs.reserve(vertexs.size());
        for (const auto& v : vertexs)
        {
            b2vertexs.push_back(global::ToMeters(v));
        }

        auto shape = std::make_unique<b2ChainShape>();
        if (loop)
        {
            shape->CreateLoop(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));
        }
        else
        {
            shape->CreateChain(&b2vertexs[0], static_cast<int32>(b2vertexs.size()));
        }

        ptr->shape_ = std::move(shape);
        ptr->param_ = param;
    }
    return ptr;
}

Fixture::Fixture()
    : fixture_(nullptr)
{
}

Fixture::~Fixture()
{
}

bool Fixture::Init(PhysicBodyPtr body)
{
    return this->Init(body.Get());
}

bool Fixture::Init(PhysicBody* body)
{
    KGE_ASSERT(fixture_ == nullptr);
    KGE_ASSERT(body);

    if (body)
    {
        b2Body* b2body = body->GetB2Body();
        if (b2body == nullptr)
        {
            // lazy init
            return true;
        }

        b2FixtureDef fd;
        fd.density     = param_.density;
        fd.friction    = param_.friction;
        fd.restitution = param_.restitution;
        fd.isSensor    = param_.is_sensor;
        fd.shape       = shape_.get();

        b2Fixture* fixture = b2body->CreateFixture(&fd);
        if (fixture)
        {
            this->SetB2Fixture(fixture);

            // Don't need shape any more, b2Fixture already has a clone
            shape_.reset();
            return true;
        }
    }
    return false;
}

PhysicBody* Fixture::GetBody() const
{
    KGE_ASSERT(fixture_);
    return static_cast<PhysicBody*>(fixture_->GetBody()->GetUserData());
}

void Fixture::GetMassData(float* mass, Point* center, float* inertia) const
{
    KGE_ASSERT(fixture_);

    b2MassData data;
    fixture_->GetMassData(&data);

    if (mass)
        *mass = data.mass;
    if (center)
        *center = global::ToPixels(data.center);
    if (inertia)
        *inertia = data.I;
}

bool Fixture::TestPoint(const Point& p) const
{
    KGE_ASSERT(fixture_);
    return fixture_->TestPoint(global::ToMeters(p));
}

void Fixture::Destroy()
{
    if (fixture_)
    {
        b2Body* body = fixture_->GetBody();
        if (body)
        {
            body->DestroyFixture(fixture_);
        }
        fixture_ = nullptr;
    }
}

}  // namespace physics
}  // namespace kiwano
