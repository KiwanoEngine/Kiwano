/******************************************************
* Easy2D Game Engine
* 
* Website: http://www.easy2d.cn
* Source Code: https://gitee.com/werelone/Easy2D
******************************************************/

#pragma once

#ifndef __cplusplus
	#error Easy2D is only for C++
#endif

#if _MSC_VER < 1900
	#error Do Visual Studio 2015/2017 specific stuff
#endif


// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              // Allow use of features specific to Windows 7 or later.
#define WINVER 0x0700       // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0700 // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used items from Windows headers.
#define WIN32_LEAN_AND_MEAN

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <wchar.h>

#include "emacros.h"
#include "ecommon.h"


#if defined(UNICODE) && (_DEBUG)
	#pragma comment(lib, "Easy2Ddw.lib")
#elif !defined(UNICODE) && (_DEBUG)
	#pragma comment(lib, "Easy2Dd.lib")
#elif defined(UNICODE)
	#pragma comment(lib, "Easy2Dw.lib")
#elif !defined(UNICODE)
	#pragma comment(lib, "Easy2D.lib")
#endif


// Classes Declare

namespace e2d
{
	class EApp;
	class Scene;
	class Object;
}


// Classes

namespace e2d
{

class EApp
{
public:
	EApp();
	~EApp();

	// 获取程序实例
	static EApp * get();

	// Register the window class and call methods for instantiating drawing resources
	bool init(
		EString title,
		ESize size,
		bool bShowConsole = false
	);

	// Register the window class and call methods for instantiating drawing resources
	bool init(
		EString title,
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
		ESize size
	);

	// 设置窗口标题
	void setWindowTitle(
		EString title
	);

	// 获取窗口标题
	EString getTitle();

	// 获取窗口宽度
	int getWidth();

	// 获取窗口高度
	int getHeight();
	
	// 切换场景
	void enterScene(
		Scene * scene, 
		bool save = true
	);

	// 返回上一场景
	void backScene();

	// 清空保存的所有场景
	void clearScene();

	// 获取当前场景
	Scene * getCurrentScene();

	// 获取正处于加载中的场景
	Scene * getLoadingScene();

	// 设置 AppName
	void setAppName(
		EString appname
	);

	// 获取 AppName
	EString getAppName();

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
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	void _mainLoop();

	void _onControl();

	// Draw content.
	bool _onRender();

	void _enterNextScene();

	// Resize the render target.
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
	EColor::Enum	m_ClearColor;
	
	Scene *	m_pCurrentScene;
	Scene *	m_pNextScene;
	Scene *	m_pLoadingScene;
};

class Scene
{
	friend EApp;
public:
	Scene();
	~Scene();

	// 重写这个函数，初始化这个场景
	virtual void init();
	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter();
	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit();
	// 添加子成员到场景
	void add(Node * child, int zOrder = 0);
	// 删除子成员
	bool del(Node * child);
	// 清空所有子成员
	void clearAllChildren();

protected:
	//std::vector<Node*> m_vChildren;

protected:
	void _exec();
	void _onDraw();
};

class Object
{
	friend FreePool;
public:
	Object();
	virtual ~Object();

	// 保留这个对象
	void retain();
	// 释放这个对象
	void release();
	// 让引擎自动释放这个对象
	void autoRelease();

protected:
	int m_nRefCount;
	bool m_bAutoRelease;
};

}	// End of easy2d namespace


// Functions Declare

using namespace e2d;