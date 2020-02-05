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
#include <kiwano/render/Color.h>
#include <kiwano/render/TextLayout.h>

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
class KGE_API TextActor : public Actor
{
public:
    /// \~chinese
    /// @brief �����յ��ı���ɫ
    TextActor();

    /// \~chinese
    /// @brief �����ı���ɫ
    /// @param text ��������
    explicit TextActor(const String& text);

    /// \~chinese
    /// @brief �����ı���ɫ
    /// @param text ��������
    /// @param style �ı���ʽ
    TextActor(const String& text, const TextStyle& style);

    virtual ~TextActor();

    /// \~chinese
    /// @brief ��ȡ�ı�
    const String& GetText() const;

    /// \~chinese
    /// @brief ��ȡ�ı���ʽ
    const TextStyle& GetStyle() const;

    /// \~chinese
    /// @brief ��ȡ�ı�����
    const TextLayout& GetLayout() const;

    /// \~chinese
    /// @brief ��ȡ�ı����ִ�С
    Size GetLayoutSize() const;

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief ��ȡ��߻�ˢ
    BrushPtr GetOutlineBrush() const;

    /// \~chinese
    /// @brief ��ȡ����
    FontPtr GetFont() const;

    /// \~chinese
    /// @brief �����ı�
    void SetText(String const& text);

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
    /// @brief �����ı��Զ����еĿ�ȣ�Ĭ��Ϊ 0��
    void SetWrapWidth(float wrap_width);

    /// \~chinese
    /// @brief �����м�ࣨĬ��Ϊ 0��
    void SetLineSpacing(float line_spacing);

    /// \~chinese
    /// @brief ���ö��뷽ʽ��Ĭ��Ϊ TextAlign::Left��
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
    void SetOutlineStroke(const StrokeStyle& outline_stroke);

    /// \~chinese
    /// @brief �����Ƿ���ʾ�»��ߣ�Ĭ��ֵΪ false��
    void SetUnderline(bool enable);

    /// \~chinese
    /// @brief �����Ƿ���ʾɾ���ߣ�Ĭ��ֵΪ false��
    void SetStrikethrough(bool enable);

    /// \~chinese
    /// @brief �������ֲ���
    /// @details ���ֲ����������µģ��ֶ��������ֲ����Ը��½ڵ�״̬
    void UpdateLayout();

    void OnRender(RenderContext& ctx) override;

    void OnUpdate(Duration dt) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    bool       show_underline_;
    bool       show_strikethrough_;
    TextLayout text_layout_;
};

/** @} */

inline const String& TextActor::GetText() const
{
    return text_layout_.GetText();
}

inline FontPtr TextActor::GetFont() const
{
    return text_layout_.GetStyle().font;
}

inline const TextStyle& TextActor::GetStyle() const
{
    return text_layout_.GetStyle();
}

inline const TextLayout& TextActor::GetLayout() const
{
    return text_layout_;
}

inline Size TextActor::GetLayoutSize() const
{
    return text_layout_.GetLayoutSize();
}

inline BrushPtr TextActor::GetFillBrush() const
{
    return text_layout_.GetFillBrush();
}

inline BrushPtr TextActor::GetOutlineBrush() const
{
    return text_layout_.GetOutlineBrush();
}

inline void TextActor::SetText(String const& text)
{
    text_layout_.SetText(text);
}

inline void TextActor::SetStyle(const TextStyle& style)
{
    text_layout_.SetStyle(style);
}

inline void TextActor::SetFont(FontPtr font)
{
    text_layout_.SetFont(font);
}

inline void TextActor::SetFontFamily(String const& family)
{
    text_layout_.SetFontFamily(family);
}

inline void TextActor::SetFontSize(float size)
{
    text_layout_.SetFontSize(size);
}

inline void TextActor::SetFontWeight(uint32_t weight)
{
    text_layout_.SetFontWeight(weight);
}

inline void TextActor::SetItalic(bool italic)
{
    text_layout_.SetItalic(italic);
}

inline void TextActor::SetWrapWidth(float wrap_width)
{
    text_layout_.SetWrapWidth(wrap_width);
}

inline void TextActor::SetLineSpacing(float line_spacing)
{
    text_layout_.SetLineSpacing(line_spacing);
}

inline void TextActor::SetAlignment(TextAlign align)
{
    text_layout_.SetAlignment(align);
}

inline void TextActor::SetUnderline(bool enable)
{
    show_underline_ = enable;
}

inline void TextActor::SetStrikethrough(bool enable)
{
    show_strikethrough_ = enable;
}

inline void TextActor::SetFillBrush(BrushPtr brush)
{
    text_layout_.SetFillBrush(brush);
}

inline void TextActor::SetOutlineBrush(BrushPtr brush)
{
    text_layout_.SetOutlineBrush(brush);
}

inline void TextActor::SetOutlineWidth(float outline_width)
{
    text_layout_.SetOutlineWidth(outline_width);
}

inline void TextActor::SetOutlineStroke(const StrokeStyle& outline_stroke)
{
    text_layout_.SetOutlineStroke(outline_stroke);
}
}  // namespace kiwano
