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

#include "ActionCombined.h"
#include "base.h"

namespace easy2d
{
	//-------------------------------------------------------
	// Loop
	//-------------------------------------------------------

	Loop::Loop(Action * action, int times)
		: action_(action)
		, times_(0)
		, total_times_(times)
	{
		E2D_WARNING_IF(action == nullptr, "Loop NULL pointer exception!");

		if (action)
		{
			action_ = action;
			action_->Retain();
		}
	}

	Loop::~Loop()
	{
		SafeRelease(action_);
	}

	Loop * Loop::Clone() const
	{
		if (action_)
		{
			return new Loop(action_->Clone());
		}
		else
		{
			return nullptr;
		}
	}

	Loop * Loop::Reverse() const
	{
		if (action_)
		{
			return new Loop(action_->Clone());
		}
		else
		{
			return nullptr;
		}
	}

	void Loop::Initialize()
	{
		Action::Initialize();

		if (action_)
		{
			action_->target_ = target_;
			action_->Initialize();
		}
	}

	void Loop::Update()
	{
		Action::Update();

		if (times_ == total_times_)
		{
			this->Stop();
			return;
		}

		if (action_)
		{
			action_->Update();

			if (action_->IsDone())
			{
				++times_;

				Action::Reset();
				action_->Reset();
			}
		}
		else
		{
			this->Stop();
		}
	}

	void Loop::Reset()
	{
		Action::Reset();

		if (action_) action_->Reset();
		times_ = 0;
	}

	void Loop::ResetTime()
	{
		if (action_) action_->ResetTime();
	}


	//-------------------------------------------------------
	// Sequence
	//-------------------------------------------------------

	Sequence::Sequence()
		: action_index_(0)
	{
	}

	Sequence::Sequence(const Actions& actions)
		: action_index_(0)
	{
		this->Add(actions);
	}

	Sequence::~Sequence()
	{
		for (auto action : actions_)
		{
			SafeRelease(action);
		}
	}

	void Sequence::Initialize()
	{
		Action::Initialize();
		// 将所有动作与目标绑定
		if (target_)
		{
			for (const auto& action : actions_)
			{
				action->target_ = target_;
			}
		}
		// 初始化第一个动作
		actions_[0]->Initialize();
	}

	void Sequence::Update()
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
				actions_[action_index_]->Initialize();
			}
		}
	}

	void Sequence::Reset()
	{
		Action::Reset();
		for (const auto& action : actions_)
		{
			action->Reset();
		}
		action_index_ = 0;
	}

	void Sequence::ResetTime()
	{
		for (const auto& action : actions_)
		{
			action->ResetTime();
		}
	}

	void Sequence::Add(Action * action)
	{
		if (action)
		{
			actions_.push_back(action);
			action->Retain();
		}
	}

	void Sequence::Add(const Actions& actions)
	{
		for (const auto &action : actions)
		{
			this->Add(action);
		}
	}

	Sequence * Sequence::Clone() const
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

	Sequence * Sequence::Reverse() const
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


	//-------------------------------------------------------
	// Spawn
	//-------------------------------------------------------

	Spawn::Spawn()
	{
	}

	Spawn::Spawn(const Actions& actions)
	{
		this->Add(actions);
	}

	Spawn::~Spawn()
	{
		for (auto action : actions_)
		{
			SafeRelease(action);
		}
	}

	void Spawn::Initialize()
	{
		Action::Initialize();

		if (target_)
		{
			for (const auto& action : actions_)
			{
				action->target_ = target_;
				action->Initialize();
			}
		}
	}

	void Spawn::Update()
	{
		Action::Update();

		size_t done_num = 0;
		for (const auto& action : actions_)
		{
			if (action->IsDone())
			{
				++done_num;
			}
			else
			{
				action->Update();
			}
		}

		if (done_num == actions_.size())
		{
			this->Stop();
		}
	}

	void Spawn::Reset()
	{
		Action::Reset();
		for (const auto& action : actions_)
		{
			action->Reset();
		}
	}

	void Spawn::ResetTime()
	{
		for (const auto& action : actions_)
		{
			action->ResetTime();
		}
	}

	void Spawn::Add(Action * action)
	{
		if (action)
		{
			actions_.push_back(action);
			action->Retain();
		}
	}

	void Spawn::Add(const Actions& actions)
	{
		for (const auto &action : actions)
		{
			this->Add(action);
		}
	}

	Spawn * Spawn::Clone() const
	{
		auto spawn = new Spawn();
		for (const auto& action : actions_)
		{
			if (action)
			{
				spawn->Add(action->Clone());
			}
		}
		return spawn;
	}

	Spawn * Spawn::Reverse() const
	{
		auto spawn = new Spawn();
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
}