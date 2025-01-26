// Copyright (c) 2016-2018 Kiwano - Nomango
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

#include <kiwano/actor/Actor.h>
#include <kiwano/actor/Stage.h>
#include <kiwano/module/Director.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

Actor::Actor()
    : active_(true)
    , stage_(nullptr)
    , transform_ability_(nullptr)
    , ordered_abilities_()
{
    transform_ability_ = new TransformAbility;
    Attach(transform_ability_);
}

Actor::~Actor()
{
    for (auto iter = ordered_abilities_.rbegin(); iter != ordered_abilities_.rend();)
        Detach(*(iter++), true);

    transform_ability_ = nullptr;
    stage_             = nullptr;
}

void Actor::Update(Duration dt)
{
    if (!active_)
        return;

    if (children_.IsEmpty())
    {
        UpdateSelf(dt);
        return;
    }

    // update children those are less than 0 in Z-Order
    auto iter = children_.begin();
    while (iter != children_.end())
    {
        RefPtr<Actor> child = *iter;
        if (child->GetZOrder() >= 0)
            break;

        child->Update(dt);
        ++iter;
    }

    UpdateSelf(dt);

    while (iter != children_.end())
    {
        RefPtr<Actor> child = *(iter++);
        child->Update(dt);
    }
}

void Actor::UpdateSelf(Duration dt)
{
    if (cb_update_)
        cb_update_(dt);

    OnUpdate(dt);

    Animator::Update(this, dt);
    TaskScheduler::Update(dt);
    ComponentManager::Update(dt);

    if (!GetAllListeners().IsEmpty())
    {
        Director::GetInstance().PushEventDispatcher(this);
    }
}

void Actor::SetActive(bool active)
{
    active_ = active;
}

void Actor::SetTransform(const Transform& transform)
{
    transform_ability_->SetTransform(transform);
}

void Actor::FollowTransform(RefPtr<Actor> parent)
{
    transform_ability_->SetParent(parent->Get<TransformAbility>());
}

void Actor::Attach(RefPtr<Ability> ability)
{
    InsertAbilityByPriority(ability);
    ability->OnAttached(this);
    Director::GetInstance().PushToInitQueue(ability);
}

void Actor::Detach(RefPtr<Ability> ability, bool immediate)
{
    if (!immediate)
    {
        Director::GetInstance().PushToDestroyQueue(ability);
    }
    else
    {
        ability->OnDetached();
        ordered_abilities_.Remove(ability);
    }
}

void Actor::InsertAbilityByPriority(RefPtr<Ability> ability)
{
    ordered_abilities_.Remove(ability);

    RefPtr<Ability> sibling = ordered_abilities_.GetLast();
    if (sibling && sibling->GetPriority() < ability->GetPriority())
    {
        sibling = sibling->GetPrev();
        while (sibling)
        {
            if (sibling->GetPriority() >= ability->GetPriority())
                break;
            sibling = sibling->GetPrev();
        }
    }

    if (sibling)
    {
        ordered_abilities_.InsertAfter(ability, sibling);
    }
    else
    {
        ordered_abilities_.PushFront(ability);
    }
}

void Actor::EnterStage(Stage* stage)
{
    if (stage_ != stage)
    {
        stage_ = stage;
        for (auto& child : children_)
        {
            child->EnterStage(stage);
        }
    }
}

void Actor::OnSerialize(Serializer* serializer) const
{
    BaseObject::OnSerialize(serializer);
    (*serializer) << active_ << GetTransform();
}

void Actor::OnDeserialize(Deserializer* deserializer)
{
    BaseObject::OnDeserialize(deserializer);

    Transform transform;
    (*deserializer) >> active_ >> transform;

    SetTransform(transform);
}

}  // namespace kiwano
