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
#include "../core/core.h"
#include "../base/time.h"
#include "../base/Component.h"
#include "../base/Event.hpp"
#include "../base/Window.h"
#include "../renderer/Renderer.h"

namespace kiwano
{
	struct Config
	{
		WindowConfig window;	// ��������
		RenderConfig render;	// ��Ⱦ����
		bool debug;				// ����ģʽ

		Config(
			String const& title	= L"Kiwano Game",
			UInt32 width		= 640,
			UInt32 height		= 480,
			UInt32 icon			= 0
		);

		Config(
			WindowConfig const& wnd_config,
			RenderConfig const& render_config = RenderConfig()
		);
	};


	// Ӧ��
	class KGE_API Application
		: protected noncopyable
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
			Component* component
		);

		// ж�����
		void Remove(
			Component* component
		);

		// ����ʱ����������
		void SetTimeScale(
			Float32 scale_factor
		);

		// �ַ��¼�
		void DispatchEvent(Event& evt);

		// �� Kiwano ���߳���ִ�к���
		// ���������̵߳��� Kiwano ����ʱʹ��
		static void PreformInMainThread(
			Function<void()> Function
		);

	protected:
		void Render();

		void Update();

		static LRESULT CALLBACK WndProc(HWND, UInt32, WPARAM, LPARAM);

	protected:
		bool	end_;
		bool	inited_;
		Float32	time_scale_;

		Vector<Component*>	components_;
	};
}
