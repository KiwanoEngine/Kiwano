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
		int windowStyle,		/* 窗口样式 */
		bool showConsole = false/* 是否显示控制台 */
	);

	// 启动程序
	void run();

	// 预设画面帧数
	void setFPS(
		UINT32 fps
	);

	// 重写这个函数，它将在窗口激活时执行
	virtual void onActivate();

	// 重写这个函数，它将在窗口非激活时执行
	virtual bool onInactive();

	// 重写这个函数，它将在关闭窗口时执行
	virtual bool onCloseWindow();

	// 释放所有内存资源
	void free();

	// 获取程序实例
	static EApp * get();

	// 显示或隐藏控制台（默认隐藏）
	static void showConsole(
		bool show
	);

	// 修改窗口大小
	static void setWindowSize(
		UINT32 width,
		UINT32 height
	);

	// 设置窗口标题
	static void setWindowTitle(
		const EString &title
	);

	// 获取窗口标题
	static EString getTitle();

	// 获取窗口宽度
	static UINT32 getWidth();

	// 获取窗口高度
	static UINT32 getHeight();

	// 切换场景
	static void enterScene(
		EScene * scene,
		bool saveCurrentScene = true
	);

	// 返回上一场景
	static void backScene();

	// 清空保存的所有场景
	static void clearScene();

	// 获取当前场景
	static EScene * getCurrentScene();

	// 获取窗口句柄
	static HWND getHWnd();

	// 获取从游戏开始到当前经过的毫秒数
	static LONGLONG getTotalDurationFromStart();

	// 获取 AppName
	static EString getAppName();

	// 设置 AppName
	static void setAppName(
		const EString &appname
	);

	// 修改窗口背景色
	static void setBkColor(
		EColor color
	);

	// 设置程序是否响应输入法
	static void setKeyboardLayoutEnable(
		bool value
	);

	// 隐藏窗口
	static void hideWindow();

	// 显示窗口
	static void showWindow();

	// 终止程序
	static void quit();

	// 终止程序
	static void end();

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
	bool _onRender();

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
	bool	m_bRunning;
	bool	m_bPaused;
	EString	m_sTitle;
	EString	m_sAppName;
	EColor	m_ClearColor;
	LONGLONG nAnimationInterval;

	EScene * m_pCurrentScene;
	EScene * m_pNextScene;
};


class EScene
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
	virtual void onActivate();

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

	// 清空所有子成员
	void clearAllChildren();

	// 绑定鼠标消息监听器
	void bindListener(EMouseListener * listener);

	// 绑定按键消息监听器
	void bindListener(EKeyboardListener * listener);

protected:
	// 渲染场景画面
	void _onRender();

	// 进入场景时需调用该函数
	virtual void _onEnter();

	// 退出场景时需调用该函数
	virtual void _onExit();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	ENode * const m_Root;
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

}