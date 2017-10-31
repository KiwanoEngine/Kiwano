#include "..\ebase.h"
#include "..\Win\winbase.h"
#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\etransitions.h"
#include <stack>
#include <thread>
#include <imm.h>
#pragma comment (lib ,"imm32.lib")

using namespace std::this_thread;
using namespace std::chrono;


// 唯一实例指针
static e2d::EApp * s_pInstance = nullptr;
// 场景栈
static std::stack<e2d::EScene*> s_SceneStack;
// 游戏开始时间
static steady_clock::time_point s_tStart;

e2d::EApp::EApp()
	: m_bEnd(false)
	, m_bPaused(false)
	, m_bManualPaused(false)
	, m_bTransitional(false)
	, m_bTopMost(false)
	, m_bShowConsole(false)
	, nAnimationInterval(17LL)
	, m_ClearColor(EColor::BLACK)
	, m_pCurrentScene(nullptr)
	, m_pNextScene(nullptr)
{
	ASSERT(s_pInstance == nullptr, "EApp instance already exists!");
	s_pInstance = this;		// 保存实例对象

	CoInitialize(NULL);
}

e2d::EApp::~EApp()
{
	// 释放资源
	SafeReleaseInterface(&GetSolidColorBrush());
	SafeReleaseInterface(&GetRenderTarget());
	SafeReleaseInterface(&GetFactory());
	SafeReleaseInterface(&GetImagingFactory());
	SafeReleaseInterface(&GetDirectWriteFactory());
	
	CoUninitialize();
}

e2d::EApp * e2d::EApp::get()
{
	ASSERT(s_pInstance != nullptr, "Nonexistent EApp instance.");
	return s_pInstance;		// 获取 EApp 的唯一实例
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height)
{
	return init(title, width, height, EWindowStyle());
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, EWindowStyle wStyle)
{
	HRESULT hr;

	// 注册窗口类
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = EApp::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"Easy2DApp";
	// 设置窗口是否有关闭按钮
	if (wStyle.m_bNoClose)
	{
		wcex.style |= CS_NOCLOSE;
	}
	// 设置程序图标
	if (wStyle.m_pIconID)
	{
		wcex.hIcon = (HICON)::LoadImage(
			GetModuleHandle(NULL), 
			wStyle.m_pIconID, 
			IMAGE_ICON, 
			0, 
			0, 
			LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	}

	RegisterClassEx(&wcex);

	// 因为 CreateWindow 函数使用的是像素大小，获取系统的 DPI 以使它
	// 适应窗口缩放
	FLOAT dpiX, dpiY;

	// 工厂将返回当前的系统 DPI，这个值也将用来创建窗口
	GetFactory()->GetDesktopDpi(&dpiX, &dpiY);

	width = static_cast<UINT>(ceil(width * dpiX / 96.f));
	height = static_cast<UINT>(ceil(height * dpiY / 96.f));

	// 获取屏幕分辨率
	UINT screenWidth = static_cast<UINT>(GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(GetSystemMetrics(SM_CYSCREEN));
	// 当输入的窗口大小比分辨率大时，给出警告
	WARN_IF(screenWidth < width || screenHeight < height, "The window is larger than screen!");
	// 取最小值
	width = min(width, screenWidth);
	height = min(height, screenHeight);
		
	// 创建窗口样式
	DWORD dwStyle = WS_OVERLAPPED | WS_SYSMENU;
	if (!wStyle.m_bNoMiniSize)
	{
		dwStyle |= WS_MINIMIZEBOX;
	}
	// 保存窗口是否置顶显示
	m_bTopMost = wStyle.m_bTopMost;
	// 保存窗口名称
	m_sTitle = title;
	// 创建窗口
	GetHWnd() = CreateWindow(
		L"Easy2DApp",
		m_sTitle.c_str(),
		dwStyle,
		0,
		0,
		width,
		height,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		this
	);

	hr = GetHWnd() ? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		// 禁用输入法
		this->setKeyboardLayoutEnable(false);
		// 重设客户区大小
		this->setWindowSize(width, height);
	}
	else
	{
		UnregisterClass(L"E2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Initialize Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

void e2d::EApp::pause()
{
	EApp::get()->m_bManualPaused = true;
}

void e2d::EApp::resume()
{
	if (isPaused())
	{
		EApp::get()->m_bPaused = false;
		EApp::get()->m_bManualPaused = false;
		// 刷新当前时间
		GetNow() = steady_clock::now();
		// 重置动画和定时器
		EActionManager::_resetAllActions();
		ETimerManager::_resetAllTimers();
	}
}

bool e2d::EApp::isPaused()
{
	return s_pInstance->m_bPaused || s_pInstance->m_bManualPaused;
}

void e2d::EApp::showConsole(bool show /* = true */)
{
	static FILE * stdoutstream = nullptr;
	static FILE * stdinstream = nullptr;
	static FILE * stderrstream = nullptr;

	EApp::get()->m_bShowConsole = show;
	// 查找已存在的控制台句柄
	HWND hwnd = GetConsoleWindow();
	// 关闭控制台
	if (show)
	{
		if (hwnd)
		{
			ShowWindow(hwnd, SW_SHOWNORMAL);
		}
		else
		{
			// 显示一个新控制台
			if (AllocConsole())
			{
				freopen_s(&stdoutstream, "CONOUT$", "w+t", stdout);
				freopen_s(&stderrstream, "CONOUT$", "w+t", stderr);
				freopen_s(&stdinstream, "CONIN$", "r+t", stdin);
			}
			else
			{
				MessageBox(nullptr, L"Alloc Console Failed!", L"Error", MB_OK);
			}
		}
	}
	else
	{
		if (hwnd)
		{
			if (stdoutstream)
			{
				fclose(stdoutstream);
				fclose(stdinstream);
				fclose(stderrstream);

				stdoutstream = stdinstream = stderrstream = nullptr;
			}
			FreeConsole();
		}
	}
}

// 运行游戏
void e2d::EApp::run()
{
	ASSERT(GetHWnd() != nullptr, "Cannot find Game Window.");
	// 进入第一个场景
	_enterNextScene();
	// 关闭控制台
	if (!m_bShowConsole)
	{
		showConsole(false);
	}
	// 显示窗口
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// 设置窗口置顶
	if (m_bTopMost)
	{
		SetWindowPos(GetHWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	// 记录开始时间
	s_tStart = steady_clock::now();

	MSG msg;

	while (!m_bEnd)
	{
		// 处理窗口消息
		while (PeekMessage(&msg, GetHWnd(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 执行主循环
		_mainLoop();
	}

	// 关闭控制台
	EApp::showConsole(false);
}

void e2d::EApp::setFPS(UINT32 fps)
{
	fps = min(max(fps, 30), 120);
	s_pInstance->nAnimationInterval = 1000 / fps;
}

bool e2d::EApp::onActivate()
{
	return true;
}

bool e2d::EApp::onInactive()
{
	return true;
}

bool e2d::EApp::onCloseWindow()
{
	return true;
}

void e2d::EApp::_mainLoop()
{
	// 时间间隔
	static LONGLONG nInterval = 0LL;
	// 挂起时长
	static LONGLONG nWaitMS = 0L;
	// 上一帧画面绘制时间
	static steady_clock::time_point tLast = steady_clock::now();

	// 刷新当前时间
	GetNow() = steady_clock::now();
	// 计算时间间隔
	nInterval = GetInterval(tLast);
	// 判断间隔时间是否足够
	if (nInterval >= nAnimationInterval)
	{
		// 记录当前时间
		tLast += microseconds(nAnimationInterval);
		// 游戏控制流程
		_onControl();
		// 刷新游戏画面
		_onRender();
	}
	else
	{
		// 计算挂起时长
		nWaitMS = nAnimationInterval - nInterval - 1;
		// 挂起线程，释放 CPU 占用
		if (nWaitMS > 1LL)
		{
			sleep_for(milliseconds(nWaitMS));
		}
	}
}

void e2d::EApp::_onControl()
{
	// 正在切换场景时，执行场景切换动画
	if (m_bTransitional)
	{
		EActionManager::ActionProc();
		// 若场景切换未结束，不执行后面的部分
		if (m_bTransitional)
		{
			return;
		}
	}

	// 下一场景指针不为空时，切换场景
	if (m_pNextScene)
	{
		// 进入下一场景
		_enterNextScene();
	}

	// 断言当前场景非空
	ASSERT(m_pCurrentScene != nullptr, "Current scene NULL pointer exception.");

	EObjectManager::__flush();		// 刷新内存池
	ETimerManager::TimerProc();		// 定时器管理器执行程序
	EActionManager::ActionProc();	// 动作管理器执行程序
}

void e2d::EApp::_onRender()
{
	HRESULT hr = S_OK;

	// 开始绘图
	GetRenderTarget()->BeginDraw();
	// 使用背景色清空屏幕
	GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor));
	// 绘制当前场景
	if (m_pCurrentScene)
	{
		m_pCurrentScene->_onRender();
	}
	// 切换场景时，同时绘制两场景
	if (m_bTransitional && m_pNextScene)
	{
		m_pNextScene->_onRender();
	}
	// 终止绘图
	hr = GetRenderTarget()->EndDraw();
	// 刷新界面
	UpdateWindow(GetHWnd());

	if (hr == D2DERR_RECREATE_TARGET)
	{
		// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
		// 并在下一次调用时重建资源
		hr = S_OK;
		SafeReleaseInterface(&GetRenderTarget());
	}

	if (FAILED(hr))
	{
		// 渲染时产生了未知的错误，退出游戏
		MessageBox(GetHWnd(), L"Game rendering failed!", L"Error", MB_OK);
		this->quit();
	}
}

void e2d::EApp::setWindowSize(UINT32 width, UINT32 height)
{
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// 获取窗口大小（包含菜单栏）
	tagRECT rcWindow;
	GetWindowRect(GetHWnd(), &rcWindow);
	// 获取客户区大小
	tagRECT rcClient;
	GetClientRect(GetHWnd(), &rcClient);
	// 计算边框大小
	width += (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	height += (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
	// 修改窗口大小，并设置窗口在屏幕居中
	MoveWindow(GetHWnd(), (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, TRUE);
}

void e2d::EApp::setWindowTitle(const EString &title)
{
	// 设置窗口标题
	SetWindowText(GetHWnd(), title.c_str());
	// 保存当前标题，用于修改窗口大小时恢复标题
	get()->m_sTitle = title;
}

e2d::EString e2d::EApp::getTitle()
{
	return get()->m_sTitle;
}

float e2d::EApp::getWidth()
{
	return GetRenderTarget()->GetSize().width;
}

float e2d::EApp::getHeight()
{
	return GetRenderTarget()->GetSize().height;
}

void e2d::EApp::enterScene(EScene * scene, bool saveCurrentScene /* = true */)
{
	enterScene(scene, nullptr, saveCurrentScene);
}

void e2d::EApp::enterScene(EScene * scene, ETransition * transition, bool saveCurrentScene /* = true */)
{
	scene->retain();
	// 保存下一场景的指针
	get()->m_pNextScene = scene;
	// 切换场景时，是否保存当前场景
	if (get()->m_pCurrentScene)
	{
		get()->m_pCurrentScene->m_bWillSave = saveCurrentScene;
	}
	// 设置切换场景动画
	if (transition)
	{
		get()->m_bTransitional = true;
		transition->_setTarget(
			get()->m_pCurrentScene, 
			get()->m_pNextScene, 
			get()->m_bTransitional
		);
	}
	else
	{
		get()->m_bTransitional = false;
	}
}

void e2d::EApp::backScene(ETransition * transition /* = nullptr */)
{
	// 栈为空时，调用返回场景函数失败
	WARN_IF(s_SceneStack.size() == 0, "Scene stack now is empty!");
	if (s_SceneStack.size() == 0) return;

	// 从栈顶取出场景指针，作为下一场景
	get()->m_pNextScene = s_SceneStack.top();
	s_SceneStack.pop();

	// 返回上一场景时，不保存当前场景
	if (get()->m_pCurrentScene)
	{
		get()->m_pCurrentScene->m_bWillSave = false;
	}

	// 设置切换场景动画
	if (transition)
	{
		get()->m_bTransitional = true;
		transition->_setTarget(
			get()->m_pCurrentScene, 
			get()->m_pNextScene, 
			get()->m_bTransitional
		);
	}
	else
	{
		// 把这个变量赋为 false，场景将在下一帧画面
		// 进行切换
		get()->m_bTransitional = false;
	}
}

void e2d::EApp::clearScene()
{
	// 清空场景栈
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeRelease(&temp);
		s_SceneStack.pop();
	}
}

e2d::EScene * e2d::EApp::getCurrentScene()
{
	return get()->m_pCurrentScene;
}

void e2d::EApp::setAppName(const EString &appname)
{
	get()->m_sAppName = appname;
}

e2d::EString e2d::EApp::getAppName()
{
	if (get()->m_sAppName.empty())
		get()->m_sAppName = get()->m_sTitle;
	return get()->m_sAppName;
}

void e2d::EApp::setBkColor(UINT32 color)
{
	get()->m_ClearColor = color;
}

void e2d::EApp::setKeyboardLayoutEnable(bool value)
{
	static HIMC hImc = NULL;

	if (value)
	{
		if (hImc != NULL)
		{
			ImmAssociateContext(GetHWnd(), hImc);
			hImc = NULL;
		}
	}
	else
	{
		if (hImc == NULL)
		{
			hImc = ImmAssociateContext(GetHWnd(), NULL);
		}
	}
}

HWND e2d::EApp::getHWnd()
{
	return GetHWnd();
}

LONGLONG e2d::EApp::getTotalDurationFromStart()
{
	return GetInterval(s_tStart);
}

void e2d::EApp::hideWindow()
{
	ShowWindow(GetHWnd(), SW_HIDE);
}

void e2d::EApp::showWindow()
{
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
}

void e2d::EApp::quit()
{
	// 这个变量将控制游戏是否结束
	get()->m_bEnd = true;
}

void e2d::EApp::end()
{
	EApp::quit();
}

void e2d::EApp::_enterNextScene()
{
	if (m_pNextScene == nullptr)
		return;

	// 执行当前场景的 onCloseWindow 函数
	if (m_pCurrentScene)
	{
		m_pCurrentScene->onExit();

		if (m_pCurrentScene->m_bWillSave)
		{
			// 若要保存当前场景，把它放入栈中
			s_SceneStack.push(m_pCurrentScene);
		}
		else
		{
			SafeRelease(&m_pCurrentScene);
		}
	}

	// 执行下一场景的 onEnter 函数
	m_pNextScene->onEnter();

	m_pCurrentScene = m_pNextScene;		// 切换场景
	m_pNextScene = nullptr;				// 下一场景置空
}

LRESULT e2d::EApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 处理窗口消息
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		// 获取发送 WM_CREATE 消息的 EApp 实例对象指针
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		e2d::EApp *pEApp = (e2d::EApp *)pcs->lpCreateParams;
		// 保存 EApp 指针到 GWLP_USERDATA 字段
		::SetWindowLongPtrW(
			hWnd,
			GWLP_USERDATA,
			PtrToUlong(pEApp)
		);

		result = 1;
	}
	else
	{
		// 从 GWLP_USERDATA 字段取出 EApp 指针
		e2d::EApp *pEApp = reinterpret_cast<e2d::EApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hWnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pEApp)
		{
			switch (message)
			{
			// 处理鼠标消息
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
				// 执行场景切换时屏蔽按键和鼠标消息
				if (!pEApp->m_bTransitional && !pEApp->m_pNextScene)
				{
					EMsgManager::MouseProc(message, wParam, lParam);
				}
			}
			result = 0;
			wasHandled = true;
			break;

			// 处理按键消息
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				// 执行场景切换时屏蔽按键和鼠标消息
				if (!pEApp->m_bTransitional && !pEApp->m_pNextScene)
				{
					EMsgManager::KeyboardProc(message, wParam, lParam);
				}
			}
			result = 0;
			wasHandled = true;
			break;

			// 处理窗口大小变化消息
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				// 如果程序接收到一个 WM_SIZE 消息，这个方法将调整渲染
				// 目标适当。它可能会调用失败，但是这里可以忽略有可能的
				// 错误，因为这个错误将在下一次调用 EndDraw 时产生
				GetRenderTarget()->Resize(D2D1::SizeU(width, height));
			}
			result = 0;
			wasHandled = true;
			break;

			// 处理分辨率变化消息
			case WM_DISPLAYCHANGE:
			{
				// 重绘客户区
				InvalidateRect(hWnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			// 重绘窗口
			case WM_PAINT:
			{
				pEApp->_onRender();
				ValidateRect(hWnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			// 窗口激活消息
			case WM_ACTIVATE:
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					if (pEApp->getCurrentScene() && 
						pEApp->getCurrentScene()->onInactive() &&
						pEApp->onInactive())
					{
						pEApp->m_bPaused = true;
					}
				}
				else
				{
					if (pEApp->getCurrentScene() && 
						pEApp->getCurrentScene()->onActivate() &&
						pEApp->onActivate())
					{
						pEApp->m_bPaused = false;
					}
				}
			}
			result = 1;
			wasHandled = true;
			break;

			// 窗口关闭消息
			case WM_CLOSE:
			{
				if (!pEApp->getCurrentScene())
				{
					if (pEApp->onCloseWindow())
					{
						DestroyWindow(hWnd);
					}
				}
				else 
				{
					if (pEApp->getCurrentScene()->onCloseWindow() &&
						pEApp->onCloseWindow())
					{
						DestroyWindow(hWnd);
					}
				}
			}
			result = 1;
			wasHandled = true;
			break;

			// 窗口被销毁
			case WM_DESTROY:
			{
				// 退出程序
				pEApp->quit();
				// 发送退出消息
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		// 对当前消息没有特定的处理程序时，执行默认的窗口函数
		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return result;
}
