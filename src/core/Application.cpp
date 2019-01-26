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
#include "Factory.h"
#include "Event.hpp"
#include "Scene.h"
#include "DebugNode.h"
#include "Transition.h"
#include <windowsx.h>
#include <imm.h>

#pragma comment (lib ,"imm32.lib")

namespace easy2d
{
	Application::Application(String const& app_name)
		: debug_(false)
		, curr_scene_(nullptr)
		, next_scene_(nullptr)
		, transition_(nullptr)
		, time_scale_(1.f)
		, app_name_(app_name)
	{
		::CoInitialize(nullptr);
	}

	Application::~Application()
	{
		::CoUninitialize();
	}

	void Application::Init(const Options& options)
	{
		debug_ = options.debug;

		ThrowIfFailed(
			Factory::Instance()->Init(debug_)
		);

		ThrowIfFailed(
			Window::Instance()->Init(
				options.title,
				options.width,
				options.height,
				options.icon,
				options.fullscreen,
				Application::WndProc,
				debug_
			)
		);

		HWND hwnd = Window::Instance()->GetHandle();

		ThrowIfFailed(
			RenderSystem::Instance()->Init(
				hwnd,
				options.vsync,
				debug_
			)
		);

		ThrowIfFailed(
			Input::Instance()->Init(
				hwnd,
				debug_
			)
		);
		
		ThrowIfFailed(
			Audio::Instance()->Init(debug_)
		);

		Setup();

		// disable imm
		::ImmAssociateContext(hwnd, nullptr);

		// show console if debug mode enabled
		HWND console = ::GetConsoleWindow();
		if (debug_ && !console)
		{
			if (::AllocConsole())
			{
				console = ::GetConsoleWindow();
				FILE * stdoutStream, *stdinStream, *stderrStream;
				freopen_s(&stdoutStream, "conout$", "w+t", stdout);
				freopen_s(&stdinStream, "conin$", "r+t", stdin);
				freopen_s(&stderrStream, "conout$", "w+t", stderr);
			}
		}
		else if (!debug_ && console)
		{
			::ShowWindow(console, SW_HIDE);
		}

		// disable the close button of console
		if (console)
		{
			HMENU hmenu = ::GetSystemMenu(console, FALSE);
			::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}

		// use Application instance in message loop
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, PtrToUlong(this));
	}

	void Application::Run()
	{
		HWND hwnd = Window::Instance()->GetHandle();

		if (hwnd)
		{
			Window::Instance()->Prepare();

			MSG msg = {};
			while (::GetMessageW(&msg, nullptr, 0, 0))
			{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}
	}

	void Application::Quit()
	{
		Window::Instance()->Destroy();
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

	void Application::SetTimeScale(float scale)
	{
		time_scale_ = scale;
	}

	void Application::Update()
	{
		static auto last = time::Now();

		const auto now = time::Now();
		const auto dt = (now - last) * time_scale_;
		last = now;

		Input::Instance()->Update();

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

		if (debug_)
			DebugNode::Instance()->Update(dt);
	}

	void Application::Render(HWND hwnd)
	{
		auto rt = RenderSystem::Instance();
		
		ThrowIfFailed(
			rt->BeginDraw(hwnd)
		);

		if (transition_)
		{
			transition_->Render();
		}
		else if (curr_scene_)
		{
			curr_scene_->Render();
		}

		if (debug_)
			DebugNode::Instance()->Render();

		ThrowIfFailed(
			rt->EndDraw()
		);

		::InvalidateRect(hwnd, NULL, FALSE);
	}

	LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		Application * app = reinterpret_cast<Application*>(::GetWindowLongW(hwnd, GWLP_USERDATA));

		if (!app)
			return ::DefWindowProcW(hwnd, msg, wparam, lparam);
		
		switch (msg)
		{
		case WM_PAINT:
		{
			app->Update();
			app->Render(hwnd);
			return 0;
		}
		break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			if (!app->transition_ && app->curr_scene_)
			{
				Event evt((msg == WM_KEYDOWN) ? KeyboardEvent::Down : KeyboardEvent::Up);
				evt.key.code = KeyCode(wparam);
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
			if (!app->transition_ && app->curr_scene_)
			{
				Event evt;

				evt.mouse.x = static_cast<float>(GET_X_LPARAM(lparam));
				evt.mouse.y = static_cast<float>(GET_Y_LPARAM(lparam));
				evt.mouse.left_btn_down = !!(wparam & MK_LBUTTON);
				evt.mouse.left_btn_down = !!(wparam & MK_RBUTTON);

				if (msg == WM_MOUSEMOVE)
					evt.type = MouseEvent::Move;
				else if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN)
					evt.type = MouseEvent::Down;
				else if (msg == WM_LBUTTONUP || msg == WM_RBUTTONUP || msg == WM_MBUTTONUP)
					evt.type = MouseEvent::Up;
				else
				{
					evt.type = MouseEvent::Wheel;
					evt.mouse.wheel_delta = GET_WHEEL_DELTA_WPARAM(wparam) / 120.f;
				}

				if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
					evt.mouse.button = MouseButton::Left;
				else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
					evt.mouse.button = MouseButton::Right;
				else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP)
					evt.mouse.button = MouseButton::Middle;

				app->curr_scene_->Dispatch(evt);
			}
		}
		break;

		case WM_SIZE:
		{
			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);

			// 如果程序接收到一个 WM_SIZE 消息，这个方法将调整渲染
			// 目标的大小。它可能会调用失败，但是这里可以忽略有可能的
			// 错误，因为这个错误将在下一次调用 EndDraw 时产生
			RenderSystem::Instance()->Resize(width, height);

			if (SIZE_MAXHIDE == wparam || SIZE_MINIMIZED == wparam)
			{
				E2D_LOG(L"Window minimized");
			}
			else
			{
				E2D_LOG(L"Window resized");

				if (app->curr_scene_)
				{
					Event evt(WindowEvent::Resized);
					evt.win.width = static_cast<int>(width);
					evt.win.height = static_cast<int>(height);
					app->curr_scene_->Dispatch(evt);
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

				Event evt(WindowEvent::Moved);
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

			Window::Instance()->SetActive(active);

			if (app->curr_scene_)
			{
				Event evt(WindowEvent::FocusChanged);
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
				Event evt(WindowEvent::TitleChanged);
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
			Window::Instance()->Destroy();
			return 0;
		}
		break;

		case WM_DESTROY:
		{
			E2D_LOG(L"Window was destroyed");

			if (app->curr_scene_)
			{
				Event evt(WindowEvent::Closed);
				app->curr_scene_->Dispatch(evt);
			}

			::PostQuitMessage(0);
			return 0;
		}
		break;
		}

		return ::DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}
