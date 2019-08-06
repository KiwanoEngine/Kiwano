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
#include "Font.h"

namespace easy2d
{
	// �ı�
	class Text
		: public Node
	{
		E2D_DISABLE_COPY(Text);

	public:
		// �ı����뷽ʽ
		enum class Align
		{
			Left,		/* ����� */
			Right,		/* �Ҷ��� */
			Center		/* ���ж��� */
		};

		// �ı���ʽ
		class Style
		{
		public:
			Color	color;				// ��ɫ
			Align	alignment;			// ���뷽ʽ
			bool	wrap;				// ���Զ�����
			float	wrap_width;			// �Զ����п��
			float	line_spacing;		// �м��
			bool	underline;			// �»���
			bool	strikethrough;		// ɾ����
			bool	outline;			// ��ʾ���
			Color	outline_color;		// �����ɫ
			float	outline_width;		// ����߿�
			StrokeStyle	outline_stroke;		// ������ཻ��ʽ

		public:
			Style();

			Style(
				Color color,
				Align alignment			= Align::Left,
				bool wrap				= false,
				float wrap_width		= 0.f,
				float line_spacing		= 0.f,
				bool underline			= false,
				bool strikethrough		= false,
				bool outline			= true,
				Color outline_color		= Color(Color::Black, 0.5),
				float outline_width		= 1.f,
				StrokeStyle outline_stroke = StrokeStyle::Round
			);
		};

	public:
		Text();

		explicit Text(
			const String& text,			/* �������� */
			const Font& font	= Font(),		/* ���� */
			const Style& style	= Style()		/* �ı���ʽ */
		);

		virtual ~Text();

		// ��ȡ�ı�
		const String& GetText() const;

		// ��ȡ����
		const Font& GetFont() const;

		// ��ȡ�ı���ʽ
		const Style& GetStyle() const;

		// ��ȡ������
		const String& GetFontFamily() const;

		// ��ȡ��ǰ�ֺ�
		float GetFontSize() const;

		// ��ȡ��ǰ�����ϸֵ
		UINT GetFontWeight() const;

		// ��ȡ������ɫ
		const Color& GetColor() const;

		// ��ȡ�����ɫ
		const Color& GetOutlineColor() const;

		// ��ȡ����߿�
		float GetOutlineWidth() const;

		// ��ȡ������ཻ��ʽ
		StrokeStyle GetOutlineStroke() const;

		// ��ȡ�ı���ʾ����
		int GetLineCount() const;

		// �Ƿ���б��
		bool IsItalic() const;

		// �Ƿ���ʾɾ����
		bool strikethrough() const;

		// �Ƿ���ʾ�»���
		bool underline() const;

		// �Ƿ���ʾ���
		bool outline() const;

		// �����ı�
		void SetText(
			const String& text
		);

		// �����ı���ʽ
		void SetStyle(
			const Style& style
		);

		// ��������
		void SetFont(
			const Font& font
		);

		// ����������
		void SetFontFamily(
			const String& family
		);

		// �����ֺţ�Ĭ��ֵΪ 22��
		void SetFontSize(
			float size
		);

		// ���������ϸֵ��Ĭ��ֵΪ Text::Font::Weight::Normal��
		void SetFontWeight(
			UINT weight
		);

		// ����������ɫ��Ĭ��ֵΪ Color::WHITE��
		void SetColor(
			Color color
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

		// ���ö��뷽ʽ��Ĭ��Ϊ Align::Left��
		void SetAlignment(
			Align align
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
			Color outline_color
		);

		// ��������߿�
		void SetOutlineWidth(
			float outline_width
		);

		// ����������ཻ��ʽ
		void SetOutlineStroke(
			StrokeStyle outline_stroke
		);

		// ��Ⱦ����
		virtual void OnDraw() const override;

	private:
		// �����Ű�����
		void Reset();

		// �������ָ�ʽ��
		void CreateFormat();

		// �������ֲ���
		void CreateLayout();

	private:
		String		text_;
		Font				font_;
		Style				style_;
		IDWriteTextFormat*	text_format_;
		IDWriteTextLayout*	text_layout_;
	};
}