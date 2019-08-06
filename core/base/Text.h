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
#include "Node.h"
#include "Font.hpp"
#include "TextStyle.hpp"

namespace easy2d
{
	// �ı�
	class Text
		: public Node
	{
	public:
		Text();

		explicit Text(
			String const& text		/* �������� */
		);

		explicit Text(
			String const& text,		/* �������� */
			const Font& font		/* ���� */
		);

		explicit Text(
			String const& text,		/* �������� */
			const TextStyle& style	/* �ı���ʽ */
		);

		explicit Text(
			String const& text,		/* �������� */
			const Font& font,		/* ���� */
			const TextStyle& style	/* �ı���ʽ */
		);

		virtual ~Text();

		// ��ȡ�ı�
		String const& GetText() const;

		// ��ȡ����
		const Font& GetFont() const;

		// ��ȡ�ı���ʽ
		const TextStyle& GetStyle() const;

		// ��ȡ������
		String const& GetFontFamily() const;

		// ��ȡ��ǰ�ֺ�
		float GetFontSize() const;

		// ��ȡ��ǰ�����ϸֵ
		unsigned int GetFontWeight() const;

		// ��ȡ������ɫ
		const Color& GetColor() const;

		// ��ȡ�����ɫ
		const Color& GetOutlineColor() const;

		// ��ȡ����߿�
		float GetOutlineWidth() const;

		// ��ȡ������ཻ��ʽ
		StrokeStyle GetOutlineStroke() const;

		// ��ȡ�ı���ʾ����
		int GetLineCount();

		// �Ƿ���б��
		bool IsItalic() const;

		// �Ƿ���ʾɾ����
		bool HasStrikethrough() const;

		// �Ƿ���ʾ�»���
		bool HasUnderline() const;

		// �Ƿ���ʾ���
		bool HasOutline() const;

		// �����ı�
		void SetText(
			String const& text
		);

		// �����ı���ʽ
		void SetStyle(
			const TextStyle& style
		);

		// ��������
		void SetFont(
			const Font& font
		);

		// ����������
		void SetFontFamily(
			String const& family
		);

		// �����ֺţ�Ĭ��ֵΪ 22��
		void SetFontSize(
			float size
		);

		// ���������ϸֵ��Ĭ��ֵΪ FontWeight::Normal��
		void SetFontWeight(
			unsigned int weight
		);

		// ����������ɫ��Ĭ��ֵΪ Color::White��
		void SetColor(
			Color const& color
		);

		// ��������б�壨Ĭ��ֵΪ false��
		void SetItalic(
			bool val
		);

		// �򿪻�ر��ı��Զ����У�Ĭ��Ϊ�رգ�
		void SetWrapEnabled(
			bool wrap
		);

		// �����ı��Զ����еĿ�ȣ�Ĭ��Ϊ 0��
		void SetWrapWidth(
			float wrap_width
		);

		// �����м�ࣨĬ��Ϊ 0��
		void SetLineSpacing(
			float line_spacing
		);

		// ���ö��뷽ʽ��Ĭ��Ϊ TextAlign::Left��
		void SetAlignment(
			TextAlign align
		);

		// �����»��ߣ�Ĭ��ֵΪ false��
		void SetUnderline(
			bool underline
		);

		// ����ɾ���ߣ�Ĭ��ֵΪ false��
		void SetStrikethrough(
			bool strikethrough
		);

		// �����Ƿ���ʾ���
		void SetOutline(
			bool outline
		);

		// ���������ɫ
		void SetOutlineColor(
			Color const& outline_color
		);

		// ��������߿�
		void SetOutlineWidth(
			float outline_width
		);

		// ����������ཻ��ʽ
		void SetOutlineStroke(
			StrokeStyle outline_stroke
		);

		// ����Ĭ������
		static void SetDefaultFont(
			Font const& font
		);

		// ����Ĭ��������ʽ
		static void SetDefaultStyle(
			TextStyle const& style
		);

		virtual void OnRender() override;

	protected:
		void UpdateLayout();

	protected:
		String			text_;
		Font			font_;
		TextStyle		style_;
		cpTextFormat	text_format_;
		cpTextLayout	text_layout_;
	};
}