// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include "../Actor.h"
#include "../../base/Logger.h"

namespace kiwano
{
	//-------------------------------------------------------
	// ActionGroup
	//-------------------------------------------------------

	ActionGroup::ActionGroup()
		: sequence_(true)
	{
	}

	ActionGroup::ActionGroup(Vector<ActionPtr> const& actions, bool sequence)
		: sequence_(sequence)
	{
		this->Add(actions);
	}

	ActionGroup::~ActionGroup()
	{
	}

	void ActionGroup::Init(ActorPtr target)
	{
		if (actions_.is_empty())
		{
			Done();
			return;
		}
		
		current_ = actions_.first_item();
		current_->Restart(target);	// init first action

		if (!sequence_)
		{
			// init all actions
			for (; current_; current_ = current_->next_item())
			{
				current_->Restart(target);
			}
		}
	}

	void ActionGroup::Update(ActorPtr target, Duration dt)
	{
		if (sequence_)
		{
			if (current_)
			{
				current_->UpdateStep(target, dt);

				if (current_->IsDone())
				{
					current_ = current_->next_item();

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
			for (current_ = actions_.first_item(); current_; current_ = current_->next_item())
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

	void ActionGroup::Add(ActionPtr action)
	{
		if (action)
		{
			actions_.push_back_item(action);
		}
	}

	void ActionGroup::Add(Vector<ActionPtr> const& actions)
	{
		for (const auto& action : actions)
			Add(action);
	}

	ActionPtr ActionGroup::Clone() const
	{
		auto group = new (std::nothrow) ActionGroup();
		if (group)
		{
			for (auto action = actions_.first_item(); action; action = action->next_item())
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
		if (group && !actions_.is_empty())
		{
			for (auto action = actions_.last_item(); action; action = action->prev_item())
			{
				group->Add(action->Reverse());
			}
		}
		return group;
	}

}
