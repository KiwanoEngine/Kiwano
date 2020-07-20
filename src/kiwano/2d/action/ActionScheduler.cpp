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
#include <kiwano/2d/action/ActionScheduler.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

void ActionScheduler::Update(Actor* target, Duration dt)
{
    if (actions_.IsEmpty() || !target)
        return;

    ActionEntityPtr next;
    for (auto action = actions_.GetFirst(); action; action = next)
    {
        next = action->GetNext();

        if (action->IsRunning())
            action->UpdateStep(target, dt);

        if (action->IsRemoveable())
            actions_.Remove(action);
    }
}

ActionEntity* ActionScheduler::AddAction(ActionEntityPtr action)
{
    KGE_ASSERT(action && "AddAction failed, NULL pointer exception");

    if (action)
    {
        actions_.PushBack(action);
    }
    return action.Get();
}

void ActionScheduler::ResumeAllActions()
{
    if (actions_.IsEmpty())
        return;

    for (auto& action : actions_)
    {
        action->Resume();
    }
}

void ActionScheduler::PauseAllActions()
{
    if (actions_.IsEmpty())
        return;

    for (auto& action : actions_)
    {
        action->Pause();
    }
}

void ActionScheduler::StopAllActions()
{
    if (actions_.IsEmpty())
        return;

    for (auto& action : actions_)
    {
        action->Stop();
    }
}

ActionEntityPtr ActionScheduler::GetAction(const String& name)
{
    if (actions_.IsEmpty())
        return nullptr;

    for (auto& action : actions_)
        if (action->IsName(name))
            return action;
    return nullptr;
}

const ActionList& ActionScheduler::GetAllActions() const
{
    return actions_;
}
}  // namespace kiwano
