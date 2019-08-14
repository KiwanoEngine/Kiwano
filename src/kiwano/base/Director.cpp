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

#include "Director.h"
#include "../2d/Actor.h"
#include "../2d/Stage.h"
#include "../2d/Transition.h"
#include "../2d/DebugActor.h"

namespace kiwano
{
	Director::Director()
		: render_border_enabled_(false)
	{
	}

	Director::~Director()
	{
	}

	void Director::EnterStage(StagePtr scene)
	{
		KGE_ASSERT(scene && "Director::EnterStage failed, NULL pointer exception");

		if (curr_scene_ == scene || next_scene_ == scene)
			return;

		next_scene_ = scene;
	}

	void Director::EnterStage(StagePtr scene, TransitionPtr transition)
	{
		EnterStage(scene);

		if (transition && next_scene_)
		{
			if (transition_)
			{
				transition_->Stop();
			}
			transition_ = transition;
			transition_->Init(curr_scene_, next_scene_);
		}
	}

	StagePtr Director::GetCurrentStage()
	{
		return curr_scene_;
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
		curr_scene_.reset();
		next_scene_.reset();
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

		if (next_scene_ && !transition_)
		{
			if (curr_scene_)
			{
				curr_scene_->OnExit();
			}

			next_scene_->OnEnter();

			curr_scene_ = next_scene_;
			next_scene_ = nullptr;
		}

		if (curr_scene_)
			curr_scene_->Update(dt);

		if (next_scene_)
			next_scene_->Update(dt);

		if (debug_actor_)
			debug_actor_->Update(dt);
	}

	void Director::OnRender(Renderer* renderer)
	{
		if (transition_)
		{
			transition_->Render(renderer);
		}
		else if (curr_scene_)
		{
			curr_scene_->Render(renderer);
		}

		if (debug_actor_)
		{
			debug_actor_->Render(renderer);
		}
	}

	void Director::AfterRender()
	{
		if (render_border_enabled_)
		{
			if (curr_scene_)
				curr_scene_->RenderBorder();
		}
	}

	void Director::HandleEvent(Event& evt)
	{
		if (debug_actor_)
			debug_actor_->Dispatch(evt);

		if (curr_scene_)
			curr_scene_->Dispatch(evt);
	}

}
