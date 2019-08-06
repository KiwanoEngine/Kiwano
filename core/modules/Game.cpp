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

#include "..\e2dmodule.h"
#include "..\e2dobject.h"
#include "..\e2dtool.h"
#include "..\e2dtransition.h"
#include <thread>
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

#define WINDOW_STYLE	WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME
#define REGISTER_CLASS	L"Easy2DApp"


static easy2d::Game * instance = nullptr;

easy2d::Game * easy2d::Game::GetInstance()
{
	return instance;
}

easy2d::Game::Game()
	: hwnd_(nullptr)
	, quit_(true)
	, curr_scene_(nullptr)
	, next_scene_(nullptr)
	, transition_(nullptr)
	, title_(L"Easy2D Game")
	, width_(640)
	, height_(480)
	, icon_(0)
	, debug_mode_(false)
{
	if (instance)
	{
		throw std::runtime_error("ͬʱֻ�ܴ���һ����Ϸʵ��");
	}
	instance = this;

	::CoInitialize(nullptr);
}

easy2d::Game::~Game()
{
	SafeRelease(transition_);
	SafeRelease(curr_scene_);
	SafeRelease(next_scene_);

	Image::ClearCache();
	Player::ClearCache();
	Device::Destroy();

	if (hwnd_)
	{
		::DestroyWindow(hwnd_);
	}

	instance = nullptr;

	::CoUninitialize();
}

void easy2d::Game::Run()
{
	// ��ʼ��
	Init();

	// ��ʼ
	OnStart();

	// ˢ�³���
	if (next_scene_)
	{
		next_scene_->OnEnter();
		curr_scene_ = next_scene_;
		next_scene_ = nullptr;
	}
	::ShowWindow(hwnd_, SW_SHOWNORMAL);
	::UpdateWindow(hwnd_);

	// ����
	const int min_interval = 5;
	Time last = Time::Now();
	MSG msg = { 0 };
	
	while (!quit_)
	{
		auto now = Time::Now();
		auto dur = now - last;

		if (dur.Milliseconds() > min_interval)
		{
			float dt = (now - last).Seconds();
			last = now;

			Device::GetInput()->Flush();
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
			// ID2D1HwndRenderTarget �����˴�ֱͬ��������Ⱦʱ��ȴ���ʾ��ˢ�£�
			// �����˷ǳ��ȶ�����ʱ���ã����Դ󲿷�ʱ����Ҫ�ֶ������߳̽�����ʱ��
			// ����Ĵ������һЩ����£����細����С��ʱ�������̣߳���ֹռ�ù��� CPU ��
			int wait = min_interval - dur.Milliseconds();
			if (wait > 1)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(wait));
			}
		}
	}
}

void easy2d::Game::Quit()
{
	quit_ = true;
}

void easy2d::Game::EnterScene(Scene * scene, Transition * transition)
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

		transition_->Init(curr_scene_, next_scene_, this);
	}
}

easy2d::Scene * easy2d::Game::GetCurrentScene()
{
	return curr_scene_;
}

bool easy2d::Game::IsTransitioning() const
{
	return transition_ != nullptr;
}

void easy2d::Game::UpdateScene(float dt)
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

void easy2d::Game::DrawScene()
{
	auto graphics = Device::GetGraphics();
	graphics->BeginDraw();

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
			graphics->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
			graphics->GetSolidBrush()->SetOpacity(1.f);
			curr_scene_->GetRoot()->DrawBorder();
		}
		if (next_scene_ && next_scene_->GetRoot())
		{
			graphics->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
			graphics->GetSolidBrush()->SetOpacity(1.f);
			next_scene_->GetRoot()->DrawBorder();
		}

		graphics->DrawDebugInfo();
	}
	graphics->EndDraw();
}

void easy2d::Game::Init()
{
	HINSTANCE hinstance	= GetModuleHandle(nullptr);
	WNDCLASSEX wcex		= { 0 };
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.lpszClassName	= REGISTER_CLASS;
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= Game::WndProc;
	wcex.hIcon			= nullptr;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof(LONG_PTR);
	wcex.hInstance		= hinstance;
	wcex.hbrBackground	= nullptr;
	wcex.lpszMenuName	= nullptr;
	wcex.hCursor		= ::LoadCursor(nullptr, IDC_ARROW);

	if (icon_ != 0)
	{
		wcex.hIcon = (HICON)::LoadImage(
			hinstance,
			MAKEINTRESOURCE(icon_),
			IMAGE_ICON,
			0,
			0,
			LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
		);
	}

	// ע�ᴰ����
	::RegisterClassEx(&wcex);

	// ���㴰�ڴ�С
	Rect client_rect = Locate(width_, height_);

	// ��������
	hwnd_ = ::CreateWindowEx(
		NULL,
		REGISTER_CLASS,
		title_.c_str(),
		WINDOW_STYLE,
		int(client_rect.origin.x),
		int(client_rect.origin.y),
		int(client_rect.size.width),
		int(client_rect.size.height),
		nullptr,
		nullptr,
		hinstance,
		this
	);

	if (hwnd_ == nullptr)
	{
		::UnregisterClass(REGISTER_CLASS, hinstance);
		throw std::runtime_error("Create window failed");
		return;
	}

	::SetWindowLongPtrW(
		hwnd_,
		GWLP_USERDATA,
		PtrToUlong(this)
	);

	// ��ʼ���豸
	Device::Init(hwnd_);

	// �������뷨
	::ImmAssociateContext(hwnd_, nullptr);

	// �������˵���ģʽ���򿪿���̨
	HWND console = ::GetConsoleWindow();
	// �رտ���̨
	if (debug_mode_)
	{
		if (console)
		{
			::ShowWindow(console, SW_SHOWNORMAL);
		}
		else
		{
			// ��ʾһ���¿���̨
			if (::AllocConsole())
			{
				console = ::GetConsoleWindow();
				// �ض����������
				FILE * stdoutStream, *stdinStream, *stderrStream;
				freopen_s(&stdoutStream, "conout$", "w+t", stdout);
				freopen_s(&stdinStream, "conin$", "r+t", stdin);
				freopen_s(&stderrStream, "conout$", "w+t", stderr);
				// ���ÿ���̨�رհ�ť
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

	quit_ = false;
}

easy2d::Rect easy2d::Game::Locate(int width, int height)
{
	int max_width = ::GetSystemMetrics(SM_CXSCREEN);
	int max_height = ::GetSystemMetrics(SM_CYSCREEN);

	float dpi = Graphics::GetDpi();
	RECT rect = { 0, 0, LONG(ceil(width * dpi / 96.f)), LONG(ceil(height * dpi / 96.f)) };

	// ������ʵĴ��ڴ�С
	::AdjustWindowRectEx(&rect, WINDOW_STYLE, FALSE, NULL);
	width = static_cast<int>(rect.right - rect.left);
	height = static_cast<int>(rect.bottom - rect.top);

	// ������Ĵ��ڴ�С�ȷֱ��ʴ�ʱ����������
	E2D_WARNING_IF(max_width < width || max_height < height, "The window Is larger than screen!");
	width = std::min(width, max_width);
	height = std::min(height, max_height);

	return Rect(
		static_cast<float>((max_width - width) / 2),
		static_cast<float>((max_height - height) / 2),
		static_cast<float>(width),
		static_cast<float>(height)
	);
}

int easy2d::Game::GetWidth() const
{
	return width_;
}

int easy2d::Game::GetHeight() const
{
	return height_;
}

easy2d::Size easy2d::Game::GetSize() const
{
	return easy2d::Size(
		static_cast<float>(width_),
		static_cast<float>(height_)
	);
}

HWND easy2d::Game::GetHWnd() const
{
	return hwnd_;
}

const std::wstring& easy2d::Game::GetTitle() const
{
	return title_;
}

void easy2d::Game::SetSize(int width, int height)
{
	if (width_ == width && height_ == height)
		return;

	width_ = width;
	height_ = height;

	if (hwnd_)
	{
		Rect rect = Locate(width, height);
		::MoveWindow(
			hwnd_,
			int(rect.origin.x),
			int(rect.origin.y),
			int(rect.size.width),
			int(rect.size.height),
			TRUE
		);
	}
}

void easy2d::Game::SetTitle(const std::wstring& title)
{
	title_ = title;
	
	if (hwnd_)
	{
		::SetWindowText(hwnd_, title.c_str());
	}
}

void easy2d::Game::SetIcon(int resource_id)
{
	icon_ = resource_id;

	if (hwnd_)
	{
		HINSTANCE hinstance = GetModuleHandle(nullptr);
		HICON icon = (HICON)::LoadImage(
			hinstance,
			MAKEINTRESOURCE(resource_id),
			IMAGE_ICON,
			0,
			0,
			LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
		);

		::SendMessage(hwnd_, WM_SETICON, ICON_BIG, (LPARAM)icon);
		::SendMessage(hwnd_, WM_SETICON, ICON_SMALL, (LPARAM)icon);
	}
}

void easy2d::Game::SetDebugMode(bool enabled)
{
	debug_mode_ = enabled;
}


LRESULT easy2d::Game::WndProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;
	bool was_handled = false;
	Game * game = reinterpret_cast<Game*>(
		static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA))
	);

	switch (msg)
	{

	// ���������Ϣ
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
		if (game->IsTransitioning())
			break;

		auto curr_scene = game->GetCurrentScene();
		if (curr_scene)
		{
			curr_scene->Dispatch(MouseEvent(msg, w_param, l_param));
		}
	}
	result = 0;
	was_handled = true;
	break;

	// ��������Ϣ
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		if (game->IsTransitioning())
			break;

		auto curr_scene = game->GetCurrentScene();
		if (curr_scene)
		{
			curr_scene->Dispatch(KeyEvent(msg, w_param, l_param));
		}
	}
	result = 0;
	was_handled = true;
	break;

	// �����ڴ�С�仯��Ϣ
	case WM_SIZE:
	{
		UINT width = LOWORD(l_param);
		UINT height = HIWORD(l_param);

		if (w_param == SIZE_RESTORED)
		{
			float dpi = Graphics::GetDpi();
			game->width_ = static_cast<int>(width * 96.f / dpi);
			game->height_ = static_cast<int>(height * 96.f / dpi);
		}

		// ���������յ�һ�� WM_SIZE ��Ϣ�����������������Ⱦ
		// Ŀ��Ĵ�С�������ܻ����ʧ�ܣ�����������Ժ����п��ܵ�
		// ������Ϊ�����������һ�ε��� EndDraw ʱ����
		auto render_target = Device::GetGraphics()->GetRenderTarget();
		if (render_target)
		{
			render_target->Resize(D2D1::SizeU(width, height));
		}
	}
	break;

	// �����ڱ���仯��Ϣ
	case WM_SETTEXT:
	{
		game->title_ = (const wchar_t*)l_param;
	}
	break;

	// ����ֱ��ʱ仯��Ϣ
	case WM_DISPLAYCHANGE:
	{
		// �ػ�ͻ���
		::InvalidateRect(hwnd, nullptr, FALSE);
	}
	result = 0;
	was_handled = true;
	break;

	// �ػ洰��
	case WM_PAINT:
	{
		game->DrawScene();
		::ValidateRect(hwnd, nullptr);
	}
	result = 0;
	was_handled = true;
	break;

	// ���ڹر���Ϣ
	case WM_CLOSE:
	{
		if (game->OnClose())
		{
			game->Quit();
		}
	}
	result = 0;
	was_handled = true;
	break;

	// ����������Ϣ
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
	}
	result = 1;
	was_handled = true;
	break;

	}

	if (!was_handled)
	{
		result = ::DefWindowProc(hwnd, msg, w_param, l_param);
	}
	return result;
}
