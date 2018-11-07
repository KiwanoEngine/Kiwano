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

#include "..\e2daction.h"


namespace easy2d
{
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

	void Sequence::Init()
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
				actions_[action_index_]->Init();
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
}