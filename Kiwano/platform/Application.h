// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include "../2d/include-forwards.h"
#include "../base/time.h"
#include "../base/window.h"
#include "../base/Component.h"

namespace kiwano
{
	struct Options
	{
		String	title;				// ����
		int		width;				// ���
		int		height;				// �߶�
		LPCWSTR	icon;				// ͼ��
		Color	clear_color;		// ������ɫ
		bool	vsync;				// ��ֱͬ��
		bool	fullscreen;			// ȫ��ģʽ

		Options(
			String const& title = L"Kiwano Game",
			int width = 640,
			int height = 480,
			LPCWSTR icon = nullptr,
			Color clear_color = Color::Black,
			bool vsync = true,
			bool fullscreen = false
		)
			: title(title)
			, width(width)
			, height(height)
			, icon(icon)
			, clear_color(clear_color)
			, vsync(vsync)
			, fullscreen(fullscreen)
		{}
	};


	class KGE_API Application
		: protected Noncopyable
	{
	public:
		Application();

		virtual ~Application();

		// ��ʼ��
		void Init(
			Options const& options
		);

		// ����ʱ
		virtual void OnStart() {}

		// �ر�ʱ
		virtual bool OnClosing() { return true; }

		// ����ʱ
		virtual void OnDestroy() {}

		// ��Ⱦʱ
		virtual void OnRender() {}

		// ����ʱ
		virtual void OnUpdate(Duration dt) { KGE_NOT_USED(dt); }

		// ����
		void Run();

		// ����
		void Quit();

		// ����
		void Destroy();

		// ������
		void Use(
			Component* component
		);

		// ж�����
		void Remove(
			Component* component
		);

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

		// ��ȡ������
		inline Window* GetWindow() const { return main_window_; }

		// ����ʱ����������
		void SetTimeScale(
			float scale_factor
		);

		// ��ʾ������Ϣ
		void ShowDebugInfo(
			bool show = true
		);

		// �� Kiwano ���߳���ִ�к���
		// ���������̵߳��� Kiwano ����ʱʹ��
		static void PreformInMainThread(
			Closure<void()> function
		);

	protected:
		void Render();

		void Update();

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	protected:
		bool			end_;
		bool			inited_;
		float			time_scale_;

		ScenePtr		curr_scene_;
		ScenePtr		next_scene_;
		NodePtr			debug_node_;
		TransitionPtr	transition_;

		Window*				main_window_;
		Array<Component*>	components_;
	};
}
