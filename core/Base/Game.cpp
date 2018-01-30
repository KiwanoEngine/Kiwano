#include "..\ebase.h"
#include "..\emanagers.h"
#include "..\enodes.h"
#include "..\etransitions.h"
#include "..\etools.h"


// 控制游戏终止
static bool s_bEndGame = false;
// 控制游戏暂停
static bool s_bPaused = false;
// 是否进行过初始化
static bool s_bInitialized = false;
// AppName
static e2d::EString s_sAppName;


bool e2d::EGame::init(LPCTSTR sTitle, UINT32 nWidth, UINT32 nHeight, LPCTSTR pIconID, bool bNoClose, bool bNoMiniSize, bool bTopMost)
{
	if (s_bInitialized)
	{
		WARN_IF(true, "The game has been initialized!");
		return false;
	}

	// 初始化 COM 组件
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		// 创建设备无关资源
		if (ERenderer::__createDeviceIndependentResources())
		{
			// 初始化窗口
			if (EWindow::__init(sTitle, nWidth, nHeight, pIconID, bNoClose, bNoMiniSize, bTopMost))
			{
				// 创建设备相关资源
				if (ERenderer::__createDeviceResources())
				{
					// 重设 Client 大小
					EWindow::setSize(nWidth, nHeight);
					// 设置默认 AppName
					if (s_sAppName.isEmpty())
						s_sAppName = EWindow::getTitle();
					// 标志初始化成功
					s_bInitialized = true;
				}
			}
		}
	}
	
	return s_bInitialized;
}

int e2d::EGame::run()
{
	if (!s_bInitialized)
	{
		ASSERT(false, "You must initialize EGame first!");
		return -1;
	}

	// 初始化 DirectInput
	EInput::__init();
	// 初始化计时操作
	ETime::__init();
	// 进入第一个场景
	ESceneManager::__enterNextScene();
	// 显示窗口
	EWindow::showWindow();
	// 刷新窗口内容
	::UpdateWindow(EWindow::getHWnd());
	// 处理窗口消息
	EWindow::__poll();
	// 刷新时间信息
	ETime::__updateNow();
	ETime::__updateLast();

	// 挂起时长
	int nWaitMS = 0;

	while (!s_bEndGame)
	{
		// 处理窗口消息
		EWindow::__poll();
		// 刷新时间信息
		ETime::__updateNow();

		// 判断是否达到了刷新状态
		if (ETime::getDeltaTime() >= 17)
		{
			ETime::__updateLast();
			EGame::__update();
		}
		else
		{
			// 挂起线程
			ETime::__sleep();
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
	// 重置窗口属性
	EWindow::__uninit();
	// 关闭输入
	EInput::__uninit();
	// 恢复计时操作
	ETime::__uninit();
	// 删除渲染相关资源
	ERenderer::__discardResources();

	CoUninitialize();
}

void e2d::EGame::__update()
{
	EInput::__updateDeviceState();		// 获取用户输入

	if (!s_bPaused)
	{
		ETimerManager::__update();		// 定时器管理器执行程序
		EActionManager::__update();		// 动作管理器执行程序
		ESceneManager::__update();		// 更新游戏内容
		ERenderer::__render();			// 渲染游戏画面
	}
}

void e2d::EGame::setAppName(const EString &appname)
{
	s_sAppName = appname;
}

e2d::EString e2d::EGame::getAppName()
{
	return s_sAppName;
}
