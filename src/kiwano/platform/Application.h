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
#include <kiwano/core/Common.h>
#include <kiwano/core/Time.h>
#include <kiwano/core/Component.h>
#include <kiwano/core/event/Event.h>
#include <kiwano/core/event/MouseEvent.h>
#include <kiwano/core/event/KeyEvent.h>
#include <kiwano/core/event/WindowEvent.h>
#include <kiwano/platform/Window.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{
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
		* @brief ��ʼ����ɴ���
		* @details ���ظú�������Ӧ�ó����ʼ����ɺ��Զ�ִ��
		*/
		virtual void OnReady();

		/**
		* \~chinese
		* @brief Ӧ�ó������ٴ���
		* @details ���ظú����Դ���Ӧ�ó�������ʱ����Ϊ���������Դ���յ�
		*/
		virtual void OnDestroy();

		/**
		* \~chinese
		* @brief ����Ӧ�ó���
		* @details ��ʼ�����й��������ִ�� OnReady ����
		* @param debug �Ƿ����õ���ģʽ
		* @note �ú����������ģ�Ӧ�ó������ʱ��������
		*/
		void Run(bool debug = false);

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
		void Use(ComponentBase* component);

		/**
		* \~chinese
		* @brief ����ʱ����������
		* @details ����ʱ���������ӿɵȱ����Ŵ����Сʱ�����
		* @param scale_factor ��������
		* @warning ����Ϊ�������ܵ��¶���ϵͳ����
		*/
		void SetTimeScale(float scale_factor);

		/**
		* \~chinese
		* @brief �ַ��¼�
		* @details ���¼��ַ��������¼��������
		* @param evt �¼�
		*/
		void DispatchEvent(Event* evt);

		/**
		* \~chinese
		* @brief �����߳���ִ�к���
		* @details �ṩ�������̵߳��� Kiwano ����������
		* @param func ��Ҫִ�еĺ���
		*/
		static void PreformInMainThread(Function<void()> func);

	private:
		/**
		* \~chinese
		* @brief �����������
		*/
		void Update();

		/**
		* \~chinese
		* @brief ��Ⱦ�������
		*/
		void Render();

	private:
		float						time_scale_;
		Time						last_update_time_;
		Vector<ComponentBase*>		comps_;
		Vector<RenderComponent*>	render_comps_;
		Vector<UpdateComponent*>	update_comps_;
		Vector<EventComponent*>		event_comps_;
	};

	inline void Application::OnReady()
	{
	}

	inline void Application::OnDestroy()
	{
	}
}
