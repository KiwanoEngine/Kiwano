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

class PhysicWorld::DebugDrawer : public b2Draw
{
public:
    DebugDrawer(const Size& size)
    {
        canvas_ = MakePtr<Canvas>(size);
        ctx_    = canvas_->GetContext2D();

        fill_brush_ = MakePtr<Brush>(Color::White);
        line_brush_ = MakePtr<Brush>(Color::White);

        b2Draw::SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit);
    }

    void BeginDraw()
    {
        ctx_->BeginDraw();
        ctx_->Clear();
    }

    void EndDraw()
    {
        ctx_->EndDraw();
    }

    void Render(RenderContext& ctx)
    {
        canvas_->OnRender(ctx);
    }

    void SetFillColor(const b2Color& color)
    {
        fill_brush_->SetColor(reinterpret_cast<const Color&>(color));
        ctx_->SetCurrentBrush(fill_brush_);
    }

    void SetLineColor(const b2Color& color)
    {
        line_brush_->SetColor(reinterpret_cast<const Color&>(color));
        ctx_->SetCurrentBrush(line_brush_);
    }

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
    {
        SetLineColor(color);

        b2Vec2 p1 = vertices[vertexCount - 1];
        for (int32 i = 0; i < vertexCount; ++i)
        {
            b2Vec2 p2 = vertices[i];
            ctx_->DrawLine(global::WorldToLocal(p1), global::WorldToLocal(p2));
            p1 = p2;
        }
    }

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
    {
        ShapeMaker maker;
        maker.BeginPath(global::WorldToLocal(vertices[0]));
        for (int32 i = 1; i < vertexCount; ++i)
        {
            maker.AddLine(global::WorldToLocal(vertices[i]));
        }
        maker.EndPath(true);

        SetFillColor(color);
        ctx_->FillShape(*maker.GetShape());
    }

    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override
    {
        SetLineColor(color);
        ctx_->DrawCircle(global::WorldToLocal(center), global::WorldToLocal(radius));
    }

    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override
    {
        SetFillColor(color);
        ctx_->FillCircle(global::WorldToLocal(center), global::WorldToLocal(radius));
    }

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
    {
        SetLineColor(color);
        ctx_->DrawLine(global::WorldToLocal(p1), global::WorldToLocal(p2));
    }

    void DrawTransform(const b2Transform& xf) override
    {
        const float k_axisScale = 0.4f;

        b2Color red(1.0f, 0.0f, 0.0f);
        b2Color green(0.0f, 1.0f, 0.0f);
        b2Vec2  p1 = xf.p, p2;

        p2 = p1 + k_axisScale * xf.q.GetXAxis();

        SetLineColor(red);
        ctx_->DrawLine(global::WorldToLocal(p1), global::WorldToLocal(p2));

        p2 = p1 + k_axisScale * xf.q.GetYAxis();

        SetLineColor(green);
        ctx_->DrawLine(global::WorldToLocal(p1), global::WorldToLocal(p2));
    }

    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override
    {
        SetFillColor(color);
        ctx_->FillCircle(global::WorldToLocal(p), global::WorldToLocal(size));
    }

private:
    CanvasPtr        canvas_;
    RenderContextPtr ctx_;
    BrushPtr         fill_brush_;
    BrushPtr         line_brush_;
};

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

PhysicWorld::PhysicWorld(const Vec2& gravity)
    : PhysicWorld()
{
    SetGravity(gravity);
}

PhysicWorld::PhysicWorld()
    : world_(b2Vec2(0, 10.0f))
    , vel_iter_(6)
    , pos_iter_(2)
{
    SetName("KGE_PHYSIC_WORLD");

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

void PhysicWorld::InitComponent(Actor* actor)
{
    Component::InitComponent(actor);

    // Update body status
    Actor* world_actor = GetBoundActor();
    BeforeSimulation(world_actor, Matrix3x2(), 0.0f);
}

void PhysicWorld::OnUpdate(Duration dt)
{
    Actor* world_actor = GetBoundActor();

    BeforeSimulation(world_actor, Matrix3x2(), 0.0f);

    // Update physic world
    world_.Step(dt.Seconds(), vel_iter_, pos_iter_);

    AfterSimulation(world_actor, Matrix3x2(), 0.0f);
}

void PhysicWorld::OnRender(RenderContext& ctx)
{
    if (drawer_)
    {
        drawer_->BeginDraw();
        world_.DrawDebugData();
        drawer_->EndDraw();

        drawer_->Render(ctx);
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

void PhysicWorld::BeforeSimulation(Actor* parent, const Matrix3x2& parent_to_world, float parent_rotation)
{
    for (auto child : parent->GetAllChildren())
    {
        Matrix3x2 child_to_world = child->GetTransformMatrixToParent() * parent_to_world;

        PhysicBody* body = child->GetPhysicBody();
        if (body)
        {
            body->BeforeSimulation(child.Get(), parent_to_world, child_to_world, parent_rotation);
        }

        float rotation = parent_rotation + child->GetRotation();
        BeforeSimulation(child.Get(), child_to_world, rotation);
    }
}

void PhysicWorld::AfterSimulation(Actor* parent, const Matrix3x2& parent_to_world, float parent_rotation)
{
    for (auto child : parent->GetAllChildren())
    {
        PhysicBody* body = child->GetPhysicBody();
        if (body)
        {
            body->AfterSimulation(child.Get(), parent_to_world, parent_rotation);
        }

        Matrix3x2 child_to_world = child->GetTransformMatrixToParent() * parent_to_world;
        float     rotation       = parent_rotation + child->GetRotation();
        AfterSimulation(child.Get(), child_to_world, rotation);
    }
}

void PhysicWorld::ShowDebugInfo(bool show)
{
    if (show)
    {
        if (!drawer_)
        {
            Size size = Renderer::GetInstance().GetOutputSize();
            drawer_   = std::unique_ptr<DebugDrawer>(new DebugDrawer(size));

            world_.SetDebugDraw(drawer_.get());
        }
    }
    else
    {
        drawer_.reset();
    }
}

}  // namespace physics
}  // namespace kiwano
