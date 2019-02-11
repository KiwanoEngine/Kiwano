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
	// Sequence
	//-------------------------------------------------------

	Sequence::Sequence()
	{
	}

	Sequence::Sequence(Array<ActionPtr> const& actions)
	{
		this->Add(actions);
	}

	Sequence::~Sequence()
	{
	}

	void Sequence::Init(NodePtr const& target)
	{
		if (actions_.IsEmpty())
			Done();
		else
		{
			current_ = actions_.First();
			current_->Restart(target);	// init
		}
	}

	void Sequence::Update(NodePtr const& target, Duration dt)
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

	void Sequence::Add(ActionPtr const& action)
	{
		if (action)
		{
			actions_.PushBack(action);
		}
	}

	void Sequence::Add(Array<ActionPtr> const& actions)
	{
		for (const auto& action : actions)
			Add(action);
	}

	ActionPtr Sequence::Clone() const
	{
		auto sequence = new (std::nothrow) Sequence();
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

	ActionPtr Sequence::Reverse() const
	{
		auto sequence = new (std::nothrow) Sequence();
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
	// Spawn
	//-------------------------------------------------------

	Spawn::Spawn()
		: size_(0)
	{
	}

	Spawn::Spawn(Array<ActionPtr> const& actions)
		: size_(0)
	{
		this->Add(actions);
	}

	Spawn::~Spawn()
	{
	}

	void Spawn::Init(NodePtr const& target)
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

	void Spawn::Update(NodePtr const& target, Duration dt)
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

	void Spawn::Add(ActionPtr const& action)
	{
		if (action)
		{
			actions_.PushBack(action);
			++size_;
		}
	}

	void Spawn::Add(Array<ActionPtr> const& actions)
	{
		for (const auto& action : actions)
			Add(action);
	}

	ActionPtr Spawn::Clone() const
	{
		auto spawn = new (std::nothrow) Spawn();
		if (spawn)
		{
			for (auto action = actions_.First(); action; action = action->NextItem())
			{
				spawn->Add(action->Clone());
			}
		}
		return spawn;
	}

	ActionPtr Spawn::Reverse() const
	{
		auto spawn = new (std::nothrow) Spawn();
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