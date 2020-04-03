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
#include <kiwano/core/Logger.h>

namespace kiwano
{

ActionGroupPtr ActionGroup::Create(const Vector<ActionPtr>& actions, bool sync)
{
    ActionGroupPtr ptr = memory::New<ActionGroup>();
    if (ptr)
    {
        ptr->sync_ = sync;
        ptr->AddActions(actions);
    }
    return ptr;
}

ActionGroup::ActionGroup()
    : sync_(false)
{
}

ActionGroup::ActionGroup(bool sync)
    : sync_(sync)
{
}

ActionGroup::~ActionGroup() {}

void ActionGroup::Init(Actor* target)
{
    if (actions_.IsEmpty())
    {
        Done();
        return;
    }

    if (sync_)
    {
        // init all actions
        for (current_ = actions_.GetFirst(); current_; current_ = current_->GetNext())
        {
            current_->Restart(target);
        }
    }
    else
    {
        current_ = actions_.GetFirst();
        current_->Restart(target);  // init first action
    }
}

void ActionGroup::Update(Actor* target, Duration dt)
{
    if (!sync_)
    {
        if (current_)
        {
            current_->UpdateStep(target, dt);

            if (current_->IsDone())
            {
                current_ = current_->GetNext();

                if (current_)
                    current_->Restart(target);  // init next action
                else
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

void ActionGroup::AddAction(ActionPtr action)
{
    if (action)
    {
        actions_.PushBack(action);
    }
}

void ActionGroup::AddActions(const Vector<ActionPtr>& actions)
{
    for (const auto& action : actions)
        AddAction(action);
}

ActionPtr ActionGroup::Clone() const
{
    Vector<ActionPtr> actions;
    if (!actions_.IsEmpty())
    {
        for (auto action = actions_.GetLast(); action; action = action->GetPrev())
        {
            actions.push_back(action->Clone());
        }
    }
    return DoClone(ActionGroup::Create(actions, sync_));
}

ActionPtr ActionGroup::Reverse() const
{
    Vector<ActionPtr> actions;
    if (!actions_.IsEmpty())
    {
        for (auto action = actions_.GetLast(); action; action = action->GetPrev())
        {
            actions.push_back(action->Reverse());
        }
    }
    return DoClone(ActionGroup::Create(actions, sync_));
}

}  // namespace kiwano
