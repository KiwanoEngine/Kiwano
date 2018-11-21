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
#include "input.h"
#include "audio.h"
#include "modules.h"
#include "Factory.h"
#include "Scene.h"
#include "Transition.h"
#include "Debuger.h"
#include "../math/Matrix.hpp"
#include <thread>
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

namespace easy2d
{
	Game::Game()
		: curr_scene_(nullptr)
		, next_scene_(nullptr)
		, transition_(nullptr)
		, debug_enabled_(false)
		, initialized_(false)
		, hwnd_(nullptr)
		, window_inactived_(false)
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
		if (initialized_)
			return;

		debug_enabled_ = options.debug;

		Window::Instance()->Init(
			options.title,
			options.width,
			options.height,
			options.icon,
			Game::WndProc,
			debug_enabled_
		);

		const auto window = Window::Instance();
		hwnd_ = window->GetHandle();

		::SetWindowLongW(hwnd_, GWLP_USERDATA, PtrToUlong(this));

		Factory::Instance()->Init(debug_enabled_);
		devices::Graphics::Instance()->Init(hwnd_, options.graphics_options, debug_enabled_);
		devices::Input::Instance()->Init(hwnd_, window->GetContentScaleX(), window->GetContentScaleY(), debug_enabled_);
		devices::Audio::Instance()->Init(debug_enabled_);

		// disable imm
		::ImmAssociateContext(hwnd_, nullptr);

		HWND console = ::GetConsoleWindow();
		if (debug_enabled_)
		{
			if (console == nullptr)
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
		}
		else
		{
			if (console)
			{
				::ShowWindow(console, SW_HIDE);
			}
		}

		if (console)
		{
			// disable the close button of console
			HMENU hmenu = ::GetSystemMenu(console, FALSE);
			::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}

		initialized_ = true;
	}

	void Game::Run()
	{
		if (next_scene_)
		{
			next_scene_->OnEnter();
			curr_scene_ = next_scene_;
			next_scene_ = nullptr;
		}

		::ShowWindow(hwnd_, SW_SHOWNORMAL);
		::UpdateWindow(hwnd_);

		MSG msg = {};
		while (::GetMessageW(&msg, nullptr, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}
	}

	void Game::Quit()
	{
		::DestroyWindow(hwnd_);
	}

	bool Game::EnterScene(spScene const & scene)
	{
		if (!scene)
		{
			logs::Warningln("Game::EnterScene failed, scene is nullptr");
			return false;
		}

		if (curr_scene_ == scene ||
			next_scene_ == scene)
			return false;

		next_scene_ = scene;
		return true;
	}

	bool Game::EnterScene(spScene const& scene, spTransition const& transition)
	{
		if (!EnterScene(scene))
			return false;
		
		if (transition)
		{
			if (transition_)
			{
				transition_->Stop();
			}
			transition_ = transition;
			transition_->Init(curr_scene_, next_scene_);
		}
		return true;
	}

	spScene const& Game::GetCurrentScene()
	{
		return curr_scene_;
	}

	void Game::Update()
	{
		static auto last = time::Now();

		const auto now = time::Now();
		const auto dt = now - last;
		last = now;

		devices::Input::Instance()->Update();

		if (curr_scene_)
			curr_scene_->Update(dt);

		if (next_scene_)
			next_scene_->Update(dt);

		if (debug_enabled_)
			Debuger::Instance()->Update(dt);

		if (transition_)
		{
			transition_->Update(dt);

			if (transition_->IsDone())
				transition_ = nullptr;
			else
				return;
		}

		if (next_scene_)
		{
			if (curr_scene_)
			{
				curr_scene_->OnExit();
			}

			next_scene_->OnEnter();

			curr_scene_ = next_scene_;
			next_scene_ = nullptr;
		}
	}

	void Game::Render()
	{
		auto graphics = devices::Graphics::Instance();
		graphics->BeginDraw(hwnd_);

		if (transition_)
		{
			transition_->Render();
		}
		else if (curr_scene_)
		{
			curr_scene_->Visit();
		}

		if (debug_enabled_)
		{
			graphics->SetTransform(math::Matrix());
			graphics->SetOpacity(1.f);

			if (curr_scene_)
			{
				curr_scene_->DrawBorder();
			}

			if (next_scene_)
			{
				next_scene_->DrawBorder();
			}

			Debuger::Instance()->Visit();
		}

		graphics->EndDraw();

		if (!window_inactived_)
			::InvalidateRect(hwnd_, NULL, FALSE);
	}

	void Game::Dispatch(MouseEvent const & e)
	{
		if (transition_)
			return;

		if (curr_scene_)
			curr_scene_->Dispatch(e, false);
	}

	void Game::Dispatch(KeyEvent const & e)
	{
		if (transition_)
			return;

		if (curr_scene_)
			curr_scene_->Dispatch(e, false);
	}

	LRESULT CALLBACK Game::WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
	{
		LRESULT result = 0;
		bool was_handled = false;

		Game * game = reinterpret_cast<Game*>(
			static_cast<LONG_PTR>(::GetWindowLongW(hwnd, GWLP_USERDATA))
			);

		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint(hwnd, &ps);

			game->Update();
			game->Render();

			::EndPaint(hwnd, &ps);
		}
		result = 0;
		was_handled = true;
		break;

		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		{
			game->Dispatch(MouseEvent(msg, w_param, l_param));
		}
		result = 0;
		was_handled = true;
		break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			game->Dispatch(KeyEvent(msg, w_param, l_param));
		}
		result = 0;
		was_handled = true;
		break;

		case WM_SIZE:
		{
			if (SIZE_MAXHIDE == w_param || SIZE_MINIMIZED == w_param)
				game->window_inactived_ = true;
			else
			{
				game->window_inactived_ = false;
				::InvalidateRect(hwnd, nullptr, FALSE);
			}

			UINT width = LOWORD(l_param);
			UINT height = HIWORD(l_param);

			// 如果程序接收到一个 WM_SIZE 消息，这个方法将调整渲染
			// 目标的大小。它可能会调用失败，但是这里可以忽略有可能的
			// 错误，因为这个错误将在下一次调用 EndDraw 时产生
			devices::Graphics::Instance()->Resize(width, height);
		}
		break;

		case WM_DISPLAYCHANGE:
		{
			::InvalidateRect(hwnd, nullptr, FALSE);
		}
		result = 0;
		was_handled = true;
		break;

		case WM_CLOSE:
		{
			if (game->OnClose())
			{
				::DestroyWindow(hwnd);
			}
		}
		result = 0;
		was_handled = true;
		break;

		case WM_DESTROY:
		{
			game->OnExit();
			::PostQuitMessage(0);
		}
		result = 1;
		was_handled = true;
		break;

		}

		if (!was_handled)
		{
			result = ::DefWindowProcW(hwnd, msg, w_param, l_param);
		}
		return result;
	}
}