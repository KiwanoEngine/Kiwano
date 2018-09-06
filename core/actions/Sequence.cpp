#include "..\e2daction.h"

e2d::Sequence::Sequence()
	: action_index_(0)
{
}

e2d::Sequence::Sequence(const Actions& actions)
	: action_index_(0)
{
	this->Add(actions);
}

e2d::Sequence::~Sequence()
{
	for (auto action : actions_)
	{
		SafeRelease(action);
	}
}

void e2d::Sequence::Init()
{
	Action::Init();
	// 将所有动作与目标绑定
	if (target_)
	{
		for (const auto& action : actions_)
		{
			action->target_ = target_;
		}
	}
	// 初始化第一个动作
	actions_[0]->Init();
}

void e2d::Sequence::Update()
{
	Action::Update();

	auto &action = actions_[action_index_];
	action->Update();

	if (action->IsDone())
	{
		++action_index_;
		if (action_index_ == actions_.size())
		{
			this->Stop();
		}
		else
		{
			actions_[action_index_]->Init();
		}
	}
}

void e2d::Sequence::Reset()
{
	Action::Reset();
	for (const auto& action : actions_)
	{
		action->Reset();
	}
	action_index_ = 0;
}

void e2d::Sequence::ResetTime()
{
	for (const auto& action : actions_)
	{
		action->ResetTime();
	}
}

void e2d::Sequence::Add(Action * action)
{
	if (action)
	{
		actions_.push_back(action);
		action->Retain();
	}
}

void e2d::Sequence::Add(const Actions& actions)
{
	for (const auto &action : actions)
	{
		this->Add(action);
	}
}

e2d::Sequence * e2d::Sequence::Clone() const
{
	auto sequence = new Sequence();
	for (const auto& action : actions_)
	{
		if (action)
		{
			sequence->Add(action->Clone());
		}
	}
	return sequence;
}

e2d::Sequence * e2d::Sequence::Reverse() const
{
	auto sequence = new Sequence();
	if (sequence && !actions_.empty())
	{
		std::vector<Action*> newActions(actions_.size());
		for (auto iter = actions_.crbegin(), iterCrend = actions_.crend(); iter != iterCrend; ++iter)
		{
			newActions.push_back((*iter)->Reverse());
		}
		sequence->Add(newActions);
	}
	return sequence;
}