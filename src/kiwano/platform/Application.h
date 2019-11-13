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
#include <kiwano/core/core.h>
#include <kiwano/base/time.h>
#include <kiwano/base/Component.h>
#include <kiwano/base/Event.h>
#include <kiwano/base/Window.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	struct Config
	{
		WindowConfig window;	// ��������
		RenderConfig render;	// ��Ⱦ����
		bool debug;				// ����ģʽ

		Config(
			String const& title		= L"Kiwano Game",
			uint32_t width		= 640,
			uint32_t height	= 480,
			uint32_t icon		= 0
		);

		Config(
			WindowConfig const& wnd_config,
			RenderConfig const& render_config = RenderConfig()
		);
	};


	// Ӧ��
	class KGE_API Application
		: protected core::noncopyable
	{
	public:
		Application();

		virtual ~Application();

		// ��ʼ��
		void Init(
			Config const& config = Config()
		);

		// ��ʼ���ɹ�ʱ
		virtual void OnReady() {}

		// ���ڹر�ʱ
		virtual bool OnClosing() { return true; }

		// ����ʱ
		virtual void OnDestroy() {}

		// ����
		void Run();

		// ����
		void Quit();

		// ����
		void Destroy();

		// ������
		void Use(
			ComponentBase* component
		);

		// ����ʱ����������
		void SetTimeScale(
			float scale_factor
		);

		// �ַ��¼�
		void DispatchEvent(Event* evt);

		// �� Kiwano ���߳���ִ�к���
		// ���������̵߳��� Kiwano ����ʱʹ��
		static void PreformInMainThread(
			Function<void()> Function
		);

	protected:
		void Render();

		void Update();

		static LRESULT CALLBACK WndProc(HWND, UINT32, WPARAM, LPARAM);

	protected:
		bool	end_;
		bool	inited_;
		float	time_scale_;

		Vector<ComponentBase*>		comps_;
		Vector<RenderComponent*>	render_comps_;
		Vector<UpdateComponent*>	update_comps_;
		Vector<EventComponent*>		event_comps_;
	};
}
