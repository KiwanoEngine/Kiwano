#include "..\ebase.h"
#include "..\emanagers.h"


// 控制游戏终止
static bool s_bEndGame = false;
// 控制游戏暂停
static bool s_bPaused = false;
// 是否进行过初始化
static bool s_bInitialized = false;
// AppName
static e2d::EString s_sAppName;


bool e2d::EGame::init(LPCTSTR sTitle, UINT32 nWidth, UINT32 nHeight, LPCTSTR pIconID, LPCTSTR sAppname)
{
	if (s_bInitialized)
	{
		WARN_IF(true, "The game has been initialized!");
		return false;
	}

	do
	{
		// 初始化 COM 组件
		if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		{
			WARN_IF(true, "CoInitializeEx Failed!");
			break;
		}

		// 创建设备无关资源
		if (!ERenderer::__createDeviceIndependentResources())
		{
			WARN_IF(true, "ERenderer::__createDeviceIndependentResources Failed!");
			break;
		}

		// 初始化窗口
		if (!EWindow::__init(sTitle, nWidth, nHeight, pIconID))
		{
			WARN_IF(true, "EWindow::__init Failed!");
			break;
		}

		// 创建设备相关资源
		if (!ERenderer::__createDeviceResources())
		{
			WARN_IF(true, "ERenderer::__createDeviceResources Failed!");
			break;
		}

		// 初始化计时
		if (!ETime::__init())
		{
			WARN_IF(true, "ETime::__init Failed!");
			break;
		}

		// 初始化 DirectInput
		if (!EInput::__init())
		{
			WARN_IF(true, "EInput::__init Failed!");
			break;
		}

		// 初始化播放器
		if (!EMusicManager::__init())
		{
			WARN_IF(true, "EMusicManager::__init Failed!");
			break;
		}

		// 重设 Client 大小
		EWindow::setSize(nWidth, nHeight);
		// 设置 AppName
		s_sAppName = (sAppname != nullptr) ? sAppname : EWindow::getTitle();
		// 标志初始化成功
		s_bInitialized = true;

	} while (0);

	return s_bInitialized;
}

int e2d::EGame::run()
{
	if (!s_bInitialized)
	{
		ASSERT(false, "You must initialize EGame first!");
		return -1;
	}

	// 初始化场景管理器
	ESceneManager::__init();
	// 显示窗口
	::ShowWindow(EWindow::getHWnd(), SW_SHOWNORMAL);
	// 刷新窗口内容
	::UpdateWindow(EWindow::getHWnd());
	// 处理窗口消息
	EWindow::__poll();
	// 刷新时间信息
	ETime::__updateNow();
	ETime::__updateLast();

	while (!s_bEndGame)
	{
		// 处理窗口消息
		EWindow::__poll();
		// 刷新时间信息
		ETime::__updateNow();

		// 判断是否达到了刷新状态
		if (ETime::getDeltaTime() >= 17)
		{
			ETime::__updateLast();		// 刷新时间信息
			EGame::__update();			// 更新游戏内容
			ERenderer::__render();		// 渲染游戏画面
		}
		else
		{
			EObjectManager::__flush();	// 刷新内存池
			ETime::__sleep();			// 挂起线程
		}
	}

	return 0;
}

void e2d::EGame::pause()
{
	s_bPaused = true;
}

void e2d::EGame::resume()
{
	if (isPaused())
	{
		s_bPaused = false;
		// 刷新当前时间
		ETime::__updateLast();
		// 重置动画和定时器
		EActionManager::__resetAllActions();
		ETimerManager::__resetAllTimers();
	}
}

bool e2d::EGame::isPaused()
{
	return s_bPaused;
}

void e2d::EGame::quit()
{
	s_bEndGame = true;	// 这个变量将控制游戏是否结束
}

void e2d::EGame::uninit()
{
	// 删除所有场景
	ESceneManager::__uninit();
	// 重置窗口属性
	EWindow::__uninit();
	// 关闭输入
	EInput::__uninit();
	// 关闭播放器
	EMusicManager::__uninit();
	// 恢复计时操作
	ETime::__uninit();
	// 清空图片缓存
	EImage::clearCache();
	// 删除渲染相关资源
	ERenderer::__discardResources();
	// 刷新内存池
	EObjectManager::__flush();

	CoUninitialize();

	s_bInitialized = false;
}

void e2d::EGame::__update()
{
	if (s_bPaused)
		return;

	EInput::__updateDeviceState();	// 获取用户输入
	ETimerManager::__update();		// 定时器管理器执行程序
	EActionManager::__update();		// 动作管理器执行程序
	ESceneManager::__update();		// 更新游戏内容
}

e2d::EString e2d::EGame::getAppName()
{
	return s_sAppName;
}
