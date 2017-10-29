#pragma once
#include "emacros.h"
#include "ecommon.h"


// Base Classes

namespace e2d
{

class EScene;
class ENode;
class EObjectManager;
class EMouseListener;
class EKeyboardListener;
class EAction;
class ETransition;

class EApp
{
public:
	EApp();

	virtual ~EApp();

	// 初始化游戏界面
	bool init(
		const EString &title,	/* 窗口标题 */
		UINT32 width,			/* 窗口宽度 */
		UINT32 height,			/* 窗口高度 */
		bool showConsole = false/* 是否显示控制台 */
	);

	// 初始化游戏界面
	bool init(
		const EString &title,	/* 窗口标题 */
		UINT32 width,			/* 窗口宽度 */
		UINT32 height,			/* 窗口高度 */
		EWindowStyle wStyle,	/* 窗口样式 */
		bool showConsole = false/* 是否显示控制台 */
	);

	// 启动程序
	void run();

	// 重写这个函数，它将在窗口激活时执行
	virtual bool onActivate();

	// 重写这个函数，它将在窗口非激活时执行
	virtual bool onInactive();

	// 重写这个函数，它将在关闭窗口时执行
	virtual bool onCloseWindow();

	// 获取程序实例
	static EApp * get();

	// 暂停游戏
	static void pause();

	// 继续游戏
	static void resume();

	// 获取游戏是否暂停
	static bool isPaused();

	// 切换场景
	static void enterScene(
		EScene * scene,					/* 下一个场景的指针 */
		bool saveCurrentScene = true	/* 是否保存当前场景 */
	);

	// 切换场景
	static void enterScene(
		EScene * scene,					/* 下一个场景的指针 */
		ETransition * transition,		/* 场景切换动画 */
		bool saveCurrentScene = true	/* 是否保存当前场景 */
	);

	// 返回上一场景
	static void backScene();

	// 返回上一场景
	static void backScene(
		ETransition * transition		/* 场景切换动画 */
	);

	// 清空保存的所有场景
	static void clearScene();

	// 隐藏窗口
	static void hideWindow();

	// 显示窗口
	static void showWindow();

	// 是否打开控制台
	static void showConsole(
		bool show
	);

	// 终止程序
	static void quit();

	// 终止程序
	static void end();

	// 获取窗口标题
	static EString getTitle();

	// 获取窗口宽度
	static float getWidth();

	// 获取窗口高度
	static float getHeight();

	// 获取当前场景
	static EScene * getCurrentScene();

	// 获取窗口句柄
	static HWND getHWnd();

	// 获取从游戏开始到当前经过的毫秒数
	static LONGLONG getTotalDurationFromStart();

	// 获取 AppName
	static EString getAppName();

	// 修改窗口大小
	static void setWindowSize(
		UINT32 width,
		UINT32 height
	);

	// 设置窗口标题
	static void setWindowTitle(
		const EString & title
	);

	// 设置 AppName
	static void setAppName(
		const EString &appname
	);

	// 修改窗口背景色
	static void setBkColor(
		UINT32 color
	);

	// 设置程序是否响应输入法
	static void setKeyboardLayoutEnable(
		bool value
	);

	// 预设画面帧数
	static void setFPS(
		UINT32 fps
	);

protected:
	// 创建设备无关资源
	HRESULT _createDeviceIndependentResources();

	// 创建设备相关资源
	HRESULT _createDeviceResources();

	// 释放设备相关资源
	void _discardDeviceResources();

	// 游戏主循环
	void _mainLoop();

	// 游戏控制流程
	void _onControl();

	// 渲染游戏画面
	void _onRender();

	// 进入下一场景
	void _enterNextScene();

	// 重定 render target 大小
	void _onResize(
		UINT32 width,
		UINT32 height
	);

	// 窗口程序
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

protected:
	bool	m_bEnd;
	bool	m_bPaused;
	bool	m_bManualPaused;
	bool	m_bTransitional;
	bool	m_bTopMost;
	EString	m_sTitle;
	EString	m_sAppName;
	UINT32	m_ClearColor;
	LONGLONG nAnimationInterval;
	EScene * m_pCurrentScene;
	EScene * m_pNextScene;
};


class EObject
{
	friend EObjectManager;

public:
	EObject();

	virtual ~EObject();

	// 引用计数加一
	void retain();

	// 引用计数减一
	void release();

	// 让引擎自动释放这个对象
	void autoRelease();

private:
	int m_nRefCount;
	bool m_bManaged;
	bool m_bAutoRelease;
};


class EScene :
	public EObject
{
	friend EApp;

public:
	EScene();

	virtual ~EScene();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter();

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit();

	// 重写这个函数，它将在窗口激活时执行
	virtual bool onActivate();

	// 重写这个函数，它将在窗口非激活时执行
	virtual bool onInactive();

	// 重写这个函数，它将在关闭窗口时执行
	virtual bool onCloseWindow();

	// 添加子节点到场景
	void add(
		ENode * child, 
		int zOrder = 0
	);

	// 删除子节点
	bool remove(
		ENode * child,
		bool release = false
	);

	// 根据名称删除子节点
	void remove(
		const EString &childName,
		bool release = false
	);

	// 获取所有子节点
	EVector<e2d::ENode*> &getChildren();

	// 获取子节点数量
	size_t getChildrenCount() const;

	// 根据名称获取子节点
	ENode * getChild(
		const EString &childName
	);

	// 获取根节点
	ENode * getRoot() const;

	// 清空所有子成员
	void clearAllChildren();

	// 执行动画
	void runAction(
		EAction * action
	);

	// 开启几何图形的渲染
	void setGeometryVisiable(
		bool visiable
	);

protected:
	// 渲染场景画面
	void _onRender();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	bool m_bGeometryVisiable;
	ENode * m_pRoot;
};

}