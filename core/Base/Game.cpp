#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dtransition.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
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
	, scenes_()
{
	::CoInitialize(nullptr);
}

e2d::Game::~Game()
{
	::CoUninitialize();
}

void e2d::Game::Start()
{
	quit_ = false;

	auto window = Window::GetInstance();
	auto input = Input::GetInstance();
	auto renderer = Renderer::GetInstance();
	auto timer = Timer::GetInstance();
	auto action_manager = ActionManager::GetInstance();

	const int minInterval = 5;
	Time last = Time::Now();
	HWND hWnd = window->GetHWnd();
	
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::UpdateWindow(hWnd);
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

			if (!paused_)
			{
				timer->Update();
				action_manager->Update();
				UpdateScene();
			}
			
			DrawScene();
			window->Poll();
			GC::GetInstance()->Flush();
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
		Timer::GetInstance()->UpdateTime();
		ActionManager::GetInstance()->UpdateTime();
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

void e2d::Game::PushScene(Scene * scene, bool save_current_scene)
{
	if (!scene)
		return;

	// 保存下一场景的指针
	if (next_scene_) next_scene_->Release();
	next_scene_ = scene;
	next_scene_->Retain();

	if (save_current_scene && curr_scene_)
	{
		scenes_.push(curr_scene_);
	}
}

void e2d::Game::PushScene(Transition * transition, bool save_current_scene)
{
	if (!transition)
		return;

	PushScene(transition->in_scene_, save_current_scene);

	if (transition_)
	{
		transition_->Stop();
		transition_->Release();
	}
	transition_ = transition;
	transition_->Retain();

	// 初始化场景切换动画
	if (!transition_->Init(this, curr_scene_))
	{
		WARN("Transition initialize failed!");
		transition_->Release();
		transition_ = nullptr;
	}
}

e2d::Scene* e2d::Game::PopScene()
{
	// 栈为空时，调用返回场景函数失败
	if (scenes_.size() == 0)
	{
		WARN("Scene stack Is empty!");
		return nullptr;
	}

	next_scene_ = scenes_.top();
	next_scene_->Release();
	scenes_.pop();

	return next_scene_;
}

e2d::Scene * e2d::Game::PopScene(Transition * transition)
{
	if (!transition)
		return nullptr;

	auto scene = PopScene();
	if (scene)
	{
		if (transition_)
		{
			transition_->Stop();
			transition_->Release();
		}
		transition_ = transition;
		transition_->Retain();

		transition_->in_scene_ = scene;
		transition_->in_scene_->Retain();

		// 初始化场景切换动画
		if (!transition_->Init(this, curr_scene_))
		{
			WARN("Transition initialize failed!");
			transition_->Release();
			transition_ = nullptr;
		}
	}

	return scene;
}

void e2d::Game::ClearAllScenes()
{
	while (!scenes_.empty())
	{
		scenes_.top()->Release();
		scenes_.pop();
	}
}

e2d::Scene * e2d::Game::GetCurrentScene()
{
	return curr_scene_;
}

const std::stack<e2d::Scene*>& e2d::Game::GetSceneStack()
{
	return scenes_;
}

bool e2d::Game::IsTransitioning() const
{
	return transition_ != nullptr;
}

void e2d::Game::UpdateScene()
{
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
			if (scenes_.empty() || scenes_.top() != curr_scene_)
			{
				curr_scene_->Release();
			}
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
			curr_scene_->Visit();
		}
	}
	Renderer::GetInstance()->EndDraw();
}
