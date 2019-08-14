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
#include "../base/Window.h"
#include "../base/Logger.h"
#include "../base/input.h"
#include "../base/Director.h"
#include "../renderer/Renderer.h"
#include "../utils/ResourceCache.h"
#include <windowsx.h>  // GET_X_LPARAM, GET_Y_LPARAM
#include <imm.h>  // ImmAssociateContext
#include <mutex>  // std::mutex

#pragma comment(lib, "imm32.lib")

namespace kiwano
{
	namespace
	{
		using FunctionToPerform = Function<void()>;

		std::mutex				 perform_mutex_;
		Queue<FunctionToPerform> functions_to_perform_;
	}

	Options::Options(String const& title, int width, int height, LPCWSTR icon, Color clear_color, bool vsync, bool fullscreen, bool debug)
		: title(title)
		, width(width)
		, height(height)
		, icon(icon)
		, clear_color(clear_color)
		, vsync(vsync)
		, fullscreen(fullscreen)
		, debug(debug)
	{}
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

		Use(Renderer::GetInstance());
		Use(Input::GetInstance());
		Use(Director::GetInstance());
	}

	Application::~Application()
	{
		Destroy();

		::CoUninitialize();
	}

	void Application::Init(const Options& options)
	{
		ThrowIfFailed(
			Window::GetInstance()->Create(
				options.title,
				options.width,
				options.height,
				options.icon,
				options.fullscreen,
				Application::WndProc
			)
		);

		Renderer::GetInstance()->SetClearColor(options.clear_color);
		Renderer::GetInstance()->SetVSyncEnabled(options.vsync);

		// Setup all components
		for (Component* c : components_)
		{
			c->SetupComponent();
		}

		if (options.debug)
		{
			Director::GetInstance()->ShowDebugInfo(true);
			Renderer::GetInstance()->SetCollectingStatus(true);
		}

		// Everything is ready
		OnReady();

		HWND hwnd = Window::GetInstance()->GetHandle();

		// disable imm
		::ImmAssociateContext(hwnd, nullptr);

		// use Application instance in message loop
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(this));

		inited_ = true;
	}

	void Application::Run()
	{
		HWND hwnd = Window::GetInstance()->GetHandle();

		if (!inited_)
			throw std::exception("Calling Application::Run before Application::Init");

		if (hwnd)
		{
			end_ = false;
			Window::GetInstance()->Prepare();

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
		// Clear all stages
		Director::GetInstance()->ClearStages();

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
		Director::DestroyInstance();
		ResourceCache::DestroyInstance();
		Input::DestroyInstance();
		Renderer::DestroyInstance();
		Window::DestroyInstance();

		// DO NOT destroy Logger instance manually
		// Logger::DestroyInstance();
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

	void Application::SetTimeScale(float scale_factor)
	{
		time_scale_ = scale_factor;
	}

	void Application::Update()
	{
		// Before update
		for (Component* c : components_)
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
			static auto last = Time::Now();

			const auto now = Time::Now();
			const auto dt = (now - last) * time_scale_;
			last = now;

			for (Component* c : components_)
			{
				c->OnUpdate(dt);
			}
		}

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
		Renderer* renderer = Renderer::GetInstance();
		for (Component* c : components_)
		{
			c->OnRender(renderer);
		}

		// After render
		for (auto rit = components_.rbegin(); rit != components_.rend(); ++rit)
		{
			(*rit)->AfterRender();
		}
	}

	void Application::DispatchEvent(Event& evt)
	{
		for (Component* c : components_)
		{
			c->HandleEvent(evt);
		}
	}

	void Application::PreformInMainThread(Function<void()> Function)
	{
		std::lock_guard<std::mutex> lock(perform_mutex_);
		functions_to_perform_.push(Function);
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
			bool down = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
			Event evt(down ? Event::KeyDown : Event::KeyUp);
			evt.key.code = static_cast<int>(wparam);
			evt.key.count = static_cast<int>(lparam & 0xFF);

			app->DispatchEvent(evt);
		}
		break;

		case WM_CHAR:
		{
			Event evt(Event::Char);
			evt.key.c = static_cast<char>(wparam);
			evt.key.count = static_cast<int>(lparam & 0xFF);

			app->DispatchEvent(evt);
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
			Event evt;

			evt.mouse.x = static_cast<float>(GET_X_LPARAM(lparam));
			evt.mouse.y = static_cast<float>(GET_Y_LPARAM(lparam));
			evt.mouse.left_btn_down = !!(wparam & MK_LBUTTON);
			evt.mouse.left_btn_down = !!(wparam & MK_RBUTTON);

			if (msg == WM_MOUSEMOVE) { evt.type = Event::MouseMove; }
			else if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN) { evt.type = Event::MouseBtnDown; }
			else if (msg == WM_LBUTTONUP || msg == WM_RBUTTONUP || msg == WM_MBUTTONUP) { evt.type = Event::MouseBtnUp; }
			else if (msg == WM_MOUSEWHEEL) { evt.type = Event::MouseWheel; evt.mouse.wheel = GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA; }

			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) { evt.mouse.button = MouseButton::Left; }
			else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP) { evt.mouse.button = MouseButton::Right; }
			else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP) { evt.mouse.button = MouseButton::Middle; }

			app->DispatchEvent(evt);
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

				Window::GetInstance()->UpdateWindowRect();

				Event evt(Event::WindowResized);
				evt.win.width = LOWORD(lparam);
				evt.win.height = HIWORD(lparam);
				app->DispatchEvent(evt);
			}
		}
		break;

		case WM_MOVE:
		{
			int x = (int)(short)LOWORD(lparam);
			int y = (int)(short)HIWORD(lparam);

			Event evt(Event::WindowMoved);
			evt.win.x = x;
			evt.win.y = y;
			app->DispatchEvent(evt);
		}
		break;

		case WM_ACTIVATE:
		{
			bool active = (LOWORD(wparam) != WA_INACTIVE);

			Window::GetInstance()->SetActive(active);

			Event evt(Event::WindowFocusChanged);
			evt.win.focus = active;
			app->DispatchEvent(evt);
		}
		break;

		case WM_SETTEXT:
		{
			KGE_LOG(L"Window title changed");

			Event evt(Event::WindowTitleChanged);
			evt.win.title = reinterpret_cast<const wchar_t*>(lparam);
			app->DispatchEvent(evt);
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

			Event evt(Event::WindowClosed);
			app->DispatchEvent(evt);
			app->OnDestroy();

			::PostQuitMessage(0);
			return 0;
		}
		break;
		}

		return ::DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}
