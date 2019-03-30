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
#include "../base/logs.h"

namespace easy2d
{
	//-------------------------------------------------------
	// ActionGroup
	//-------------------------------------------------------

	ActionGroup::ActionGroup()
		: sequence_(true)
	{
	}

	ActionGroup::ActionGroup(Array<ActionPtr> const& actions, bool sequence)
		: sequence_(sequence)
	{
		this->Add(actions);
	}

	ActionGroup::~ActionGroup()
	{
	}

	void ActionGroup::Init(NodePtr const& target)
	{
		if (actions_.IsEmpty())
		{
			Done();
			return;
		}
		
		current_ = actions_.First();
		current_->Restart(target);	// init first action

		if (!sequence_)
		{
			// init all actions
			for (; current_; current_ = current_->NextItem())
			{
				current_->Restart(target);
			}
		}
	}

	void ActionGroup::Update(NodePtr const& target, Duration dt)
	{
		if (sequence_)
		{
			if (current_)
			{
				current_->UpdateStep(target, dt);

				if (current_->IsDone())
				{
					current_ = current_->NextItem();

					if (current_)
						current_->Restart(target);	// init next action
					else
						Complete(target);
				}
			}
		}
		else
		{
			bool done = true;
			for (current_ = actions_.First(); current_; current_ = current_->NextItem())
			{
				if (!current_->IsDone())
				{
					done = false;
					current_->UpdateStep(target, dt);
				}
			}

			if (done)
			{
				Complete(target);
			}
		}
	}

	void ActionGroup::Add(ActionPtr const& action)
	{
		if (action)
		{
			actions_.PushBack(action);
		}
	}

	void ActionGroup::Add(Array<ActionPtr> const& actions)
	{
		for (const auto& action : actions)
			Add(action);
	}

	ActionPtr ActionGroup::Clone() const
	{
		auto group = new (std::nothrow) ActionGroup();
		if (group)
		{
			for (auto action = actions_.First(); action; action = action->NextItem())
			{
				if (action)
				{
					group->Add(action->Clone());
				}
			}
		}
		return group;
	}

	ActionPtr ActionGroup::Reverse() const
	{
		auto group = new (std::nothrow) ActionGroup();
		if (group && !actions_.IsEmpty())
		{
			for (auto action = actions_.Last(); action; action = action->PrevItem())
			{
				group->Add(action->Reverse());
			}
		}
		return group;
	}

}