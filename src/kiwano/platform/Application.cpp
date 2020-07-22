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

#include <kiwano/platform/Application.h>
#include <kiwano/core/Defer.h>
#include <kiwano/base/Director.h>
#include <kiwano/base/ObjectPool.h>
#include <kiwano/render/Renderer.h>
#include <kiwano/render/TextureCache.h>
#include <kiwano/utils/ResourceCache.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

int GetVersion()
{
    return KGE_VERSION;
}

Application::Application()
    : running_(false)
    , is_paused_(false)
    , time_scale_(1.f)
{
}

Application::~Application()
{
}

void Application::Run(RunnerPtr runner)
{
    KGE_ASSERT(runner);
    running_   = true;
    is_paused_ = false;
    runner_    = runner;
    timer_     = MakePtr<Timer>();

    // Initialize runner
    runner->InitSettings();

    // Setup all modules
    for (auto c : modules_)
    {
        c->SetupModule();
    }

    // Ensure resources are destroyed before exiting
    KGE_DEFER[=]()
    {
        this->Destroy();
    };

    // Everything is ready
    runner->OnReady();

    // Update everything
    this->Update(0);

    // Start the loop
    while (running_)
    {
        timer_->Tick();

        // Execute main loop
        if (!runner->MainLoop(timer_->GetDeltaTime()))
            running_ = false;

        // Clear objects
        ObjectPool::GetInstance().Clear();
    }
}

void Application::Pause()
{
    is_paused_ = true;

    if (timer_)
        timer_->Pause();
}

void Application::Resume()
{
    is_paused_ = false;

    if (timer_)
        timer_->Resume();
}

void Application::Quit()
{
    running_ = false;
}

void Application::UpdateFrame(Duration dt)
{
    this->Render();
    this->Update(dt);
}

void Application::Destroy()
{
    if (runner_)
    {
        runner_->OnDestroy();
        runner_ = nullptr;
    }

    // Clear user resources
    Director::GetInstance().ClearStages();
    ResourceCache::GetInstance().Clear();

    for (auto iter = modules_.rbegin(); iter != modules_.rend(); ++iter)
    {
        (*iter)->DestroyModule();
    }
    modules_.clear();

    // Clear device resources
    TextureCache::GetInstance().Clear();
    Renderer::GetInstance().Destroy();

    // Clear objects
    ObjectPool::GetInstance().Clear();
}

void Application::Use(Module& module)
{
#if defined(KGE_DEBUG)
    if (std::find(modules_.begin(), modules_.end(), &module) != modules_.end())
    {
        KGE_ASSERT(false && "Module already exists!");
    }
#endif

    modules_.push_back(&module);
}

void Application::SetTimeScale(float scale_factor)
{
    time_scale_ = scale_factor;
}

void Application::DispatchEvent(EventPtr evt)
{
    this->DispatchEvent(evt.Get());
}

void Application::DispatchEvent(Event* evt)
{
    if (!running_ /* Dispatch events even if application is paused */)
        return;

    auto ctx = EventModuleContext(modules_, evt);
    ctx.Next();
}

void Application::Update(Duration dt)
{
    if (!running_ || is_paused_)
        return;

    auto ctx = UpdateModuleContext(modules_, dt);
    ctx.Next();

    // perform functions
    if (!functions_to_perform_.empty())
    {
        perform_mutex_.lock();
        auto functions = std::move(functions_to_perform_);
        perform_mutex_.unlock();

        while (!functions.empty())
        {
            auto& func = functions.front();
            if (func)
            {
                func();
            }
            functions.pop();
        }
    }
}

void Application::Render()
{
    if (!running_ /* Render even if application is paused */)
        return;

    Renderer& renderer = Renderer::GetInstance();
    renderer.Clear();

    {
        auto ctx = RenderModuleContext(modules_, renderer.GetContext());
        ctx.Next();
    }

    renderer.Present();
}

void Application::PreformInMainThread(Function<void()> func)
{
    std::lock_guard<std::mutex> lock(perform_mutex_);
    functions_to_perform_.push(func);
}

}  // namespace kiwano
