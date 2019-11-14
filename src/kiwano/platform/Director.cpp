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

#include <kiwano/platform/Director.h>
#include <kiwano/2d/Actor.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/2d/Transition.h>
#include <kiwano/2d/DebugActor.h>
#include <kiwano/renderer/RenderTarget.h>

namespace kiwano
{
	Director::Director()
		: render_border_enabled_(false)
	{
	}

	Director::~Director()
	{
	}

	void Director::EnterStage(StagePtr stage, TransitionPtr transition)
	{
		KGE_ASSERT(stage && "Director::EnterStage failed, NULL pointer exception");

		if (current_stage_ == stage || next_stage_ == stage)
			return;

		next_stage_ = stage;

		if (transition && next_stage_)
		{
			if (transition_)
			{
				transition_->Stop();
			}
			transition_ = transition;
			transition_->Init(current_stage_, next_stage_);
		}
	}

	void Director::PushStage(StagePtr stage, TransitionPtr transition)
	{
		EnterStage(stage, transition);

		if (current_stage_)
		{
			stages_.push(current_stage_);
		}
	}

	void Director::PopStage(TransitionPtr transition)
	{
		KGE_ASSERT(!stages_.empty() && "Director::PopStage failed, calling pop() on empty stage stack");

		if (!stages_.empty())
		{
			next_stage_ = stages_.top();
			stages_.pop();
		}

		if (transition && next_stage_)
		{
			if (transition_)
			{
				transition_->Stop();
			}
			transition_ = transition;
			transition_->Init(current_stage_, next_stage_);
		}
	}

	StagePtr Director::GetCurrentStage()
	{
		return current_stage_;
	}

	void Director::SetRenderBorderEnabled(bool enabled)
	{
		render_border_enabled_ = enabled;
	}

	void Director::ShowDebugInfo(bool show)
	{
		if (show)
		{
			if (!debug_actor_)
				debug_actor_ = new DebugActor;
		}
		else
		{
			debug_actor_.reset();
		}
	}

	void Director::ClearStages()
	{
		while (!stages_.empty())
			stages_.pop();

		current_stage_.reset();
		next_stage_.reset();
		debug_actor_.reset();
		transition_.reset();
	}

	void Director::OnUpdate(Duration dt)
	{
		if (transition_)
		{
			transition_->Update(dt);

			if (transition_->IsDone())
				transition_ = nullptr;
		}

		if (next_stage_ && !transition_)
		{
			if (current_stage_)
			{
				current_stage_->OnExit();
			}

			next_stage_->OnEnter();

			current_stage_ = next_stage_;
			next_stage_ = nullptr;
		}

		if (current_stage_)
			current_stage_->Update(dt);

		if (next_stage_)
			next_stage_->Update(dt);

		if (debug_actor_)
			debug_actor_->Update(dt);
	}

	void Director::OnRender(RenderTarget* rt)
	{
		if (transition_)
		{
			transition_->Render(rt);
		}
		else if (current_stage_)
		{
			current_stage_->Render(rt);
		}

		if (render_border_enabled_)
		{
			rt->SetOpacity(1.f);
			if (current_stage_)
			{
				current_stage_->RenderBorder(rt);
			}
		}

		if (debug_actor_)
		{
			debug_actor_->Render(rt);
		}
	}

	void Director::HandleEvent(Event& evt)
	{
		if (debug_actor_)
			debug_actor_->Dispatch(evt);

		if (current_stage_)
			current_stage_->Dispatch(evt);
	}

}
