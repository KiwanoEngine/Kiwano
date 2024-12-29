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

#include <kiwano/utils/Logger.h>
#include <kiwano/module/Runner.h>
#include <kiwano/module/Input.h>
#include <kiwano/platform/Application.h>
#include <kiwano/render/Renderer.h>
#include <kiwano/module/Director.h>

namespace kiwano
{

Runner::Runner() {}

Runner::Runner(const Settings& settings)
    : settings_(settings)
{
}

RefPtr<Runner> Runner::Create(const Settings& settings, const Function<void()>& on_ready)
{
    KGE_ASSERT(on_ready);
    class CallbackRunner : public Runner
    {
    public:
        Function<void()> on_ready;

        CallbackRunner(const Function<void()>& on_ready)
            : on_ready(on_ready)
        {
        }

        void OnReady() override
        {
            on_ready();
        }
    };

    RefPtr<Runner> runner = new CallbackRunner(on_ready);
    return runner;
}

void Runner::SetupModule()
{
    if (settings_.debug_mode)
    {
        // Show console window before creating main window
        Logger::GetInstance().ShowConsole(true);
    }

    // Create game window
    RefPtr<Window> window = Window::Create(settings_.window);
    SetWindow(window);

    // Update renderer settings
    Renderer::GetInstance().MakeContextForWindow(window);
    Renderer::GetInstance().SetClearColor(settings_.bg_color);
    Renderer::GetInstance().SetVSyncEnabled(settings_.vsync_enabled);

    // Use defaut modules
    Application::GetInstance().Use(*window);
    Application::GetInstance().Use(Renderer::GetInstance());
    Application::GetInstance().Use(Input::GetInstance());
    Application::GetInstance().Use(Director::GetInstance());

    // Enable debug mode
    if (settings_.debug_mode)
    {
        Director::GetInstance().ShowDebugInfo(true);
        Renderer::GetInstance().GetContext().SetCollectingStatus(true);
    }

    // Create frame ticker
    if (!settings_.frame_interval.IsZero())
    {
        frame_ticker_ = MakePtr<Ticker>(settings_.frame_interval, -1);
    }
}

void Runner::DestroyModule()
{
    this->OnDestroy();
}

void Runner::OnUpdate(UpdateModuleContext& ctx)
{
    Application& app = Application::GetInstance();
    if (!main_window_)
    {
        app.Quit();
        return;
    }

    if (main_window_->ShouldClose())
    {
        if (this->OnClose())
        {
            app.Quit();
            return;
        }

        main_window_->SetShouldClose(false);
    }

    if (!frame_ticker_)
    {
        ctx.Next();
        return;
    }

    // Update frame ticker
    if (frame_ticker_->Tick(ctx.dt))
    {
        ctx.dt = frame_ticker_->GetDeltaTime();
        ctx.Next();
    }
    else
    {
        // Releases CPU
        Duration total_dt = frame_ticker_->GetDeltaTime() + frame_ticker_->GetErrorTime();
        Duration sleep_dt = frame_ticker_->GetInterval() - total_dt;
        if (sleep_dt.GetMilliseconds() > 1LL)
        {
            sleep_dt.Sleep();
        }
        ctx.Abort();
    }
}

}  // namespace kiwano
