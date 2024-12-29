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
    , runner_(nullptr)
{
}

void Application::Run(Runner& runner)
{
    running_   = true;
    is_paused_ = false;
    runner_    = &runner;
    timer_     = MakePtr<Timer>();

    // Ensure resources are destroyed before exiting
    KGE_DEFER[=]()
    {
        this->Destroy();
    };

    Use(runner, 0);

    // Setup all modules
    for (size_t i = 0; i < modules_.size(); ++i)
    {
        modules_[i]->SetupModule();
    }

    // Everything is ready
    runner_->OnReady();

    // Start the loop
    while (running_)
    {
        timer_->Tick();
        this->Update(timer_->GetDeltaTime());
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

void Application::Destroy()
{
    // Clear user resources
    for (auto iter = modules_.rbegin(); iter != modules_.rend(); ++iter)
    {
        (*iter)->DestroyModule();
    }
    modules_.clear();

    runner_ = nullptr;
}

void Application::Use(Module& m, int index)
{
#if defined(KGE_DEBUG)
    if (std::find(modules_.begin(), modules_.end(), &m) != modules_.end())
    {
        KGE_ASSERT(false && "Module already existed!");
    }
#endif

    if (index < 0)
        modules_.push_back(&m);
    else
        modules_.insert(modules_.begin() + index, &m);
}

const ModuleList& Application::GetModules() const
{
    return modules_;
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
        Queue<Function<void()>> functions = std::move(functions_to_perform_);
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

void Application::PerformInMainThread(Function<void()> func)
{
    std::lock_guard<std::mutex> lock(perform_mutex_);
    functions_to_perform_.push(func);
}

}  // namespace kiwano
