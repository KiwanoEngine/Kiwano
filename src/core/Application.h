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

#pragma once
#include "include-forwards.h"
#include "time.h"
#include "window.h"
#include "render.h"
#include "input.h"
#include "audio.h"

namespace easy2d
{
	struct Options
	{
		String	title;		// ����
		int		width;		// ���
		int		height;		// �߶�
		LPCWSTR	icon;		// ͼ��
		bool	vsync;		// ��ֱͬ��
		bool	fullscreen;	// ȫ��ģʽ
		bool	debug;		// ����ģʽ

		E2D_API Options();
	};


	class E2D_API Application
		: protected Noncopyable
	{
	public:
		Application(
			String const& app_name = L"Easy2dGame"
		);

		virtual ~Application();

		// ��ʼ��
		void Init(
			Options const& options = Options{}
		);

		// ����ʱ
		virtual void OnStart() {}

		// �ر�ʱ
		virtual bool OnClosing() { return true; }

		// ����ʱ
		virtual void OnDestroy() {}

		// ����ʱ
		virtual void OnUpdate(Duration dt) { E2D_NOT_USED(dt); }

		// ����
		void Run();

		// ����
		void Quit();

		// ����
		void Destroy();

		// �л�����
		void EnterScene(
			ScenePtr const& scene			/* ���� */
		);

		// �л�����
		void EnterScene(
			ScenePtr const& scene,			/* ���� */
			TransitionPtr const& transition	/* �������� */
		);

		// ��ȡ��ǰ����
		ScenePtr const& GetCurrentScene();

		// ���ñ���
		void SetTimeScale(float scale);

	private:
		void Render(HWND);

		void Update();

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	private:
		bool			end_;
		bool			inited_;
		bool			debug_;
		float			time_scale_;
		String			app_name_;
		ScenePtr		curr_scene_;
		ScenePtr		next_scene_;
		TransitionPtr	transition_;
	};
}
