#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include "..\e2dcollider.h"


e2d::Game * e2d::Game::_instance = nullptr;

e2d::Game::Game()
	: _ended(false)
	, _paused(false)
	, _initialized(false)
{
}

e2d::Game::~Game()
{
}

e2d::Game * e2d::Game::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) Game;
	return _instance;
}

void e2d::Game::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

bool e2d::Game::init(const String& mutexName)
{
	if (_initialized)
	{
		WARN("The game has been initialized!");
		return false;
	}

	if (!mutexName.isEmpty())
	{
		// 创建进程互斥体
		String fullMutexName = L"Easy2DApp-" + mutexName;
		HANDLE hMutex = ::CreateMutex(nullptr, TRUE, (LPCWSTR)fullMutexName);

		if (hMutex == nullptr)
		{
			WARN("CreateMutex Failed!");
		}
		else if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			// 如果程序已经存在并且正在运行，弹窗提示
			Window::getInstance()->info(L"游戏已在其他窗口中打开！", L"提示");
			// 关闭进程互斥体
			::CloseHandle(hMutex);
			return false;
		}
	}

	// 初始化 COM 组件
	CoInitialize(nullptr);

	bool bInputInit = false,
		bMusicInit = false;

	auto DestroyResources = [&]()
	{
		if (bInputInit) Input::__uninit();
		if (bMusicInit) Music::__uninit();
	};

	// 初始化 DirectInput
	if (Input::__init())
	{
		bInputInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"初始化 DirectInput 组件失败");
	}

	// 初始化播放器
	if (Music::__init())
	{
		bMusicInit = true;
	}
	else
	{
		DestroyResources();
		throw SystemException(L"初始化 XAudio2 组件失败");
	}

	// 初始化路径
	if (!Path::__init())
	{
		DestroyResources();
		throw SystemException(L"必要系统路径访问失败");
	}

	// 初始化成功
	_initialized = true;

	return _initialized;
}

void e2d::Game::start(bool cleanup)
{
	if (!_initialized)
	{
		throw Exception(L"开始游戏前未进行初始化");
	}

	auto window = Window::getInstance();
	auto renderer = Renderer::getInstance();

	// 初始化场景管理器
	SceneManager::__init();
	// 显示窗口
	::ShowWindow(window->getHWnd(), SW_SHOWNORMAL);
	// 刷新窗口内容
	::UpdateWindow(window->getHWnd());
	// 处理窗口消息
	window->__poll();
	// 初始化计时
	Time::__init();

	_ended = false;

	while (!_ended)
	{
		// 处理窗口消息
		window->__poll();
		// 刷新时间
		Time::__updateNow();

		// 判断是否达到了刷新状态
		if (Time::__isReady())
		{
			Input::__update();			// 获取用户输入
			Timer::__update();			// 更新定时器
			ActionManager::__update();	// 更新动作管理器
			SceneManager::__update();	// 更新场景内容
			renderer->__render();		// 渲染游戏画面

			Time::__updateLast();		// 刷新时间信息
		}
		else
		{
			Time::__sleep();				// 挂起线程
			GC::getInstance()->__update();	// 刷新内存池
		}
	}

	_ended = true;

	if (cleanup)
	{
		Game::cleanup();
	}
}

void e2d::Game::pause()
{
	_paused = true;
}

void e2d::Game::resume()
{
	if (_initialized && _paused)
	{
		Game::reset();
	}
	_paused = false;
}

void e2d::Game::reset()
{
	if (_initialized && !_ended)
	{
		Time::__reset();
		ActionManager::__resetAll();
		Timer::__resetAll();
	}
}

bool e2d::Game::isPaused()
{
	return _paused;
}

void e2d::Game::quit()
{
	_ended = true;	// 这个变量将控制游戏是否结束
}

void e2d::Game::cleanup()
{
	if (!_initialized)
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
	// 清空图片缓存
	Image::clearCache();
	// 回收音乐相关资源
	Music::__uninit();
	// 清空定时器
	Timer::__uninit();
	// 关闭输入
	Input::__uninit();
	// 删除所有对象
	GC::getInstance()->clear();

	CoUninitialize();

	_initialized = false;
}
