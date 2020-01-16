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

#include <mutex>
#include <kiwano/platform/Application.h>
#include <kiwano/platform/Input.h>
#include <kiwano/core/Director.h>
#include <kiwano/core/Logger.h>
#include <kiwano/renderer/TextureCache.h>
#include <kiwano/utils/ResourceCache.h>

namespace kiwano
{
	namespace
	{
		using FunctionToPerform = Function<void()>;

		std::mutex				 perform_mutex_;
		Queue<FunctionToPerform> functions_to_perform_;
	}

	Config::Config(String const& title, uint32_t width, uint32_t height, uint32_t icon)
		: debug(false)
	{
		window.title = title;
		window.width = width;
		window.height = height;
		window.icon = icon;
	}

	Config::Config(WindowConfig const& wnd_config, RenderConfig const& render_config)
		: debug(false)
	{
		window = wnd_config;
		render = render_config;
	}
}

namespace kiwano
{
	Application::Application()
		: time_scale_(1.f)
	{
		Use(&Renderer::instance());
		Use(&Input::instance());
		Use(&Director::instance());
	}

	Application::~Application()
	{
		Destroy();
	}

	void Application::Run(const Config& config)
	{
		Window::instance().Init(config.window);
		Renderer::instance().Init(config.render);

		// Setup all components
		for (auto c : comps_)
		{
			c->SetupComponent();
		}

		if (config.debug)
		{
			Director::instance().ShowDebugInfo(true);
			Renderer::instance().SetCollectingStatus(true);
		}

		// Everything is ready
		OnReady();

		last_update_time_ = Time::Now();

		Window& window = Window::instance();
		while (!window.ShouldClose())
		{
			while (EventPtr evt = window.PollEvent())
			{
				DispatchEvent(evt.get());
			}

			Update();
			Render();
		}
	}

	void Application::Quit()
	{
		Window::instance().Destroy();
	}

	void Application::Destroy()
	{
		// Clear all resources
		Director::instance().ClearStages();
		ResourceCache::instance().Clear();
		TextureCache::instance().Clear();

		for (auto iter = comps_.rbegin(); iter != comps_.rend(); ++iter)
		{
			(*iter)->DestroyComponent();
		}
		comps_.clear();
	}

	void Application::Use(ComponentBase* component)
	{
		if (component)
		{

#if defined(KGE_DEBUG)
			if (comps_.contains(component))
			{
				KGE_ASSERT(false && "Component already exists!");
			}
#endif

			comps_.push_back(component);

			if (component->Check(RenderComponent::flag))
				render_comps_.push_back(dynamic_cast<RenderComponent*>(component));

			if (component->Check(UpdateComponent::flag))
				update_comps_.push_back(dynamic_cast<UpdateComponent*>(component));

			if (component->Check(EventComponent::flag))
				event_comps_.push_back(dynamic_cast<EventComponent*>(component));
		}
	}

	void Application::SetTimeScale(float scale_factor)
	{
		time_scale_ = scale_factor;
	}

	void Application::Update()
	{
		// Before update
		for (auto c : update_comps_)
		{
			c->BeforeUpdate();
		}

		// perform functions
		{
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

		// Updating
		{
			const Time now = Time::Now();
			const Duration dt = (now - last_update_time_) * time_scale_;

			last_update_time_ = now;

			for (auto c : update_comps_)
			{
				c->OnUpdate(dt);
			}
		}

		// After update
		for (auto rit = update_comps_.rbegin(); rit != update_comps_.rend(); ++rit)
		{
			(*rit)->AfterUpdate();
		}
	}

	void Application::Render()
	{
		// Before render
		for (auto c : render_comps_)
		{
			c->BeforeRender();
		}

		// Rendering
		Renderer& renderer = Renderer::instance();
		for (auto c : render_comps_)
		{
			c->OnRender(renderer);
		}

		// After render
		for (auto rit = render_comps_.rbegin(); rit != render_comps_.rend(); ++rit)
		{
			(*rit)->AfterRender();
		}
	}

	void Application::DispatchEvent(Event* evt)
	{
		for (auto c : event_comps_)
		{
			c->HandleEvent(evt);
		}
	}

	void Application::PreformInMainThread(Function<void()> func)
	{
		std::lock_guard<std::mutex> lock(perform_mutex_);
		functions_to_perform_.push(func);
	}
}
