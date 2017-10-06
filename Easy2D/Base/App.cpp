#include "..\easy2d.h"
#include "..\EasyX\easyx.h"
#include <time.h>
#include <assert.h>
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// App 的唯一实例
static App * s_pInstance = nullptr;
// 坐标原点的物理坐标
static int originX = 0;
static int originY = 0;

App::App() : 
	m_pCurrentScene(nullptr), 
	m_pNextScene(nullptr), 
	m_bRunning(false), 
	m_nWindowMode(0),
	m_bSaveScene(true)
{
	assert(!s_pInstance);	// 不能同时存在两个 App 实例
	s_pInstance = this;		// 保存实例对象
	setFPS(60);				// 默认 FPS 为 60
}

App::~App()
{
}

App * App::get()
{
	assert(s_pInstance);	// 断言实例存在
	return s_pInstance;		// 获取 App 的唯一实例
}

int App::run()
{
	// 开启批量绘图
	BeginBatchDraw();
	// 修改时间精度
	timeBeginPeriod(1);
	// 获取 CPU 每秒滴答声个数
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	// 创建时间变量
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;
	// 记录当前时间
	QueryPerformanceCounter(&nLast);
	// 时间间隔
	LONGLONG interval = 0LL;
	// 挂起时长
	LONG waitMS = 0L;
	// 将隐藏的窗口显示
	ShowWindow(GetHWnd(), SW_NORMAL);
	// 运行游戏
	m_bRunning = true;

	// 进入主循环
	while (m_bRunning)
	{
		// 获取当前时间
		QueryPerformanceCounter(&nNow);
		// 计算时间间隔
		interval = nNow.QuadPart - nLast.QuadPart;
		// 判断间隔时间是否足够
		if (interval >= m_nAnimationInterval.QuadPart)
		{
			// 记录当前时间
			nLast.QuadPart = nNow.QuadPart;
			// 执行游戏逻辑
			_mainLoop();
		}
		else
		{
			// 计算挂起时长
			waitMS = LONG((m_nAnimationInterval.QuadPart - interval) * 1000LL / freq.QuadPart) - 1L;
			// 挂起线程，释放 CPU 占用
			if (waitMS > 1L) Sleep(waitMS);
		}
	}
	// 停止批量绘图
	EndBatchDraw();
	// 关闭窗口
	close();
	// 释放所有内存占用
	free();
	// 重置时间精度
	timeEndPeriod(1);

	return 0;
}

void App::_initGraph()
{
	// 创建绘图环境
	initgraph(m_Size.cx, m_Size.cy, m_nWindowMode);
	// 隐藏当前窗口（防止在加载阶段显示黑窗口）
	ShowWindow(GetHWnd(), SW_HIDE);
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// 获取窗口大小
	CRect rcWindow;
	GetWindowRect(GetHWnd(), &rcWindow);
	// 设置窗口在屏幕居中
	SetWindowPos(GetHWnd(), HWND_TOP,
		(screenWidth - rcWindow.Size().cx) / 2,
		(screenHeight - rcWindow.Size().cy) / 2,
		rcWindow.Size().cx,
		rcWindow.Size().cy,
		SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOSIZE);
	// 禁用输入法
	ImmAssociateContext(GetHWnd(), NULL);
	// 重置绘图环境
	reset();
	// 设置窗口标题
	if (m_sTitle.empty())
	{
		// 保存当前标题
		TCHAR title[31];
		GetWindowText(GetHWnd(), title, 30);
		m_sTitle = title;
		if (m_sAppName.empty()) m_sAppName = title;
	}
	else
	{
		setWindowTitle(m_sTitle);
	}
}

void App::_mainLoop()
{
	// 下一场景指针不为空时，切换场景
	if (m_pNextScene)
	{
		// 进入下一场景
		_enterNextScene();
	}
	// 断言当前场景非空
	assert(m_pCurrentScene);
	
	cleardevice();				// 清空画面
	m_pCurrentScene->_onDraw();	// 绘制当前场景
	FlushBatchDraw();			// 刷新画面

	// 其他执行程序
	MouseMsg::__exec();			// 鼠标检测
	KeyMsg::__exec();			// 键盘按键检测
	Timer::__exec();			// 定时器执行程序
	ActionManager::__exec();	// 动作管理器执行程序
	FreePool::__flush();		// 刷新内存池
}

void App::createWindow(int width, int height, int mode)
{
	// 保存窗口信息
	m_Size.cx = width;
	m_Size.cy = height;
	m_nWindowMode = mode;
	// 创建窗口
	_initGraph();
}

void App::createWindow(CSize size, int mode)
{
	createWindow(size.cx, size.cy, mode);
}

void App::createWindow(TString title, int width, int height, int mode)
{
	// 保存窗口信息
	m_Size.cx = width;
	m_Size.cy = height;
	m_nWindowMode = mode;
	m_sTitle = title;
	if (m_sAppName.empty()) m_sAppName = title;
	// 创建窗口
	_initGraph();
}

void App::createWindow(TString title, CSize size, int mode)
{
	createWindow(title, size.cx, size.cy, mode);
}

void App::setWindowSize(int width, int height)
{
	// 游戏正在运行时才允许修改窗口大小
	assert(s_pInstance->m_bRunning);

	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// 获取窗口大小（包含菜单栏）
	CRect rcWindow;
	GetWindowRect(GetHWnd(), &rcWindow);
	// 获取客户区大小
	CRect rcClient;
	GetClientRect(GetHWnd(), &rcClient);
	// 计算边框大小
	width += (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	height += (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
	// 销毁当前窗口
	// DestroyWindow(GetHWnd());/* 无法操作多线程导致失效 */
	// 修改窗口大小，并设置窗口在屏幕居中
	SetWindowPos(GetHWnd(), HWND_TOP,
		(screenWidth - width) / 2,
		(screenHeight - height) / 2,
		width,
		height,
		SWP_SHOWWINDOW);
	// 重置窗口属性
	reset();
}

void App::setWindowSize(CSize size)
{
	setWindowSize(size.cx, size.cy);
}

void App::setWindowTitle(TString title)
{
	// 设置窗口标题
	SetWindowText(GetHWnd(), title.c_str());
	// 保存当前标题，用于修改窗口大小时恢复标题
	s_pInstance->m_sTitle = title;
}

TString App::getWindowTitle()
{
	return s_pInstance->m_sTitle;
}

void App::close()
{
	closegraph();	// 关闭绘图环境
}

void App::enterScene(Scene * scene, bool save)
{
	// 保存下一场景的指针
	s_pInstance->m_pNextScene = scene;
	// 切换场景时，是否保存当前场景
	s_pInstance->m_bSaveScene = save;
}

void App::backScene()
{
	// 从栈顶取出场景指针，作为下一场景
	s_pInstance->m_pNextScene = s_pInstance->m_SceneStack.top();
	// 不保存当前场景
	s_pInstance->m_bSaveScene = false;
}

void App::clearScene()
{
	// 清空场景栈
	while (s_pInstance->m_SceneStack.size())
	{
		auto temp = s_pInstance->m_SceneStack.top();
		SafeDelete(temp);
		s_pInstance->m_SceneStack.pop();
	}
}

void App::setAppName(TString appname)
{
	s_pInstance->m_sAppName = appname;
}

TString App::getAppName()
{
	return s_pInstance->m_sAppName;
}

void App::setBkColor(COLORREF color)
{
	setbkcolor(color);
}

void App::_enterNextScene()
{
	bool bBackScene = false;

	// 若下一场景处于栈顶，说明正在返回上一场景
	if (m_SceneStack.size() && m_pNextScene == m_SceneStack.top())
	{
		bBackScene = true;
		// 删除栈顶场景
		m_SceneStack.pop();
	}

	// 执行当前场景的 onExit 函数
	if (m_pCurrentScene)
	{
		m_pCurrentScene->onExit();
		if (m_bSaveScene)
		{
			// 若要保存当前场景，把它放入栈中
			m_SceneStack.push(m_pCurrentScene);
			// 暂停当前场景上运行的所有定时器
			Timer::stopAllSceneTimers(m_pCurrentScene);
			MouseMsg::stopAllSceneListeners(m_pCurrentScene);
			KeyMsg::stopAllSceneListeners(m_pCurrentScene);
			ActionManager::pauseAllSceneActions(m_pCurrentScene);
		}
		else
		{
			// 不保存场景时，停止当前场景上运行的所有定时器，并删除当前场景
			Timer::clearAllSceneTimers(m_pCurrentScene);
			MouseMsg::clearAllSceneListeners(m_pCurrentScene);
			KeyMsg::clearAllSceneListeners(m_pCurrentScene);
			ActionManager::stopAllSceneActions(m_pCurrentScene);
			SafeDelete(m_pCurrentScene);
		}
	}

	m_pCurrentScene = m_pNextScene;		// 切换场景
	m_pNextScene = nullptr;				// 下一场景置空

	if (bBackScene)
	{
		// 返回上一场景时，恢复场景上的定时器
		Timer::startAllSceneTimers(m_pCurrentScene);
		MouseMsg::startAllSceneListeners(m_pCurrentScene);
		KeyMsg::startAllSceneListeners(m_pCurrentScene);
		ActionManager::startAllSceneActions(m_pCurrentScene);
	}
	else
	{
		m_pCurrentScene->init();		// 进入一个新场景时，执行它的 init 函数
	}

	m_pCurrentScene->onEnter();			// 执行下一场景的 onEnter 函数
}

void App::quit()
{
	s_pInstance->m_bRunning = false;
}

void App::end()
{
	s_pInstance->m_bRunning = false;
}

void App::reset()
{
	// 重置绘图环境
	graphdefaults();
	setbkmode(TRANSPARENT);
	setbkcolor(Color::black);
}

Scene * App::getCurrentScene()
{
	// 获取当前场景的指针
	if (s_pInstance->m_pCurrentScene)
	{
		return s_pInstance->m_pCurrentScene;
	}
	else
	{
		return s_pInstance->m_pNextScene;
	}
}

void App::setFPS(DWORD fps)
{
	// 设置画面帧率，以毫秒为单位
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	s_pInstance->m_nAnimationInterval.QuadPart = (LONGLONG)(1.0 / fps * nFreq.QuadPart);
}

int App::getWidth()
{
	return s_pInstance->m_Size.cx;
}

int App::getHeight()
{
	return s_pInstance->m_Size.cy;
}

void App::setOrigin(int originX, int originY)
{
	::originX = originX;
	::originY = originY;
	setorigin(originX, originY);
}

int App::getOriginX()
{
	return ::originX;
}

int App::getOriginY()
{
	return ::originY;
}

void App::free()
{
	// 释放场景内存
	SafeDelete(m_pCurrentScene);
	SafeDelete(m_pNextScene);
	// 清空场景栈
	while (m_SceneStack.size())
	{
		auto temp = m_SceneStack.top();
		SafeDelete(temp);
		m_SceneStack.pop();
	}
	// 删除所有定时器
	Timer::clearAllTimers();
	MouseMsg::clearAllListeners();
	KeyMsg::clearAllListeners();
	ActionManager::clearAllActions();
	// 删除所有对象
	FreePool::__clearAllObjects();
}
