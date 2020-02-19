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
#include <kiwano-physics/PhysicWorld.h>

namespace kiwano
{
namespace physics
{

PhysicBodyPtr PhysicBody::Create(ActorPtr actor, Type type)
{
    return PhysicBody::Create(actor.Get(), type);
}

PhysicBodyPtr PhysicBody::Create(Actor* actor, Type type)
{
    if (!actor)
    {
        return nullptr;
    }

    PhysicBodyPtr ptr = new (std::nothrow) PhysicBody;
    if (ptr)
    {
        ptr->SetType(type);
        actor->AddComponent(ptr);
    }
    return ptr;
}

PhysicBody::PhysicBody()
    : body_(nullptr)
    , world_(nullptr)
    , type_(Type::Static)
    , category_bits_(0x0001)
    , mask_bits_(0xFFFF)
    , group_index_(0)
{
}

PhysicBody::~PhysicBody() {}

void PhysicBody::InitComponent(Actor* actor)
{
    Component::InitComponent(actor);
    UpdateFromActor();
}

void PhysicBody::DestroyComponent()
{
    // Detach from actor first
    Component::DestroyComponent();

    // Destruction of fixtures may cause contact end
    RemoveAllFixtures();

    if (world_)
    {
        world_->RemoveBody(this);
    }
}

bool PhysicBody::Init(PhysicWorldPtr world)
{
    return Init(world.Get());
}

bool PhysicBody::Init(PhysicWorld* world)
{
    KGE_ASSERT(body_ == nullptr);
    KGE_ASSERT(world);

    world_ = world;

    b2BodyDef def;
    def.type = b2BodyType(type_);

    b2Body* b2body = world->GetB2World()->CreateBody(&def);
    if (b2body)
    {
        SetB2Body(b2body);
        UpdateFromActor();

        // lazy init fixtures
        for (auto fixture : fixtures_)
        {
            bool success = fixture->Init(this);
            KGE_ASSERT(success);
        }
        return true;
    }
    return false;
}

void PhysicBody::AddFixture(FixturePtr fixture)
{
    if (fixture)
    {
        bool success = fixture->Init(this);
        KGE_ASSERT(success);

        fixtures_.push_back(fixture);
    }
}

Fixture* PhysicBody::AddCircleShape(float radius, float density, float friction)
{
    FixturePtr fixture = Fixture::CreateCircle(Fixture::Param(density, friction), radius);
    AddFixture(fixture);
    return fixture.Get();
}

Fixture* PhysicBody::AddRectShape(Vec2 const& size, float density, float friction)
{
    FixturePtr fixture = Fixture::CreateRect(Fixture::Param(density, friction), size);
    AddFixture(fixture);
    return fixture.Get();
}

Fixture* PhysicBody::AddPolygonShape(Vector<Point> const& vertexs, float density, float friction)
{
    FixturePtr fixture = Fixture::CreatePolygon(Fixture::Param(density, friction), vertexs);
    AddFixture(fixture);
    return fixture.Get();
}

Fixture* PhysicBody::AddEdgeShape(Point const& p1, Point const& p2, float density, float friction)
{
    FixturePtr fixture = Fixture::CreateEdge(Fixture::Param(density, friction), p1, p2);
    AddFixture(fixture);
    return fixture.Get();
}

Fixture* PhysicBody::AddChainShape(Vector<Point> const& vertexs, bool loop, float density, float friction)
{
    FixturePtr fixture = Fixture::CreateChain(Fixture::Param(density, friction), vertexs, loop);
    AddFixture(fixture);
    return fixture.Get();
}

void PhysicBody::RemoveFixture(FixturePtr fixture)
{
    if (fixture)
    {
        auto iter = std::find(fixtures_.begin(), fixtures_.end(), fixture);
        if (iter != fixtures_.end())
        {
            fixture->Destroy();
            fixtures_.erase(iter);
        }
    }
}

void PhysicBody::RemoveAllFixtures()
{
    for (auto fixture : fixtures_)
    {
        fixture->Destroy();
    }
    fixtures_.clear();
}

void PhysicBody::SetCategoryBits(uint16_t category_bits)
{
    KGE_ASSERT(body_);

    if (category_bits != category_bits_)
    {
        category_bits_ = category_bits;

        b2Fixture* fixture = body_->GetFixtureList();
        while (fixture)
        {
            UpdateFixtureFilter(fixture);
            fixture = fixture->GetNext();
        }
    }
}

void PhysicBody::SetMaskBits(uint16_t mask_bits)
{
    KGE_ASSERT(body_);

    if (mask_bits != mask_bits_)
    {
        mask_bits_ = mask_bits;

        b2Fixture* fixture = body_->GetFixtureList();
        while (fixture)
        {
            UpdateFixtureFilter(fixture);
            fixture = fixture->GetNext();
        }
    }
}

void PhysicBody::SetGroupIndex(int16_t index)
{
    KGE_ASSERT(body_);

    if (index != group_index_)
    {
        group_index_ = index;

        b2Fixture* fixture = body_->GetFixtureList();
        while (fixture)
        {
            UpdateFixtureFilter(fixture);
            fixture = fixture->GetNext();
        }
    }
}

void PhysicBody::GetMassData(float* mass, Point* center, float* inertia) const
{
    KGE_ASSERT(body_);

    b2MassData data;
    body_->GetMassData(&data);

    if (mass)
        *mass = data.mass;
    if (center)
        *center = global::ToPixels(data.center);
    if (inertia)
        *inertia = data.I;
}

void PhysicBody::SetMassData(float mass, Point const& center, float inertia)
{
    KGE_ASSERT(body_);

    b2MassData data;
    data.mass   = mass;
    data.center = global::ToMeters(center);
    data.I      = inertia;
    body_->SetMassData(&data);
}

void PhysicBody::ResetMassData()
{
    KGE_ASSERT(body_);
    body_->ResetMassData();
}

Point PhysicBody::GetPosition() const
{
    KGE_ASSERT(body_);
    return global::ToPixels(body_->GetPosition());
}

void PhysicBody::SetTransform(Point const& pos, float angle)
{
    KGE_ASSERT(body_);
    body_->SetTransform(global::ToMeters(pos), math::Degree2Radian(angle));
}

Point PhysicBody::GetLocalPoint(Point const& world) const
{
    KGE_ASSERT(body_);
    return global::ToPixels(body_->GetLocalPoint(global::ToMeters(world)));
}

Point PhysicBody::GetWorldPoint(Point const& local) const
{
    KGE_ASSERT(body_);
    return global::ToPixels(body_->GetWorldPoint(global::ToMeters(local)));
}

Point PhysicBody::GetLocalCenter() const
{
    KGE_ASSERT(body_);
    return global::ToPixels(body_->GetLocalCenter());
}

Point PhysicBody::GetWorldCenter() const
{
    KGE_ASSERT(body_);
    return global::ToPixels(body_->GetWorldCenter());
}

void PhysicBody::ApplyForce(Vec2 const& force, Point const& point, bool wake)
{
    KGE_ASSERT(body_);
    body_->ApplyForce(b2Vec2(force.x, force.y), global::ToMeters(point), wake);
}

void PhysicBody::ApplyForceToCenter(Vec2 const& force, bool wake)
{
    KGE_ASSERT(body_);
    body_->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
}

void PhysicBody::ApplyTorque(float torque, bool wake)
{
    KGE_ASSERT(body_);
    body_->ApplyTorque(torque, wake);
}

void PhysicBody::SetB2Body(b2Body* body)
{
    body_ = body;
    if (body_)
    {
        body_->SetUserData(this);
        type_ = PhysicBody::Type(body_->GetType());
    }
}

void PhysicBody::Destroy()
{
    RemoveAllFixtures();

    if (body_ && world_)
    {
        b2World* b2world = world_->GetB2World();
        b2world->DestroyBody(body_);
    }

    body_  = nullptr;
    world_ = nullptr;

    Component::RemoveFromActor();
}

void PhysicBody::UpdateActor()
{
    Actor* actor = GetBoundActor();
    if (actor && body_)
    {
        Point position = global::ToPixels(body_->GetPosition());
        float rotation = math::Radian2Degree(body_->GetAngle());
        actor->SetPosition(position);
        actor->SetRotation(rotation);
    }
}

void PhysicBody::UpdateFromActor()
{
    Actor* actor = GetBoundActor();
    if (actor && body_)
    {
        b2Vec2 position = global::ToMeters(actor->GetPosition());
        float  angle    = math::Degree2Radian(actor->GetRotation());
        body_->SetTransform(position, angle);
    }
}

void PhysicBody::UpdateFixtureFilter(b2Fixture* fixture)
{
    b2Filter filter;
    filter.categoryBits = category_bits_;
    filter.maskBits     = mask_bits_;
    filter.groupIndex   = group_index_;
    fixture->SetFilterData(filter);
}

}  // namespace physics
}  // namespace kiwano
