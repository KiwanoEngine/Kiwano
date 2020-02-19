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

#include <kiwano-physics/PhysicWorld.h>
#include <kiwano-physics/ContactEvent.h>

namespace kiwano
{
namespace physics
{

class DestructionListener : public b2DestructionListener
{
    Function<void(b2Joint*)> joint_destruction_callback_;

public:
    DestructionListener(Function<void(b2Joint*)> callback)
        : joint_destruction_callback_(callback)
    {
    }

    void SayGoodbye(b2Joint* joint) override
    {
        joint_destruction_callback_(joint);
    }

    void SayGoodbye(b2Fixture* fixture) override {}
};

class ContactListener : public b2ContactListener
{
    Function<void(Event*)> dispatcher_;

public:
    ContactListener(Function<void(Event*)> dispatcher)
        : dispatcher_(dispatcher)
    {
    }

    void BeginContact(b2Contact* b2contact) override
    {
        Contact contact;
        contact.SetB2Contact(b2contact);

        ContactBeginEventPtr evt = new ContactBeginEvent(contact);
        dispatcher_(evt.Get());
    }

    void EndContact(b2Contact* b2contact) override
    {
        PhysicBody* body_a = static_cast<PhysicBody*>(b2contact->GetFixtureA()->GetBody()->GetUserData());
        PhysicBody* body_b = static_cast<PhysicBody*>(b2contact->GetFixtureB()->GetBody()->GetUserData());
        if (!body_a || !body_b || !body_a->GetBoundActor() || !body_b->GetBoundActor())
        {
            // Don't dispatch contact event after the body has been detached
            return;
        }

        Contact contact;
        contact.SetB2Contact(b2contact);

        ContactEndEventPtr evt = new ContactEndEvent(contact);
        dispatcher_(evt.Get());
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

PhysicWorldPtr PhysicWorld::Create()
{
    PhysicWorldPtr ptr = new (std::nothrow) PhysicWorld;
    return ptr;
}

PhysicWorldPtr PhysicWorld::Create(const Vec2& gravity)
{
    PhysicWorldPtr ptr = new (std::nothrow) PhysicWorld;
    if (ptr)
    {
        ptr->SetGravity(gravity);
    }
    return ptr;
}

PhysicWorld::PhysicWorld()
    : world_(b2Vec2(0, 10.0f))
    , vel_iter_(6)
    , pos_iter_(2)
{
    destroy_listener_ = std::make_unique<DestructionListener>(Closure(this, &PhysicWorld::JointRemoved));
    world_.SetDestructionListener(destroy_listener_.get());

    contact_listener_ = std::make_unique<ContactListener>(Closure(this, &PhysicWorld::DispatchEvent));
    world_.SetContactListener(contact_listener_.get());
}

PhysicWorld::~PhysicWorld()
{
    world_.SetDestructionListener(nullptr);
    world_.SetContactListener(nullptr);

    // Make sure b2World was destroyed after b2Body
    RemoveAllJoints();
    RemoveAllBodies();
}

void PhysicWorld::AddBody(PhysicBodyPtr body)
{
    if (body)
    {
        bool success = body->Init(this);
        KGE_ASSERT(success);

        bodies_.push_back(body);
    }
}

void PhysicWorld::RemoveBody(PhysicBodyPtr body)
{
    if (body)
    {
        auto iter = std::find(bodies_.begin(), bodies_.end(), body);
        if (iter != bodies_.end())
        {
            body->Destroy();
            bodies_.erase(iter);
        }
    }
}

void PhysicWorld::RemoveAllBodies()
{
    for (auto body : bodies_)
    {
        body->Destroy();
    }
    bodies_.clear();
}

const List<PhysicBodyPtr>& PhysicWorld::GetAllBodies() const
{
    return bodies_;
}

void PhysicWorld::AddJoint(JointPtr joint)
{
    if (joint)
    {
        bool success = joint->Init(this);
        KGE_ASSERT(success);

        joints_.push_back(joint);
    }
}

void PhysicWorld::RemoveJoint(JointPtr joint)
{
    if (joint)
    {
        auto iter = std::find(joints_.begin(), joints_.end(), joint);
        if (iter != joints_.end())
        {
            joint->Destroy();
            joints_.erase(iter);
        }
    }
}

void PhysicWorld::RemoveAllJoints()
{
    for (auto joint : joints_)
    {
        joint->Destroy();
    }
    joints_.clear();
}

const List<JointPtr>& PhysicWorld::GetAllJoints() const
{
    return joints_;
}

b2World* PhysicWorld::GetB2World()
{
    return &world_;
}

const b2World* PhysicWorld::GetB2World() const
{
    return &world_;
}

Vec2 PhysicWorld::GetGravity() const
{
    b2Vec2 g = world_.GetGravity();
    return Vec2(g.x, g.y);
}

void PhysicWorld::SetGravity(Vec2 gravity)
{
    world_.SetGravity(b2Vec2(gravity.x, gravity.y));
}

ContactList PhysicWorld::GetContactList()
{
    Contact contact;
    contact.SetB2Contact(world_.GetContactList());
    return ContactList(contact);
}

void PhysicWorld::OnUpdate(Duration dt)
{
    // Update bodies transform from actors
    for (auto& body : bodies_)
    {
        body->UpdateFromActor();
    }

    world_.Step(dt.Seconds(), vel_iter_, pos_iter_);

    // Update actors transform from bodies
    for (auto& body : bodies_)
    {
        if (body->GetType() != PhysicBody::Type::Static)
            body->UpdateActor();
    }
}

void PhysicWorld::DispatchEvent(Event* evt)
{
    Actor* actor = GetBoundActor();
    if (actor)
    {
        actor->DispatchEvent(evt);
    }
}

void PhysicWorld::JointRemoved(b2Joint* b2joint)
{
    Joint* joint = static_cast<Joint*>(b2joint->GetUserData());
    if (joint)
    {
        auto iter = std::find(joints_.begin(), joints_.end(), joint);
        if (iter != joints_.end())
        {
            joints_.erase(iter);
        }
    }
}

}  // namespace physics
}  // namespace kiwano
