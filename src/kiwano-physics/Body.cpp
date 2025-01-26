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
#include <kiwano-physics/World.h>

namespace kiwano
{
namespace physics
{

Body::Body(b2Body* body, b2World* world)
    : b2body_(body)
    , b2world_(world)
{
    SetName(KGE_COMP_PHYSIC_BODY);

    body->SetUserData(this);
}

Body::~Body() {}

void Body::OnAttached(Actor* actor)
{
    Ability::OnAttached(actor);

    UpdateFromActor(actor);
}

void Body::OnDetached()
{
    // Detach from actor first
    Ability::OnDetached();

    if (b2body_ && b2world_)
    {
        b2world_->DestroyBody(b2body_);
    }
}

void Body::BeforeSimulation(Actor* actor, const Matrix3x2& parent_to_world, const Matrix3x2& actor_to_world,
                                  float parent_rotation)
{
    UpdateFromActor(actor, actor_to_world, parent_rotation + actor->GetRotation());

    /*if (actor->GetAnchor() != Vec2(0.5f, 0.5f))
    {
        Point position = parent_to_world.Invert().Transform(position_cached_);

        offset_ = position - actor->GetPosition();
    }*/
}

void Body::AfterSimulation(Actor* actor, const Matrix3x2& parent_to_world, float parent_rotation)
{
    Point position_in_parent = WorldToLocal(b2body_->GetPosition());
    if (position_cached_ != position_in_parent)
    {
        /*position_in_parent = parent_to_world.Invert().Transform(position_in_parent);
        actor->SetPosition(position_in_parent - offset_);*/

        position_in_parent = parent_to_world.Invert().Transform(position_in_parent);
        actor->SetPosition(position_in_parent);
    }
    actor->SetRotation(math::Radian2Degree(b2body_->GetAngle()) -parent_rotation);
}

void Body::UpdateFromActor(Actor* actor)
{
    KGE_ASSERT(b2body_);

    float     rotation = 0.0f;
    Matrix3x2 transform_to_world;

    Actor* ptr = actor;
    while (ptr)
    {
        auto world = dynamic_cast<World*>(ptr->GetComponent(KGE_COMP_PHYSIC_WORLD));
        if (world && world->GetB2World() == b2body_->GetWorld())
        {
            break;
        }
        rotation += ptr->GetRotation();
        transform_to_world *= ptr->GetTransformMatrixToParent();

        ptr = ptr->GetParent();
    }

    UpdateFromActor(actor, transform_to_world, rotation);
}

void Body::UpdateFromActor(Actor* actor, const Matrix3x2& actor_to_world, float rotation)
{
    /*Point center   = actor->GetSize() / 2;
    Point position = actor_to_world.Transform(center);*/
    Point anchor   = actor->GetAnchor();
    Point size     = actor->GetSize();
    Point position = actor_to_world.Transform(Point(anchor.x * size.x, anchor.y * size.y));
    b2body_->SetTransform(LocalToWorld(position), math::Degree2Radian(rotation));

    position_cached_ = WorldToLocal(b2body_->GetPosition());
}

Point Body::GetLocalPoint(const Point& world) const
{
    KGE_ASSERT(b2body_);
    return WorldToLocal(b2body_->GetLocalPoint(LocalToWorld(world)));
}

Vec2 Body::GetLocalVector(const Vec2& world) const
{
    KGE_ASSERT(b2body_);
    return WorldToLocal(b2body_->GetLocalVector(LocalToWorld(world)));
}

Point Body::GetWorldPoint(const Point& local) const
{
    KGE_ASSERT(b2body_);
    return WorldToLocal(b2body_->GetWorldPoint(LocalToWorld(local)));
}

Vec2 Body::GetWorldVector(const Vec2& local) const
{
    KGE_ASSERT(b2body_);
    return WorldToLocal(b2body_->GetWorldVector(LocalToWorld(local)));
}

}  // namespace physics
}  // namespace kiwano
