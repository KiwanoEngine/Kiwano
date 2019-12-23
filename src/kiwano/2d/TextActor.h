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
#include <kiwano/2d/Actor.h>
#include <kiwano/renderer/Color.h>
#include <kiwano/renderer/TextLayout.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(TextActor);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief �ı���ɫ
	*/
	class KGE_API TextActor
		: public Actor
	{
	public:
		/// \~chinese
		/// @brief �����յ��ı���ɫ
		TextActor();

		/// \~chinese
		/// @brief �����յ��ı���ɫ
		/// @param text ��������
		explicit TextActor(const String& text);

		/// \~chinese
		/// @brief �����յ��ı���ɫ
		/// @param text ��������
		/// @param font ����
		TextActor(const String& text, const Font& font);

		/// \~chinese
		/// @brief �����յ��ı���ɫ
		/// @param text ��������
		/// @param style �ı���ʽ
		TextActor(const String& text, const TextStyle& style);

		/// \~chinese
		/// @brief �����յ��ı���ɫ
		/// @param text ��������
		/// @param font ����
		/// @param style �ı���ʽ
		TextActor(const String& text, const Font& font, const TextStyle& style);

		virtual ~TextActor();

		/// \~chinese
		/// @brief ��ȡ�ı�
		String const& GetText() const;

		/// \~chinese
		/// @brief ��ȡ����
		Font GetFont() const;

		/// \~chinese
		/// @brief ��ȡ�ı���ʽ
		TextStyle GetStyle() const;

		/// \~chinese
		/// @brief ��ȡ�ı�����
		TextLayout GetLayout() const;

		/// \~chinese
		/// @brief �����ı�
		void SetText(String const& text);

		/// \~chinese
		/// @brief �����ı���ʽ
		void SetStyle(const TextStyle& style);

		/// \~chinese
		/// @brief ��������
		void SetFont(const Font& font);

		/// \~chinese
		/// @brief ����������
		void SetFontFamily(String const& family);

		/// \~chinese
		/// @brief �����ֺţ�Ĭ��ֵΪ 18��
		void SetFontSize(
			float size
		);

		/// \~chinese
		/// @brief ���������ϸֵ��Ĭ��ֵΪ FontWeight::Normal��
		void SetFontWeight(uint32_t weight);

		/// \~chinese
		/// @brief ����������ɫ��Ĭ��ֵΪ Color::White��
		void SetColor(Color const& color);

		/// \~chinese
		/// @brief ��������б�壨Ĭ��ֵΪ false��
		void SetItalic(bool italic);

		/// \~chinese
		/// @brief �����ı��Զ����еĿ�ȣ�Ĭ��Ϊ 0��
		void SetWrapWidth(float wrap_width);

		/// \~chinese
		/// @brief �����м�ࣨĬ��Ϊ 0��
		void SetLineSpacing(float line_spacing);

		/// \~chinese
		/// @brief ���ö��뷽ʽ��Ĭ��Ϊ TextAlign::Left��
		void SetAlignment(TextAlign align);

		/// \~chinese
		/// @brief �����»��ߣ�Ĭ��ֵΪ false��
		void SetUnderline(bool underline);

		/// \~chinese
		/// @brief ����ɾ���ߣ�Ĭ��ֵΪ false��
		void SetStrikethrough(bool strikethrough);

		/// \~chinese
		/// @brief �����Ƿ���ʾ���
		void SetOutline(bool outline);

		/// \~chinese
		/// @brief ���������ɫ
		void SetOutlineColor(Color const& outline_color);

		/// \~chinese
		/// @brief ��������߿�
		void SetOutlineWidth(float outline_width);

		/// \~chinese
		/// @brief ����������ཻ��ʽ
		void SetOutlineStroke(StrokeStyle outline_stroke);

		/// \~chinese
		/// @brief �����ı�����
		void UpdateLayout();

		/// \~chinese
		/// @brief ����Ĭ������
		static void SetDefaultFont(Font const& font);

		/// \~chinese
		/// @brief ����Ĭ��������ʽ
		static void SetDefaultStyle(TextStyle const& style);

		void OnRender(RenderTarget* rt) override;

	private:
		bool		format_dirty_;
		bool		layout_dirty_;
		TextLayout	text_layout_;
		String		text_;
		Font		font_;
	};

	/** @} */

	inline String const&	TextActor::GetText() const		{ return text_; }

	inline Font				TextActor::GetFont() const		{ return font_; }

	inline TextStyle		TextActor::GetStyle() const		{ return text_layout_.GetTextStyle(); }

	inline TextLayout		TextActor::GetLayout() const	{ return text_layout_; }

}
