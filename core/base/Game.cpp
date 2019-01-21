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

#include "Game.h"
#include "logs.h"
#include "modules.h"
#include "Factory.h"
#include "Scene.h"
#include "DebugNode.h"
#include "Transition.h"
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "../math/Matrix.hpp"
#include <windowsx.h>
#include <imm.h>

#pragma comment (lib ,"imm32.lib")

namespace easy2d
{
	Game::Game()
		: active_(false)
		, debug_(false)
		, curr_scene_(nullptr)
		, next_scene_(nullptr)
		, transition_(nullptr)
		, time_scale_(1.f)
	{
		::CoInitialize(nullptr);
	}

	Game::Game(Options const & options)
		: Game()
	{
		Init(options);
	}

	Game::~Game()
	{
		::CoUninitialize();
	}

	void Game::Init(const Options& options)
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
				Game::WndProc,
				debug_
			)
		);

		HWND hwnd = Window::Instance()->GetHandle();

		ThrowIfFailed(
			Graphics::Instance()->Init(
				hwnd,
				options.vsync,
				debug_
			)
		);

		ThrowIfFailed(
			Input::Instance()->Init(
				hwnd,
				Window::Instance()->GetContentScaleX(),
				Window::Instance()->GetContentScaleY(),
				debug_
			)
		);
		
		ThrowIfFailed(
			Audio::Instance()->Init(debug_)
		);

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

		// use Game instance in message loop
		::SetWindowLongW(hwnd, GWLP_USERDATA, PtrToUlong(this));
	}

	void Game::Run()
	{
		HWND hwnd = Window::Instance()->GetHandle();

		if (hwnd)
		{
			::ShowWindow(hwnd, SW_SHOWNORMAL);
			::UpdateWindow(hwnd);

			MSG msg = {};
			while (::GetMessageW(&msg, nullptr, 0, 0))
			{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}
	}

	void Game::Quit()
	{
		Window::Instance()->Destroy();
	}

	void Game::EnterScene(SpScene const & scene)
	{
		E2D_ASSERT(scene && "Game::EnterScene failed, NULL pointer exception");

		if (curr_scene_ == scene || next_scene_ == scene)
			return;

		next_scene_ = scene;
	}

	void Game::EnterScene(SpScene const& scene, SpTransition const& transition)
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

	SpScene const& Game::GetCurrentScene()
	{
		return curr_scene_;
	}

	void Game::SetTimeScale(float scale)
	{
		time_scale_ = scale;
	}

	void Game::Update()
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

	void Game::Render(HWND hwnd)
	{
		auto graphics = Graphics::Instance();
		
		ThrowIfFailed(
			graphics->BeginDraw(hwnd)
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
			graphics->EndDraw()
		);

		if (active_)
			::InvalidateRect(hwnd, NULL, FALSE);
	}

	void Game::Dispatch(Event * event)
	{
		if (transition_)
			return;

		if (curr_scene_)
			curr_scene_->DispatchEvent(event);
	}

	LRESULT CALLBACK Game::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		Game * game = reinterpret_cast<Game*>(::GetWindowLongW(hwnd, GWLP_USERDATA));

		if (!game)
			return ::DefWindowProcW(hwnd, msg, wparam, lparam);
		
		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint(hwnd, &ps);

			game->Update();
			game->Render(hwnd);

			::EndPaint(hwnd, &ps);

			return 0;
		}
		break;

		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		//case WM_LBUTTONDBLCLK:
		//case WM_MBUTTONUP:
		//case WM_MBUTTONDOWN:
		//case WM_MBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		//case WM_RBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		{
			float x = GET_X_LPARAM(lparam) * Window::Instance()->GetContentScaleX();
			float y = GET_Y_LPARAM(lparam) * Window::Instance()->GetContentScaleY();
			float wheel_delta = 0.f;

			MouseEvent::Type type;
			if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN)
				type = MouseEvent::Down;
			else if (msg == WM_LBUTTONUP || msg == WM_RBUTTONUP)
				type = MouseEvent::Up;
			else if (msg == WM_MOUSEMOVE)
				type = MouseEvent::Move;
			else
			{
				type = MouseEvent::Wheel;
				wheel_delta = GET_WHEEL_DELTA_WPARAM(wparam) / 120.f;
			}

			MouseEvent event(type, x, y, wheel_delta);

			if (wparam & MK_LBUTTON || wparam & MK_RBUTTON)
				event.button_down = true;

			game->Dispatch(&event);
		}
		break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			KeyEvent event(msg, KeyCode(wparam));
			game->Dispatch(&event);
		}
		break;

		case WM_DISPLAYCHANGE:
		{
			E2D_LOG("The display resolution has changed");

			::InvalidateRect(hwnd, nullptr, FALSE);
		}
		break;

		case WM_CLOSE:
		{
			E2D_LOG("Received a message to close the window");

			SysEvent event(SysEvent::WindowClose);
			game->Dispatch(&event);

			if (game->OnClose())
			{
				::DestroyWindow(hwnd);
			}
			return 0;
		}
		break;

		case WM_DESTROY:
		{
			E2D_LOG("Window was destroyed");

			game->OnExit();
			::PostQuitMessage(0);
			return 0;
		}
		break;

		case WM_SIZE:
		{
			if (SIZE_MAXHIDE == wparam || SIZE_MINIMIZED == wparam)
			{
				game->active_ = false;

				E2D_LOG("Window minimized");
			}
			else if (SIZE_RESTORED == wparam)
			{
				game->active_ = true;
				::InvalidateRect(hwnd, nullptr, FALSE);

				E2D_LOG("Window restored");
			}

			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);

			// 如果程序接收到一个 WM_SIZE 消息，这个方法将调整渲染
			// 目标的大小。它可能会调用失败，但是这里可以忽略有可能的
			// 错误，因为这个错误将在下一次调用 EndDraw 时产生
			Graphics::Instance()->Resize(width, height);
		}
		break;

		case WM_ACTIVATE:
		{
			bool active = (LOWORD(wparam) != WA_INACTIVE);
			if (active)
			{
				E2D_LOG("Window activated");

				SysEvent event(SysEvent::WindowActivate);
				game->Dispatch(&event);
			}
			else
			{
				E2D_LOG("Window deactivated");

				SysEvent event(SysEvent::WindowDeavtivate);
				game->Dispatch(&event);
			}
		}
		break;

		case WM_SETTEXT:
		{
			E2D_LOG("Window title changed");
		}
		break;

		case WM_SETICON:
		{
			E2D_LOG("Window icon changed");
		}
		break;
		}

		return ::DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}
