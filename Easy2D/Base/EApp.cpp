#include "..\ebase.h"
#include "..\Win\winbase.h"
#include "..\emsg.h"
#include "..\etools.h"
#include <stack>
#include <chrono>
#include <thread>
using namespace std::chrono;
using namespace std::this_thread;


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


e2d::EApp * s_pInstance = nullptr;
std::stack<e2d::EScene*> s_SceneStack;

e2d::EApp::EApp()
	: m_bRunning(false)
	, m_ClearColor(EColor::Black)
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

e2d::EApp * e2d::EApp::get()
{
	ASSERT(s_pInstance != nullptr);
	return s_pInstance;		// 获取 EApp 的唯一实例
}

bool e2d::EApp::init(e2d::EString title, e2d::ESize size, bool bShowConsole /* = false */)
{
	return init(title, size.cx, size.cy, bShowConsole);
}

bool e2d::EApp::init(e2d::EString title, UINT32 width, UINT32 height, bool bShowConsole /* = false */)
{
	HRESULT hr;
	hr = CoInitialize(NULL);

	if (SUCCEEDED(hr))
	{
		// 关闭控制台
		if (!bShowConsole)
		{
			HWND consoleHWnd = FindWindow(L"ConsoleWindowClass", NULL);

			if (consoleHWnd)
			{
				ShowWindow(consoleHWnd, SW_HIDE);
			}
		}

		// 初始化 device-indpendent 资源
		// 比如 Direct2D factory.
		hr = _createDeviceIndependentResources();
	}

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

		m_sTitle = title;

		// Create the window.
		GetHWnd() = CreateWindow(
			L"E2DApp",
			m_sTitle.c_str(),
			WS_OVERLAPPEDWINDOW | CS_DBLCLKS,
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
		if (FAILED(hr))
		{
			UnregisterClass(L"E2DApp", HINST_THISCOMPONENT);
			MessageBox(nullptr, L"Create Window Failed!", L"Error", MB_OK);
		}
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Initialize Failed!", L"Error", MB_OK);
	}

	return SUCCEEDED(hr);
}

// 运行游戏
void e2d::EApp::run()
{
	ASSERT(m_pNextScene != nullptr);
	// 进入第一个场景
	_enterNextScene();
	// 显示窗口
	ShowWindow(GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(GetHWnd());
	// 运行游戏
	m_bRunning = true;

	MSG msg;

	while (m_bRunning)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 处理窗口消息
			if (msg.message == WM_QUIT)
			{
				m_bRunning = false;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 执行主循环
			_mainLoop();
		}
	}
	// 游戏结束后再执行一次循环
	_mainLoop();
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
		// 执行游戏控制
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
	ASSERT(m_pCurrentScene != nullptr);

	//MouseMsg::__exec();			// 鼠标检测
	//KeyMsg::__exec();			// 键盘按键检测
	//Timer::__exec();			// 定时器执行程序
	//ActionManager::__exec();	// 动作管理器执行程序
	EObjectManager::__flush();		// 刷新内存池
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

		GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

		GetRenderTarget()->Clear(D2D1::ColorF(m_ClearColor));

		m_pCurrentScene->_onRender();	// 绘制当前场景

		hr = GetRenderTarget()->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		_discardDeviceResources();
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

void e2d::EApp::setWindowSize(e2d::ESize size)
{
	setWindowSize(size.cx, size.cy);
}

void e2d::EApp::setWindowTitle(e2d::EString title)
{
	// 设置窗口标题
	SetWindowText(GetHWnd(), title.c_str());
	// 保存当前标题，用于修改窗口大小时恢复标题
	m_sTitle = title;
}

e2d::EString e2d::EApp::getTitle()
{
	return m_sTitle;
}

e2d::ESize e2d::EApp::getSize()
{
	return e2d::ESize(GetRenderTarget()->GetPixelSize().width, GetRenderTarget()->GetPixelSize().height);
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

e2d::EScene * e2d::EApp::getCurrentScene()
{
	return m_pCurrentScene;
}

e2d::EScene * e2d::EApp::getLoadingScene()
{
	return m_pLoadingScene;
}

void e2d::EApp::setLoadingScene(EScene * scene)
{
	m_pLoadingScene = scene;
}

void e2d::EApp::setAppName(e2d::EString appname)
{
	s_pInstance->m_sAppName = appname;
}

e2d::EString e2d::EApp::getAppName()
{
	return s_pInstance->m_sAppName;
}

void e2d::EApp::setBkColor(EColor::Enum color)
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
	// 删除所有定时器
	//Timer::clearAllTimers();
	//MouseMsg::clearAllListeners();
	//KeyMsg::clearAllListeners();
	//ActionManager::clearAllActions();
	// 删除所有对象
	//EObjectManager::__clearAllObjects();
}

void e2d::EApp::quit()
{
	m_bRunning = false;
}

void e2d::EApp::end()
{
	m_bRunning = false;
}

void e2d::EApp::_enterNextScene()
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
		EMsgManager::notifyAllListenersOnScene(m_pCurrentScene);
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

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Create Device Resources Failed!", L"Error", MB_OK);
	}

	return hr;
}

// Discards device-dependent resources. These resources must be
// recreated when the Direct3D device is lost.
void e2d::EApp::_discardDeviceResources()
{
	SafeReleaseInterface(&GetRenderTarget());
}

//  If the application receives a WM_SIZE message, this method
//  re2d::ESizes the render target appropriately.
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
		e2d::EApp *pEApp = (e2d::EApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hWnd,
			GWLP_USERDATA,
			PtrToUlong(pEApp)
		);

		result = 1;
	}
	else
	{
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
			/*case WM_ACTIVATE:
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					MSG msg;
					do
					{
						GetMessage(&msg, nullptr, 0, 0);
					} while (msg.wParam != WA_ACTIVE);
				}
			}*/
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
			break;

			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_CHAR:
			{
				EMsgManager::KeyboardProc(message, wParam, lParam);
			}
			result = 0;
			break;

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
				InvalidateRect(hWnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pEApp->_onRender();
				ValidateRect(hWnd, NULL);
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
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return result;
}
