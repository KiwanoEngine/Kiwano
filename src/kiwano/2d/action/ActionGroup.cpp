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

ActionGroupPtr ActionGroup::Create(Vector<ActionPtr> const& actions, bool sync)
{
    ActionGroupPtr ptr = new (std::nothrow) ActionGroup(sync);
    if (ptr)
    {
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
    if (actions_.empty())
    {
        Done();
        return;
    }

    current_ = actions_.first_item();
    current_->Restart(target);  // init first action

    if (sync_)
    {
        // init all actions
        for (; current_; current_ = current_->next_item())
        {
            current_->Restart(target);
        }
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
                current_ = current_->next_item();

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
        for (current_ = actions_.first_item(); current_; current_ = current_->next_item())
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
        actions_.push_back(action);
    }
}

void ActionGroup::AddActions(Vector<ActionPtr> const& actions)
{
    for (const auto& action : actions)
        AddAction(action);
}

ActionPtr ActionGroup::Clone() const
{
    Vector<ActionPtr> actions;
    if (!actions_.empty())
    {
        for (auto action = actions_.last_item(); action; action = action->prev_item())
        {
            actions.push_back(action->Clone());
        }
    }
    return ActionGroup::Create(actions, sync_);
}

ActionPtr ActionGroup::Reverse() const
{
    Vector<ActionPtr> actions;
    if (!actions_.empty())
    {
        for (auto action = actions_.last_item(); action; action = action->prev_item())
        {
            actions.push_back(action->Reverse());
        }
    }
    return ActionGroup::Create(actions, sync_);
}

}  // namespace kiwano
