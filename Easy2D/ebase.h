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
class EMouseListener;
class EKeyboardListener;

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
	static void setWindowSize(
		int width,
		int height
	);

	// 修改窗口大小
	static void setWindowSize(
		e2d::ESize size
	);

	// 设置窗口标题
	static void setWindowTitle(
		e2d::EString title
	);

	// 获取窗口标题
	static e2d::EString getTitle();

	// 获取窗口大小
	static e2d::ESize getSize();

	// 获取窗口宽度
	static UINT32 getWidth();

	// 获取窗口高度
	static UINT32 getHeight();

	// 切换场景
	static void enterScene(
		EScene * scene,
		bool save = true
	);

	// 返回上一场景
	static void backScene();

	// 清空保存的所有场景
	static void clearScene();

	// 获取当前场景
	static EScene * getCurrentScene();

	// 获取 AppName
	static e2d::EString getAppName();

	// 设置 AppName
	static void setAppName(
		e2d::EString appname
	);

	// 修改窗口背景色
	static void setBkColor(
		EColor::Enum color
	);

	// 释放所有内存资源
	static void free();

	// 关闭窗口
	static void close();

	// 显示窗口
	static void show();

	// 终止程序
	static void quit();

	// 终止程序
	static void end();

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
	void _onRender();

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
	EString	m_sTitle;
	EString	m_sAppName;
	EColor::Enum m_ClearColor;

	EScene *	m_pCurrentScene;
	EScene *	m_pNextScene;
};


class EScene
{
	friend EApp;

public:
	EScene();

	~EScene();

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
	bool remove(
		e2d::ENode * child,
		bool autoRelease = true
	);

	// 获取所有子节点
	std::vector<e2d::ENode*> &getChildren();

	// 获取子节点数量
	size_t getChildrenCount() const;

	// 根据名称获取子节点
	ENode * getChild(
		EString childName
	) const;

	// 清空所有子成员
	void clearAllChildren();

	// 绑定鼠标消息监听器
	void bindListener(EMouseListener * listener);

	// 绑定按键消息监听器
	void bindListener(EKeyboardListener * listener);

protected:
	// 渲染场景画面
	void _onRender();

	// 子节点排序
	void _sortChildren();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	std::vector<e2d::ENode*> m_vChildren;
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