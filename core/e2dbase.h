#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"
#include "e2dcustom.h"


// Base Classes

namespace e2d
{


// 游戏控制
class Game
{
public:
	// 获取游戏实例
	static Game * getInstance();

	// 销毁实例
	static void destroyInstance();

	// 启动游戏
	void start(
		bool cleanup = true		/* 自动清理资源 */
	);

	// 暂停游戏
	void pause();

	// 继续游戏
	void resume();

	// 结束游戏
	void quit();

	// 清理资源
	void cleanup();

	// 重置游戏内部计时
	void reset();

	// 游戏是否暂停
	bool isPaused();

private:
	Game();

	~Game();

	E2D_DISABLE_COPY(Game);

private:
	bool	_ended;
	bool	_paused;
	bool	_initialized;
	static Game * _instance;
};


// 窗口控制
class Window
{
	friend class Game;

public:
	// 鼠标指针样式
	enum class Cursor : int
	{
		Normal,		/* 默认指针样式 */
		Hand,		/* 手状指针 */
		No,			/* 禁止指针 */
		Wait,		/* 沙漏指针 */
		ArrowWait	/* 默认指针和小沙漏 */
	};

public:
	// 获取窗口实例
	static Window * getInstance();

	// 销毁窗口实例
	static void destroyInstance();

	// 创建窗口互斥体
	bool createMutex(
		const String& mutex = L""	/* 进程互斥体名称 */
	);

	// 修改窗口大小
	void setSize(
		int width,			/* 窗口宽度 */
		int height			/* 窗口高度 */
	);

	// 设置窗口标题
	void setTitle(
		const String& title	/* 窗口标题 */
	);

	// 设置窗口图标
	void setIcon(
		int iconID
	);

	// 设置鼠标指针样式
	void setCursor(
		Cursor cursor
	);

	// 获取窗口标题
	String getTitle();

	// 获取窗口宽度
	double getWidth();

	// 获取窗口高度
	double getHeight();

	// 获取窗口大小
	Size getSize();

	// 获取窗口句柄
	HWND getHWnd();

	// 打开/隐藏控制台
	void showConsole(
		bool show = true
	);

	// 是否允许响应输入法
	void setTypewritingEnable(
		bool enable
	);

	// 弹出提示窗口
	void info(
		const String& text,					/* 内容 */
		const String& title = L"Infomation"	/* 窗口标题 */
	);

	// 弹出警告窗口
	void warning(
		const String& text,					/* 内容 */
		const String& title = L"Warning"	/* 窗口标题 */
	);

	// 弹出错误窗口
	void error(
		const String& text,					/* 内容 */
		const String& title = L"Error"		/* 窗口标题 */
	);

private:
	Window();

	~Window();

	E2D_DISABLE_COPY(Window);

	// 注册窗口
	HWND __create();

	// 处理窗口消息
	void __poll();

	// Win32 窗口消息回调程序
	static LRESULT CALLBACK WndProc(
		HWND hWnd, 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	);

private:
	HWND	_hWnd;
	Size	_size;
	String	_title;
	int		_iconID;

	static Window * _instance;
};


// 时间控制
class Time
{
	friend class Game;

public:
	// 获取上一帧的时间间隔（秒）
	static double getDeltaTime();

	// 获取游戏总时长（秒）
	static double getTotalTime();

private:
	// 初始化计时操作
	static bool __init();

	// 是否达到更新时间
	static bool __isReady();

	// 更新当前时间
	static void __updateNow();

	// 更新时间信息
	static void __updateLast();

	// 重置时间信息
	static void __reset();

	// 挂起线程
	static void __sleep();

private:
	static std::chrono::steady_clock::time_point _start;
	static std::chrono::steady_clock::time_point _now;
	static std::chrono::steady_clock::time_point _last;
	static std::chrono::steady_clock::time_point _fixedLast;
	static std::chrono::milliseconds _interval;
};


class Listener;

// 输入设备
class Input
{
	friend class Game;

public:
	// 鼠标键值
	enum class Mouse : int
	{
		Left,		/* 鼠标左键 */
		Right,		/* 鼠标右键 */
		Middle		/* 鼠标中键 */
	};


	// 键盘键值
	enum class Key : int
	{
		Up = 0xC8,
		Left = 0xCB,
		Right = 0xCD,
		Down = 0xD0,
		Enter = 0x1C,
		Space = 0x39,
		Esc = 0x01,
		Q = 0x10,
		W = 0x11,
		E = 0x12,
		R = 0x13,
		T = 0x14,
		Y = 0x15,
		U = 0x16,
		I = 0x17,
		O = 0x18,
		P = 0x19,
		A = 0x1E,
		S = 0x1F,
		D = 0x20,
		F = 0x21,
		G = 0x22,
		H = 0x23,
		J = 0x24,
		K = 0x25,
		L = 0x26,
		Z = 0x2C,
		X = 0x2D,
		C = 0x2E,
		V = 0x2F,
		B = 0x30,
		N = 0x31,
		M = 0x32,
		Num1 = 0x02,
		Num2 = 0x03,
		Num3 = 0x04,
		Num4 = 0x05,
		Num5 = 0x06,
		Num6 = 0x07,
		Num7 = 0x08,
		Num8 = 0x09,
		Num9 = 0x0A,
		Num0 = 0x0B,
		Numpad7 = 0x47,
		Numpad8 = 0x48,
		Numpad9 = 0x49,
		Numpad4 = 0x4B,
		Numpad5 = 0x4C,
		Numpad6 = 0x4D,
		Numpad1 = 0x4F,
		Numpad2 = 0x50,
		Numpad3 = 0x51,
		Numpad0 = 0x52
	};

public:
	// 获取输入设备实例
	static Input * getInstance();

	// 销毁输入设备实例
	static void destroyInstance();

	// 检测键盘某按键是否正被按下
	bool isDown(
		Key key
	);

	// 检测键盘某按键是否被点击
	bool isPress(
		Key key
	);

	// 检测键盘某按键是否正在松开
	bool isRelease(
		Key key
	);

	// 检测鼠标按键是否正被按下
	bool isDown(
		Mouse code
	);

	// 检测鼠标按键是否被点击
	bool isPress(
		Mouse code
	);

	// 检测鼠标按键是否正在松开
	bool isRelease(
		Mouse code
	);

	// 获得鼠标X轴坐标值
	double getMouseX();

	// 获得鼠标Y轴坐标值
	double getMouseY();

	// 获得鼠标坐标值
	Point getMousePos();

	// 获得鼠标X轴坐标增量
	double getMouseDeltaX();

	// 获得鼠标Y轴坐标增量
	double getMouseDeltaY();

	// 获得鼠标Z轴（鼠标滚轮）坐标增量
	double getMouseDeltaZ();

	// 添加输入监听
	static Listener * addListener(
		const Function& func,		/* 监听到用户输入时的执行函数 */
		const String& name = L"",	/* 监听器名称 */
		bool paused = false			/* 是否暂停 */
	);

	// 添加碰撞监听
	static void addListener(
		Listener * listener			/* 监听器 */
	);

	// 移除监听器
	static void removeListener(
		Listener * listener			/* 监听器 */
	);

	// 启动输入监听
	static void startListener(
		const String& name
	);

	// 停止输入监听
	static void stopListener(
		const String& name
	);

	// 移除输入监听
	static void removeListener(
		const String& name
	);

	// 启动所有监听器
	static void startAllListeners();

	// 停止所有监听器
	static void stopAllListeners();

	// 移除所有监听器
	static void removeAllListeners();

private:
	Input();

	~Input();

	E2D_DISABLE_COPY(Input);

	// 刷新输入信息
	void __update();

	// 刷新设备状态
	void __updateDeviceState();

	// 更新监听器
	static void __updateListeners();

	// 清空监听器
	static void __clearListeners();

private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboardDevice;
	IDirectInputDevice8* _mouseDevice;
	DIMOUSESTATE _mouseState;
	DIMOUSESTATE _mouseStateRecord;
	POINT _mousePos;

	static Input * _instance;
};


// 渲染器
class Renderer
{
	friend class Game;
	friend class Window;

public:
	// 获取渲染器实例
	static Renderer * getInstance();

	// 销毁实例
	static void destroyInstance();

	// 获取背景色
	Color getBackgroundColor();

	// 修改背景色
	void setBackgroundColor(
		Color color
	);

	// 显示 FPS
	void showFps(
		bool show = true
	);

	// 获取文字渲染器
	TextRenderer * getTextRenderer();

	// 获取 ID2D1HwndRenderTarget 对象
	ID2D1HwndRenderTarget * getRenderTarget();

	// 获取 ID2D1SolidColorBrush 对象
	ID2D1SolidColorBrush * getSolidColorBrush();

	// 获取 ID2D1Factory 对象
	static ID2D1Factory * getFactory();

	// 获取 IWICImagingFactory 对象
	static IWICImagingFactory * getImagingFactory();

	// 获取 IDWriteFactory 对象
	static IDWriteFactory * getWriteFactory();

	// 获取 FPS 文本格式化对象
	static IDWriteTextFormat * getFpsTextFormat();

	// 获取 Miter 样式的 ID2D1StrokeStyle
	static ID2D1StrokeStyle * getMiterStrokeStyle();

	// 获取 Bevel 样式的 ID2D1StrokeStyle
	static ID2D1StrokeStyle * getBevelStrokeStyle();

	// 获取 Round 样式的 ID2D1StrokeStyle
	static ID2D1StrokeStyle * getRoundStrokeStyle();

private:
	Renderer();

	~Renderer();

	E2D_DISABLE_COPY(Renderer);

	// 渲染游戏画面
	void __render();

	// 创建设备相关资源
	bool __createDeviceResources();

	// 删除设备相关资源
	void __discardDeviceResources();

private:
	bool					_showFps;
	D2D1_COLOR_F			_clearColor;
	ID2D1HwndRenderTarget*	_renderTarget;
	ID2D1SolidColorBrush*	_solidBrush;
	TextRenderer*			_textRenderer;

	static ID2D1Factory*		_d2dFactory;
	static IWICImagingFactory*	_imagingFactory;
	static IDWriteFactory*		_writeFactory;
	static IDWriteTextFormat*	_textFormat;
	static ID2D1StrokeStyle*	_miterStrokeStyle;
	static ID2D1StrokeStyle*	_bevelStrokeStyle;
	static ID2D1StrokeStyle*	_roundStrokeStyle;
	static Renderer *			_instance;
};


// 垃圾回收装置
class GC
{
	friend class Game;

public:
	//  获取 GC 实例
	static GC* getInstance();

	// 保留对象
	template <typename Type>
	static inline void retain(Type*& p)
	{
		if (p != nullptr)
		{
			p->retain();
		}
	}

	// 释放对象
	template <typename Type>
	static inline void release(Type*& p)
	{
		if (p != nullptr)
		{
			p->release();
			p = nullptr;
		}
	}

	// 通知 GC 回收垃圾内存
	void notify();

	// 手动回收垃圾内存
	void flush();

	// 将对象放入 GC
	void addObject(
		Object * pObject
	);

	// 清空所有对象
	void clear();

private:
	GC() {}

	~GC();

	E2D_DISABLE_COPY(GC);

	// 更新 GC
	void __update();

private:
	bool				_notifyed;
	std::set<Object*>	_pool;

	static GC _instance;
};

}