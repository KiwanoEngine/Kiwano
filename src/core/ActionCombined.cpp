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
#include "logs.h"

namespace easy2d
{
	//-------------------------------------------------------
	// Loop
	//-------------------------------------------------------

	Loop::Loop(SpAction const& action, int times)
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

	SpAction Loop::Clone() const
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

	SpAction Loop::Reverse() const
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
		Action::Init(target);

		if (action_)
		{
			action_->Init(target);
		}
	}

	void Loop::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		if (action_)
		{
			action_->Update(target, dt);

			if (action_->IsDone())
			{
				++times_;

				Action::Reset();
				action_->Reset();
			}

			if (times_ == total_times_)
			{
				this->Stop();
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

	Sequence::Sequence(Array<SpAction> const& actions)
		: action_index_(0)
	{
		this->Add(actions);
	}

	Sequence::~Sequence()
	{
	}

	void Sequence::Init(Node* target)
	{
		Action::Init(target);
		actions_[0]->Init(target);
	}

	void Sequence::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		auto& action = actions_[action_index_];
		action->Update(target, dt);

		if (action->IsDone())
		{
			++action_index_;
			if (action_index_ == actions_.size())
			{
				this->Stop();
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

	void Sequence::Add(SpAction const& action)
	{
		if (action)
		{
			actions_.push_back(action);
		}
	}

	void Sequence::Add(Array<SpAction> const& actions)
	{
		for (const auto &action : actions)
		{
			this->Add(action);
		}
	}

	SpAction Sequence::Clone() const
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

	SpAction Sequence::Reverse() const
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

	Spawn::Spawn(Array<SpAction> const& actions)
	{
		this->Add(actions);
	}

	Spawn::~Spawn()
	{
	}

	void Spawn::Init(Node* target)
	{
		Action::Init(target);

		if (target)
		{
			for (const auto& action : actions_)
			{
				action->Init(target);
			}
		}
	}

	void Spawn::Update(Node* target, Duration const& dt)
	{
		Action::Update(target, dt);

		size_t done_num = 0;
		for (const auto& action : actions_)
		{
			if (action->IsDone())
			{
				++done_num;
			}
			else
			{
				action->Update(target, dt);
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

	void Spawn::Add(SpAction const& action)
	{
		if (action)
		{
			actions_.push_back(action);
		}
	}

	void Spawn::Add(Array<SpAction> const& actions)
	{
		for (const auto &action : actions)
		{
			this->Add(action);
		}
	}

	SpAction Spawn::Clone() const
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

	SpAction Spawn::Reverse() const
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