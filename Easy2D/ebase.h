#pragma once
#include "emacros.h"
#include "ecommon.h"
#include <vector>

// Base Classes

namespace e2d
{

class EScene;
class ENode;
class EObjectManager;

class EApp
{
public:
	EApp();

	~EApp();

	// 获取程序实例
	static EApp * get();

	// Register the window class and call methods for instantiating drawing resources
	bool init(
		e2d::EString title,
		e2d::ESize size,
		bool bShowConsole = false
	);

	// Register the window class and call methods for instantiating drawing resources
	bool init(
		e2d::EString title,
		UINT32 width,
		UINT32 height,
		bool bShowConsole = false
	);

	// 启动程序
	void run();

	// 修改窗口大小
	void setWindowSize(
		int width,
		int height
	);

	// 修改窗口大小
	void setWindowSize(
		e2d::ESize size
	);

	// 设置窗口标题
	void setWindowTitle(
		e2d::EString title
	);

	// 获取窗口标题
	e2d::EString getTitle();

	// 获取窗口大小
	e2d::ESize getSize();

	// 获取窗口宽度
	UINT32 getWidth();

	// 获取窗口高度
	UINT32 getHeight();

	// 切换场景
	void enterScene(
		EScene * scene,
		bool save = true
	);

	// 返回上一场景
	void backScene();

	// 清空保存的所有场景
	void clearScene();

	// 获取当前场景
	EScene * getCurrentScene();

	// 获取正处于加载中的场景
	EScene * getLoadingScene();

	// 设置正处于加载中的场景
	void setLoadingScene(EScene * scene);

	// 获取 AppName
	e2d::EString getAppName();

	// 设置 AppName
	void setAppName(
		e2d::EString appname
	);

	// 修改窗口背景色
	void setBkColor(
		EColor::Enum color
	);

	// 释放所有内存资源
	void free();

	// 关闭窗口
	void close();

	// 显示窗口
	void show();

	// 终止程序
	void quit();

	// 终止程序
	void end();

protected:
	// Initialize device-independent resources.
	HRESULT _createDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT _createDeviceResources();

	// Release device-dependent resource.
	void _discardDeviceResources();

	void _mainLoop();

	void _onControl();

	// Draw content.
	bool _onRender();

	void _enterNextScene();

	// ReSize the render target.
	void _onResize(
		UINT width,
		UINT height
	);

	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

protected:
	bool	m_bRunning;
	bool	m_bSaveScene;
	EString	m_sTitle;
	EString	m_sAppName;
	EColor::Enum m_ClearColor;

	EScene *	m_pCurrentScene;
	EScene *	m_pNextScene;
	EScene *	m_pLoadingScene;
};


class EScene
{
public:

	EScene();

	~EScene();

	// 重写这个函数，初始化这个场景
	virtual void init();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter();

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit();

	// 添加子成员到场景
	void add(
		e2d::ENode * child, 
		int zOrder = 0
	);

	// 删除子成员
	bool del(
		e2d::ENode * child
	);

	// 获取所有子节点
	std::vector<e2d::ENode*> &getChildren();

	// 清空所有子成员
	void clearAllChildren();

protected:
	friend EApp;
	std::vector<e2d::ENode*> m_vChildren;

protected:
	void _exec();

	void _onDraw();
};


class EObject
{
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
	friend EObjectManager;
	int m_nRefCount;
	bool m_bManaged;
	bool m_bAutoRelease;
};

}