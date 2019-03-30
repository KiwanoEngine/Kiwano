// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "ActionManager.h"
#include "../base/logs.h"

namespace easy2d
{
	void ActionManager::UpdateActions(NodePtr const& target, Duration dt)
	{
		if (actions_.IsEmpty() || !target)
			return;

		ActionPtr next;
		for (auto action = actions_.First(); action; action = next)
		{
			next = action->NextItem();

			if (action->IsRunning())
				action->UpdateStep(target, dt);

			if (action->IsRemoveable())
				actions_.Remove(action);
		}
	}

	ActionPtr ActionManager::AddAction(ActionPtr const& action)
	{
		E2D_ASSERT(action && "AddAction failed, NULL pointer exception");

		if (action)
		{
			actions_.PushBack(action);
		}
		return action;
	}

	ActionPtr ActionManager::GetAction(String const & name)
	{
		if (actions_.IsEmpty())
			return nullptr;

		for (auto action = actions_.First().Get(); action; action = action->NextItem().Get())
			if (action->IsName(name))
				return action;
		return nullptr;
	}

	void ActionManager::ResumeAllActions()
	{
		if (actions_.IsEmpty())
			return;

		for (auto action = actions_.First().Get(); action; action = action->NextItem().Get())
		{
			action->Resume();
		}
	}

	void ActionManager::PauseAllActions()
	{
		if (actions_.IsEmpty())
			return;

		for (auto action = actions_.First().Get(); action; action = action->NextItem().Get())
		{
			action->Pause();
		}
	}

	void ActionManager::StopAllActions()
	{
		if (actions_.IsEmpty())
			return;

		for (auto action = actions_.First().Get(); action; action = action->NextItem().Get())
		{
			action->Stop();
		}
	}

	const ActionManager::Actions& ActionManager::GetAllActions() const
	{
		return actions_;
	}
}
