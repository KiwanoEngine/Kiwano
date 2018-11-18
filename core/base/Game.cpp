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
#include "Scene.h"
#include "Transition.h"
#include "Image.h"
#include "../utils/Player.h"
#include "../math/Matrix.hpp"
#include "logs.h"
#include "render.h"
#include "input.h"
#include "audio.h"
#include "modules.h"
#include <thread>

namespace easy2d
{
	Game::Game()
		: quit_(true)
		, curr_scene_(nullptr)
		, next_scene_(nullptr)
		, transition_(nullptr)
		, debug_enabled_(false)
		, initialized_(false)
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

		Window::Instance()->Init(options.title, options.width, options.height, options.icon, debug_enabled_);
		devices::Graphics::Instance()->Init(Window::Instance()->GetHandle(), debug_enabled_);
		devices::Input::Instance()->Init(debug_enabled_);
		devices::Audio::Instance()->Init(debug_enabled_);

		HWND console = ::GetConsoleWindow();
		if (debug_enabled_)
		{
			if (console == nullptr)
			{
				// 显示一个新控制台
				if (::AllocConsole())
				{
					console = ::GetConsoleWindow();
					// 重定向输入输出
					FILE * stdoutStream, *stdinStream, *stderrStream;
					freopen_s(&stdoutStream, "conout$", "w+t", stdout);
					freopen_s(&stdinStream, "conin$", "r+t", stdin);
					freopen_s(&stderrStream, "conout$", "w+t", stderr);
					// 禁用控制台关闭按钮
					HMENU hmenu = ::GetSystemMenu(console, FALSE);
					::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
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

		::SetWindowLongPtrW(
			Window::Instance()->GetHandle(),
			GWLP_USERDATA,
			PtrToUlong(this)
		);

		initialized_ = true;
	}

	void Game::Run()
	{
		quit_ = false;

		if (next_scene_)
		{
			next_scene_->OnEnter();
			curr_scene_ = next_scene_;
			next_scene_ = nullptr;
		}

		const auto window = Window::Instance();
		::ShowWindow(window->GetHandle(), SW_SHOWNORMAL);
		::UpdateWindow(window->GetHandle());
		window->Poll();

		const int64_t min_interval = 5;
		auto last = time::Now();

		while (!quit_)
		{
			auto now = time::Now();
			auto dur = now - last;

			if (dur.Milliseconds() > min_interval)
			{
				const auto dt = now - last;
				last = now;

				devices::Input::Instance()->Update(
					window->GetHandle(),
					window->GetContentScaleX(),
					window->GetContentScaleY()
				);

				UpdateScene(dt);
				DrawScene();

				window->Poll();
			}
			else
			{
				// ID2D1HwndRenderTarget 开启了垂直同步，在渲染时会等待显示器刷新，
				// 它起到了非常稳定的延时作用，所以大部分时候不需要手动挂起线程进行延时。
				// 下面的代码仅在一些情况下（例如窗口最小化时）挂起线程，防止占用过高 CPU 。
				int64_t wait = min_interval - dur.Milliseconds();
				if (wait > 1LL)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(wait));
				}
			}
		}
	}

	void Game::Quit()
	{
		quit_ = true;
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

	void Game::UpdateScene(Duration const& dt)
	{
		if (curr_scene_)
		{
			curr_scene_->Update(dt);
		}

		if (next_scene_)
		{
			next_scene_->Update(dt);
		}

		if (transition_)
		{
			transition_->Update(dt);

			if (transition_->IsDone())
			{
				transition_ = nullptr;
			}
			else
			{
				return;
			}
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

	void Game::DrawScene()
	{
		auto graphics = devices::Graphics::Instance();
		graphics->BeginDraw(Window::Instance()->GetHandle());

		if (transition_)
		{
			transition_->Draw();
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

			graphics->DrawDebugInfo();
		}

		graphics->EndDraw();
	}
}