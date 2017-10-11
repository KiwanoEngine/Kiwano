#include "..\easy2d.h"
#include "..\Win\winbase.h"
#include <stack>
#include <chrono>
#include <thread>
using namespace std::chrono;
using namespace std::this_thread;


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


EApp * s_pInstance = nullptr;
std::stack<Scene*> s_SceneStack;

e2d::EApp::EApp()
	: m_bRunning(false)
	, m_ClearColor(EColor::White)
	, m_bSaveScene(true)
	, m_pCurrentScene(nullptr)
	, m_pNextScene(nullptr)
	, m_pLoadingScene(nullptr)
{
	s_pInstance = this;		// 保存实例对象
}

e2d::EApp::~EApp()
{
	SafeReleaseInterface(&GetFactory());
	SafeReleaseInterface(&GetRenderTarget());
}

EApp * e2d::EApp::get()
{
	Assert(s_pInstance);	// 断言实例存在
	return s_pInstance;		// 获取 EApp 的唯一实例
}

bool e2d::EApp::init(EString title, ESize size, bool bShowConsole /* = false */)
{
	return init(title, size.width, size.height, bShowConsole);
}

bool e2d::EApp::init(EString title, UINT32 width, UINT32 height, bool bShowConsole /* = false */)
{
	m_sTitle = title;

	HRESULT hr;
	hr = CoInitialize(NULL);

	if (SUCCEEDED(hr))
	{
		// 关闭控制台.
		if (bShowConsole)
		{
			HWND hwnd = FindWindow(L"ConsoleWindowClass", NULL);

			if (hwnd)
			{
				hr = ShowWindow(hwnd, SW_HIDE);
			}
		}
	}

	if (SUCCEEDED(hr))
	{
		// 初始化 device-indpendent 资源
		// 比如 Direct2D factory.
		hr = CreateDeviceIndependentResources();

		if (SUCCEEDED(hr))
		{
			// 注册窗口类
			WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = EApp::WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(LONG_PTR);
			wcex.hInstance = HINST_THISCOMPONENT;
			wcex.hbrBackground = NULL;
			wcex.lpszMenuName = NULL;
			wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
			wcex.lpszClassName = L"E2DApp";

			RegisterClassEx(&wcex);


			// Because the CreateWindow function takes its size in pixels,
			// obtain the system DPI and use it to scale the window size.
			FLOAT dpiX, dpiY;

			// The factory returns the current system DPI. This is also the value it will use
			// to create its own windows.
			GetFactory()->GetDesktopDpi(&dpiX, &dpiY);


			// Create the window.
			GetHWnd() = CreateWindow(
				L"E2DApp",
				m_sTitle.c_str(),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				static_cast<UINT>(ceil(width * dpiX / 96.f)),
				static_cast<UINT>(ceil(height * dpiY / 96.f)),
				NULL,
				NULL,
				HINST_THISCOMPONENT,
				this
			);
			hr = GetHWnd() ? S_OK : E_FAIL;
		}
	}

	return SUCCEEDED(hr);
}

// 运行游戏
void e2d::EApp::run()
{
	// 显示窗口
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// 运行游戏
	m_bRunning = true;

	MSG msg;

	while (m_bRunning)
	{
		// 处理窗口消息
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_bRunning = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 执行主循环
		_mainLoop();
	}
	// 关闭窗口
	close();
	// 释放所有内存占用
	free();
}

void e2d::EApp::_mainLoop()
{
	static steady_clock::time_point nNow;
	static steady_clock::time_point nLast = steady_clock::now();
	// 帧间隔
	static LONGLONG nAnimationInterval = 17LL;
	// 时间间隔
	static LONGLONG nInterval = 0LL;
	// 挂起时长
	static LONGLONG nWaitMS = 0L;

	// 刷新计时
	nNow = steady_clock::now();
	// 计算时间间隔
	nInterval = duration_cast<milliseconds>(nNow - nLast).count();
	// 判断间隔时间是否足够
	if (nInterval >= nAnimationInterval)
	{
		// 记录当前时间
		nLast = nNow;
		// 
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
			std::this_thread::sleep_for(milliseconds(nWaitMS));
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
	Assert(m_pCurrentScene);

	//MouseMsg::__exec();			// 鼠标检测
	//KeyMsg::__exec();			// 键盘按键检测
	//Timer::__exec();			// 定时器执行程序
	//ActionManager::__exec();	// 动作管理器执行程序
	//FreePool::__flush();		// 刷新内存池
}

// This method discards device-specific
// resources if the Direct3D device dissapears during execution and
// recreates the resources the next time it's invoked.
bool e2d::EApp::_onRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		GetRenderTarget()->BeginDraw();

		GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

		GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor));

		m_pCurrentScene->_onDraw();	// 绘制当前场景

		hr = GetRenderTarget()->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return SUCCEEDED(hr);
}

void e2d::EApp::setWindowSize(int width, int height)
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
	// 销毁当前窗口
	// DestroyWindow(m_);
	// 修改窗口大小，并设置窗口在屏幕居中
	SetWindowPos(
		GetHWnd(), 
		HWND_TOP,
		(screenWidth - width) / 2,
		(screenHeight - height) / 2,
		width,
		height,
		SWP_SHOWWINDOW
	);
}

void e2d::EApp::setWindowSize(ESize size)
{
	setWindowSize(size.width, size.height);
}

void e2d::EApp::setWindowTitle(EString title)
{
	// 设置窗口标题
	SetWindowText(GetHWnd(), title.c_str());
	// 保存当前标题，用于修改窗口大小时恢复标题
	m_sTitle = title;
}

EString e2d::EApp::getTitle()
{
	return m_sTitle;
}

int e2d::EApp::getWidth()
{
	return GetRenderTarget()->GetPixelSize().width;
}

int e2d::EApp::getHeight()
{
	return GetRenderTarget()->GetPixelSize().height;
}

void e2d::EApp::enterScene(Scene * scene, bool save /* = true */)
{
	// 保存下一场景的指针
	m_pNextScene = scene;
	// 切换场景时，是否保存当前场景
	m_bSaveScene = save;
}

void e2d::EApp::backScene()
{
	// 从栈顶取出场景指针，作为下一场景
	m_pNextScene = s_SceneStack.top();
	// 不保存当前场景
	m_bSaveScene = false;
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

Scene * e2d::EApp::getCurrentScene()
{
	return m_pCurrentScene;
}

Scene * e2d::EApp::getLoadingScene()
{
	return m_pLoadingScene;
}

void e2d::EApp::setAppName(EString appname)
{
	s_pInstance->m_sAppName = appname;
}

EString e2d::EApp::getAppName()
{
	return s_pInstance->m_sAppName;
}

void EApp::setBkColor(EColor::Enum color)
{
	m_ClearColor = color;
}

void e2d::EApp::close()
{
	ShowWindow(GetHWnd(), SW_HIDE);
}

void e2d::EApp::show()
{
	ShowWindow(GetHWnd(), SW_NORMAL);
}

void EApp::free()
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
	// 删除所有定时器
	//Timer::clearAllTimers();
	//MouseMsg::clearAllListeners();
	//KeyMsg::clearAllListeners();
	//ActionManager::clearAllActions();
	// 删除所有对象
	//FreePool::__clearAllObjects();
}

void EApp::quit()
{
	m_bRunning = false;
}

void EApp::end()
{
	m_bRunning = false;
}

void EApp::_enterNextScene()
{
	bool bBackScene = false;

	// 若下一场景处于栈顶，说明正在返回上一场景
	if (s_SceneStack.size() && m_pNextScene == s_SceneStack.top())
	{
		bBackScene = true;
		// 删除栈顶场景
		s_SceneStack.pop();
	}

	// 执行当前场景的 onExit 函数
	if (m_pCurrentScene)
	{
		m_pCurrentScene->onExit();
		if (m_bSaveScene)
		{
			// 若要保存当前场景，把它放入栈中
			s_SceneStack.push(m_pCurrentScene);
			// 暂停当前场景上运行的所有定时器
			//Timer::waitAllSceneTimers(m_pCurrentScene);
			//MouseMsg::waitAllSceneListeners(m_pCurrentScene);
			//KeyMsg::waitAllSceneListeners(m_pCurrentScene);
			//ActionManager::waitAllSceneActions(m_pCurrentScene);
		}
		else
		{
			// 不保存场景时，停止当前场景上运行的所有定时器，并删除当前场景
			//Timer::clearAllSceneTimers(m_pCurrentScene);
			//MouseMsg::clearAllSceneListeners(m_pCurrentScene);
			//KeyMsg::clearAllSceneListeners(m_pCurrentScene);
			//ActionManager::stopAllSceneActions(m_pCurrentScene);
			SafeDelete(&m_pCurrentScene);
		}
	}

	m_pCurrentScene = m_pNextScene;		// 切换场景
	m_pNextScene = nullptr;				// 下一场景置空

	if (bBackScene)
	{
		// 返回上一场景时，恢复场景上的定时器
		//Timer::notifyAllSceneTimers(m_pCurrentScene);
		//MouseMsg::notifyAllSceneListeners(m_pCurrentScene);
		//KeyMsg::notifyAllSceneListeners(m_pCurrentScene);
		//ActionManager::notifyAllSceneActions(m_pCurrentScene);
	}
	else
	{
		m_pCurrentScene->init();		// 进入一个新场景时，执行它的 init 函数
	}

	m_pCurrentScene->onEnter();			// 执行下一场景的 onEnter 函数
}

// Creates resources that are not bound to a particular device.
// Their lifetime effectively extends for the duration of the
// application.
HRESULT e2d::EApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &GetFactory());

	return hr;
}

// Creates resources that are bound to a particular
// Direct3D device. These resources need to be recreated
// if the Direct3D device dissapears, such as when the display
// changes, the window is remoted, etc.
HRESULT e2d::EApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!GetRenderTarget())
	{
		RECT rc;
		GetClientRect(GetHWnd(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = GetFactory()->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(GetHWnd(), size),
			&GetRenderTarget()
		);
	}

	return hr;
}

// Discards device-dependent resources. These resources must be
// recreated when the Direct3D device is lost.
void e2d::EApp::DiscardDeviceResources()
{
	SafeReleaseInterface(&GetRenderTarget());
}

//  If the application receives a WM_SIZE message, this method
//  resizes the render target appropriately.
void e2d::EApp::_onResize(UINT width, UINT height)
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
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		EApp *pEApp = (EApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			GetHWnd(),
			GWLP_USERDATA,
			PtrToUlong(pEApp)
		);

		result = 1;
	}
	else
	{
		EApp *pEApp = reinterpret_cast<EApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				GetHWnd(),
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pEApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pEApp->_onResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(GetHWnd(), NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pEApp->_onRender();
				ValidateRect(GetHWnd(), NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(GetHWnd(), message, wParam, lParam);
		}
	}

	return result;
}


