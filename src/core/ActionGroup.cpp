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

#include "ActionGroup.h"
#include "logs.h"

namespace easy2d
{
	//-------------------------------------------------------
	// Loop
	//-------------------------------------------------------

	Loop::Loop(ActionPtr const& action, int times)
		: action_(action)
		, times_(0)
		, total_times_(times)
	{
		E2D_ASSERT(action && "Loop action contains a null action");

		action_ = action;
	}

	Loop::~Loop()
	{
	}

	ActionPtr Loop::Clone() const
	{
		if (action_)
		{
			return new (std::nothrow) Loop(action_->Clone());
		}
		else
		{
			return nullptr;
		}
	}

	ActionPtr Loop::Reverse() const
	{
		if (action_)
		{
			return new (std::nothrow) Loop(action_->Clone());
		}
		else
		{
			return nullptr;
		}
	}

	void Loop::Init(Node* target)
	{
		if (action_)
		{
			action_->Init(target);
		}
	}

	void Loop::Update(Node* target, Duration dt)
	{
		if (action_)
		{
			action_->Step(target, dt);

			if (action_->IsDone())
			{
				++times_;

				Action::Reset();
				action_->Reset();
			}

			if (times_ == total_times_)
			{
				Stop();
			}
		}
		else
		{
			Stop();
		}
	}

	void Loop::Reset()
	{
		Action::Reset();

		if (action_) action_->Reset();
		times_ = 0;
	}

	bool Loop::IsRunning()
	{
		return Action::IsRunning() && times_ != total_times_;
	}


	//-------------------------------------------------------
	// Sequence
	//-------------------------------------------------------

	Sequence::Sequence()
		: action_index_(0)
	{
	}

	Sequence::Sequence(Array<ActionPtr> const& actions)
		: action_index_(0)
	{
		this->Add(actions);
	}

	Sequence::~Sequence()
	{
	}

	void Sequence::Init(Node* target)
	{
		if (!actions_.empty())
			actions_[0]->Init(target);
	}

	void Sequence::Update(Node* target, Duration dt)
	{
		if (actions_.empty())
		{
			Stop();
			return;
		}

		auto& action = actions_[action_index_];
		action->Step(target, dt);

		if (action->IsDone())
		{
			++action_index_;
			if (action_index_ == actions_.size())
			{
				Stop();
			}
			else
			{
				actions_[action_index_]->Init(target);
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

	void Sequence::Add(ActionPtr const& action)
	{
		if (action)
		{
			actions_.push_back(action);
		}
	}

	void Sequence::Add(Array<ActionPtr> const& actions)
	{
		if (actions_.empty())
			actions_ = actions;
		else
		{
			actions_.reserve(actions_.size() + actions.size());
			for (const auto& action : actions)
				Add(action);
		}
	}

	ActionPtr Sequence::Clone() const
	{
		auto sequence = new (std::nothrow) Sequence();
		if (sequence)
		{
			for (const auto& action : actions_)
			{
				if (action)
				{
					sequence->Add(action->Clone());
				}
			}
		}
		return sequence;
	}

	ActionPtr Sequence::Reverse() const
	{
		auto sequence = new (std::nothrow) Sequence();
		if (sequence && !actions_.empty())
		{
			for (auto iter = actions_.crbegin(), crend = actions_.crend(); iter != crend; ++iter)
			{
				if (*iter)
					sequence->Add((*iter)->Reverse());
			}
		}
		return sequence;
	}


	//-------------------------------------------------------
	// Spawn
	//-------------------------------------------------------

	Spawn::Spawn()
	{
	}

	Spawn::Spawn(Array<ActionPtr> const& actions)
	{
		this->Add(actions);
	}

	Spawn::~Spawn()
	{
	}

	void Spawn::Init(Node* target)
	{
		for (const auto& action : actions_)
		{
			action->Init(target);
		}
	}

	void Spawn::Update(Node* target, Duration dt)
	{
		int done_num = 0;
		for (const auto& action : actions_)
		{
			if (action->IsDone())
			{
				++done_num;
			}
			else
			{
				action->Step(target, dt);
			}
		}

		if (done_num == actions_.size())
		{
			Stop();
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

	void Spawn::Add(ActionPtr const& action)
	{
		if (action)
		{
			actions_.push_back(action);
		}
	}

	void Spawn::Add(Array<ActionPtr> const& actions)
	{
		if (actions_.empty())
			actions_ = actions;
		else
		{
			actions_.reserve(actions_.size() + actions.size());
			for (const auto& action : actions)
				Add(action);
		}
	}

	ActionPtr Spawn::Clone() const
	{
		auto spawn = new (std::nothrow) Spawn();
		if (spawn)
		{
			for (const auto& action : actions_)
			{
				if (action)
				{
					spawn->Add(action->Clone());
				}
			}
		}
		return spawn;
	}

	ActionPtr Spawn::Reverse() const
	{
		auto spawn = new (std::nothrow) Spawn();
		if (spawn && !actions_.empty())
		{
			for (auto iter = actions_.crbegin(), crend = actions_.crend(); iter != crend; ++iter)
			{
				if (*iter)
					spawn->Add((*iter)->Reverse());
			}
		}
		return spawn;
	}
}