#include "..\e2daction.h"

e2d::Spawn::Spawn()
{
}

e2d::Spawn::Spawn(const Actions& actions)
{
	this->Add(actions);
}

e2d::Spawn::~Spawn()
{
	for (const auto& action : actions_)
	{
		GC::GetInstance()->SafeRelease(action);
	}
}

void e2d::Spawn::Init()
{
	Action::Init();

	if (target_)
	{
		for (const auto& action : actions_)
		{
			action->target_ = target_;
			action->Init();
		}
	}
}

void e2d::Spawn::Update()
{
	Action::Update();

	size_t doneNum = 0;
	for (const auto& action : actions_)
	{
		if (action->IsDone())
		{
			++doneNum;
		}
		else
		{
			action->Update();
		}
	}

	if (doneNum == actions_.size())
	{
		this->Stop();
	}
}

void e2d::Spawn::Reset()
{
	Action::Reset();
	for (const auto& action : actions_)
	{
		action->Reset();
	}
}

void e2d::Spawn::ResetTime()
{
	for (const auto& action : actions_)
	{
		action->ResetTime();
	}
}

void e2d::Spawn::Add(Action * action)
{
	if (action)
	{
		actions_.push_back(action);
		action->Retain();
	}
}

void e2d::Spawn::Add(const Actions& actions)
{
	for (const auto &action : actions)
	{
		this->Add(action);
	}
}

e2d::Spawn * e2d::Spawn::Clone() const
{
	auto spawn = new (e2d::autorelease) Spawn();
	for (const auto& action : actions_)
	{
		if (action)
		{
			spawn->Add(action->Clone());
		}
	}
	return spawn;
}

e2d::Spawn * e2d::Spawn::Reverse() const
{
	auto spawn = new (e2d::autorelease) Spawn();
	if (spawn && !actions_.empty())
	{
		std::vector<Action*> newActions(actions_.size());
		for (auto iter = actions_.crbegin(), iterCrend = actions_.crend(); iter != iterCrend; ++iter)
		{
			newActions.push_back((*iter)->Reverse());
		}
		spawn->Add(newActions);
	}
	return spawn;
}