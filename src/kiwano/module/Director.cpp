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

#include <kiwano/2d/Actor.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/module/Director.h>
#include <kiwano/component/DebugComponent.h>

namespace kiwano
{

Director::Director()
    : render_border_enabled_(false)
{
}

Director::~Director() {}

void Director::DestroyModule()
{
    ClearStages();
}

void Director::EnterStage(RefPtr<Stage> stage, RefPtr<Transition> transition)
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
        transition_->Init(current_stage_.Get(), next_stage_.Get());
    }
}

void Director::PushStage(RefPtr<Stage> stage, RefPtr<Transition> transition)
{
    EnterStage(stage, transition);

    if (current_stage_)
    {
        stages_.push(current_stage_);
    }
}

void Director::PopStage(RefPtr<Transition> transition)
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
        transition_->Init(current_stage_.Get(), next_stage_.Get());
    }
}

RefPtr<Stage> Director::GetCurrentStage()
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
        {
            debug_actor_ = MakePtr<Actor>();
            debug_actor_->AddComponent(new DebugComponent);
        }
    }
    else
    {
        debug_actor_.Reset();
    }
}

void Director::ClearStages()
{
    dispatcher_list_.Clear(false);
    stages_ = Stack<RefPtr<Stage>>();

    current_stage_.Reset();
    next_stage_.Reset();
    transition_.Reset();
    debug_actor_.Reset();
}

void Director::PushEventDispatcher(EventDispatcher* dispatcher)
{
    dispatcher_list_.PushBack(dispatcher);
}

void Director::OnUpdate(UpdateModuleContext& ctx)
{
    dispatcher_list_.Clear(false);

    if (transition_)
    {
        transition_->Update(ctx.dt);

        if (transition_->IsDone())
            transition_ = nullptr;
    }

    if (!transition_ && next_stage_)
    {
        if (current_stage_)
        {
            current_stage_->OnExit();
        }

        next_stage_->OnEnter();

        current_stage_ = next_stage_;
        next_stage_    = nullptr;
    }

    if (!transition_)
    {
        if (current_stage_)
            current_stage_->Update(ctx.dt);

        if (next_stage_)
            next_stage_->Update(ctx.dt);
    }

    if (debug_actor_)
        debug_actor_->Update(ctx.dt);
}

void Director::HandleEvent(EventModuleContext& ctx)
{
    for (auto dispatcher : dispatcher_list_)
    {
        dispatcher->DispatchEvent(ctx.evt);
    }
}

}  // namespace kiwano
