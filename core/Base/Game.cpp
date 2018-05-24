#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include "..\e2dcollider.h"


// 控制游戏终止
static bool s_bEndGame = true;
// 控制游戏暂停
static bool s_bPaused = false;
// 是否进行过初始化
static bool s_bInitialized = false;
// 游戏名称
static e2d::String s_sGameName;


bool e2d::Game::init(const String& name, const String& mutexName)
{
	if (s_bInitialized)
	{
		WARN("The game has been initialized!");
		return false;
	}

	if (!mutexName.isEmpty())
	{
		// 创建进程互斥体
		HANDLE hMutex = ::CreateMutex(nullptr, TRUE, L"Easy2DApp-" + mutexName);

		if (hMutex == nullptr)
		{
			WARN("CreateMutex Failed!");
		}
		else if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// 如果程序已经存在并且正在运行，弹窗提示
			Window::info(L"游戏已在其他窗口中打开！", L"提示");
			// 关闭进程互斥体
			::CloseHandle(hMutex);
			return false;
		}
	}

	// 初始化 COM 组件
	CoInitialize(nullptr);

	bool bRendererDevIndResInit = false,
		bWindowInit = false,
		bRendererDevResInit = false,
		bInputInit = false,
		bMusicInit = false;

	auto DestroyResources = [&]()
	{
		if (bRendererDevIndResInit) Renderer::__discardResources();
		if (bWindowInit) Window::__init();
		if (bRendererDevResInit) Renderer::__discardDeviceResources();
		if (bInputInit) Input::__uninit();
		if (bMusicInit) Music::__uninit();
	};

	// 创建设备无关资源
	if (Renderer::__createDeviceIndependentResources())
	{
		bRendererDevIndResInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"渲染器设备无关资源创建失败");
	}

	// 初始化窗口
	if (Window::__init())
	{
		bWindowInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"初始化窗口失败");
	}

	// 创建设备相关资源
	if (Renderer::__createDeviceResources())
	{
		bRendererDevResInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"渲染器设备相关资源创建失败");
	}

	// 初始化 DirectInput
	if (Input::__init())
	{
		bInputInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"初始化 DirectInput 失败");
	}

	// 初始化播放器
	if (Music::__init())
	{
		bMusicInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"初始化 XAudio2 失败");
	}

	// 保存游戏名称
	s_sGameName = name;

	// 初始化路径
	if (!Path::__init())
	{
		WARN("Path::__init failed!");
	}

	// 初始化成功
	s_bInitialized = true;

	return s_bInitialized;
}

void e2d::Game::start(bool autoRelease/* true */)
{
	if (!s_bInitialized)
	{
		throw Exception(L"开始游戏前未进行初始化");
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

	s_bEndGame = false;

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
			GC::__update();		// 刷新内存池
		}
		else
		{
			Time::__sleep();				// 挂起线程
		}
	}

	s_bEndGame = true;

	if (autoRelease)
	{
		Game::destroy();
	}
}

void e2d::Game::pause()
{
	s_bPaused = true;
}

void e2d::Game::resume()
{
	if (s_bInitialized && s_bPaused)
	{
		Game::reset();
	}
	s_bPaused = false;
}

void e2d::Game::reset()
{
	if (s_bInitialized && !s_bEndGame)
	{
		Time::__reset();
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
	if (!s_bInitialized)
		return;

	// 删除所有场景
	SceneManager::__uninit();
	// 删除输入监听器
	Input::__clearListeners();
	// 删除碰撞监听器
	Collision::__clearListeners();
	// 删除动作
	ActionManager::__uninit();
	// 回收音乐播放器资源
	Player::__uninit();
	// 删除所有对象
	GC::__clear();
	// 清空图片缓存
	Image::clearCache();
	// 回收音乐相关资源
	Music::__uninit();
	// 清空定时器
	Timer::__uninit();
	// 关闭输入
	Input::__uninit();
	// 回收渲染相关资源
	Renderer::__discardResources();
	// 销毁窗口
	Window::__uninit();

	CoUninitialize();

	s_bInitialized = false;
}

e2d::String e2d::Game::getName()
{
	return s_sGameName;
}
