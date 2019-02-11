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
	// ActionSequence
	//-------------------------------------------------------

	ActionSequence::ActionSequence()
	{
	}

	ActionSequence::ActionSequence(Array<ActionPtr> const& actions)
	{
		this->Add(actions);
	}

	ActionSequence::~ActionSequence()
	{
	}

	void ActionSequence::Init(NodePtr const& target)
	{
		if (actions_.IsEmpty())
			Done();
		else
		{
			current_ = actions_.First();
			current_->Restart(target);	// init
		}
	}

	void ActionSequence::Update(NodePtr const& target, Duration dt)
	{
		if (current_)
		{
			current_->UpdateStep(target, dt);

			if (current_->IsDone())
			{
				current_ = current_->NextItem();
				if (current_)
					current_->Restart(target);
			}
		}

		if (!current_)
		{
			Complete(target);
		}
	}

	void ActionSequence::Add(ActionPtr const& action)
	{
		if (action)
		{
			actions_.PushBack(action);
		}
	}

	void ActionSequence::Add(Array<ActionPtr> const& actions)
	{
		for (const auto& action : actions)
			Add(action);
	}

	ActionPtr ActionSequence::Clone() const
	{
		auto sequence = new (std::nothrow) ActionSequence();
		if (sequence)
		{
			for (auto action = actions_.First(); action; action = action->NextItem())
			{
				if (action)
				{
					sequence->Add(action->Clone());
				}
			}
		}
		return sequence;
	}

	ActionPtr ActionSequence::Reverse() const
	{
		auto sequence = new (std::nothrow) ActionSequence();
		if (sequence && !actions_.IsEmpty())
		{
			for (auto action = actions_.Last(); action; action = action->PrevItem())
			{
				sequence->Add(action->Reverse());
			}
		}
		return sequence;
	}


	//-------------------------------------------------------
	// ActionSpawn
	//-------------------------------------------------------

	ActionSpawn::ActionSpawn()
		: size_(0)
	{
	}

	ActionSpawn::ActionSpawn(Array<ActionPtr> const& actions)
		: size_(0)
	{
		this->Add(actions);
	}

	ActionSpawn::~ActionSpawn()
	{
	}

	void ActionSpawn::Init(NodePtr const& target)
	{
		if (actions_.IsEmpty())
			Done();
		else
		{
			for (auto action = actions_.First(); action; action = action->NextItem())
			{
				action->Restart(target);	// init
			}
		}
	}

	void ActionSpawn::Update(NodePtr const& target, Duration dt)
	{
		int done_num = 0;
		for (auto action = actions_.First(); action; action = action->NextItem())
		{
			if (action->IsDone())
			{
				++done_num;
			}
			else
			{
				action->UpdateStep(target, dt);
			}
		}

		if (done_num == size_)
		{
			Complete(target);
		}
	}

	void ActionSpawn::Add(ActionPtr const& action)
	{
		if (action)
		{
			actions_.PushBack(action);
			++size_;
		}
	}

	void ActionSpawn::Add(Array<ActionPtr> const& actions)
	{
		for (const auto& action : actions)
			Add(action);
	}

	ActionPtr ActionSpawn::Clone() const
	{
		auto spawn = new (std::nothrow) ActionSpawn();
		if (spawn)
		{
			for (auto action = actions_.First(); action; action = action->NextItem())
			{
				spawn->Add(action->Clone());
			}
		}
		return spawn;
	}

	ActionPtr ActionSpawn::Reverse() const
	{
		auto spawn = new (std::nothrow) ActionSpawn();
		if (spawn && !actions_.IsEmpty())
		{
			for (auto action = actions_.Last(); action; action = action->PrevItem())
			{
				spawn->Add(action->Reverse());
			}
		}
		return spawn;
	}
}