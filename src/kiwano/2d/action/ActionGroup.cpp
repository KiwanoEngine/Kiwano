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

#include <kiwano/2d/Actor.h>
#include <kiwano/2d/action/ActionGroup.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

ActionGroup::ActionGroup(const Vector<ActionEntityPtr>& actions, bool parallel)
{
    SetEntity(ActionGroupEntity::Create(actions, parallel));
}

ActionGroupEntityPtr ActionGroupEntity::Create(const Vector<ActionEntityPtr>& actions, bool parallel)
{
    ActionGroupEntityPtr ptr = new (autogc) ActionGroupEntity;
    if (ptr)
    {
        ptr->parallel_ = parallel;
        ptr->AddActions(actions);
    }
    return ptr;
}

ActionGroupEntity::ActionGroupEntity()
    : parallel_(false)
{
}

ActionGroupEntity::ActionGroupEntity(bool parallel)
    : parallel_(parallel)
{
}

ActionGroupEntity::~ActionGroupEntity() {}

void ActionGroupEntity::Init(Actor* target)
{
    if (actions_.IsEmpty())
    {
        Done();
        return;
    }

    // reset all actions
    for (current_ = actions_.GetFirst(); current_; current_ = current_->GetNext())
    {
        current_->Reset();
    }

    if (!parallel_)
    {
        current_ = actions_.GetFirst();
    }
}

void ActionGroupEntity::Update(Actor* target, Duration dt)
{
    if (!parallel_)
    {
        if (current_)
        {
            current_->UpdateStep(target, dt);

            if (current_->IsDone())
            {
                current_ = current_->GetNext();

                if (!current_)
                    Complete(target);
            }
        }
    }
    else
    {
        bool done = true;
        for (current_ = actions_.GetFirst(); current_; current_ = current_->GetNext())
        {
            if (!current_->IsDone())
            {
                done = false;
                current_->UpdateStep(target, dt);
            }
        }

        if (done)
        {
            Complete(target);
        }
    }
}

void ActionGroupEntity::AddAction(ActionEntityPtr action)
{
    if (action)
    {
        actions_.PushBack(action);
    }
}

void ActionGroupEntity::AddActions(const Vector<ActionEntityPtr>& actions)
{
    for (const auto& action : actions)
        AddAction(action);
}

ActionEntityPtr ActionGroupEntity::Clone() const
{
    Vector<ActionEntityPtr> actions;
    if (!actions_.IsEmpty())
    {
        for (auto action = actions_.GetFirst(); action; action = action->GetNext())
        {
            actions.push_back(action->Clone());
        }
    }
    return DoClone(ActionGroupEntity::Create(actions, parallel_));
}

ActionEntityPtr ActionGroupEntity::Reverse() const
{
    Vector<ActionEntityPtr> actions;
    if (!actions_.IsEmpty())
    {
        for (auto action = actions_.GetLast(); action; action = action->GetPrev())
        {
            actions.push_back(action->Reverse());
        }
    }
    return DoClone(ActionGroupEntity::Create(actions, parallel_));
}

}  // namespace kiwano
