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

#include "World.h"

#include <kiwano-physics/ContactEvent.h>

namespace kiwano
{
namespace physics
{
namespace
{
const float default_global_scale = 100.f;  // 100 pixels per meters
}

class World::DestructionListener : public b2DestructionListener
{
    World* world_;

public:
    DestructionListener(World* world)
        : world_(world)
    {
    }

    void SayGoodbye(b2Joint* joint) override
    {
        if (world_)
        {
            world_->JointRemoved(joint);
        }
    }

    void SayGoodbye(b2Fixture* fixture) override {}
};

class World::ContactListener : public b2ContactListener
{
    World* world_;

public:
    ContactListener(World* world)
        : world_(world)
    {
    }

    void BeginContact(b2Contact* contact) override
    {
        ContactBeginEventPtr evt = new ContactBeginEvent(contact);
        world_->DispatchEvent(evt.get());
    }

    void EndContact(b2Contact* contact) override
    {
        ContactEndEventPtr evt = new ContactEndEvent(contact);
        world_->DispatchEvent(evt.get());
    }

    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
    {
        KGE_NOT_USED(contact);
        KGE_NOT_USED(oldManifold);
    }
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
    {
        KGE_NOT_USED(contact);
        KGE_NOT_USED(impulse);
    }
};

World::World()
    : world_(b2Vec2(0, 10.0f))
    , vel_iter_(6)
    , pos_iter_(2)
    , global_scale_(default_global_scale)
    , destruction_listener_(nullptr)
    , contact_listener_(nullptr)
    , removing_joint_(false)
{
    destruction_listener_ = new DestructionListener(this);
    world_.SetDestructionListener(destruction_listener_);

    contact_listener_ = new ContactListener(this);
    world_.SetContactListener(contact_listener_);
}

World::~World()
{
    world_.SetDestructionListener(nullptr);
    if (destruction_listener_)
    {
        delete destruction_listener_;
        destruction_listener_ = nullptr;
    }

    world_.SetContactListener(nullptr);
    if (contact_listener_)
    {
        delete contact_listener_;
        contact_listener_ = nullptr;
    }

    // Make sure b2World was destroyed after b2Body
    RemoveAllChildren();
    RemoveAllBodies();
    RemoveAllJoints();
}

void World::RemoveBody(Body* body)
{
    if (body && body->GetB2Body())
    {
        world_.DestroyBody(body->GetB2Body());
    }
}

void World::RemoveAllBodies()
{
    if (world_.GetBodyCount())
    {
        b2Body* body = world_.GetBodyList();
        while (body)
        {
            b2Body* next = body->GetNext();
            world_.DestroyBody(body);
            body = next;
        }
    }
}

void World::AddJoint(Joint* joint)
{
    if (joint)
    {
        joints_.push_back(joint);
    }
}

void World::RemoveJoint(Joint* joint)
{
    if (joint)
    {
        auto iter = std::find(joints_.begin(), joints_.end(), joint);
        if (iter != joints_.end())
        {
            joints_.erase(iter);

            if (joint->GetB2Joint())
            {
                removing_joint_ = true;
                world_.DestroyJoint(joint->GetB2Joint());
                removing_joint_ = false;
            }
        }
    }
}

void World::RemoveAllJoints()
{
    if (world_.GetJointCount())
    {
        removing_joint_ = true;
        {
            b2Joint* joint = world_.GetJointList();
            while (joint)
            {
                b2Joint* next = joint->GetNext();
                world_.DestroyJoint(joint);
                joint = next;
            }
        }
        removing_joint_ = false;
    }
    joints_.clear();
}

void World::JointRemoved(b2Joint* joint)
{
    if (!removing_joint_ && joint)
    {
        auto iter = std::find_if(joints_.begin(), joints_.end(),
                                 [joint](Joint* j) -> bool { return j->GetB2Joint() == joint; });

        if (iter != joints_.end())
        {
            joints_.erase(iter);
        }
    }
}

b2World* World::GetB2World()
{
    return &world_;
}

const b2World* World::GetB2World() const
{
    return &world_;
}

Vec2 World::GetGravity() const
{
    b2Vec2 g = world_.GetGravity();
    return Vec2(g.x, g.y);
}

void World::SetGravity(Vec2 gravity)
{
    world_.SetGravity(b2Vec2(gravity.x, gravity.y));
}

ContactList World::GetContactList()
{
    return ContactList(Contact(world_.GetContactList()));
}

void World::Update(Duration dt)
{
    world_.Step(dt.Seconds(), vel_iter_, pos_iter_);

    b2Body* b2body = world_.GetBodyList();
    while (b2body)
    {
        Body* body = static_cast<Body*>(b2body->GetUserData());
        if (body && body->GetType() != Body::Type::Static)
        {
            body->UpdateActor();
        }

        b2body = b2body->GetNext();
    }

    Stage::Update(dt);
}

}  // namespace physics
}  // namespace kiwano
