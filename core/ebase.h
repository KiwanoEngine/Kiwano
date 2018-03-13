#pragma once
#include "emacros.h"
#include "ecommon.h"


// Base Classes

namespace e2d
{


class Game
{
public:
	// 初始化游戏
	static bool init(
		const String & sTitle,			/* 窗口标题 */
		UINT32 nWidth,					/* 窗口宽度 */
		UINT32 nHeight,					/* 窗口高度 */
		LPCTSTR pIconID = nullptr,		/* 窗口图标 */
		const String & sAppname = L""	/* AppName */
	);

	// 启动游戏
	static int run();

	// 暂停游戏
	static void pause();

	// 继续游戏
	static void resume();

	// 结束游戏
	static void quit();

	// 回收游戏资源
	static void uninit();

	// 游戏是否暂停
	static bool isPaused();

	// 获取 AppName
	static String getAppName();
};


// 控制窗口属性
class Window
{
	friend Game;

public:
	// 获取窗口标题
	static String getTitle();

	// 获取窗口宽度
	static double getWidth();

	// 获取窗口高度
	static double getHeight();

	// 获取窗口大小
	static Size getSize();

	// 获取窗口句柄
	static HWND getHWnd();

	// 修改窗口大小
	static void setSize(
		UINT32 nWidth,
		UINT32 nHeight
	);

	// 设置窗口标题
	static void setTitle(
		const String & sTitle
	);

	// 打开/隐藏控制台
	static void showConsole(
		bool bShow = true
	);

	// 是否允许响应输入法
	static void setTypewritingEnable(
		bool bEnable
	);

private:
	// 初始化窗口
	static bool __init(
		const String & sTitle,
		UINT32 nWidth,
		UINT32 nHeight,
		LPCTSTR pIconID
	);

	// 创建进程互斥体
	static bool __initMutex(
		const String & sTitle
	);

	// 重置窗口属性
	static void __uninit();

	// 处理窗口消息
	static void __poll();

	// Win32 窗口消息回调程序
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


// 控制游戏时间
class Time
{
	friend Game;

public:
	// 获取上一帧与当前帧的时间间隔（毫秒）
	static int getDeltaTime();

	// 获取游戏开始时长（秒）
	static double getTotalTime();

private:
	// 初始化计时操作
	static bool __init();

	// 重置计时操作
	static void __uninit();

	// 是否达到更新时间
	static bool __isReady();

	// 更新当前时间
	static void __updateNow();

	// 更新时间信息
	static void __updateLast();

	// 挂起线程
	static void __sleep();
};


// 控制键盘和鼠标的输入
class Input
{
	friend Game;
	friend Listener;

public:
	// 添加输入监听
	static void add(
		ListenerCallback callback,	/* 回调函数 */
		const String & name = L""	/* 监听器名称 */
	);

	// 启动输入监听
	static void start(
		const String & name
	);

	// 停止输入监听
	static void stop(
		const String & name
	);

	// 清除输入监听
	static void clear(
		const String & name
	);

	// 启动所有监听器
	static void startAll();

	// 停止所有监听器
	static void stopAll();

	// 清除所有监听器
	static void clearAll();

	// 获取监听器
	static std::vector<Listener*> get(
		const String & name
	);

	// 获取全部监听器
	static std::vector<Listener*> getAll();

	// 检测键盘某按键是否正被按下
	static bool isKeyDown(
		int nKeyCode
	);

	// 检测键盘某按键是否被点击
	static bool isKeyPress(
		int nKeyCode
	);

	// 检测键盘某按键是否正在松开
	static bool isKeyRelease(
		int nKeyCode
	);

	// 检测鼠标左键是否正被按下
	static bool isMouseLButtonDown();

	// 检测鼠标右键是否正被按下
	static bool isMouseRButtonDown();

	// 检测鼠标中键是否正被按下
	static bool isMouseMButtonDown();

	// 检测鼠标左键是否被点击
	static bool isMouseLButtonPress();

	// 检测鼠标右键是否被点击
	static bool isMouseRButtonPress();

	// 检测鼠标中键是否被点击
	static bool isMouseMButtonPress();

	// 检测鼠标左键是否正在松开
	static bool isMouseLButtonRelease();

	// 检测鼠标右键是否正在松开
	static bool isMouseRButtonRelease();

	// 检测鼠标中键是否正在松开
	static bool isMouseMButtonRelease();

	// 获得鼠标X轴坐标值
	static double getMouseX();

	// 获得鼠标Y轴坐标值
	static double getMouseY();

	// 获得鼠标坐标值
	static Point getMousePos();

	// 获得鼠标X轴坐标增量
	static double getMouseDeltaX();

	// 获得鼠标Y轴坐标增量
	static double getMouseDeltaY();

	// 获得鼠标Z轴（鼠标滚轮）坐标增量
	static double getMouseDeltaZ();

private:
	// 初始化 DirectInput 以及键盘鼠标设备
	static bool __init();

	// 刷新输入信息
	static void __update();

	// 刷新设备状态
	static void __updateDeviceState();

	// 添加输入监听
	static void __add(
		Listener * pListener
	);

	// 卸载 DirectInput
	static void __uninit();
};


// 渲染器
class Renderer
{
	friend Game;
	friend Window;

public:
	// 修改背景色
	static void setBackgroundColor(
		UINT32 color
	);

	// 获取 ID2D1Factory 对象
	static ID2D1Factory * getID2D1Factory();

	// 获取 ID2D1HwndRenderTarget 对象
	static ID2D1HwndRenderTarget * getRenderTarget();

	// 获取 ID2D1SolidColorBrush 对象
	static ID2D1SolidColorBrush * getSolidColorBrush();

	// 获取 IWICImagingFactory 对象
	static IWICImagingFactory * getIWICImagingFactory();

	// 获取 IDWriteFactory 对象
	static IDWriteFactory * getIDWriteFactory();

private:
	// 渲染游戏画面
	static void __render();

	// 创建设备无关资源
	static bool __createDeviceIndependentResources();

	// 创建设备相关资源
	static bool __createDeviceResources();

	// 删除设备相关资源
	static void __discardDeviceResources();

	// 删除所有渲染相关资源
	static void __discardResources();
};

}