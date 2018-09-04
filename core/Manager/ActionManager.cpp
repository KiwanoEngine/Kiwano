#include "..\e2dmanager.h"
#include "..\e2daction.h"
#include "..\e2dnode.h"


e2d::ActionManager * e2d::ActionManager::GetInstance()
{
	static ActionManager instance;
	return &instance;
}

e2d::ActionManager::ActionManager()
	: actions_()
	, running_actions_()
{
}

e2d::ActionManager::~ActionManager()
{
}

void e2d::ActionManager::Update()
{
	if (running_actions_.empty())
		return;

	std::vector<Action*> currActions;
	currActions.reserve(running_actions_.size());
	std::copy_if(
		running_actions_.begin(),
		running_actions_.end(),
		std::back_inserter(currActions),
		[](Action* action) { return action->IsRunning() && !action->IsDone(); }
	);

	// 遍历所有正在运行的动作
	for (const auto& action : currActions)
		action->Update();

	// 清除完成的动作
	for (auto iter = running_actions_.begin(); iter != running_actions_.end();)
	{
		if ((*iter)->IsDone())
		{
			(*iter)->Release();
			(*iter)->target_ = nullptr;
			iter = running_actions_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void e2d::ActionManager::Add(Action * action)
{
	if (action)
	{
		auto iter = std::find(actions_.begin(), actions_.end(), action);
		if (iter == actions_.end())
		{
			actions_.push_back(action);
		}
	}
}

void e2d::ActionManager::Remove(Action * action)
{
	if (actions_.empty() || action == nullptr)
		return;

	auto iter = std::find(actions_.begin(), actions_.end(), action);
	if (iter != actions_.end())
	{
		actions_.erase(iter);
	}
}

void e2d::ActionManager::ResumeAllBindedWith(Node * target)
{
	if (running_actions_.empty() || target == nullptr)
		return;

	for (const auto& action : running_actions_)
	{
		if (action->GetTarget() == target)
		{
			action->Resume();
		}
	}
}

void e2d::ActionManager::PauseAllBindedWith(Node * target)
{
	if (running_actions_.empty() || target == nullptr)
		return;

	for (const auto& action : running_actions_)
	{
		if (action->GetTarget() == target)
		{
			action->Pause();
		}
	}
}

void e2d::ActionManager::StopAllBindedWith(Node * target)
{
	if (running_actions_.empty() || target == nullptr)
		return;

	for (const auto& action : running_actions_)
	{
		if (action->GetTarget() == target)
		{
			action->Stop();
		}
	}
}

void e2d::ActionManager::Start(Action * action, Node * target, bool paused)
{
	WARN_IF(action == nullptr, "Action NULL pointer exception!");
	WARN_IF(target == nullptr, "GetTarget node NULL pointer exception!");

	if (action && target)
	{
		if (action->target_ == nullptr)
		{
			auto iter = std::find(running_actions_.begin(), running_actions_.end(), action);
			if (iter == running_actions_.end())
			{
				action->Retain();
				action->StartWithTarget(target);
				action->running_ = !paused;
				running_actions_.push_back(action);
			}
		}
		else
		{
			throw Exception("该 Action 已有执行目标");
		}
	}
}

void e2d::ActionManager::Resume(const String& name)
{
	if (running_actions_.empty() || name.IsEmpty())
		return;

	for (const auto& action : running_actions_)
	{
		if (action->GetName() == name)
		{
			action->Resume();
		}
	}
}

void e2d::ActionManager::Pause(const String& name)
{
	if (running_actions_.empty() || name.IsEmpty())
		return;

	for (const auto& action : running_actions_)
	{
		if (action->GetName() == name)
		{
			action->Pause();
		}
	}
}

void e2d::ActionManager::Stop(const String& name)
{
	if (running_actions_.empty() || name.IsEmpty())
		return;

	for (const auto& action : running_actions_)
	{
		if (action->GetName() == name)
		{
			action->Stop();
		}
	}
}

void e2d::ActionManager::ClearAllBindedWith(Node * target)
{
	if (target)
	{
		auto iter = std::find_if(
			running_actions_.begin(),
			running_actions_.end(),
			[target](Action* action) ->bool { return action->GetTarget() == target; }
		);

		if (iter != running_actions_.end())
		{
			(*iter)->Release();
			running_actions_.erase(iter);
		}
	}
}

void e2d::ActionManager::ClearAll()
{
	if (!running_actions_.empty())
	{
		for (const auto& action : running_actions_)
		{
			action->Release();
		}
		running_actions_.clear();
	}
	
	actions_.clear();
}

std::vector<e2d::Action*> e2d::ActionManager::Get(const String& name)
{
	std::vector<Action*> actions;
	for (const auto& action : actions_)
	{
		if (action->GetName() == name)
		{
			actions.push_back(action);
		}
	}
	return std::move(actions);
}

const std::vector<e2d::Action*>& e2d::ActionManager::GetAll()
{
	return actions_;
}

void e2d::ActionManager::UpdateTime()
{
	for (const auto& action : running_actions_)
	{
		action->ResetTime();
	}
}
