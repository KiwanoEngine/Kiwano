#include "..\ebase.h"
#include "..\Win\winbase.h"
#include "..\emsg.h"
#include "..\etools.h"
#include "..\enodes.h"
#include <stack>
#include <thread>
#include <imm.h>  
#pragma comment (lib ,"imm32.lib")

using namespace std::this_thread;
using namespace std::chrono;


// 唯一实例指针
e2d::EApp * s_pInstance = nullptr;
// 场景栈
std::stack<e2d::EScene*> s_SceneStack;

e2d::EApp::EApp()
	: m_bRunning(false)
	, nAnimationInterval(17LL)
	, m_ClearColor(EColor::Black)
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
	SafeReleaseInterface(&GetRenderTarget());
	SafeReleaseInterface(&GetFactory());

	CoUninitialize();
}

e2d::EApp * e2d::EApp::get()
{
	ASSERT(s_pInstance != nullptr, "Nonexistent EApp instance.");
	return s_pInstance;		// 获取 EApp 的唯一实例
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, bool showConsole /* = false */)
{
	return init(title, width, height, WS_OVERLAPPEDWINDOW, showConsole);
}

bool e2d::EApp::init(const EString &title, UINT32 width, UINT32 height, int windowStyle, bool showConsole /* = false */)
{
	HRESULT hr;

	// 显示或关闭控制台
	EApp::showConsole(showConsole);

	// 创建设备无关资源
	hr = _createDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
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

		RegisterClassEx(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		GetFactory()->GetDesktopDpi(&dpiX, &dpiY);

		width = static_cast<UINT>(ceil(width * dpiX / 96.f));
		height = static_cast<UINT>(ceil(height * dpiY / 96.f));

		// 获取屏幕分辨率
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		// 创建屏幕居中的矩形
		RECT rtWindow;
		rtWindow.left = (screenWidth - width) / 2;
		rtWindow.top = (screenHeight - height) / 2;
		rtWindow.right = rtWindow.left + width;
		rtWindow.bottom = rtWindow.top + height;
		// 计算客户区大小
		AdjustWindowRectEx(&rtWindow, windowStyle, FALSE, 0L);
		// 保存窗口名称
		m_sTitle = title;
		// 创建窗口
		GetHWnd() = CreateWindow(
			L"Easy2DApp",
			m_sTitle.c_str(),
			windowStyle,
			rtWindow.left,
			rtWindow.top,
			rtWindow.right - rtWindow.left,
			rtWindow.bottom - rtWindow.top,
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
		}
		else
		{
			UnregisterClass(L"E2DApp", HINST_THISCOMPONENT);
		}
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Initialize Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

void e2d::EApp::showConsole(bool show)
{
	// 查找已存在的控制台句柄
	HWND hwnd = GetConsoleWindow();
	static FILE * stdoutstream = nullptr;
	static FILE * stdinstream = nullptr;
	static FILE * stderrstream = nullptr;
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
	ASSERT(m_pCurrentScene != nullptr, "Current scene NULL pointer exception.");
	// 显示窗口
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// 运行游戏
	m_bRunning = true;

	MSG msg;

	while (m_bRunning)
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
	// 释放所有内存资源
	this->free();
}

void e2d::EApp::setFPS(UINT32 fps)
{
	fps = min(max(fps, 30), 120);
	nAnimationInterval = 1000 / fps;
}

bool e2d::EApp::onExit()
{
	return true;
}

void e2d::EApp::_mainLoop()
{
	// 时间间隔
	static LONGLONG nInterval = 0LL;
	// 挂起时长
	static LONGLONG nWaitMS = 0L;
	// 刷新计时
	static steady_clock::time_point tLast = steady_clock::now();

	GetNow() = steady_clock::now();
	// 计算时间间隔
	nInterval = GetInterval(tLast);
	// 判断间隔时间是否足够
	if (nInterval >= nAnimationInterval)
	{
		// 记录当前时间
		tLast = GetNow();
		// 游戏控制流程
		_onControl();
		// 刷新游戏画面
		if (!_onRender())
		{
			MessageBox(GetHWnd(), L"Game Render Failed!", L"Error", MB_OK);
			this->quit();
		}
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
	// 下一场景指针不为空时，切换场景
	if (m_pNextScene)
	{
		// 进入下一场景
		_enterNextScene();
	}
	// 断言当前场景非空
	ASSERT(m_pCurrentScene != nullptr, "Current scene NULL pointer exception.");

	ETimerManager::TimerProc();		// 定时器执行程序
	//ActionManager::__exec();	// 动作管理器执行程序
	EObjectManager::__flush();	// 刷新内存池
}

// This method discards device-specific
// resources if the Direct3D device dissapears during execution and
// recreates the resources the next time it's invoked.
bool e2d::EApp::_onRender()
{
	HRESULT hr = S_OK;

	hr = _createDeviceResources();

	if (SUCCEEDED(hr))
	{
		GetRenderTarget()->BeginDraw();
		// 使用背景色清空屏幕
		GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor));
		// 绘制当前场景
		m_pCurrentScene->_onRender();
		
		hr = GetRenderTarget()->EndDraw();
		// 刷新界面
		UpdateWindow(GetHWnd());
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		_discardDeviceResources();
	}

	return SUCCEEDED(hr);
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

UINT32 e2d::EApp::getWidth()
{
	return GetRenderTarget()->GetPixelSize().width;
}

UINT32 e2d::EApp::getHeight()
{
	return GetRenderTarget()->GetPixelSize().height;
}

void e2d::EApp::enterScene(EScene * scene, bool save /* = true */)
{
	// 保存下一场景的指针
	get()->m_pNextScene = scene;
	// 切换场景时，是否保存当前场景
	if (get()->m_pCurrentScene)
	{
		get()->m_pCurrentScene->m_bWillSave = save;
	}
}

void e2d::EApp::backScene()
{
	ASSERT(s_SceneStack.size(), "Scene stack now is empty!");
	// 从栈顶取出场景指针，作为下一场景
	get()->m_pNextScene = s_SceneStack.top();
	s_SceneStack.pop();
	// 不保存当前场景
	if (get()->m_pCurrentScene)
	{
		get()->m_pCurrentScene->m_bWillSave = false;
	}
}

void e2d::EApp::clearScene()
{
	// 清空场景栈
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeDelete(&temp);
		s_SceneStack.pop();
	}
}

e2d::EScene * e2d::EApp::getCurrentScene()
{
	return get()->m_pCurrentScene;
}

void e2d::EApp::setAppName(const EString &appname)
{
	s_pInstance->m_sAppName = appname;
}

e2d::EString e2d::EApp::getAppName()
{
	if (s_pInstance->m_sAppName.empty())
		s_pInstance->m_sAppName = s_pInstance->m_sTitle;
	return s_pInstance->m_sAppName;
}

void e2d::EApp::setBkColor(EColor color)
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

void e2d::EApp::closeWindow()
{
	ShowWindow(GetHWnd(), SW_HIDE);
}

void e2d::EApp::showWindow()
{
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
}

void e2d::EApp::free()
{
	// 释放场景内存
	SafeDelete(&m_pCurrentScene);
	SafeDelete(&m_pNextScene);
	// 清空场景栈
	while (s_SceneStack.size())
	{
		auto temp = s_SceneStack.top();
		SafeDelete(&temp);
		s_SceneStack.pop();
	}
	// 删除图片缓存
	ESprite::clearCache();
	// 删除所有定时器、监听器和动画
	ETimerManager::clearAllTimers();
	EMsgManager::clearAllKeyboardListeners();
	EMsgManager::clearAllMouseListeners();
	//ActionManager::clearAllActions();
	// 删除所有对象
	EObjectManager::clearAllObjects();
}

void e2d::EApp::quit()
{
	get()->m_bRunning = false;
}

void e2d::EApp::end()
{
	get()->m_bRunning = false;
}

void e2d::EApp::_enterNextScene()
{
	// 执行当前场景的 onExit 函数
	if (m_pCurrentScene)
	{
		m_pCurrentScene->onExit();
		m_pCurrentScene->_onExit();

		if (m_pCurrentScene->m_bWillSave)
		{
			// 若要保存当前场景，把它放入栈中
			s_SceneStack.push(m_pCurrentScene);
		}
		else
		{
			SafeDelete(&m_pCurrentScene);
		}
	}

	// 执行下一场景的 onEnter 函数
	m_pNextScene->_onEnter();
	m_pNextScene->onEnter();

	m_pCurrentScene = m_pNextScene;		// 切换场景
	m_pNextScene = nullptr;				// 下一场景置空
}

// Creates resources that are not bound to a particular device.
// Their lifetime effectively extends for the duration of the
// application.
HRESULT e2d::EApp::_createDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &GetFactory());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Create Device Independent Resources Failed!", L"Error", MB_OK);
	}

	return hr;
}

// Creates resources that are bound to a particular
// Direct3D device. These resources need to be recreated
// if the Direct3D device dissapears, such as when the isVisiable
// changes, the window is remoted, etc.
HRESULT e2d::EApp::_createDeviceResources()
{
	// 这个函数将自动创建设备相关资源
	GetRenderTarget();

	return S_OK;
}

// Discards device-dependent resources. These resources must be
// recreated when the Direct3D device is lost.
void e2d::EApp::_discardDeviceResources()
{
	SafeReleaseInterface(&GetRenderTarget());
}

//  If the application receives a WM_SIZE message, this method
//  re2d::ESizes the render target appropriately.
void e2d::EApp::_onResize(UINT32 width, UINT32 height)
{
	if (GetRenderTarget())
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		GetRenderTarget()->Resize(D2D1::SizeU(width, height));
	}
}

// Handles window messages.
LRESULT e2d::EApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
				EMsgManager::MouseProc(message, wParam, lParam);
			}
			result = 0;
			wasHandled = true;
			break;

			// 处理按键消息
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				EMsgManager::KeyboardProc(message, wParam, lParam);
			}
			result = 0;
			wasHandled = true;
			break;

			// 处理窗口大小变化消息
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pEApp->_onResize(width, height);
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

			// 窗口关闭消息
			case WM_CLOSE:
			{
				if (pEApp->onExit())
				{
					DestroyWindow(hWnd);
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

		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return result;
}
