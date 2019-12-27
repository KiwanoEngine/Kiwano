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
#include <kiwano/math/math.h>
#include <kiwano/renderer/TextStyle.hpp>

namespace kiwano
{
	class RenderTarget;
	class Renderer;

	/// \~chinese
	/// @brief �ı�����
	class KGE_API TextLayout
	{
		friend class RenderTarget;
		friend class Renderer;

	public:
		/// \~chinese
		/// @brief ����յ��ı�����
		TextLayout();

		/// \~chinese
		/// @brief �ı������Ƿ���Ч
		bool IsValid() const;

		/// \~chinese
		/// @brief �ı������Ƿ�¾�
		bool IsDirty() const;

		/// \~chinese
		/// @brief �����ı�����
		/// @note �ı������������µģ����޸��ı����ֵ����Ժ���Ҫ�ֶ�����
		void Update();

		/// \~chinese
		/// @brief ��ȡ�ı�
		const String& GetText() const;

		/// \~chinese
		/// @brief ��ȡ�ı���ʽ
		const TextStyle& GetStyle() const;

		/// \~chinese
		/// @brief ��ȡ�ı�����
		uint32_t GetLineCount() const;

		/// \~chinese
		/// @brief ��ȡ�ı����ִ�С
		Size GetLayoutSize() const;

		/// \~chinese
		/// @brief �����ı�
		void SetText(const String& text);

		/// \~chinese
		/// @brief �����ı���ʽ
		void SetStyle(const TextStyle& style);

		/// \~chinese
		/// @brief ��������
		void SetFont(FontPtr font);

		/// \~chinese
		/// @brief ����������
		void SetFontFamily(String const& family);

		/// \~chinese
		/// @brief �����ֺţ�Ĭ��ֵΪ 18��
		void SetFontSize(float size);

		/// \~chinese
		/// @brief ���������ϸֵ��Ĭ��ֵΪ FontWeight::Normal��
		void SetFontWeight(uint32_t weight);

		/// \~chinese
		/// @brief ����������仭ˢ
		void SetFillBrush(BrushPtr brush);

		/// \~chinese
		/// @brief �������������ɫ��Ĭ��ֵΪ Color::White��
		void SetFillColor(Color const& color);

		/// \~chinese
		/// @brief ��������б�壨Ĭ��ֵΪ false��
		void SetItalic(bool italic);

		/// \~chinese
		/// @brief �����ı��Զ����еĿ��
		void SetWrapWidth(float wrap_width);

		/// \~chinese
		/// @brief �����м�ࣨĬ��Ϊ 0��
		void SetLineSpacing(float line_spacing);

		/// \~chinese
		/// @brief ���ö��뷽ʽ
		void SetAlignment(TextAlign align);

		/// \~chinese
		/// @brief ����������߻�ˢ
		void SetOutlineBrush(BrushPtr brush);

		/// \~chinese
		/// @brief �������������ɫ
		void SetOutlineColor(Color const& outline_color);

		/// \~chinese
		/// @brief ������������߿�
		void SetOutlineWidth(float outline_width);

		/// \~chinese
		/// @brief ��������������ཻ��ʽ
		void SetOutlineStroke(StrokeStyle outline_stroke);

		/// \~chinese
		/// @brief �����»���
		/// @param enable �Ƿ���ʾ�»���
		/// @param start ��ʼλ��
		/// @param length ����
		void SetUnderline(bool enable, uint32_t start, uint32_t length);

		/// \~chinese
		/// @brief ����ɾ����
		/// @param enable �Ƿ���ʾɾ����
		/// @param start ��ʼλ��
		/// @param length ����
		void SetStrikethrough(bool enable, uint32_t start, uint32_t length);

	private:
		ComPtr<IDWriteTextFormat> GetTextFormat() const;

		void SetTextFormat(ComPtr<IDWriteTextFormat> format);

		ComPtr<IDWriteTextLayout> GetTextLayout() const;

		void SetTextLayout(ComPtr<IDWriteTextLayout> layout);

	private:
		enum DirtyFlag : uint8_t
		{
			Clean = 0,
			DirtyFormat = 1,
			DirtyLayout = 1 << 1,
		};
		uint8_t dirty_flag_;

		ComPtr<IDWriteTextFormat> text_format_;
		ComPtr<IDWriteTextLayout> text_layout_;

		String text_;
		TextStyle style_;
	};

	inline bool TextLayout::IsValid() const
	{
		return !!text_layout_;
	}

	inline bool TextLayout::IsDirty() const
	{
		return dirty_flag_ != DirtyFlag::Clean;
	}

	inline const String& TextLayout::GetText() const
	{
		return text_;
	}

	inline const TextStyle& TextLayout::GetStyle() const
	{
		return style_;
	}

	inline ComPtr<IDWriteTextFormat> TextLayout::GetTextFormat() const
	{
		return text_format_;
	}

	inline ComPtr<IDWriteTextLayout> TextLayout::GetTextLayout() const
	{
		return text_layout_;
	}

	inline void TextLayout::SetFillBrush(BrushPtr brush)
	{
		style_.fill_brush = brush;
	}

	inline void TextLayout::SetFillColor(Color const& color)
	{
		style_.SetFillColor(color);
	}

	inline void TextLayout::SetTextFormat(ComPtr<IDWriteTextFormat> format)
	{
		text_format_ = format;
	}

	inline void TextLayout::SetTextLayout(ComPtr<IDWriteTextLayout> layout)
	{
		text_layout_ = layout;
	}

	inline void TextLayout::SetOutlineBrush(BrushPtr brush)
	{
		style_.outline_brush = brush;
	}

	inline void TextLayout::SetOutlineColor(Color const& outline_color)
	{
		style_.SetOutlineColor(outline_color);
	}

	inline void TextLayout::SetOutlineWidth(float outline_width)
	{
		style_.outline_width = outline_width;
	}

	inline void TextLayout::SetOutlineStroke(StrokeStyle outline_stroke)
	{
		style_.outline_stroke = outline_stroke;
	}
}
