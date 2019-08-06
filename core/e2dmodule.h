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

	// ͼ���豸
	class Graphics
	{
	public:
		Graphics(
			HWND hwnd
		);

		~Graphics();

		// ��ʼ��Ⱦ
		void BeginDraw();

		// ������Ⱦ
		void EndDraw();

		// ��Ⱦ������Ϣ
		void DrawDebugInfo();

		// ��ȡ ID2D1Factory ����
		ID2D1Factory* GetFactory() const;

		// ��ȡ IWICImagingFactory ����
		IWICImagingFactory* GetImagingFactory() const;

		// ��ȡ IDWriteFactory ����
		IDWriteFactory* GetWriteFactory() const;

		// ��ȡ ID2D1HwndRenderTarget ����
		ID2D1HwndRenderTarget* GetRenderTarget() const;

		// ��ȡ ID2D1SolidColorBrush ����
		ID2D1SolidColorBrush* GetSolidBrush() const;

		// ��ȡ������Ⱦ����
		IDWriteTextRenderer* GetTextRender() const;

		// ��ȡ Miter ��ʽ�� ID2D1StrokeStyle
		ID2D1StrokeStyle* GetMiterStrokeStyle();

		// ��ȡ Bevel ��ʽ�� ID2D1StrokeStyle
		ID2D1StrokeStyle* GetBevelStrokeStyle();

		// ��ȡ Round ��ʽ�� ID2D1StrokeStyle
		ID2D1StrokeStyle* GetRoundStrokeStyle();

		// ����������Ⱦ��ʽ
		void SetTextRendererStyle(
			const Color& fill_color,
			bool has_outline,
			const Color& outline_color,
			float outline_width,
			Stroke outline_stroke
		);

		// ��ȡ DPI
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


	// �����豸
	class Input
	{
	public:
		Input(
			HWND hwnd
		);

		~Input();

		// ������ĳ�����Ƿ���������
		bool IsDown(
			KeyCode key
		);

		// �����갴���Ƿ���������
		bool IsDown(
			MouseCode code
		);

		// ������X������ֵ
		float GetMouseX();

		// ������Y������ֵ
		float GetMouseY();

		// ����������ֵ
		Point GetMousePos();

		// ������X����������
		float GetMouseDeltaX();

		// ������Y����������
		float GetMouseDeltaY();

		// ������Z�ᣨ�����֣���������
		float GetMouseDeltaZ();

		// ˢ���豸״̬
		void Flush();

	protected:
		IDirectInput8W *		direct_input_;
		IDirectInputDevice8W*	keyboard_device_;
		IDirectInputDevice8W*	mouse_device_;
		DIMOUSESTATE			mouse_state_;
		char					key_buffer_[256];
	};

	// ��Ƶ�豸
	class Audio
	{
	public:
		Audio();

		~Audio();

		// �����豸
		void Open();

		// �ر��豸
		void Close();

		// ������Դ
		HRESULT CreateVoice(
			IXAudio2SourceVoice ** voice,
			WAVEFORMATEX * wfx
		);

	protected:
		IXAudio2 * x_audio2_;
		IXAudio2MasteringVoice*	mastering_voice_;
	};


	// �豸
	class Device
	{
	public:
		// ��ȡͼ���豸
		static Graphics * GetGraphics();

		// ��ȡ�����豸
		static Input * GetInput();

		// ��ȡ��Ƶ�豸
		static Audio * GetAudio();

		// ��ʼ��
		static void Init(
			HWND hwnd
		);

		// ������Դ
		static void Destroy();
	};


	// ��Ϸ������
	class Game
	{
	public:
		// ��ʼʱ
		virtual void OnStart() = 0;

		// ����ʱ
		virtual void OnUpdate(float dt) {}

		// �˳�ʱ
		virtual void OnExit() {}

		// ���ڹر�ʱ
		// ����ֵ������ false ����ֹ���ڹر�
		virtual bool OnClose() { return true; }

		// ����
		void Run();

		// ����
		void Quit();

		// ���ô��ڴ�С
		void SetSize(
			int width,			/* ���ڿ�� */
			int height			/* ���ڸ߶� */
		);

		// ���ô������
		void SetTitle(
			const std::wstring& title	/* ������� */
		);

		// ���ô���ͼ��
		void SetIcon(
			int resource_id		/* ͼ����Դ ID */
		);

		// ����ģʽ
		void SetDebugMode(
			bool enabled
		);

		// ��ȡ�������
		const std::wstring& GetTitle() const;

		// ��ȡ������
		int GetWidth() const;

		// ��ȡ����߶�
		int GetHeight() const;

		// ��ȡ�����С
		Size GetSize() const;

		// ��ȡ���ھ��
		HWND GetHWnd() const;

		// �л�����
		void EnterScene(
			Scene * scene,						/* ���� */
			Transition * transition	= nullptr	/* �������� */
		);

		// ��ȡ��ǰ����
		Scene * GetCurrentScene();

		// ��ȡʵ��
		static Game * GetInstance();

	protected:
		Game();

		~Game();

		E2D_DISABLE_COPY(Game);

	private:
		// ��ʼ��
		void Init();

		// ���ݿͻ�����С��λ����
		Rect Locate(
			int width,
			int height
		);

		// �Ƿ����ڽ��г�������
		bool IsTransitioning() const;

		// ���³�������
		void UpdateScene(
			float dt
		);

		// ��Ⱦ��������
		void DrawScene();

		// Win32 ������Ϣ�ص�����
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
