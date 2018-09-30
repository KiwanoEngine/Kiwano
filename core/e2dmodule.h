#pragma once
#include "e2dimpl.h"
#include "e2dobject.h"
#include "e2dtransition.h"


namespace e2d
{


	// 窗口
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
		// 设置鼠标指针样式
		static void SetCursor(
			Cursor cursor
		);

		// 打开或隐藏控制台
		static void ShowConsole(
			bool enabled
		);

		// 弹出窗口
		// 返回值：当窗口包含取消按钮时，返回值表示用户是否点击确认按钮
		static bool Popup(
			const String& text,					/* 窗口内容 */
			const String& title,				/* 窗口标题 */
			PopupStyle style = PopupStyle::Info,/* 弹窗样式 */
			bool has_cancel = false				/* 包含取消按钮 */
		);

		// 获取屏幕大小
		static Size GetScreenSize();
	};


	// 渲染器
	class Graphics
	{
	public:
		// 获取渲染器实例
		static Graphics * Get();

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

		// 显示或隐藏 FPS
		// 默认：隐藏
		void ShowFps(
			bool show
		);

		// 开始渲染
		void BeginDraw();

		// 结束渲染
		void EndDraw();

		// 渲染调试信息
		void DrawDebugInfo();

	protected:
		Graphics();

		~Graphics();

		E2D_DISABLE_COPY(Graphics);

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
		static Graphics *			instance_;
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
		static Audio * Get();

		// 获取 XAudio2 实例对象
		IXAudio2 * GetXAudio2() const;

		// 获取 MasteringVoice 实例对象
		IXAudio2MasteringVoice* GetMasteringVoice() const;

	protected:
		Audio();

		virtual ~Audio();

		E2D_DISABLE_COPY(Audio);

	protected:
		IXAudio2 * x_audio2_;
		IXAudio2MasteringVoice*	mastering_voice_;
	};


	// 选项
	struct Option
	{
		String	title;				// 窗口标题
		int		width;				// 窗口宽度
		int		height;				// 窗口高度
		int		icon;				// 窗口图标
		Color	background_color;	// 背景色
		bool	debug_mode;			// Debug 模式

		Option()
			: title(L"Easy2D Game")
			, width(640)
			, height(480)
			, icon(0)
			, background_color(Color::Black)
			, debug_mode(false)
		{
		}
	};


	// 游戏控制器
	class Game
	{
	public:
		static Game * New(
			const Option& option
		);

		// 获取控制器
		static Game * Get();

		// 启动游戏
		void Run();

		// 结束游戏
		void Quit();

		// 获取窗体标题
		const String& GetTitle() const;

		// 获取窗体宽度
		int GetWidth() const;

		// 获取窗体高度
		int GetHeight() const;

		// 获取窗体大小
		Size GetSize() const;

		// 获取窗口句柄
		HWND GetHWnd() const;

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

		// 初始化
		void Init();

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
		HWND		hwnd_;
		String		title_;
		int			width_;
		int			height_;
		int			icon_;
		bool		debug_mode_;
		bool		quit_;
		Scene*		curr_scene_;
		Scene*		next_scene_;
		Transition*	transition_;

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
