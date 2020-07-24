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

#define KGE_PHYSIC_COMP_NAME "__KGE_PHYSIC_BODY__"

namespace kiwano
{
namespace physics
{

PhysicBody::PhysicBody(PhysicWorld* world, Type type)
    : body_(nullptr)
    , world_(world)
    , type_(type)
    , category_bits_(0x0001)
    , mask_bits_(0xFFFF)
    , group_index_(0)
{
    SetName(KGE_PHYSIC_COMP_NAME);

    if (Init(world))
    {
        world->AddBody(this);
    }
}

PhysicBody::PhysicBody(PhysicWorldPtr world, Type type)
    : PhysicBody(world.Get(), type)
{
}

PhysicBody::~PhysicBody() {}

void PhysicBody::InitComponent(Actor* actor)
{
    Component::InitComponent(actor);

    actor->SetPhysicBody(this);

    UpdateFromActor(actor);
}

void PhysicBody::DestroyComponent()
{
    GetBoundActor()->SetPhysicBody(nullptr);

    // Detach from actor first
    Component::DestroyComponent();

    // Destruction of fixtures may cause contact end
    RemoveAllFixtures();

    if (world_)
    {
        world_->RemoveBody(this);
    }
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
        return true;
    }
    return false;
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

void PhysicBody::BeforeSimulation(Actor* actor, const Matrix3x2& parent_to_world, const Matrix3x2& actor_to_world,
                                  float parent_rotation)
{
    UpdateFromActor(actor, actor_to_world, parent_rotation + actor->GetRotation());

    /*if (actor->GetAnchor() != Vec2(0.5f, 0.5f))
    {
        Point position = parent_to_world.Invert().Transform(position_cached_);

        offset_ = position - actor->GetPosition();
    }*/
}

void PhysicBody::AfterSimulation(Actor* actor, const Matrix3x2& parent_to_world, float parent_rotation)
{
    Point position_in_parent = GetPosition();
    if (position_cached_ != position_in_parent)
    {
        /*position_in_parent = parent_to_world.Invert().Transform(position_in_parent);
        actor->SetPosition(position_in_parent - offset_);*/

        position_in_parent = parent_to_world.Invert().Transform(position_in_parent);
        actor->SetPosition(position_in_parent);
    }
    actor->SetRotation(GetRotation() - parent_rotation);
}

void PhysicBody::UpdateFromActor(Actor* actor)
{
    KGE_ASSERT(actor);
    KGE_ASSERT(world_);

    Actor* world_actor = world_->GetBoundActor();
    if (world_actor)
    {
        float     rotation = 0.0f;
        Matrix3x2 transform_to_world;

        Actor* ptr = actor;
        while (ptr && ptr != world_actor)
        {
            rotation += ptr->GetRotation();
            transform_to_world *= ptr->GetTransformMatrixToParent();

            ptr = ptr->GetParent();
        }

        UpdateFromActor(actor, transform_to_world, rotation);
    }
}

void PhysicBody::UpdateFromActor(Actor* actor, const Matrix3x2& actor_to_world, float rotation)
{
    /*Point center   = actor->GetSize() / 2;
    Point position = actor_to_world.Transform(center);*/
    Point anchor   = actor->GetAnchor();
    Point size     = actor->GetSize();
    Point position = actor_to_world.Transform(Point(anchor.x * size.x, anchor.y * size.y));
    SetTransform(position, rotation);

    position_cached_ = GetPosition();
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

Fixture* PhysicBody::AddRectShape(const Vec2& size, float density, float friction)
{
    FixturePtr fixture = Fixture::CreateRect(Fixture::Param(density, friction), size);
    AddFixture(fixture);
    return fixture.Get();
}

Fixture* PhysicBody::AddPolygonShape(const Vector<Point>& vertexs, float density, float friction)
{
    FixturePtr fixture = Fixture::CreatePolygon(Fixture::Param(density, friction), vertexs);
    AddFixture(fixture);
    return fixture.Get();
}

Fixture* PhysicBody::AddEdgeShape(const Point& p1, const Point& p2, float density, float friction)
{
    FixturePtr fixture = Fixture::CreateEdge(Fixture::Param(density, friction), p1, p2);
    AddFixture(fixture);
    return fixture.Get();
}

Fixture* PhysicBody::AddChainShape(const Vector<Point>& vertices, bool loop, float density, float friction)
{
    FixturePtr fixture = Fixture::CreateChain(Fixture::Param(density, friction), vertices, loop);
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
        *center = global::WorldToLocal(data.center);
    if (inertia)
        *inertia = data.I;
}

void PhysicBody::SetMassData(float mass, const Point& center, float inertia)
{
    KGE_ASSERT(body_);

    b2MassData data;
    data.mass   = mass;
    data.center = global::LocalToWorld(center);
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
    return global::WorldToLocal(body_->GetPosition());
}

void PhysicBody::SetTransform(const Point& pos, float angle)
{
    KGE_ASSERT(body_);
    body_->SetTransform(global::LocalToWorld(pos), math::Degree2Radian(angle));
}

Point PhysicBody::GetLocalPoint(const Point& world) const
{
    KGE_ASSERT(body_);
    return global::WorldToLocal(body_->GetLocalPoint(global::LocalToWorld(world)));
}

Vec2 PhysicBody::GetLocalVector(const Vec2& world) const
{
    KGE_ASSERT(body_);
    return global::WorldToLocal(body_->GetLocalVector(global::LocalToWorld(world)));
}

Point PhysicBody::GetWorldPoint(const Point& local) const
{
    KGE_ASSERT(body_);
    return global::WorldToLocal(body_->GetWorldPoint(global::LocalToWorld(local)));
}

Vec2 PhysicBody::GetWorldVector(const Vec2& local) const
{
    KGE_ASSERT(body_);
    return global::WorldToLocal(body_->GetWorldVector(global::LocalToWorld(local)));
}

Point PhysicBody::GetLocalCenter() const
{
    KGE_ASSERT(body_);
    return global::WorldToLocal(body_->GetLocalCenter());
}

Point PhysicBody::GetWorldCenter() const
{
    KGE_ASSERT(body_);
    return global::WorldToLocal(body_->GetWorldCenter());
}

void PhysicBody::ApplyForce(const Vec2& force, const Point& point, bool wake)
{
    KGE_ASSERT(body_);
    body_->ApplyForce(b2Vec2(force.x, force.y), global::LocalToWorld(point), wake);
}

void PhysicBody::ApplyForceToCenter(const Vec2& force, bool wake)
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
