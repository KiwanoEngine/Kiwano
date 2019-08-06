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
#include "../base/Node.h"
#include <functional>

namespace easy2d
{
	namespace ui
	{
		class Button
			: public Node
		{
			E2D_DISABLE_COPY(Button);

			typedef std::function<void()> Callback;

		public:
			Button();

			explicit Button(
				Node * normal,					/* ��ͨ״̬ */
				const Callback& func = nullptr	/* ��ť�����Ļص����� */
			);

			explicit Button(
				Node * normal,					/* ��ͨ״̬ */
				Node * selected,				/* ��갴��״̬ */
				const Callback& func = nullptr	/* ��ť�����Ļص����� */
			);

			explicit Button(
				Node * normal,					/* ��ͨ״̬ */
				Node * mouseover,				/* �������״̬ */
				Node * selected,				/* ��갴��״̬ */
				const Callback& func = nullptr	/* ��ť�����Ļص����� */
			);

			explicit Button(
				Node * normal,					/* ��ͨ״̬ */
				Node * mouseover,				/* �������״̬ */
				Node * selected,				/* �������״̬ */
				Node * disabled,				/* ��ť����״̬ */
				const Callback& func = nullptr	/* ��ť�����Ļص����� */
			);

			// ��ȡ��ť״̬�����û��ǽ���
			bool IsEnable() const;

			// ���ð�ť���û����
			void SetEnabled(
				bool enabled
			);

			// ����һ���������ʾ�İ�ť
			virtual void SetNormal(
				Node * normal
			);

			// ����������밴ťʱ��ʾ�İ�ť
			virtual void SetMouseOver(
				Node * mouseover
			);

			// ������갴�°�ťʱ��ʾ�İ�ť
			virtual void SetSelected(
				Node * selected
			);

			// ���ð�ť������ʱ��ʾ�İ�ť
			virtual void SetDisabled(
				Node * disabled
			);

			// ���ð�ť�����Ļص�����
			void SetCallbackOnClick(
				const Callback& func
			);

			// ����֧��λ��
			// Ĭ��Ϊ (0, 0), ��Χ [0, 1]
			virtual void SetPivot(
				float pivot_x,
				float pivot_y
			) override;

		private:
			// ��ť״̬ö��
			enum class Status { Normal, Mouseover, Selected };

			// ���ð�ť״̬
			virtual void SetStatus(
				Status status
			);

			// ˢ�°�ť��ʾ
			virtual void UpdateVisible();

			// �ַ������Ϣ
			virtual bool Dispatch(
				const MouseEvent& e,
				bool handled
			) override;

			// �����ڵ�
			virtual void Visit() override;

		private:
			Node * normal_;
			Node *		mouseover_;
			Node *		selected_;
			Node *		disabled_;
			bool		enabled_;
			bool		is_selected_;
			Status		status_;
			Callback	callback_;
		};
	}
}