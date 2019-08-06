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
#include "../core/Sprite.h"
#include <functional>

namespace easy2d
{
	namespace ui
	{
		class Button
			: public Sprite
		{
			using Callback = std::function<void()>;

		public:
			Button();

			explicit Button(
				Callback const& click		/* ��ť����ص����� */
			);

			explicit Button(
				Callback const& click,		/* ��ť����ص����� */
				Callback const& pressed,	/* ��ť���»ص����� */
				Callback const& mouse_over,	/* ��ť����ص����� */
				Callback const& mouse_out	/* ��ť�Ƴ��ص����� */
			);

			virtual ~Button();

			// ��ȡ��ť״̬�����û��ǽ���
			bool IsEnable() const;

			// ���ð�ť���û����
			void SetEnabled(
				bool enabled
			);

			// ���ð�ť�����Ļص�����
			void SetClickCallback(
				const Callback& func
			);

			// ���ð�ť������ʱ�Ļص�����
			void SetPressedCallback(
				const Callback& func
			);

			// ����������밴ťʱ�Ļص�����
			void SetMouseOverCallback(
				const Callback& func
			);

			// ��������Ƴ���ťʱ�Ļص�����
			void SetMouseOutCallback(
				const Callback& func
			);

		private:
			enum class Status { Normal, Hover, Pressed };

			void SetStatus(
				Status status
			);

			void UpdateStatus(Event const& evt);

		private:
			bool		enabled_;
			bool		is_selected_;
			Status		status_;
			Callback	click_callback_;
			Callback	pressed_callback_;
			Callback	mouse_over_callback_;
			Callback	mouse_out_callback_;
		};
	}
}