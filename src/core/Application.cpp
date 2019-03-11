// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include "logs.h"
#include "modules.h"
#include "render.h"
#include "window.h"
#include "input.h"
#include "Event.hpp"
#include "Scene.h"
#include "DebugNode.h"
#include "Transition.h"
#include <windowsx.h>
#include <imm.h>
#include <iostream>

#pragma comment (lib ,"imm32.lib")

namespace easy2d
{
	namespace
	{
		LRESULT(*pre_proc)(HWND, UINT, WPARAM, LPARAM) = nullptr;	// Custom message proc for user
	}

	Application::Application(String const& app_name)
		: end_(true)
		, inited_(false)
		, curr_scene_(nullptr)
		, next_scene_(nullptr)
		, transition_(nullptr)
		, time_scale_(1.f)
		, app_name_(app_name)
	{
		::CoInitialize(nullptr);

		Use(&Renderer::Instance());
		Use(&Input::Instance());
	}

	Application::~Application()
	{
		Destroy();

		::CoUninitialize();
	}

	void Application::Init(const Options& options)
	{
		ThrowIfFailed(
			Window::Instance().Create(
				options.title,
				options.width,
				options.height,
				options.icon,
				options.fullscreen,
				Application::WndProc
			)
		);

		HWND hwnd = Window::Instance().GetHandle();

		Renderer::Instance().SetTargetWindow(hwnd);
		Renderer::Instance().SetClearColor(options.clear_color);
		Renderer::Instance().SetVSyncEnabled(options.vsync);

		// Setup all components
		for (Component* c : components_)
		{
			c->Setup();
		}

		OnStart();

		// disable imm
		::ImmAssociateContext(hwnd, nullptr);

		// use Application instance in message loop
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(this));

		inited_ = true;
	}

	void Application::Run()
	{
		HWND hwnd = Window::Instance().GetHandle();

		if (hwnd)
		{
			end_ = false;

			Window::Instance().Prepare();

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
		if (inited_)
		{
			inited_ = false;

			if (curr_scene_)
				curr_scene_->OnExit();

			transition_.Reset();
			next_scene_.Reset();
			curr_scene_.Reset();
			debug_node_.Reset();

			for (auto iter = components_.rbegin(); iter != components_.rend(); ++iter)
			{
				(*iter)->Destroy();
			}

			Window::Instance().Destroy();
		}
	}

	void Application::Use(Component* component)
	{
		if (component)
		{

#if defined(E2D_DEBUG)
			if (components_.contains(component))
			{
				E2D_ASSERT(false && "Component already exists!");
			}
#endif

			components_.push_back(component);
		}
	}

	void Application::Remove(Component* component)
	{
		if (component)
		{
			for (auto iter = components_.begin(); iter != components_.end(); iter++)
			{
				if ((*iter) == component)
				{
					(*iter)->Destroy();
					components_.erase(iter);
					break;
				}
			}
		}
	}

	void Application::EnterScene(ScenePtr const & scene)
	{
		E2D_ASSERT(scene && "Application::EnterScene failed, NULL pointer exception");

		if (curr_scene_ == scene || next_scene_ == scene)
			return;

		next_scene_ = scene;
	}

	void Application::EnterScene(ScenePtr const& scene, TransitionPtr const& transition)
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

	ScenePtr const& Application::GetCurrentScene()
	{
		return curr_scene_;
	}

	void Application::SetTimeScale(float scale_factor)
	{
		time_scale_ = scale_factor;
	}

	void Application::SetPreMessageProc(LRESULT(*proc)(HWND, UINT, WPARAM, LPARAM))
	{
		pre_proc = proc;
	}

	void Application::ShowDebugInfo(bool show)
	{
		if (show)
		{
			debug_node_ = new DebugNode;
			Renderer::Instance().StartCollectData();
		}
		else
		{
			debug_node_.Reset();
			Renderer::Instance().StopCollectData();
		}
	}

	void Application::Update()
	{
		static auto last = time::Now();

		const auto now = time::Now();
		const auto dt = (now - last) * time_scale_;
		last = now;

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

		OnUpdate(dt);

		if (curr_scene_)
			curr_scene_->Update(dt);

		if (next_scene_)
			next_scene_->Update(dt);

		if (debug_node_)
			debug_node_->Update(dt);

		Input::Instance().Update();
	}

	void Application::Render()
	{
		ThrowIfFailed(
			Renderer::Instance().BeginDraw()
		);

		if (transition_)
		{
			transition_->Render();
		}
		else if (curr_scene_)
		{
			curr_scene_->Render();
		}

		if (debug_node_)
			debug_node_->Render();

		OnRender();

		ThrowIfFailed(
			Renderer::Instance().EndDraw()
		);
	}

	void Application::AllocConsole()
	{
		if (!::GetConsoleWindow())
		{
			if (!::AllocConsole())
			{
				E2D_WARNING_LOG(L"AllocConsole failed");
			}
			else
			{
				HWND console = ::GetConsoleWindow();
				FILE * dummy;
				freopen_s(&dummy, "CONOUT$", "w+t", stdout);
				freopen_s(&dummy, "CONIN$", "r+t", stdin);
				freopen_s(&dummy, "CONOUT$", "w+t", stderr);
				(void)dummy;

				std::cout.clear();
				std::wcout.clear();
				std::cin.clear();
				std::wcin.clear();
				std::cerr.clear();
				std::wcerr.clear();

				// disable the close button of console
				if (console)
				{
					HMENU hmenu = ::GetSystemMenu(console, FALSE);
					::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
				}
			}
		}
	}

	LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (pre_proc && pre_proc(hwnd, msg, wparam, lparam))
			return 1;

		Application * app = reinterpret_cast<Application*>(
			static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA))
			);

		if (!app)
			return ::DefWindowProcW(hwnd, msg, wparam, lparam);
		
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
		case WM_KEYUP:
		{
			Input::Instance().UpdateKey((int)wparam, (msg == WM_KEYDOWN) ? true : false);

			if (!app->transition_ && app->curr_scene_)
			{
				Event evt((msg == WM_KEYDOWN) ? Event::KeyDown : Event::KeyUp);
				evt.key.code = static_cast<int>(wparam);
				evt.key.count = static_cast<int>(lparam & 0xFF);

				app->curr_scene_->Dispatch(evt);
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
			if		(msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) { Input::Instance().UpdateKey(VK_LBUTTON, (msg == WM_LBUTTONDOWN) ? true : false); }
			else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP) { Input::Instance().UpdateKey(VK_RBUTTON, (msg == WM_RBUTTONDOWN) ? true : false); }
			else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP) { Input::Instance().UpdateKey(VK_MBUTTON, (msg == WM_MBUTTONDOWN) ? true : false); }

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

				app->curr_scene_->Dispatch(evt);
			}

			if (msg == WM_MOUSEMOVE)
			{
				Input::Instance().UpdateMousePos(
					static_cast<float>(GET_X_LPARAM(lparam)),
					static_cast<float>(GET_Y_LPARAM(lparam))
				);
			}
		}
		break;

		case WM_SIZE:
		{
			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);

			Renderer::Instance().GetDeviceResources()->SetLogicalSize(Size{ (float)width, (float)height });

			if (SIZE_MAXHIDE == wparam || SIZE_MINIMIZED == wparam)
			{
				E2D_LOG(L"Window minimized");
			}
			else
			{
				E2D_LOG(L"Window resized");

				if (app->curr_scene_)
				{
					Event evt(Event::WindowResized);
					evt.win.width = static_cast<int>(width);
					evt.win.height = static_cast<int>(height);
					app->curr_scene_->Dispatch(evt);
				}

				Window::Instance().UpdateWindowRect();
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
				app->curr_scene_->Dispatch(evt);
			}
		}
		break;

		case WM_ACTIVATE:
		{
			bool active = (LOWORD(wparam) != WA_INACTIVE);

			E2D_LOG(active ? L"Window activated" : L"Window deactivated");

			Window::Instance().SetActive(active);

			if (app->curr_scene_)
			{
				Event evt(Event::WindowFocusChanged);
				evt.win.focus = active;
				app->curr_scene_->Dispatch(evt);
			}
		}
		break;

		case WM_SETTEXT:
		{
			E2D_LOG(L"Window title changed");

			if (app->curr_scene_)
			{
				Event evt(Event::WindowTitleChanged);
				evt.win.title = reinterpret_cast<const wchar_t*>(lparam);
				app->curr_scene_->Dispatch(evt);
			}
		}
		break;

		case WM_SETICON:
		{
			E2D_LOG(L"Window icon changed");
		}
		break;

		case WM_DISPLAYCHANGE:
		{
			E2D_LOG(L"The display resolution has changed");

			::InvalidateRect(hwnd, nullptr, FALSE);
		}
		break;

		case WM_CLOSE:
		{
			E2D_LOG(L"Window is closing");

			if (app->OnClosing())
			{
				Window::Instance().Destroy();
			}
			return 0;
		}
		break;

		case WM_DESTROY:
		{
			E2D_LOG(L"Window was destroyed");

			if (app->curr_scene_)
			{
				Event evt(Event::WindowClosed);
				app->curr_scene_->Dispatch(evt);
			}

			app->OnDestroy();

			::PostQuitMessage(0);
			return 0;
		}
		break;
		}

		return ::DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}
