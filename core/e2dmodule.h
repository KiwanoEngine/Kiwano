// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __E2D_MODULE_H__
#define __E2D_MODULE_H__


#include "e2dobject.h"
#include "e2dtransition.h"


namespace easy2d
{

	// 图形设备
	class Graphics
	{
	public:
		Graphics(
			HWND hwnd
		);

		~Graphics();

		// 开始渲染
		void BeginDraw();

		// 结束渲染
		void EndDraw();

		// 渲染调试信息
		void DrawDebugInfo();

		// 获取 ID2D1Factory 对象
		ID2D1Factory* GetFactory() const;

		// 获取 IWICImagingFactory 对象
		IWICImagingFactory* GetImagingFactory() const;

		// 获取 IDWriteFactory 对象
		IDWriteFactory* GetWriteFactory() const;

		// 获取 ID2D1HwndRenderTarget 对象
		ID2D1HwndRenderTarget* GetRenderTarget() const;

		// 获取 ID2D1SolidColorBrush 对象
		ID2D1SolidColorBrush* GetSolidBrush() const;

		// 获取文字渲染工具
		IDWriteTextRenderer* GetTextRender() const;

		// 获取 Miter 样式的 ID2D1StrokeStyle
		ID2D1StrokeStyle* GetMiterStrokeStyle();

		// 获取 Bevel 样式的 ID2D1StrokeStyle
		ID2D1StrokeStyle* GetBevelStrokeStyle();

		// 获取 Round 样式的 ID2D1StrokeStyle
		ID2D1StrokeStyle* GetRoundStrokeStyle();

		// 设置文字渲染样式
		void SetTextRendererStyle(
			const Color& fill_color,
			bool has_outline,
			const Color& outline_color,
			float outline_width,
			Stroke outline_stroke
		);

		// 获取 DPI
		static float GetDpi();

	protected:
		D2D1_COLOR_F			clear_color_;
		ID2D1Factory*			factory_;
		IWICImagingFactory*		imaging_factory_;
		IDWriteFactory*			write_factory_;
		ID2D1StrokeStyle*		miter_stroke_style_;
		ID2D1StrokeStyle*		bevel_stroke_style_;
		ID2D1StrokeStyle*		round_stroke_style_;
		IDWriteTextRenderer*	text_renderer_;
		IDWriteTextFormat*		fps_text_format_;
		IDWriteTextLayout*		fps_text_layout_;
		ID2D1SolidColorBrush*	solid_brush_;
		ID2D1HwndRenderTarget*	render_target_;
	};


	// 输入设备
	class Input
	{
	public:
		Input(
			HWND hwnd
		);

		~Input();

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

		// 刷新设备状态
		void Flush();

	protected:
		IDirectInput8W *		direct_input_;
		IDirectInputDevice8W*	keyboard_device_;
		IDirectInputDevice8W*	mouse_device_;
		DIMOUSESTATE			mouse_state_;
		char					key_buffer_[256];
	};

	// 音频设备
	class Audio
	{
	public:
		Audio();

		~Audio();

		// 开启设备
		void Open();

		// 关闭设备
		void Close();

		// 创建音源
		HRESULT CreateVoice(
			IXAudio2SourceVoice ** voice,
			WAVEFORMATEX * wfx
		);

	protected:
		IXAudio2 * x_audio2_;
		IXAudio2MasteringVoice*	mastering_voice_;
	};


	// 设备
	class Device
	{
	public:
		// 获取图形设备
		static Graphics * GetGraphics();

		// 获取输入设备
		static Input * GetInput();

		// 获取音频设备
		static Audio * GetAudio();

		// 初始化
		static void Init(
			HWND hwnd
		);

		// 销毁资源
		static void Destroy();
	};


	// 游戏控制器
	class Game
	{
	public:
		// 开始时
		virtual void OnStart() = 0;

		// 更新时
		virtual void OnUpdate(float dt) {}

		// 退出时
		virtual void OnExit() {}

		// 窗口关闭时
		// 返回值：返回 false 将阻止窗口关闭
		virtual bool OnClose() { return true; }

		// 运行
		void Run();

		// 结束
		void Quit();

		// 设置窗口大小
		void SetSize(
			int width,			/* 窗口宽度 */
			int height			/* 窗口高度 */
		);

		// 设置窗体标题
		void SetTitle(
			const std::wstring& title	/* 窗体标题 */
		);

		// 设置窗体图标
		void SetIcon(
			int resource_id		/* 图标资源 ID */
		);

		// 调试模式
		void SetDebugMode(
			bool enabled
		);

		// 获取窗体标题
		const std::wstring& GetTitle() const;

		// 获取窗体宽度
		int GetWidth() const;

		// 获取窗体高度
		int GetHeight() const;

		// 获取窗体大小
		Size GetSize() const;

		// 获取窗口句柄
		HWND GetHWnd() const;

		// 切换场景
		void EnterScene(
			Scene * scene,						/* 场景 */
			Transition * transition	= nullptr	/* 场景动画 */
		);

		// 获取当前场景
		Scene * GetCurrentScene();

		// 获取实例
		static Game * GetInstance();

	protected:
		Game();

		~Game();

		E2D_DISABLE_COPY(Game);

	private:
		// 初始化
		void Init();

		// 根据客户区大小定位窗口
		Rect Locate(
			int width,
			int height
		);

		// 是否正在进行场景过渡
		bool IsTransitioning() const;

		// 更新场景内容
		void UpdateScene(
			float dt
		);

		// 渲染场景画面
		void DrawScene();

		// Win32 窗口消息回调程序
		static LRESULT CALLBACK WndProc(
			HWND hwnd,
			UINT msg,
			WPARAM w_param,
			LPARAM l_param
		);

	private:
		HWND		hwnd_;
		std::wstring		title_;
		int			width_;
		int			height_;
		int			icon_;
		bool		debug_mode_;
		bool		quit_;
		Scene*		curr_scene_;
		Scene*		next_scene_;
		Transition*	transition_;
	};

} // end of easy2d namespace


#endif // __E2D_MODULE_H__
