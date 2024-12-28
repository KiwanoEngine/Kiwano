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

#include <kiwano-physics/World.h>

namespace kiwano
{
namespace physics
{

const float FIXED_TIMESTEP = 1.f / 60.f;

class World::DebugDrawer : public b2Draw
{
public:
    DebugDrawer(const Size& size)
    {
        canvas_ = MakePtr<Canvas>(PixelSize(uint32_t(size.x), uint32_t(size.y)));
        ctx_    = canvas_->GetContext2D();

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
        // TODO
        //canvas_->OnRender(ctx);
    }

    void SetFillColor(const b2Color& color)
    {
        ctx_->SetFillColor(reinterpret_cast<const Color&>(color));
    }

    void SetLineColor(const b2Color& color)
    {
        ctx_->SetStrokeColor(reinterpret_cast<const Color&>(color));
    }

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
    {
        SetLineColor(color);

        b2Vec2 p1 = vertices[vertexCount - 1];
        for (int32 i = 0; i < vertexCount; ++i)
        {
            b2Vec2 p2 = vertices[i];
            ctx_->DrawLine(WorldToLocal(p1), WorldToLocal(p2));
            p1 = p2;
        }
    }

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
    {
        Vector<Point> local_vertices;
        for (int32 i = 0; i < vertexCount; ++i)
        {
            local_vertices.push_back(WorldToLocal(vertices[i]));
        }
        RefPtr<Shape> polygon = Shape::CreatePolygon(local_vertices);

        SetFillColor(color);
        ctx_->FillShape(polygon);
    }

    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override
    {
        SetLineColor(color);
        ctx_->DrawCircle(WorldToLocal(center), WorldToLocal(radius));
    }

    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override
    {
        SetFillColor(color);
        ctx_->FillCircle(WorldToLocal(center), WorldToLocal(radius));
    }

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
    {
        SetLineColor(color);
        ctx_->DrawLine(WorldToLocal(p1), WorldToLocal(p2));
    }

    void DrawTransform(const b2Transform& xf) override
    {
        const float k_axisScale = 0.4f;

        b2Color red(1.0f, 0.0f, 0.0f);
        b2Color green(0.0f, 1.0f, 0.0f);
        b2Vec2  p1 = xf.p, p2;

        p2 = p1 + k_axisScale * xf.q.GetXAxis();

        SetLineColor(red);
        ctx_->DrawLine(WorldToLocal(p1), WorldToLocal(p2));

        p2 = p1 + k_axisScale * xf.q.GetYAxis();

        SetLineColor(green);
        ctx_->DrawLine(WorldToLocal(p1), WorldToLocal(p2));
    }

    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override
    {
        SetFillColor(color);
        ctx_->FillCircle(WorldToLocal(p), WorldToLocal(size));
    }

private:
    RefPtr<Canvas>              canvas_;
    RefPtr<CanvasRenderContext> ctx_;
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
        RefPtr<ContactBeginEvent> evt = new ContactBeginEvent(b2contact);
        dispatcher_(evt.Get());
    }

    void EndContact(b2Contact* b2contact) override
    {
        Body* body_a = static_cast<Body*>(b2contact->GetFixtureA()->GetBody()->GetUserData());
        Body* body_b = static_cast<Body*>(b2contact->GetFixtureB()->GetBody()->GetUserData());
        if (!body_a || !body_b || !body_a->GetBoundActor() || !body_b->GetBoundActor())
        {
            // Don't dispatch contact event after the body has been detached
            return;
        }

        RefPtr<ContactEndEvent> evt = new ContactEndEvent(b2contact);
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

World::World(const b2Vec2& gravity)
    : world_(gravity)
    , vel_iter_(6)
    , pos_iter_(2)
    , fixed_acc_(0.f)
{
    SetName(KGE_COMP_PHYSIC_WORLD);

    contact_listener_ = std::make_unique<ContactListener>(Closure(this, &World::DispatchEvent));
    world_.SetContactListener(contact_listener_.get());
}

World::~World()
{
    world_.SetContactListener(nullptr);
}

RefPtr<Body> World::AddBody(b2BodyDef* def)
{
    b2Body* body = world_.CreateBody(def);
    return MakePtr<Body>(body, &world_);
}

b2Joint* World::AddJoint(b2JointDef* def)
{
    return world_.CreateJoint(def);
}

b2World* World::GetB2World()
{
    return &world_;
}

const b2World* World::GetB2World() const
{
    return &world_;
}

ContactList World::GetContactList()
{
    return ContactList(world_.GetContactList());
}

void World::InitComponent(Actor* actor)
{
    Component::InitComponent(actor);

    // Update body status
    Actor* world_actor = GetBoundActor();
    BeforeSimulation(world_actor, Matrix3x2(), 0.0f);
}

void World::OnUpdate(Duration dt)
{
    Actor* world_actor = GetBoundActor();

    BeforeSimulation(world_actor, Matrix3x2(), 0.0f);

    // Update physic world
    // The implementation referenced this article. https://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
    const int MAX_STEPS = 5;

    fixed_acc_ += dt.GetSeconds();
    const int steps = static_cast<int>(std::floor(fixed_acc_ / FIXED_TIMESTEP));
    if (steps > 0)
    {
        fixed_acc_ -= steps * FIXED_TIMESTEP;
    }

    const int steps_clamped = std::min(steps, MAX_STEPS);
    for (int i = 0; i < steps_clamped; ++i)
    {
        world_.Step(FIXED_TIMESTEP, vel_iter_, pos_iter_);
    }

    AfterSimulation(world_actor, Matrix3x2(), 0.0f);
}

bool World::CheckVisibility(RenderContext& ctx)
{
    return !!drawer_;
}

void World::OnRender(RenderContext& ctx)
{
    RenderComponent::OnRender(ctx);
    if (drawer_)
    {
        drawer_->BeginDraw();
        world_.DrawDebugData();
        drawer_->EndDraw();

        drawer_->Render(ctx);
    }
}

void World::DispatchEvent(Event* evt)
{
    Actor* actor = GetBoundActor();
    if (actor)
    {
        actor->DispatchEvent(evt);
    }
}

void World::BeforeSimulation(Actor* parent, const Matrix3x2& parent_to_world, float parent_rotation)
{
    for (auto child : parent->GetAllChildren())
    {
        Matrix3x2 child_to_world = child->GetTransformMatrixToParent() * parent_to_world;

        auto body = dynamic_cast<Body*>(child->GetComponent(KGE_COMP_PHYSIC_BODY));
        if (body)
        {
            body->BeforeSimulation(child.Get(), parent_to_world, child_to_world, parent_rotation);
        }

        float rotation = parent_rotation + child->GetRotation();
        BeforeSimulation(child.Get(), child_to_world, rotation);
    }
}

void World::AfterSimulation(Actor* parent, const Matrix3x2& parent_to_world, float parent_rotation)
{
    for (auto child : parent->GetAllChildren())
    {
        auto body = dynamic_cast<Body*>(child->GetComponent(KGE_COMP_PHYSIC_BODY));
        if (body)
        {
            body->AfterSimulation(child.Get(), parent_to_world, parent_rotation);
        }

        Matrix3x2 child_to_world = child->GetTransformMatrixToParent() * parent_to_world;
        float     rotation       = parent_rotation + child->GetRotation();
        AfterSimulation(child.Get(), child_to_world, rotation);
    }
}

void World::ShowDebugInfo(bool show)
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
