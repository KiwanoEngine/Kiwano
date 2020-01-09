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
#include <kiwano/2d/Sprite.h>
#include <kiwano/2d/TextActor.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Button);
	KGE_DECLARE_SMART_PTR(SpriteButton);
	KGE_DECLARE_SMART_PTR(TextButton);

	/**
	* \~chinese
	* @brief ��ť
	*/
	class KGE_API Button
		: public virtual ObjectBase
	{
	public:
		/// \~chinese
		/// @brief ��ť�ص�����
		using Callback = Function<void(Button* /* self */)>;

		Button();

		/// \~chinese
		/// @brief ���찴ť
		/// @param click ��ť����ص�����
		explicit Button(Callback const& click);

		/// \~chinese
		/// @brief ���찴ť
		/// @param click ��ť����ص�����
		/// @param pressed ��ť���»ص�����
		/// @param mouse_over ��ť����ص�����
		/// @param mouse_out ��ť�Ƴ��ص�����
		Button(Callback const& click, Callback const& pressed, Callback const& mouse_over, Callback const& mouse_out);

		virtual ~Button();

		/// \~chinese
		/// @brief ��ȡ��ť״̬�����û��ǽ���
		bool IsEnable() const;

		/// \~chinese
		/// @brief ���ð�ť���û����
		void SetEnabled(bool enabled);

		/// \~chinese
		/// @brief ���ð�ť�����Ļص�����
		void SetClickCallback(const Callback& func);

		/// \~chinese
		/// @brief ���ð�ť������ʱ�Ļص�����
		void SetPressedCallback(const Callback& func);

		/// \~chinese
		/// @brief ���ð�ť��̧��ʱ�Ļص�����
		void SetReleasedCallback(const Callback& func);

		/// \~chinese
		/// @brief ����������밴ťʱ�Ļص�����
		void SetMouseOverCallback(const Callback& func);

		/// \~chinese
		/// @brief ��������Ƴ���ťʱ�Ļص�����
		void SetMouseOutCallback(const Callback& func);

		/// \~chinese
		/// @brief ��ť״̬
		enum class Status
		{
			Normal,		///< ��ͨ
			Hover,		///< ����ڰ�ť��
			Pressed		///< ������
		};

		/// \~chinese
		/// @brief ���ð�ť״̬
		void SetStatus(Status status);

		/// \~chinese
		/// @brief ��ȡ��ť״̬
		Status GetStatus() const;

	protected:
		/// \~chinese
		/// @brief ���°�ť״̬
		void UpdateStatus(Event& evt);

	private:
		bool		enabled_;
		Status		status_;
		Callback	click_callback_;
		Callback	pressed_callback_;
		Callback	released_callback_;
		Callback	mouse_over_callback_;
		Callback	mouse_out_callback_;
	};


	/// \~chinese
	/// @brief ���鰴ť
	class SpriteButton
		: public Sprite
		, public Button
	{
	public:
		SpriteButton();

		/// \~chinese
		/// @brief ���쾫�鰴ť
		/// @param click ��ť����ص�����
		explicit SpriteButton(Callback const& click);

		/// \~chinese
		/// @brief ���쾫�鰴ť
		/// @param click ��ť����ص�����
		/// @param pressed ��ť���»ص�����
		/// @param mouse_over ��ť����ص�����
		/// @param mouse_out ��ť�Ƴ��ص�����
		SpriteButton(Callback const& click, Callback const& pressed, Callback const& mouse_over, Callback const& mouse_out);
	};


	/// \~chinese
	/// @brief ���ְ�ť
	class TextButton
		: public TextActor
		, public Button
	{
	public:
		TextButton();

		/// \~chinese
		/// @brief �������ְ�ť
		/// @param click ��ť����ص�����
		explicit TextButton(Callback const& click);

		/// \~chinese
		/// @brief �������ְ�ť
		/// @param click ��ť����ص�����
		/// @param pressed ��ť���»ص�����
		/// @param mouse_over ��ť����ص�����
		/// @param mouse_out ��ť�Ƴ��ص�����
		TextButton(Callback const& click, Callback const& pressed, Callback const& mouse_over, Callback const& mouse_out);
	};
}
