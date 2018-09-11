#include "..\e2dmodule.h"
#include "..\e2dobject.h"
#include "..\e2dtransition.h"
#include "..\e2dmanager.h"
#include <thread>


e2d::Game * e2d::Game::instance_ = nullptr;

e2d::Game * e2d::Game::GetInstance()
{
	if (!instance_)
		instance_ = new (std::nothrow) Game;
	return instance_;
}

void e2d::Game::DestroyInstance()
{
	if (instance_)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

e2d::Game::Game()
	: quit_(true)
	, paused_(false)
	, curr_scene_(nullptr)
	, next_scene_(nullptr)
	, transition_(nullptr)
{
	::CoInitialize(nullptr);
}

e2d::Game::~Game()
{
	SafeRelease(transition_);
	SafeRelease(curr_scene_);
	SafeRelease(next_scene_);

	::CoUninitialize();
}

void e2d::Game::Start()
{
	quit_ = false;

	auto window = Window::GetInstance();
	auto input = Input::GetInstance();
	auto renderer = Renderer::GetInstance();

	const int minInterval = 5;
	Time last = Time::Now();
	HWND hwnd = window->GetHWnd();
	
	::ShowWindow(hwnd, SW_SHOWNORMAL);
	::UpdateWindow(hwnd);
	window->Poll();
	UpdateScene();
	
	while (!quit_)
	{
		auto now = Time::Now();
		auto dur = now - last;

		if (dur.Milliseconds() > minInterval)
		{
			last = now;
			input->Update();

			UpdateScene();
			DrawScene();

			window->Poll();
		}
		else
		{
			// ID2D1HwndRenderTarget 开启了垂直同步，在渲染时会等待显示器刷新，
			// 它起到了非常稳定的延时作用，所以大部分时候不需要手动挂起线程进行延时。
			// 下面的代码仅在一些情况下（例如窗口最小化时）挂起线程，防止占用过高 CPU 。
			int wait = minInterval - dur.Milliseconds();
			if (wait > 1)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(wait));
			}
		}
	}
}

void e2d::Game::Pause()
{
	paused_ = true;
}

void e2d::Game::Resume()
{
	if (paused_ && !quit_)
	{
		if (curr_scene_)
		{
			curr_scene_->GetRoot()->UpdateTime();
		}
	}
	paused_ = false;
}

bool e2d::Game::IsPaused()
{
	return paused_;
}

void e2d::Game::Quit()
{
	quit_ = true;
}

void e2d::Game::EnterScene(Scene * scene, Transition * transition)
{
	if (scene == nullptr)
	{
		WARN("Next scene is null pointer!");
		return;
	}

	if (next_scene_ != nullptr)
	{
		WARN("Scene is transitioning...");
		return;
	}

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

		transition_->Init(curr_scene_, next_scene_);
	}
}

e2d::Scene * e2d::Game::GetCurrentScene()
{
	return curr_scene_;
}

bool e2d::Game::IsTransitioning() const
{
	return transition_ != nullptr;
}

void e2d::Game::UpdateScene()
{
	if (paused_)
		return;

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

void e2d::Game::DrawScene()
{
	Renderer::GetInstance()->BeginDraw();
	{
		if (transition_)
		{
			transition_->Draw();
		}
		else if (curr_scene_)
		{
			curr_scene_->Draw();
		}
	}
	Renderer::GetInstance()->EndDraw();
}
