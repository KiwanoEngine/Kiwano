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
#include "logs.h"

namespace easy2d
{
	void ActionManager::UpdateActions(Node* target, Duration const& dt)
	{
		if (actions_.empty())
			return;

		std::vector<spAction> currActions;
		currActions.reserve(actions_.size());
		std::copy_if(
			actions_.begin(),
			actions_.end(),
			std::back_inserter(currActions),
			[](spAction action) { return action->IsRunning() && !action->IsDone(); }
		);

		// 遍历所有正在运行的动作
		for (const auto& action : currActions)
			action->Update(target, dt);

		// 清除完成的动作
		for (auto iter = actions_.begin(); iter != actions_.end();)
		{
			if ((*iter)->IsDone())
			{
				iter = actions_.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	void ActionManager::RunAction(spAction const& action)
	{
		if (!action)
			logs::Warningln("Node::RunAction failed, action is nullptr");

		if (action)
		{
			auto iter = std::find(actions_.begin(), actions_.end(), action);
			if (iter == actions_.end())
			{
				action->Start();
				actions_.push_back(action);
			}
		}
	}

	void ActionManager::ResumeAllActions()
	{
		if (actions_.empty())
			return;

		for (const auto& action : actions_)
		{
			action->Resume();
		}
	}

	void ActionManager::PauseAllActions()
	{
		if (actions_.empty())
			return;

		for (const auto& action : actions_)
		{
			action->Pause();
		}
	}

	void ActionManager::StopAllActions()
	{
		if (actions_.empty())
			return;

		for (const auto& action : actions_)
		{
			action->Stop();
		}
	}

	const Actions& ActionManager::GetAllActions() const
	{
		return actions_;
	}
}
