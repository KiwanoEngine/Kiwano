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

#include <kiwano/2d/action/ActionManager.h>
#include <kiwano/2d/Actor.h>
#include <kiwano/core/Logger.h>

namespace kiwano
{
	void ActionManager::UpdateActions(Actor* target, Duration dt)
	{
		if (actions_.empty() || !target)
			return;

		ActionPtr next;
		for (auto action = actions_.first_item(); action; action = next)
		{
			next = action->next_item();

			if (action->IsRunning())
				action->UpdateStep(target, dt);

			if (action->IsRemoveable())
				actions_.remove(action);
		}
	}

	Action* ActionManager::AddAction(ActionPtr action)
	{
		return AddAction(action.get());
	}

	Action* ActionManager::AddAction(Action* action)
	{
		KGE_ASSERT(action && "AddAction failed, NULL pointer exception");

		if (action)
		{
			actions_.push_back(action);
		}
		return action;
	}

	void ActionManager::ResumeAllActions()
	{
		if (actions_.empty())
			return;

		for (auto& action : actions_)
		{
			action.Resume();
		}
	}

	void ActionManager::PauseAllActions()
	{
		if (actions_.empty())
			return;

		for (auto& action : actions_)
		{
			action.Pause();
		}
	}

	void ActionManager::StopAllActions()
	{
		if (actions_.empty())
			return;

		for (auto& action : actions_)
		{
			action.Stop();
		}
	}

	ActionPtr ActionManager::GetAction(String const& name)
	{
		if (actions_.empty())
			return nullptr;

		for (auto& action : actions_)
			if (action.IsName(name))
				return &action;

		return nullptr;
	}

	const ActionManager::Actions& ActionManager::GetAllActions() const
	{
		return actions_;
	}
}
