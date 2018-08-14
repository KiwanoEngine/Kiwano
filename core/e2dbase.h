#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"
#include "e2dcustom.h"


// Base Classes

namespace e2d
{


// 配置
class Config
{
public:
	Config();

	virtual ~Config();

	// 修改游戏名称
	// 默认：空
	void setGameName(
		const String& name
	);

	// 显示或隐藏 FPS
	// 默认：隐藏
	void showFps(
		bool show
	);

	// 显示或隐藏节点轮廓
	// 默认：隐藏
	void setOutlineVisible(
		bool visible
	);

	// 打开或关闭碰撞监听
	// 默认：关闭
	void setCollisionEnabled(
		bool enabled
	);

	// 打开或关闭碰撞体可视化
	// 默认：关闭
	void setColliderVisible(
		bool visible
	);

	// 获取游戏名称
	String getGameName() const;

	// 获取 FPS 显示状态
	bool isFpsShow() const;

	// 获取节点轮廓显示状态
	bool isOutlineVisible() const;

	// 获取碰撞监听状态
	bool isCollisionEnabled() const;

	// 获取碰撞体可视化状态
	bool isColliderVisible() const;

protected:
	bool			_showFps;
	bool			_outlineVisible;
	bool			_collisionEnabled;
	bool			_colliderVisible;
	String			_gameName;
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
	Window(
		const String& title,	/* 窗体标题 */
		int width,				/* 窗体宽度 */
		int height,				/* 窗体高度 */
		int iconID = 0			/* 窗体图标 */
	);

	~Window();

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
	String getTitle() const;

	// 获取窗体宽度
	int getWidth() const;

	// 获取窗体高度
	int getHeight() const;

	// 获取窗体大小
	Size getSize() const;

	// 获取窗口 DPI
	float getDpi() const;

	// 获取窗口句柄
	HWND getHWnd() const;

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
	// 根据客户区大小计算合适的窗口区域
	Rect __adjustWindow(
		int width,
		int height
	);

	// Win32 窗口消息回调程序
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
	);

private:
	HWND	_hWnd;
	MSG		_msg;
	int		_width;
	int		_height;
	String	_title;
	int		_iconID;
	float	_dpi;
};


// 输入设备
class Input
{
public:
	Input();

	~Input();

	// 检测键盘某按键是否正被按下
	bool isDown(
		KeyCode key
	);

	// 检测鼠标按键是否正被按下
	bool isDown(
		MouseCode code
	);

	// 获得鼠标X轴坐标值
	float getMouseX();

	// 获得鼠标Y轴坐标值
	float getMouseY();

	// 获得鼠标坐标值
	Point getMousePos();

	// 获得鼠标X轴坐标增量
	float getMouseDeltaX();

	// 获得鼠标Y轴坐标增量
	float getMouseDeltaY();

	// 获得鼠标Z轴（鼠标滚轮）坐标增量
	float getMouseDeltaZ();

	// 刷新输入设备状态
	void update();

	// 初始化渲染器（不应手动调用该函数）
	void init(
		Window * window
	);

private:
	IDirectInput8W* _directInput;
	IDirectInputDevice8W* _keyboardDevice;
	IDirectInputDevice8W* _mouseDevice;
	DIMOUSESTATE _mouseState;
	char _keyBuffer[256];
};


// 图形设备
class Renderer
{
public:
	Renderer();

	~Renderer();

	// 获取背景色
	Color getBackgroundColor();

	// 修改背景色
	void setBackgroundColor(
		Color color
	);

	// 渲染游戏画面
	void render();

	// 获取文字渲染器
	TextRenderer * getTextRenderer() const { return _textRenderer; }

	// 获取 ID2D1HwndRenderTarget 对象
	ID2D1HwndRenderTarget * getRenderTarget() const { return _renderTarget; }

	// 获取 ID2D1SolidColorBrush 对象
	ID2D1SolidColorBrush * getSolidColorBrush() const { return _solidBrush; }

	// 获取 ID2D1Factory 对象
	ID2D1Factory * getFactory() const { return _factory; }

	// 获取 IWICImagingFactory 对象
	IWICImagingFactory * getImagingFactory() const { return _imagingFactory; }

	// 获取 IDWriteFactory 对象
	IDWriteFactory * getWriteFactory() const { return _writeFactory; }

	// 获取 Miter 样式的 ID2D1StrokeStyle
	ID2D1StrokeStyle * getMiterStrokeStyle();

	// 获取 Bevel 样式的 ID2D1StrokeStyle
	ID2D1StrokeStyle * getBevelStrokeStyle();

	// 获取 Round 样式的 ID2D1StrokeStyle
	ID2D1StrokeStyle * getRoundStrokeStyle();

	// 初始化渲染器（不应手动调用该函数）
	void init(
		Window * window
	);

private:
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
	ID2D1Factory*			_factory;
	IWICImagingFactory*		_imagingFactory;
	IDWriteFactory*			_writeFactory;
	ID2D1StrokeStyle*		_miterStrokeStyle;
	ID2D1StrokeStyle*		_bevelStrokeStyle;
	ID2D1StrokeStyle*		_roundStrokeStyle;
};


// 游戏
class Game
{
public:
	// 获取 Game 实例
	static Game * getInstance();

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
	const Config& getConfig() const;

	// 设置窗体
	void setWindow(
		Window * window
	);

	// 获取窗体
	Window * getWindow() const { return _window; }

	// 获取输入设备
	Input * getInput() const { return _input; }

	// 获取图形设备
	Renderer * getRenderer() const { return _renderer; }

protected:
	Game();

	~Game();

	E2D_DISABLE_COPY(Game);

private:
	bool		_quit;
	bool		_paused;
	Config		_config;
	Window *	_window;
	Input *		_input;
	Renderer *	_renderer;
	
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