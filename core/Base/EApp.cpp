#include "..\ebase.h"
#include "..\Win\winbase.h"
#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\etransitions.h"
#include "..\etools.h"
#include <stack>
#include <imm.h>
#pragma comment (lib ,"imm32.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


// 唯一实例指针
static e2d::EApp * s_pInstance = nullptr;
// 场景栈
static std::stack<e2d::EScene*> s_SceneStack;
// 游戏开始时间
static LARGE_INTEGER s_tStart;


e2d::EApp::EApp()
	: m_bEnd(false)
	, m_bPaused(false)
	, m_bManualPaused(false)
	, m_bShowConsole(false)
	, m_nAnimationInterval()
	, m_ClearColor(EColor::BLACK)
	, m_pTransition(nullptr)
	, m_pCurrentScene(nullptr)
	, m_pNextScene(nullptr)
{
	CoInitialize(NULL);

	// 获取时钟频率
	LARGE_INTEGER tFreq;
	QueryPerformanceFrequency(&tFreq);
	// 默认帧率为 60
	m_nAnimationInterval.QuadPart = static_cast<LONGLONG>(1.0 / 60 * tFreq.QuadPart);
}

e2d::EApp::~EApp()
{
	SafeReleaseInterface(&GetSolidColorBrush());
	SafeReleaseInterface(&GetRenderTarget());
	SafeReleaseInterface(&GetFactory());
	SafeReleaseInterface(&GetImagingFactory());
	SafeReleaseInterface(&GetDirectWriteFactory());
	CoUninitialize();
}

e2d::EApp * e2d::EApp::getInstance()
{
	if (!s_pInstance)
	{
		s_pInstance = new EApp();
	}
	return s_pInstance;
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, const EWindowStyle &wStyle /* = nullptr */)
{
	EApp * pApp = EApp::getInstance();
	HRESULT hr;

	// 保存窗口样式
	pApp->m_WindowStyle = wStyle;
	// 保存窗口名称
	pApp->m_sTitle = title;

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
	// 创建窗口
	GetHWnd() = CreateWindow(
		L"Easy2DApp",
		title,
		dwStyle,
		0, 0, width, height,
		NULL,
		NULL,
		HINST_THISCOMPONENT,
		NULL
	);

	hr = GetHWnd() ? S_OK : E_FAIL;

	if (SUCCEEDED(hr))
	{
		// 禁用输入法
		EApp::setKeyboardLayoutEnable(false);
		// 重设客户区大小
		EApp::setWindowSize(width, height);
	}
	else
	{
		UnregisterClass(L"Easy2DApp", HINST_THISCOMPONENT);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Initialize Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

int e2d::EApp::run()
{
	if (GetHWnd() == nullptr)
	{
		MessageBox(nullptr, L"Invalid window handle!", L"Error", MB_OK);
		return -1;
	}

	EApp * pApp = EApp::getInstance();
	// 进入第一个场景
	pApp->_enterNextScene();
	// 关闭控制台
	if (!pApp->m_bShowConsole)
	{
		showConsole(false);
	}
	// 显示窗口
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// 设置窗口置顶
	if (pApp->m_WindowStyle.m_bTopMost)
	{
		SetWindowPos(GetHWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	// 挂起时长
	LONG nWait = 0L;
	// 时间间隔
	LONGLONG nInterval;
	// 上一帧画面绘制时间
	LARGE_INTEGER tLast;
	// 时钟频率
	LARGE_INTEGER tFreq;

	// 修改时间精度
	timeBeginPeriod(1);
	// 获取时钟频率
	QueryPerformanceFrequency(&tFreq);
	// 刷新当前时间
	QueryPerformanceCounter(&GetNow());
	// 记录开始时间
	s_tStart = GetNow();
	tLast = GetNow();
	// 窗口消息
	MSG msg;

	while (!pApp->m_bEnd)
	{
		// 处理窗口消息
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 刷新当前时间
		QueryPerformanceCounter(&GetNow());
		// 计算时间间隔
		nInterval = GetNow().QuadPart - tLast.QuadPart;
		// 判断间隔时间是否足够
		if (nInterval >= pApp->m_nAnimationInterval.QuadPart)
		{
			// 记录当前时间
			tLast.QuadPart += pApp->m_nAnimationInterval.QuadPart;
			// 更新游戏内容
			pApp->_update();
			// 刷新游戏画面
			pApp->_render();
		}
		else
		{
			// 计算挂起时长
			nWait = static_cast<LONG>(ToMilliseconds(pApp->m_nAnimationInterval.QuadPart - nInterval) - 1LL);
			// 挂起线程，释放 CPU 占用
			if (nWait > 1L)
			{
				Sleep(nWait);
			}
		}
	}

	// 关闭控制台
	EApp::showConsole(false);
	// 重置时间精度
	timeEndPeriod(1);

	return 0;
}

void e2d::EApp::pause()
{
	EApp::getInstance()->m_bManualPaused = true;
}

void e2d::EApp::resume()
{
	if (isPaused())
	{
		EApp::getInstance()->m_bPaused = false;
		EApp::getInstance()->m_bManualPaused = false;
		EApp::getInstance()->_updateTime();
	}
}

bool e2d::EApp::isPaused()
{
	return s_pInstance->m_bPaused || s_pInstance->m_bManualPaused;
}

void e2d::EApp::quit()
{
	getInstance()->m_bEnd = true;	// 这个变量将控制游戏是否结束
}

void e2d::EApp::showConsole(bool show /* = true */)
{
	static FILE * stdoutstream = nullptr;
	static FILE * stdinstream = nullptr;
	static FILE * stderrstream = nullptr;

	EApp::getInstance()->m_bShowConsole = show;
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

bool e2d::EApp::onActivate()
{
	return false;
}

bool e2d::EApp::onInactive()
{
	return false;
}

bool e2d::EApp::onCloseWindow()
{
	return true;
}

void e2d::EApp::_update()
{
	if (isPaused())
	{
		return;
	}

	// 正在切换场景时，执行场景切换动画
	if (m_pTransition)
	{
		m_pTransition->_update();
		if (m_pTransition->isEnding())
		{
			m_pTransition->release();
			m_pTransition = nullptr;
			// 进入下一场景
			_enterNextScene();
			// 刷新计时器
			_updateTime();
		}
		return;
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

void e2d::EApp::_render()
{
	HRESULT hr = S_OK;

	// 开始绘图
	GetRenderTarget()->BeginDraw();
	// 使用背景色清空屏幕
	GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor));
	// 绘制当前场景
	if (m_pCurrentScene)
	{
		m_pCurrentScene->_render();
	}
	// 切换场景时，同时绘制两场景
	if (m_pTransition && m_pNextScene)
	{
		m_pNextScene->_render();
	}
	// 终止绘图
	hr = GetRenderTarget()->EndDraw();

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
	SetWindowText(GetHWnd(), title);
	// 保存当前标题，用于修改窗口大小时恢复标题
	getInstance()->m_sTitle = title;
}

e2d::EString e2d::EApp::getTitle()
{
	return getInstance()->m_sTitle;
}

float e2d::EApp::getWidth()
{
	return GetRenderTarget()->GetSize().width;
}

float e2d::EApp::getHeight()
{
	return GetRenderTarget()->GetSize().height;
}

e2d::ESize e2d::EApp::getSize()
{
	return ESize(GetRenderTarget()->GetSize().width, GetRenderTarget()->GetSize().height);
}

void e2d::EApp::enterScene(EScene * scene, ETransition * transition /* = nullptr */, bool saveCurrentScene /* = true */)
{
	ASSERT(scene != nullptr, "Next scene NULL pointer exception!");
	scene->retain();
	// 保存下一场景的指针
	getInstance()->m_pNextScene = scene;
	// 切换场景时，是否保存当前场景
	if (getInstance()->m_pCurrentScene)
	{
		getInstance()->m_pCurrentScene->m_bWillSave = saveCurrentScene;
	}
	// 设置切换场景动画
	if (transition)
	{
		getInstance()->m_pTransition = transition;
		transition->retain();
		transition->_setTarget(
			getInstance()->m_pCurrentScene, 
			getInstance()->m_pNextScene
		);
	}
}

void e2d::EApp::backScene(ETransition * transition /* = nullptr */)
{
	// 栈为空时，调用返回场景函数失败
	WARN_IF(s_SceneStack.size() == 0, "Scene stack now is empty!");
	if (s_SceneStack.size() == 0) return;

	// 从栈顶取出场景指针，作为下一场景
	getInstance()->m_pNextScene = s_SceneStack.top();
	s_SceneStack.pop();

	// 返回上一场景时，不保存当前场景
	if (getInstance()->m_pCurrentScene)
	{
		getInstance()->m_pCurrentScene->m_bWillSave = false;
	}

	// 设置切换场景动画
	if (transition)
	{
		getInstance()->m_pTransition = transition;
		transition->retain();
		transition->_setTarget(
			getInstance()->m_pCurrentScene, 
			getInstance()->m_pNextScene
		);
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

void e2d::EApp::setFPS(UINT32 fps)
{
	fps = min(max(fps, 30), 120);

	// 获取时钟频率
	LARGE_INTEGER tFreq;
	QueryPerformanceFrequency(&tFreq);
	EApp::getInstance()->m_nAnimationInterval.QuadPart = static_cast<LONGLONG>(1.0 / fps * tFreq.QuadPart);
}

e2d::EScene * e2d::EApp::getCurrentScene()
{
	return getInstance()->m_pCurrentScene;
}

void e2d::EApp::setAppName(const EString &appname)
{
	getInstance()->m_sAppName = appname;
}

e2d::EString e2d::EApp::getAppName()
{
	if (getInstance()->m_sAppName.isEmpty())
		getInstance()->m_sAppName = getInstance()->m_sTitle;
	return getInstance()->m_sAppName;
}

void e2d::EApp::setBkColor(UINT32 color)
{
	getInstance()->m_ClearColor = color;
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

e2d::EWindowStyle e2d::EApp::getWindowStyle()
{
	return getInstance()->m_WindowStyle;
}

LONGLONG e2d::EApp::getTotalDurationFromStart()
{
	return (s_tStart.QuadPart - GetNow().QuadPart) * 1000LL / GetFreq().QuadPart;
}

void e2d::EApp::hideWindow()
{
	ShowWindow(GetHWnd(), SW_HIDE);
}

void e2d::EApp::showWindow()
{
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
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

void e2d::EApp::_updateTime()
{
	// 刷新当前时间
	QueryPerformanceCounter(&GetNow());
	// 重置动画和定时器
	EActionManager::_resetAllActions();
	ETimerManager::_resetAllTimers();
}

LRESULT e2d::EApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (s_pInstance == nullptr)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	else
	{
		LRESULT result = 0;

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
			if (!s_pInstance->m_pTransition && !s_pInstance->m_pNextScene)
			{
				EMsgManager::MouseProc(message, wParam, lParam);
			}
		}
		result = 0;
		break;

		// 处理按键消息
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			// 执行场景切换时屏蔽按键和鼠标消息
			if (!s_pInstance->m_pTransition && !s_pInstance->m_pNextScene)
			{
				EMsgManager::KeyboardProc(message, wParam, lParam);
			}
		}
		result = 0;
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
		break;

		// 处理分辨率变化消息
		case WM_DISPLAYCHANGE:
		{
			// 重绘客户区
			InvalidateRect(hWnd, NULL, FALSE);
		}
		result = 0;
		break;

		// 重绘窗口
		case WM_PAINT:
		{
			s_pInstance->_render();
			ValidateRect(hWnd, NULL);
		}
		result = 0;
		break;

		// 窗口激活消息
		case WM_ACTIVATE:
		{
			EScene * pCurrentScene = s_pInstance->getCurrentScene();

			if (LOWORD(wParam) == WA_INACTIVE)
			{
				if (pCurrentScene &&
					pCurrentScene->onInactive() &&
					s_pInstance->onInactive())
				{
					s_pInstance->m_bPaused = true;
				}
			}
			else if (s_pInstance->m_bPaused)
			{
				if (pCurrentScene &&
					pCurrentScene->onActivate() &&
					s_pInstance->onActivate())
				{
					s_pInstance->m_bPaused = false;
					if (!s_pInstance->m_bManualPaused)
					{
						s_pInstance->_updateTime();
					}
				}
			}
		}
		result = 1;
		break;

		// 窗口关闭消息
		case WM_CLOSE:
		{
			EScene * pCurrentScene = s_pInstance->getCurrentScene();

			if (pCurrentScene)
			{
				if (pCurrentScene->onCloseWindow() && 
					s_pInstance->onCloseWindow())
				{
					DestroyWindow(hWnd);
				}
			}
			else 
			{
				if (s_pInstance->onCloseWindow())
				{
					DestroyWindow(hWnd);
				}
			}
		}
		result = 1;
		break;

		// 窗口被销毁
		case WM_DESTROY:
		{
			// 退出程序
			s_pInstance->quit();
			// 发送退出消息
			PostQuitMessage(0);
		}
		result = 1;
		break;

		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}

		return result;
	}
}
