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
#include <kiwano-physics/Contact.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
namespace physics
{

Contact::Contact()
    : contact_(nullptr)
{
}

Fixture* Contact::GetFixtureA() const
{
    KGE_ASSERT(contact_);

    Fixture* fixture = static_cast<Fixture*>(contact_->GetFixtureA()->GetUserData());
    return fixture;
}

Fixture* Contact::GetFixtureB() const
{
    KGE_ASSERT(contact_);

    Fixture* fixture = static_cast<Fixture*>(contact_->GetFixtureB()->GetUserData());
    return fixture;
}

Body* Contact::GetBodyA() const
{
    return GetFixtureA()->GetBody();
}

Body* Contact::GetBodyB() const
{
    return GetFixtureB()->GetBody();
}

void Contact::SetTangentSpeed(float speed)
{
    KGE_ASSERT(contact_);

    Body* body = GetFixtureA()->GetBody();
    KGE_ASSERT(body);

    World* world = body->GetWorld();
    KGE_ASSERT(world);

    contact_->SetTangentSpeed(world->Stage2World(speed));
}

float Contact::GetTangentSpeed() const
{
    KGE_ASSERT(contact_);

    const Body* body = GetFixtureA()->GetBody();
    KGE_ASSERT(body);

    const World* world = body->GetWorld();
    KGE_ASSERT(world);

    return world->World2Stage(contact_->GetTangentSpeed());
}

}  // namespace physics
}  // namespace kiwano
