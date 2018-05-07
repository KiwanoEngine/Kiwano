#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"


// Base Classes

namespace e2d
{


class Game
{
public:
	// 初始化游戏
	static bool init(
		const String& name = L""	/* 游戏英文名称 */
	);

	// 启动游戏
	static int start(
		bool autoRelease = true		/* 游戏结束时自动回收资源 */
	);

	// 暂停游戏
	static void pause();

	// 继续游戏
	static void resume();

	// 结束游戏
	static void quit();

	// 回收游戏资源
	static void destroy();

	// 游戏是否暂停
	static bool isPaused();

	// 创建进程互斥体
	static bool createMutex(
		const String& sMutexName,			/* 互斥体名称 */
		const String& sWindowTitle = L""	/* 窗口标题 */
	);

	// 获取游戏名称
	static String getName();
};


// 控制窗口属性
class Window
{
	friend Game;

public:
	// 修改窗口大小
	static void setSize(
		int width,			/* 窗口宽度 */
		int height			/* 窗口高度 */
	);

	// 设置窗口标题
	static void setTitle(
		const String& title	/* 窗口标题 */
	);

	// 设置窗口图标
	static void setIcon(
		int iconID
	);

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

	// 打开/隐藏控制台
	static void showConsole(
		bool show
	);

	// 是否允许响应输入法
	static void setTypewritingEnable(
		bool enable
	);

private:
	// 初始化窗口
	static bool __init();

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
	// 获取上一帧与当前帧的时间间隔（秒）
	static double getDeltaTime();

	// 获取上一帧与当前帧的时间间隔（毫秒）
	static unsigned int getDeltaTimeMilliseconds();

	// 获取游戏总时长（秒）
	static double getTotalTime();

	// 获取游戏总时长（毫秒）
	static unsigned int getTotalTimeMilliseconds();

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

public:
	// 检测键盘某按键是否正被按下
	static bool isKeyDown(
		KeyCode key
	);

	// 检测键盘某按键是否被点击
	static bool isKeyPress(
		KeyCode key
	);

	// 检测键盘某按键是否正在松开
	static bool isKeyRelease(
		KeyCode key
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

	// 卸载 DirectInput
	static void __uninit();
};


// 渲染器
class Renderer
{
	friend Game;
	friend Window;

public:
	// 获取背景色
	static Color getBackgroundColor();

	// 修改背景色
	static void setBackgroundColor(
		Color color
	);

	// 显示 FPS
	static void showFps(
		bool show = true
	);

	// 获取系统 DPI 缩放
	static float getDpiScaleX();

	// 获取系统 DPI 缩放
	static float getDpiScaleY();

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

	// 获取自定义的文字渲染器
	static CustomTextRenderer * getCustomTextRenderer();

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