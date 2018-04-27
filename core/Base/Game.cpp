#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"


// 控制游戏终止
static bool s_bEndGame = false;
// 控制游戏暂停
static bool s_bPaused = false;
// 是否进行过初始化
static bool s_bInitialized = false;
// 游戏名称
static e2d::String s_sGameName;


bool e2d::Game::init(String sGameName)
{
	if (s_bInitialized)
	{
		WARN_IF(true, "The game has been initialized!");
		return false;
	}

	// 初始化 COM 组件
	CoInitialize(NULL);

	// 创建设备无关资源
	if (!Renderer::__createDeviceIndependentResources())
	{
		WARN_IF(true, "Renderer::__createDeviceIndependentResources Failed!");
		goto dev_ind_res_fail;
	}

	// 初始化窗口
	if (!Window::__init())
	{
		WARN_IF(true, "Window::__init Failed!");
		goto window_fail;
	}

	// 创建设备相关资源
	if (!Renderer::__createDeviceResources())
	{
		WARN_IF(true, "Renderer::__createDeviceResources Failed!");
		goto dev_res_fail;
	}

	// 初始化 DirectInput
	if (!Input::__init())
	{
		WARN_IF(true, "Input::__init Failed!");
		goto input_fail;
	}

	// 初始化播放器
	if (!Music::__init())
	{
		WARN_IF(true, "Music::__init Failed!");
		Music::__uninit();
	}

	// 保存游戏名称
	s_sGameName = sGameName;

	// 初始化路径
	if (!Path::__init())
	{
		WARN_IF(true, "Path::__init Failed!");
	}

	// 初始化成功
	s_bInitialized = true;
	goto succeeded;

input_fail:
	Input::__uninit();

dev_res_fail:
	Renderer::__discardDeviceResources();

window_fail:
	Window::__init();

dev_ind_res_fail:
	Renderer::__discardResources();

succeeded:
	return s_bInitialized;
}

int e2d::Game::start(bool bAutoRelease/* true */)
{
	if (!s_bInitialized)
	{
		ASSERT(false, "You must initialize Game first!");
		return -1;
	}

	// 初始化场景管理器
	SceneManager::__init();
	// 显示窗口
	::ShowWindow(Window::getHWnd(), SW_SHOWNORMAL);
	// 刷新窗口内容
	::UpdateWindow(Window::getHWnd());
	// 处理窗口消息
	Window::__poll();
	// 初始化计时
	Time::__init();

	while (!s_bEndGame)
	{
		// 处理窗口消息
		Window::__poll();
		// 刷新时间
		Time::__updateNow();

		// 判断是否达到了刷新状态
		if (Time::__isReady())
		{
			Timer::__update();				// 定时器管理器执行程序
			ActionManager::__update();		// 动作管理器执行程序
			while (Time::__isReady())
			{
				Input::__update();			// 获取用户输入
				SceneManager::__update();	// 更新场景内容
				Time::__updateLast();		// 刷新时间信息
			}
			Renderer::__render();			// 渲染游戏画面
		}
		else
		{
			ObjectManager::__update();		// 刷新内存池
			Time::__sleep();				// 挂起线程
		}
	}

	if (bAutoRelease)
	{
		Game::destroy();
	}

	return 0;
}

void e2d::Game::pause()
{
	s_bPaused = true;
}

void e2d::Game::resume()
{
	if (isPaused())
	{
		s_bPaused = false;
		// 刷新当前时间
		Time::__updateLast();
		// 重置动画和定时器
		ActionManager::__resetAll();
		Timer::__resetAll();
	}
}

bool e2d::Game::isPaused()
{
	return s_bPaused;
}

void e2d::Game::quit()
{
	s_bEndGame = true;	// 这个变量将控制游戏是否结束
}

void e2d::Game::destroy()
{
	// 删除所有场景
	SceneManager::__uninit();
	// 删除监听器
	InputManager::__uninit();
	ColliderManager::__uninit();
	// 删除动画
	ActionManager::__uninit();
	// 删除所有对象
	ObjectManager::__clear();
	// 清空图片缓存
	Image::clearCache();
	// 回收音乐相关资源
	Music::__uninit();
	// 清空定时器
	Timer::__uninit();
	// 关闭输入
	Input::__uninit();
	// 恢复计时操作
	Time::__uninit();
	// 删除渲染相关资源
	Renderer::__discardResources();
	// 销毁窗口
	Window::__uninit();

	CoUninitialize();

	s_bInitialized = false;
}

bool e2d::Game::createMutex(String sMutexName, String sWindowTitle)
{
	// 创建进程互斥体
	HANDLE m_hMutex = ::CreateMutex(NULL, TRUE, L"Easy2DApp-" + sMutexName);

	if (m_hMutex == nullptr)
	{
		WARN_IF(true, "CreateMutex Failed!");
		return true;
	}

	// 如果程序已经存在并且正在运行
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// 关闭进程互斥体
		::CloseHandle(m_hMutex);
		// 打开指定窗口
		if (!sWindowTitle.isEmpty())
		{
			// 获取窗口句柄
			HWND hProgramWnd = ::FindWindow(L"Easy2DApp", sWindowTitle);
			if (hProgramWnd)
			{
				// 获取窗口显示状态
				WINDOWPLACEMENT wpm;
				::GetWindowPlacement(hProgramWnd, &wpm);
				// 将运行的程序窗口还原成正常状态
				wpm.showCmd = SW_SHOW;
				::SetWindowPlacement(hProgramWnd, &wpm);
				::SetWindowPos(hProgramWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
		}
		return false;
	}
	return true;
}

e2d::String e2d::Game::getName()
{
	return s_sGameName;
}
