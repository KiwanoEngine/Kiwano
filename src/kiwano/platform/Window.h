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
#include <kiwano/macros.h>
#include <kiwano/math/math.h>
#include <kiwano/core/common.h>
#include <kiwano/core/event/Event.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief ���ָ������
	*/
	enum class CursorType
	{
		Arrow,		///< ָ��
		TextInput,	///< �ı�
		Hand,		///< ��
		SizeAll,	///< ָ���ĸ�����ļ�ͷ
		SizeWE,		///< ָ�����ҷ���ļ�ͷ
		SizeNS,		///< ָ�����·���ļ�ͷ
		SizeNESW,	///< ָ�����µ����Ϸ���ļ�ͷ
		SizeNWSE,	///< ָ�����ϵ����·���ļ�ͷ
	};

	/**
	* \~chinese
	* @brief ��������
	*/
	struct WindowConfig
	{
		String		title;			///< ����
		uint32_t	width;			///< ���
		uint32_t	height;			///< �߶�
		uint32_t	icon;			///< ͼ����Դ ID
		bool		resizable;		///< ���ڴ�С������
		bool		fullscreen;		///< ȫ��ģʽ

		/**
		* \~chinese
		* @brief ������������
		* @param title ����
		* @param width ���
		* @param height �߶�
		* @param icon ͼ����ԴID
		* @param resizable ���ڴ�С������
		* @param fullscreen ȫ��ģʽ
		*/
		WindowConfig(
			String const&	title = L"Kiwano Game",
			uint32_t		width = 640,
			uint32_t		height = 480,
			uint32_t		icon = 0,
			bool			resizable = false,
			bool			fullscreen = false
		);
	};

#if defined(KGE_WIN32)
	typedef HWND WindowHandle;
#endif


	/**
	* \~chinese
	* @brief �����࣬���ƴ��ڱ��⡢��С��ͼ���
	*/
	class KGE_API Window
		: public Singleton<Window>
	{
		friend Singleton<Window>;

	public:
		/**
		* \~chinese
		* @brief ��ʼ������
		* @param config ��������
		*/
		void Init(WindowConfig const& config);

		/**
		* \~chinese
		* @brief ��ȡ���ڱ���
		* @return ���ڱ���
		*/
		String GetTitle() const;

		/**
		* \~chinese
		* @brief ��ȡ���ڴ�С
		* @return ���ڴ�С
		*/
		Size GetSize() const;

		/**
		* \~chinese
		* @brief ��ȡ���ڿ��
		* @return ���ڿ��
		*/
		float GetWidth() const;

		/**
		* \~chinese
		* @brief ��ȡ���ڸ߶�
		* @return ���ڸ߶�
		*/
		float GetHeight() const;

		/**
		* \~chinese
		* @brief ���ñ���
		* @param title ����
		*/
		void SetTitle(String const& title);

		/**
		* \~chinese
		* @brief ���ô���ͼ��
		* @param icon_resource ͼ����ԴID
		*/
		void SetIcon(uint32_t icon_resource);

		/**
		* \~chinese
		* @brief ���贰�ڴ�С
		* @param width ���ڿ��
		* @param height ���ڸ߶�
		*/
		void Resize(int width, int height);

		/**
		* \~chinese
		* @brief ����ȫ��ģʽ
		* @param fullscreen �Ƿ�ȫ��
		* @param width ���ڿ��
		* @param height ���ڸ߶�
		*/
		void SetFullscreen(bool fullscreen, int width, int height);

		/**
		* \~chinese
		* @brief �������ָ������
		* @param cursor ���ָ������
		*/
		void SetCursor(CursorType cursor);

		/**
		* \~chinese
		* @brief ��ѯ�����¼�
		* @return �����¼������еĵ�һ���¼�������Ӷ������Ƴ������¼�����Ϊ���򷵻ؿ�ָ��
		*/
		EventPtr PollEvent();

		/**
		* \~chinese
		* @brief ��ȡ���ھ��
		*/
		WindowHandle GetHandle() const;

		/**
		* \~chinese
		* @brief �Ƿ���Ҫ�ر�
		*/
		bool ShouldClose();

		/**
		* \~chinese
		* @brief ���ٴ���
		*/
		void Destroy();

	private:
		Window();

		~Window();

		void PushEvent(EventPtr evt);

#if defined(KGE_WIN32)
		DWORD GetWindowStyle() const;

		void UpdateCursor();

		void SetActive(bool actived);

		static LRESULT CALLBACK WndProc(HWND, UINT32, WPARAM, LPARAM);
#endif

	private:
		bool					resizable_;
		bool					is_fullscreen_;
		WindowHandle			handle_;
		uint32_t				width_;
		uint32_t				height_;
		wchar_t*				device_name_;
		CursorType				mouse_cursor_;
		std::queue<EventPtr>	event_queue_;
	};
}
