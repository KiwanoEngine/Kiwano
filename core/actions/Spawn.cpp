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