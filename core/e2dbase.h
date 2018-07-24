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
	void start();

	// 暂停游戏
	void pause();

	// 继续游戏
	void resume();

	// 结束游戏
	void quit();

	// 清理资源
	void cleanup();

	// 游戏是否暂停
	bool isPaused();

	// 修改游戏配置
	void setConfig(
		const Config& config
	);

	// 获取游戏配置
	const Config& getConfig();

	// 获取游戏总时长
	Duration getTotalDuration() const;

private:
	Game();

	~Game();

	E2D_DISABLE_COPY(Game);

	void __update();

private:
	bool		_quit;
	bool		_paused;
	Config		_config;
	Time		_start;
	Time		_now;
	Time		_last;
	Duration	_frameInterval;

	static Game * _instance;
};


// 窗体
class Window
{
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

	// 弹窗样式
	enum class Popup : int
	{
		Information,	/* 信息 */
		Warning,		/* 警告 */
		Error			/* 错误 */
	};

public:
	// 获取窗体实例
	static Window * getInstance();

	// 销毁窗体实例
	static void destroyInstance();

	// 创建窗体互斥体
	bool createMutex(
		const String& mutex = L""	/* 进程互斥体名称 */
	);

	// 修改窗体大小
	void setSize(
		int width,			/* 窗体宽度 */
		int height			/* 窗体高度 */
	);

	// 设置窗体标题
	void setTitle(
		const String& title	/* 窗体标题 */
	);

	// 设置窗体图标
	void setIcon(
		int iconID
	);

	// 设置鼠标指针样式
	void setCursor(
		Cursor cursor
	);

	// 获取窗体标题
	String getTitle();

	// 获取窗体宽度
	double getWidth();

	// 获取窗体高度
	double getHeight();

	// 获取窗体大小
	Size getSize();

	// 获取窗口句柄
	HWND getHWnd();

	// 打开或隐藏控制台
	void setConsoleEnabled(
		bool enabled
	);

	// 是否允许响应输入法
	void setTypewritingEnabled(
		bool enabled
	);

	// 弹出窗口
	// 返回值：当窗口包含取消按钮时，返回值表示用户是否点击确认按钮
	bool popup(
		const String& text,					/* 窗口内容 */
		const String& title,				/* 窗口标题 */
		Popup style = Popup::Information,	/* 弹窗样式 */
		bool hasCancel = false				/* 包含取消按钮 */
	);

	// 处理窗体消息
	void poll();

private:
	Window();

	~Window();

	E2D_DISABLE_COPY(Window);

	// 注册窗口
	HWND __create();

	// Win32 窗口消息回调程序
	static LRESULT CALLBACK WndProc(
		HWND hWnd, 
		UINT message, 
		WPARAM wParam, 
		LPARAM lParam
	);

private:
	HWND	_hWnd;
	MSG		_msg;
	Size	_size;
	String	_title;
	int		_iconID;

	static Window * _instance;
};


// 输入设备
class Input
{
public:
	// 获取输入设备实例
	static Input * getInstance();

	// 销毁输入设备实例
	static void destroyInstance();

	// 检测键盘某按键是否正被按下
	bool isDown(
		KeyCode key
	);

	// 检测键盘某按键是否被点击
	bool isPress(
		KeyCode key
	);

	// 检测键盘某按键是否正在松开
	bool isRelease(
		KeyCode key
	);

	// 检测鼠标按键是否正被按下
	bool isDown(
		MouseCode code
	);

	// 检测鼠标按键是否被点击
	bool isPress(
		MouseCode code
	);

	// 检测鼠标按键是否正在松开
	bool isRelease(
		MouseCode code
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

	// 刷新输入设备状态
	void update();

private:
	Input();

	~Input();

	E2D_DISABLE_COPY(Input);

private:
	IDirectInput8W* _directInput;
	IDirectInputDevice8W* _keyboardDevice;
	IDirectInputDevice8W* _mouseDevice;
	DIMOUSESTATE _mouseState;
	DIMOUSESTATE _mouseRecordState;
	char _keyBuffer[256];
	char _keyRecordBuffer[256];

	static Input * _instance;
};


// 渲染器
class Renderer
{
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

	// 渲染游戏画面
	void render();

	// 删除设备相关资源
	void discardDeviceResources();

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

	// 渲染 FPS
	void _renderFps();

private:
	int						_renderTimes;
	Time					_lastRenderTime;
	D2D1_COLOR_F			_clearColor;
	TextRenderer*			_textRenderer;
	IDWriteTextFormat*		_fpsFormat;
	IDWriteTextLayout*		_fpsLayout;
	ID2D1SolidColorBrush*	_solidBrush;
	ID2D1HwndRenderTarget*	_renderTarget;

	static ID2D1Factory*		_d2dFactory;
	static IWICImagingFactory*	_imagingFactory;
	static IDWriteFactory*		_writeFactory;
	static ID2D1StrokeStyle*	_miterStrokeStyle;
	static ID2D1StrokeStyle*	_bevelStrokeStyle;
	static ID2D1StrokeStyle*	_roundStrokeStyle;
	static Renderer *			_instance;
};


// 垃圾回收器
class GC
{
public:
	// 获取 GC 实例
	static GC * getInstance();

	// 自动释放
	void autorelease(
		Ref* ref
	);

	// 安全地释放对象
	void safeRelease(
		Ref* ref
	);

	// 刷新内存池
	void flush();

	// 回收内存池中的所有对象
	void clear();

private:
	GC();

	~GC();

	E2D_DISABLE_COPY(GC);

private:
	bool _notifyed;
	bool _cleanup;
	std::set<Ref*> _pool;

	static GC _instance;
};

}