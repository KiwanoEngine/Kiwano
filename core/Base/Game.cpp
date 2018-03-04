#include "..\ebase.h"
#include "..\emanagers.h"


// 控制游戏终止
static bool s_bEndGame = false;
// 控制游戏暂停
static bool s_bPaused = false;
// 是否进行过初始化
static bool s_bInitialized = false;
// AppName
static e2d::String s_sAppName;


bool e2d::Game::init(const String & sTitle, UINT32 nWidth, UINT32 nHeight, LPCTSTR pIconID, const String & sAppname)
{
	if (s_bInitialized)
	{
		WARN_IF(true, "The game has been initialized!");
		return false;
	}

	do
	{
		// 初始化 COM 组件
		CoInitializeEx(NULL, COINIT_MULTITHREADED);

		// 设置 AppName
		s_sAppName = sAppname;

		// 创建设备无关资源
		if (!Renderer::__createDeviceIndependentResources())
		{
			WARN_IF(true, "Renderer::__createDeviceIndependentResources Failed!");
			break;
		}

		// 初始化窗口
		if (!Window::__init(sTitle, nWidth, nHeight, pIconID))
		{
			WARN_IF(true, "Window::__init Failed!");
			break;
		}

		// 创建设备相关资源
		if (!Renderer::__createDeviceResources())
		{
			WARN_IF(true, "Renderer::__createDeviceResources Failed!");
			break;
		}

		// 初始化 DirectInput
		if (!Input::__init())
		{
			WARN_IF(true, "Input::__init Failed!");
			break;
		}

		// 初始化播放器
		if (!MusicManager::__init())
		{
			WARN_IF(true, "MusicManager::__init Failed!");
			break;
		}

		// 重设 Client 大小
		Window::setSize(nWidth, nHeight);
		// 标志初始化成功
		s_bInitialized = true;

	} while (0);

	return s_bInitialized;
}

int e2d::Game::run()
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
			TimerManager::__update();	// 定时器管理器执行程序
			ActionManager::__update();	// 动作管理器执行程序
			while (Time::__isReady())
			{
				Input::__updateDeviceState();	// 获取用户输入
				SceneManager::__update();		// 更新场景内容
				Time::__updateLast();			// 刷新时间信息
			}
			Renderer::__render();		// 渲染游戏画面
		}
		else
		{
			ObjectManager::__update();	// 刷新内存池
			Time::__sleep();			// 挂起线程
		}
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
		ActionManager::__resetAllActions();
		TimerManager::__resetAllTimers();
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

void e2d::Game::uninit()
{
	// 删除所有场景
	SceneManager::__uninit();
	// 关闭输入
	Input::__uninit();
	// 关闭播放器
	MusicManager::__uninit();
	// 清空定时器
	TimerManager::__uninit();
	// 恢复计时操作
	Time::__uninit();
	// 清空图片缓存
	Image::clearCache();
	// 刷新内存池
	ObjectManager::__clearAllObjects();
	// 删除渲染相关资源
	Renderer::__discardResources();
	// 销毁窗口
	Window::__uninit();

	CoUninitialize();

	s_bInitialized = false;
}

e2d::String e2d::Game::getAppName()
{
	return s_sAppName;
}
