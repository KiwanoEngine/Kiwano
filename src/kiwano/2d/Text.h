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
#include "Actor.h"
#include "../renderer/TextLayout.h"

namespace kiwano
{
	// �ı�
	class KGE_API Text
		: public Actor
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
		inline String const& GetText() const	{ return text_; }

		// ��ȡ����
		inline Font GetFont() const				{ return font_; }

		// ��ȡ�ı���ʽ
		inline TextStyle GetStyle() const		{ return style_; }

		// ��ȡ�ı�����
		inline TextLayout GetLayout() const		{ return text_layout_; }

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

		// �����ֺţ�Ĭ��ֵΪ 18��
		void SetFontSize(
			Float32 size
		);

		// ���������ϸֵ��Ĭ��ֵΪ FontWeight::Normal��
		void SetFontWeight(
			UInt32 weight
		);

		// ����������ɫ��Ĭ��ֵΪ Color::White��
		void SetColor(
			Color const& color
		);

		// ��������б�壨Ĭ��ֵΪ false��
		void SetItalic(
			bool val
		);

		// �����ı��Զ����еĿ�ȣ�Ĭ��Ϊ 0��
		void SetWrapWidth(
			Float32 wrap_width
		);

		// �����м�ࣨĬ��Ϊ 0��
		void SetLineSpacing(
			Float32 line_spacing
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
			Float32 outline_width
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

		void OnRender(Renderer* renderer) override;

	protected:
		void UpdateLayout();

	protected:
		bool		format_dirty_;
		bool		layout_dirty_;
		TextLayout	text_layout_;
		String		text_;
		Font		font_;
		TextStyle	style_;
	};
}
