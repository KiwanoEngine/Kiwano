#pragma once
#include "e2dmacros.h"
#include "e2dcommon.h"
#include "e2dcustom.h"


// Base Classes

namespace e2d
{


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
	Window& size(
		int width,			/* 窗体宽度 */
		int height			/* 窗体高度 */
	);

	// 设置窗体标题
	Window& title(
		const String& title	/* 窗体标题 */
	);

	// 设置窗体图标
	Window& icon(
		int iconID
	);

	// 设置鼠标指针样式
	Window& cursor(
		Cursor c
	);

	// 获取窗体标题
	const String& title() const;

	// 获取窗体宽度
	int width() const;

	// 获取窗体高度
	int height() const;

	// 获取窗体大小
	Size size() const;

	// 获取窗口 DPI
	float dpi() const;

	// 获取窗口句柄
	HWND hWnd() const;

	// 打开或隐藏控制台
	Window& showConsole(
		bool enabled
	);

	// 是否允许响应输入法
	Window& typewritingEnabled(
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
	// 根据客户区大小定位窗口
	Rect _locate(
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
	float mouseX();

	// 获得鼠标Y轴坐标值
	float mouseY();

	// 获得鼠标坐标值
	Point mousePos();

	// 获得鼠标X轴坐标增量
	float mouseDeltaX();

	// 获得鼠标Y轴坐标增量
	float mouseDeltaY();

	// 获得鼠标Z轴（鼠标滚轮）坐标增量
	float mouseDeltaZ();

	// 初始化输入设备
	void initWithWindow(
		Window * window
	);

	// 刷新输入设备状态
	void update();

private:
	IDirectInput8W* _directInput;
	IDirectInputDevice8W* _keyboardDevice;
	IDirectInputDevice8W* _mouseDevice;
	DIMOUSESTATE _mouseState;
	char _keyBuffer[256];
};


// 渲染器
class Renderer
{
public:
	Renderer();

	~Renderer();

	// 获取背景色
	Color backgroundColor();

	// 修改背景色
	void backgroundColor(
		const Color& color
	);

	// 显示或隐藏 FPS
	// 默认：隐藏
	void showFps(
		bool show
	);

	// 获取文字渲染器
	TextRenderer * textRenderer() const { return _textRenderer; }

	// 获取 ID2D1HwndRenderTarget 对象
	ID2D1HwndRenderTarget * renderTarget() const { return _renderTarget; }

	// 获取 ID2D1SolidColorBrush 对象
	ID2D1SolidColorBrush * solidBrush() const { return _solidBrush; }

	// 获取 ID2D1Factory 对象
	ID2D1Factory * factory() const { return _factory; }

	// 获取 IWICImagingFactory 对象
	IWICImagingFactory * imagingFactory() const { return _imagingFactory; }

	// 获取 IDWriteFactory 对象
	IDWriteFactory * writeFactory() const { return _writeFactory; }

	// 获取 Miter 样式的 ID2D1StrokeStyle
	ID2D1StrokeStyle * miterStrokeStyle();

	// 获取 Bevel 样式的 ID2D1StrokeStyle
	ID2D1StrokeStyle * bevelStrokeStyle();

	// 获取 Round 样式的 ID2D1StrokeStyle
	ID2D1StrokeStyle * roundStrokeStyle();

	// 初始化渲染器
	void initWithWindow(
		Window * window
	);

	// 开始渲染
	void beginDraw();

	// 结束渲染
	void endDraw();

private:
	bool					_showFps;
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


class Timer;
class ActionManager;
class Scene;
class Transition;

// 游戏
class Game
{
public:
	// 获取 Game 实例
	static Game * instance();

	// 初始化
	void initWithWindow(
		Window * window
	);

	// 获取窗体
	Window * window() const { return _window; }

	// 获取输入设备
	Input * input() const { return _input; }

	// 获取图形设备
	Renderer * renderer() const { return _renderer; }

	// 启动游戏
	void start();

	// 暂停游戏
	void pause();

	// 继续游戏
	void resume();

	// 结束游戏
	void quit();

	// 游戏是否暂停
	bool paused();

	// 场景入栈
	void pushScene(
		Scene * scene,					/* 下一个场景的指针 */
		bool saveCurrentScene = true	/* 是否保存当前场景 */
	);

	// 场景入栈
	void pushScene(
		Transition * transition,		/* 场景动画 */
		bool saveCurrentScene = true	/* 是否保存当前场景 */
	);

	// 场景出栈
	Scene* popScene();

	// 场景出栈
	Scene* popScene(
		Transition * transition			/* 场景动画 */
	);

	// 清空保存的所有场景
	void clearAllScenes();

	// 获取当前场景
	Scene * currentScene();

	// 获取场景栈
	const std::stack<Scene*>& sceneStack();

	// 是否正在进行场景动画
	bool transitioning() const;

	// 更新场景内容
	void updateScene();

	// 渲染场景画面
	void drawScene();

protected:
	Game();

	~Game();

	E2D_DISABLE_COPY(Game);

private:
	bool		_quit;
	bool		_paused;
	Window*		_window;
	Input*		_input;
	Renderer*	_renderer;
	Timer*		_timer;
	Scene*		_currScene;
	Scene*		_nextScene;
	Transition*	_transition;
	ActionManager*		_actionManager;
	std::stack<Scene*>	_scenes;
};


// 垃圾回收池
class GC
{
public:
	// 获取 GC 实例
	static GC * instance();

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
};

}