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

#include "Application.h"
#include "modules.h"
#include "../base/logs.h"
#include "../base/input.h"
#include "../renderer/render.h"
#include "../2d/Scene.h"
#include "../2d/DebugNode.h"
#include "../2d/Transition.h"
#include <windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM
#include <imm.h>  // ImmAssociateContext
#include <mutex>  // std::mutex

#pragma comment(lib, "imm32.lib")

namespace kiwano
{
	using FunctionToPerform = Closure<void()>;

	namespace
	{
		std::mutex				 perform_mutex_;
		Queue<FunctionToPerform> functions_to_perform_;
	}
}

namespace kiwano
{
	Application::Application()
		: end_(true)
		, inited_(false)
		, time_scale_(1.f)
	{
		ThrowIfFailed(
			::CoInitialize(nullptr)
		);

		Use(Renderer::Instance());
		Use(Input::Instance());
	}

	Application::~Application()
	{
		Destroy();

		::CoUninitialize();
	}

	void Application::Init(const Options& options)
	{
		ThrowIfFailed(
			Window::Instance()->Create(
				options.title,
				options.width,
				options.height,
				options.icon,
				options.fullscreen,
				Application::WndProc
			)
		);

		Renderer::Instance()->SetClearColor(options.clear_color);
		Renderer::Instance()->SetVSyncEnabled(options.vsync);

		// Setup all components
		for (Component* c : components_)
		{
			c->SetupComponent(this);
		}

		// Everything is ready
		OnStart();

		HWND hwnd = Window::Instance()->GetHandle();

		// disable imm
		::ImmAssociateContext(hwnd, nullptr);

		// use Application instance in message loop
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(this));

		inited_ = true;
	}

	void Application::Run()
	{
		HWND hwnd = Window::Instance()->GetHandle();

		if (!hwnd)
			throw std::exception("Calling Application::Run before Application::Init");

		if (hwnd)
		{
			end_ = false;
			Window::Instance()->Prepare();

			MSG msg = {};
			while (::GetMessageW(&msg, nullptr, 0, 0) && !end_)
			{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}
	}

	void Application::Quit()
	{
		end_ = true;
	}

	void Application::Destroy()
	{
		transition_.Reset();
		next_scene_.Reset();
		curr_scene_.Reset();
		debug_node_.Reset();

		OnDestroy();

		if (inited_)
		{
			inited_ = false;

			for (auto iter = components_.rbegin(); iter != components_.rend(); ++iter)
			{
				(*iter)->DestroyComponent();
			}
			components_.clear();
		}

		// Destroy all instances
		Input::Destroy();
		Renderer::Destroy();
		Window::Destroy();

		// DO NOT destroy Logger instance manually
		// Logger::Destroy();
	}

	void Application::Use(Component* component)
	{
		if (component)
		{

#if defined(KGE_DEBUG)
			if (components_.contains(component))
			{
				KGE_ASSERT(false && "Component already exists!");
			}
#endif

			components_.push_back(component);
		}
	}

	void Application::Remove(Component* component)
	{
		if (component)
		{
			for (auto iter = components_.begin(); iter != components_.end(); ++iter)
			{
				if ((*iter) == component)
				{
					(*iter)->DestroyComponent();
					components_.erase(iter);
					break;
				}
			}
		}
	}

	void Application::EnterScene(ScenePtr scene)
	{
		KGE_ASSERT(scene && "Application::EnterScene failed, NULL pointer exception");

		if (curr_scene_ == scene || next_scene_ == scene)
			return;

		next_scene_ = scene;
	}

	void Application::EnterScene(ScenePtr scene, TransitionPtr transition)
	{
		EnterScene(scene);
		
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

	ScenePtr Application::GetCurrentScene()
	{
		return curr_scene_;
	}

	void Application::SetTimeScale(float scale_factor)
	{
		time_scale_ = scale_factor;
	}

	void Application::ShowDebugInfo(bool show)
	{
		if (show)
		{
			debug_node_ = new DebugNode;
			Renderer::Instance()->SetCollectingStatus(true);
		}
		else
		{
			debug_node_.Reset();
			Renderer::Instance()->SetCollectingStatus(false);
		}
	}

	void Application::Dispatch(Event& evt)
	{
		if (debug_node_)
			debug_node_->Dispatch(evt);

		if (curr_scene_)
			curr_scene_->Dispatch(evt);
	}

	void Application::Update()
	{
		static auto last = Time::Now();

		const auto now = Time::Now();
		const auto dt = (now - last) * time_scale_;
		last = now;

		// Before update
		for (Component* c : components_)
		{
			c->BeforeUpdate(dt.Seconds());
		}

		// Updating
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

		OnUpdate(dt);

		if (curr_scene_)
			curr_scene_->Update(dt);

		if (next_scene_)
			next_scene_->Update(dt);

		if (debug_node_)
			debug_node_->Update(dt);

		// After update
		for (auto rit = components_.rbegin(); rit != components_.rend(); ++rit)
		{
			(*rit)->AfterUpdate();
		}
	}

	void Application::Render()
	{
		// Before render
		for (Component* c : components_)
		{
			c->BeforeRender();
		}

		// Rendering
		if (transition_)
		{
			transition_->Render();
		}
		else if (curr_scene_)
		{
			curr_scene_->Render();
		}

		OnRender();

		if (debug_node_)
			debug_node_->Render();

		// After render
		for (auto rit = components_.rbegin(); rit != components_.rend(); ++rit)
		{
			(*rit)->AfterRender();
		}
	}

	void Application::PreformInMainThread(Closure<void()> function)
	{
		std::lock_guard<std::mutex> lock(perform_mutex_);
		functions_to_perform_.push(function);
	}

	LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		Application * app = reinterpret_cast<Application*>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

		if (!app)
			return ::DefWindowProcW(hwnd, msg, wparam, lparam);

		// Handle Message
		for (Component* c : app->components_)
		{
			c->HandleMessage(hwnd, msg, wparam, lparam);
		}
		
		switch (msg)
		{
		case WM_PAINT:
		{
			app->Update();
			app->Render();

			::InvalidateRect(hwnd, NULL, FALSE);
			return 0;
		}
		break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			if (!app->transition_ && app->curr_scene_)
			{
				bool down = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
				Event evt(down ? Event::KeyDown : Event::KeyUp);
				evt.key.code = static_cast<int>(wparam);
				evt.key.count = static_cast<int>(lparam & 0xFF);

				app->Dispatch(evt);
			}
		}
		break;

		case WM_CHAR:
		{
			if (!app->transition_ && app->curr_scene_)
			{
				Event evt(Event::Char);
				evt.key.c = static_cast<char>(wparam);
				evt.key.count = static_cast<int>(lparam & 0xFF);

				app->Dispatch(evt);
			}
		}
		break;

		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		//case WM_LBUTTONDBLCLK:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		//case WM_MBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		//case WM_RBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		{
			if (!app->transition_ && app->curr_scene_)
			{
				Event evt;

				evt.mouse.x = static_cast<float>(GET_X_LPARAM(lparam));
				evt.mouse.y = static_cast<float>(GET_Y_LPARAM(lparam));
				evt.mouse.left_btn_down = !!(wparam & MK_LBUTTON);
				evt.mouse.left_btn_down = !!(wparam & MK_RBUTTON);

				if		(msg == WM_MOUSEMOVE) { evt.type = Event::MouseMove; }
				else if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN) { evt.type = Event::MouseBtnDown; }
				else if (msg == WM_LBUTTONUP   || msg == WM_RBUTTONUP   || msg == WM_MBUTTONUP) { evt.type = Event::MouseBtnUp; }
				else if (msg == WM_MOUSEWHEEL) { evt.type = Event::MouseWheel; evt.mouse.wheel = GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA; }

				if		(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) { evt.mouse.button = MouseButton::Left; }
				else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP) { evt.mouse.button = MouseButton::Right; }
				else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP) { evt.mouse.button = MouseButton::Middle; }

				app->Dispatch(evt);
			}
		}
		break;

		case WM_SIZE:
		{
			if (SIZE_MAXHIDE == wparam || SIZE_MINIMIZED == wparam)
			{
				KGE_LOG(L"Window minimized");
			}
			else
			{
				KGE_LOG(L"Window resized");

				if (app->curr_scene_)
				{
					Event evt(Event::WindowResized);
					evt.win.width = LOWORD(lparam);
					evt.win.height = HIWORD(lparam);
					app->Dispatch(evt);
				}

				Window::Instance()->UpdateWindowRect();
			}
		}
		break;

		case WM_MOVE:
		{
			if (app->curr_scene_)
			{
				int x = (int)(short)LOWORD(lparam);
				int y = (int)(short)HIWORD(lparam);

				Event evt(Event::WindowMoved);
				evt.win.x = x;
				evt.win.y = y;
				app->Dispatch(evt);
			}
		}
		break;

		case WM_ACTIVATE:
		{
			bool active = (LOWORD(wparam) != WA_INACTIVE);

			Window::Instance()->SetActive(active);

			if (app->curr_scene_)
			{
				Event evt(Event::WindowFocusChanged);
				evt.win.focus = active;
				app->Dispatch(evt);
			}
		}
		break;

		case WM_SETTEXT:
		{
			KGE_LOG(L"Window title changed");

			if (app->curr_scene_)
			{
				Event evt(Event::WindowTitleChanged);
				evt.win.title = reinterpret_cast<const wchar_t*>(lparam);
				app->Dispatch(evt);
			}
		}
		break;

		case WM_SETICON:
		{
			KGE_LOG(L"Window icon changed");
		}
		break;

		case WM_DISPLAYCHANGE:
		{
			KGE_LOG(L"The display resolution has changed");

			::InvalidateRect(hwnd, nullptr, FALSE);
		}
		break;

		case WM_CLOSE:
		{
			KGE_LOG(L"Window is closing");

			if (!app->OnClosing())
			{
				return 0;
			}
		}
		break;

		case WM_DESTROY:
		{
			KGE_LOG(L"Window was destroyed");

			if (app->curr_scene_)
			{
				Event evt(Event::WindowClosed);
				app->Dispatch(evt);
			}

			::PostQuitMessage(0);
			return 0;
		}
		break;
		}

		return ::DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}
