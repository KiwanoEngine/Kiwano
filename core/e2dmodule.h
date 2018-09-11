#pragma once
#include "e2dimpl.h"
#include "e2dobject.h"
#include "e2dtransition.h"


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
		enum class PopupStyle : int
		{
			Info,		/* 提示 */
			Warning,	/* 警告 */
			Error		/* 错误 */
		};

	public:
		// 获取窗体实例
		static Window * GetInstance();

		// 销毁窗体实例
		static void DestroyInstance();

		// 获取屏幕大小
		static Size GetScreenSize();

		// 获取窗体标题
		const String& GetTitle() const;

		// 获取窗体宽度
		int GetWidth() const;

		// 获取窗体高度
		int GetHeight() const;

		// 获取窗体大小
		Size GetSize() const;

		// 获取窗口 DPI
		float GetDpi() const;

		// 获取窗口句柄
		HWND GetHWnd();

		// 修改窗体大小
		void SetSize(
			int width,			/* 窗体宽度 */
			int height			/* 窗体高度 */
		);

		// 设置窗体标题
		void SetTitle(
			const String& title	/* 窗体标题 */
		);

		// 设置窗体图标
		void SetIcon(
			int resource_id		/* 图标资源 ID */
		);

		// 设置鼠标指针样式
		void SetCursor(
			Cursor cursor
		);

		// 打开或隐藏控制台
		void SetConsoleEnabled(
			bool enabled
		);

		// 是否允许响应输入法
		void SetTypewritingEnabled(
			bool enabled
		);

		// 检测窗口是否唯一
		// 返回值：返回 false 说明游戏已经正在进行，可以防止用户同时打开多个游戏窗口
		bool CheckUnique();

		// 弹出窗口
		// 返回值：当窗口包含取消按钮时，返回值表示用户是否点击确认按钮
		bool Popup(
			const String& text,					/* 窗口内容 */
			const String& title,				/* 窗口标题 */
			PopupStyle style = PopupStyle::Info,/* 弹窗样式 */
			bool has_cancel = false				/* 包含取消按钮 */
		);

		// 处理窗体消息
		void Poll();

	private:
		Window();

		~Window();

		E2D_DISABLE_COPY(Window);

		// 根据客户区大小定位窗口
		Rect Locate(
			int width,
			int height
		);

		// Win32 窗口消息回调程序
		static LRESULT CALLBACK WndProc(
			HWND hwnd,
			UINT msg,
			WPARAM w_param,
			LPARAM l_param
		);

	private:
		HWND	hwnd_;
		MSG		msg_;
		int		width_;
		int		height_;
		int		icon_id_;
		float	dpi_;
		String	title_;

		static Window * instance_;
	};


	// 渲染器
	class Renderer
	{
	public:
		// 获取渲染器实例
		static Renderer * GetInstance();

		// 销毁实例
		static void DestroyInstance();

		// 获取 ID2D1Factory 对象
		static ID2D1Factory * GetFactory();

		// 获取 IWICImagingFactory 对象
		static IWICImagingFactory * GetImagingFactory();

		// 获取 IDWriteFactory 对象
		static IDWriteFactory * GetWriteFactory();

		// 获取 Miter 样式的 ID2D1StrokeStyle
		static ID2D1StrokeStyle * GetMiterStrokeStyle();

		// 获取 Bevel 样式的 ID2D1StrokeStyle
		static ID2D1StrokeStyle * GetBevelStrokeStyle();

		// 获取 Round 样式的 ID2D1StrokeStyle
		static ID2D1StrokeStyle * GetRoundStrokeStyle();

		// 获取文字渲染器
		E2DTextRenderer * GetTextRenderer();

		// 获取 ID2D1HwndRenderTarget 对象
		ID2D1HwndRenderTarget * GetRenderTarget();

		// 获取 ID2D1SolidColorBrush 对象
		ID2D1SolidColorBrush * GetSolidBrush();

		// 获取背景色
		Color GetBackgroundColor();

		// 修改背景色
		void SetBackgroundColor(
			const Color& color
		);

		// 显示或隐藏 FPS
		// 默认：隐藏
		void ShowFps(
			bool show
		);

		// 开始渲染
		void BeginDraw();

		// 结束渲染
		void EndDraw();

	protected:
		Renderer();

		~Renderer();

		E2D_DISABLE_COPY(Renderer);

	protected:
		bool					show_fps_;
		int						render_times_;
		Time					last_render_time_;
		D2D1_COLOR_F			clear_color_;
		E2DTextRenderer*		text_renderer_;
		IDWriteTextFormat*		fps_text_format_;
		IDWriteTextLayout*		fps_text_layout_;
		ID2D1SolidColorBrush*	solid_brush_;
		ID2D1HwndRenderTarget*	render_target_;

		static ID2D1Factory*		factory_;
		static IWICImagingFactory*	imaging_factory_;
		static IDWriteFactory*		write_factory_;
		static ID2D1StrokeStyle*	miter_stroke_style_;
		static ID2D1StrokeStyle*	bevel_stroke_style_;
		static ID2D1StrokeStyle*	round_stroke_style_;
		static Renderer *			instance_;
	};


	// 输入设备
	class Input
	{
	public:
		// 获取输入设备实例
		static Input * GetInstance();

		// 销毁输入设备实例
		static void DestroyInstance();

		// 检测键盘某按键是否正被按下
		bool IsDown(
			KeyCode key
		);

		// 检测鼠标按键是否正被按下
		bool IsDown(
			MouseCode code
		);

		// 获得鼠标X轴坐标值
		float GetMouseX();

		// 获得鼠标Y轴坐标值
		float GetMouseY();

		// 获得鼠标坐标值
		Point GetMousePos();

		// 获得鼠标X轴坐标增量
		float GetMouseDeltaX();

		// 获得鼠标Y轴坐标增量
		float GetMouseDeltaY();

		// 获得鼠标Z轴（鼠标滚轮）坐标增量
		float GetMouseDeltaZ();

		// 刷新输入设备状态
		void Update();

	protected:
		Input();

		~Input();

		E2D_DISABLE_COPY(Input);

	protected:
		IDirectInput8W *		direct_input_;
		IDirectInputDevice8W*	keyboard_device_;
		IDirectInputDevice8W*	mouse_device_;
		DIMOUSESTATE			mouse_state_;
		char					key_buffer_[256];

		static Input * instance_;
	};


	// 音频设备
	class Audio
	{
	public:
		// 获取音频设备实例
		static Audio * GetInstance();

		// 销毁实例
		static void DestroyInstance();

		// 获取 XAudio2 实例对象
		IXAudio2 * GetXAudio2();

		// 获取 MasteringVoice 实例对象
		IXAudio2MasteringVoice* GetMasteringVoice();

	protected:
		Audio();

		virtual ~Audio();

		E2D_DISABLE_COPY(Audio);

	protected:
		IXAudio2 * x_audio2_;
		IXAudio2MasteringVoice*	mastering_voice_;

		static Audio * instance_;
	};


	// 游戏
	class Game
	{
	public:
		// 获取 Game 实例
		static Game * GetInstance();

		// 销毁实例
		static void DestroyInstance();

		// 启动游戏
		void Start();

		// 暂停游戏
		void Pause();

		// 继续游戏
		void Resume();

		// 结束游戏
		void Quit();

		// 游戏是否暂停
		bool IsPaused();

		// 切换场景
		void EnterScene(
			Scene * scene,						/* 场景 */
			Transition * transition	= nullptr	/* 场景动画 */
		);

		// 获取当前场景
		Scene * GetCurrentScene();

		// 是否正在进行场景动画
		bool IsTransitioning() const;

		// 更新场景内容
		void UpdateScene();

		// 渲染场景画面
		void DrawScene();

	protected:
		Game();

		~Game();

		E2D_DISABLE_COPY(Game);

	private:
		bool			quit_;
		bool			paused_;
		Scene*			curr_scene_;
		Scene*			next_scene_;
		Transition*		transition_;

		static Game * instance_;
	};


	// 垃圾回收
	class GC
	{
	private:
		GC();

		~GC();

		E2D_DISABLE_COPY(GC);

		static GC instance_;
	};

}
