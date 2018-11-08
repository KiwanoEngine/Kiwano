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
#include "Node.h"
#include "Scene.h"
#include "Transition.h"
#include "Image.h"
#include "../utils/Player.h"
#include "time.h"
#include "render.h"
#include "input.h"
#include "audio.h"
#include "modules.h"
#include <thread>

namespace easy2d
{
	namespace
	{
		Game * instance = nullptr;
	}

	Game * Game::GetInstance()
	{
		return instance;
	}

	Game::Game()
		: quit_(true)
		, curr_scene_(nullptr)
		, next_scene_(nullptr)
		, transition_(nullptr)
		, debug_mode_(false)
	{
		if (instance)
		{
			throw std::runtime_error("同时只能存在一个游戏实例");
		}
		instance = this;

		::CoInitialize(nullptr);
	}

	Game::~Game()
	{
		SafeRelease(transition_);
		SafeRelease(curr_scene_);
		SafeRelease(next_scene_);

		Image::ClearCache();
		Player::ClearCache();

		render::Uninitialize();
		audio::instance.Uninitialize();
		window::instance.Destroy();
		modules::Uninitialize();

		instance = nullptr;

		::CoUninitialize();
	}

	void Game::Initialize(const window::Property& property)
	{
		modules::Initialize();
		window::instance.Initialize(property);
		audio::instance.Initialize();
		render::Initialize(window::instance.handle);

		// 若开启了调试模式，打开控制台
		HWND console = ::GetConsoleWindow();
		// 关闭控制台
		if (debug_mode_)
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
			window::instance.handle,
			GWLP_USERDATA,
			PtrToUlong(this)
		);
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

		::ShowWindow(window::instance.handle, SW_SHOWNORMAL);
		::UpdateWindow(window::instance.handle);

		const int min_interval = 5;
		auto last = time::Now();
		MSG msg = { 0 };

		while (!quit_)
		{
			auto now = time::Now();
			auto dur = now - last;

			if (dur.Milliseconds() > min_interval)
			{
				float dt = (now - last).Seconds();
				last = now;

				input::instance.Update(
					window::instance.handle,
					window::instance.xscale,
					window::instance.yscale
				);

				OnUpdate(dt);
				UpdateScene(dt);
				DrawScene();

				while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
			else
			{
				// ID2D1HwndRenderTarget 开启了垂直同步，在渲染时会等待显示器刷新，
				// 它起到了非常稳定的延时作用，所以大部分时候不需要手动挂起线程进行延时。
				// 下面的代码仅在一些情况下（例如窗口最小化时）挂起线程，防止占用过高 CPU 。
				int wait = min_interval - dur.Milliseconds();
				if (wait > 1)
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

	void Game::EnterScene(Scene * scene, Transition * transition)
	{
		if (scene == nullptr)
		{
			E2D_WARNING("Next scene is null pointer!");
			return;
		}

		if (curr_scene_ == scene) { return; }

		if (next_scene_)
		{
			next_scene_->Release();
		}
		next_scene_ = scene;
		next_scene_->Retain();

		if (transition)
		{
			if (transition_)
			{
				transition_->Stop();
				transition_->Release();
			}
			transition_ = transition;
			transition_->Retain();

			transition_->Initialize(curr_scene_, next_scene_, this);
		}
	}

	Scene * Game::GetCurrentScene()
	{
		return curr_scene_;
	}

	bool Game::IsTransitioning() const
	{
		return transition_ != nullptr;
	}

	void Game::UpdateScene(float dt)
	{
		auto update = [&](Scene * scene) -> void
		{
			if (scene)
			{
				scene->OnUpdate(dt);
				Node * root = scene->GetRoot();
				if (root)
				{
					root->UpdateChildren(dt);
				}
			}
		};

		update(curr_scene_);
		update(next_scene_);

		if (transition_)
		{
			transition_->Update();

			if (transition_->IsDone())
			{
				transition_->Release();
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
				curr_scene_->Release();
			}

			next_scene_->OnEnter();

			curr_scene_ = next_scene_;
			next_scene_ = nullptr;
		}
	}

	void Game::DrawScene()
	{
		render::instance.BeginDraw(window::instance.handle);

		if (transition_)
		{
			transition_->Draw();
		}
		else if (curr_scene_)
		{
			curr_scene_->Draw();
		}

		if (debug_mode_)
		{
			if (curr_scene_ && curr_scene_->GetRoot())
			{
				render::D2D.HwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				render::D2D.SolidColorBrush->SetOpacity(1.f);
				curr_scene_->GetRoot()->DrawBorder();
			}
			if (next_scene_ && next_scene_->GetRoot())
			{
				render::D2D.HwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
				render::D2D.SolidColorBrush->SetOpacity(1.f);
				next_scene_->GetRoot()->DrawBorder();
			}

			render::instance.DrawDebugInfo();
		}

		render::instance.EndDraw();
	}

	void Game::SetDebugMode(bool enabled)
	{
		debug_mode_ = enabled;
	}
}