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
#include <kiwano/common/common.h>
#include <kiwano/core/time.h>
#include <kiwano/core/Component.h>
#include <kiwano/core/Event.h>
#include <kiwano/platform/Window.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief Ӧ�ó�������
	* @details ���� Kiwano Ӧ�ó���ǰ�ĳ�ʼ��ѡ��
	*/
	struct Config
	{
		WindowConfig window;	///< ��������
		RenderConfig render;	///< ��Ⱦ����
		bool debug;				///< ���õ���ģʽ

		/**
		* \~chinese
		* @param title ���ڱ���
		* @param width ���ڿ��
		* @param height ���ڸ߶�
		* @param icon ����ͼ�����ԴID
		*/
		Config(
			String const& title	= L"Kiwano Game",
			uint32_t width		= 640,
			uint32_t height		= 480,
			uint32_t icon		= 0
		);

		/**
		* \~chinese
		* @param wnd_config ��������
		* @param render_config ��Ⱦ����
		*/
		Config(
			WindowConfig const& wnd_config,
			RenderConfig const& render_config = RenderConfig()
		);
	};


	/**
	* \~chinese
	* @brief Ӧ�ó��򣬿�����Ϸ�������������ڣ�������ʼ���������������Լ��¼��ַ���
	*/
	class KGE_API Application
		: protected Noncopyable
	{
	public:
		Application();

		virtual ~Application();

		/**
		* \~chinese
		* @brief ��ʼ��Ӧ�ó���
		* @details ��ʼ�����й��������ִ�� OnReady ����
		* @param config ��ʼ������
		* @attention Ӧ��ʹ����������ǰִ�г�ʼ����������������쳣
		*/
		void Init(
			Config const& config = Config()
		);

		/**
		* \~chinese
		* @brief ��ʼ����ɴ���
		* @details ���ظú�������Ӧ�ó����ʼ����ɺ��Զ�ִ��
		*/
		virtual void OnReady() {}

		/**
		* \~chinese
		* @brief ���ڹرմ���
		* @details ���ظú����Դ����û��ر�Ӧ�ó��򴰿�ʱ����Ϊ
		* @return ���� true �������رմ��ڣ�������ֹ���ڹر�
		*/
		virtual bool OnClosing() { return true; }

		/**
		* \~chinese
		* @brief Ӧ�ó������ٴ���
		* @details ���ظú����Դ���Ӧ�ó�������ʱ����Ϊ���������Դ���յ�
		*/
		virtual void OnDestroy() {}

		/**
		* \~chinese
		* @brief ����Ӧ�ó���
		* @note �ú����������ģ�Ӧ�ó������ʱ��������
		*/
		void Run();

		/**
		* \~chinese
		* @brief ��ֹӦ�ó���
		*/
		void Quit();

		/**
		* \~chinese
		* @brief ������Ϸ���й����в�����������Դ
		*/
		void Destroy();

		/**
		* \~chinese
		* @brief ��ӹ������
		* @param[in] component �������
		*/
		void Use(
			ComponentBase* component
		);

		/**
		* \~chinese
		* @brief ����ʱ����������
		* @details ����ʱ���������ӿɵȱ����Ŵ����Сʱ�����
		* @param scale_factor ��������
		* @warning ����Ϊ�������ܵ��¶���ϵͳ����
		*/
		void SetTimeScale(
			float scale_factor
		);

		/**
		* \~chinese
		* @brief �ַ��¼�
		* @details ���¼��ַ��������¼��������
		* @param evt �¼�
		*/
		void DispatchEvent(Event& evt);

		/**
		* \~chinese
		* @brief �����߳���ִ�к���
		* @details �ṩ�������̵߳��� Kiwano ����������
		* @param func ��Ҫִ�еĺ���
		*/
		static void PreformInMainThread(
			Function<void()> func
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
