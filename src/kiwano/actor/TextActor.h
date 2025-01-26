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
#include <kiwano/actor/Actor.h>
#include <kiwano/render/Color.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/ability/RenderAbility.h>

namespace kiwano
{

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
    TextActor();

    /// \~chinese
    /// @brief �����ı���ɫ
    /// @param text ��������
    /// @param style �ı���ʽ
    TextActor(StringView text, const TextStyle& style = TextStyle());

    virtual ~TextActor();

    /// \~chinese
    /// @brief ��ȡ�ı�
    StringView GetText() const;

    /// \~chinese
    /// @brief ��ȡ�ı���ʽ
    TextStyle GetStyle() const;

    /// \~chinese
    /// @brief ��ȡ�ı�����
    RefPtr<TextLayout> GetLayout() const;

    /// \~chinese
    /// @brief ��ȡ��С
    Size GetSize() const override;

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief ��ȡ��߻�ˢ
    RefPtr<Brush> GetOutlineBrush() const;

    /// \~chinese
    /// @brief ��ȡ���������ʽ
    RefPtr<StrokeStyle> GetOutlineStrokeStyle() const;

    /// \~chinese
    /// @brief ��ȡ����
    const Font& GetFont() const;

    /// \~chinese
    /// @brief �����ı�
    void SetText(StringView text);

    /// \~chinese
    /// @brief �����ı���ʽ
    void SetStyle(const TextStyle& style);

    /// \~chinese
    /// @brief ��������
    void SetFont(const Font& font);

    /// \~chinese
    /// @brief ����������仭ˢ
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief �������������ɫ��Ĭ��ֵΪ Color::White��
    void SetFillColor(const Color& color);

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
    void SetOutlineBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief �������������ɫ
    void SetOutlineColor(const Color& outline_color);

    /// \~chinese
    /// @brief �������������ʽ
    void SetOutlineStrokeStyle(RefPtr<StrokeStyle> stroke);

    /// \~chinese
    /// @brief �����Ƿ���ʾ�»��ߣ�Ĭ��ֵΪ false��
    void SetUnderline(bool enable);

    /// \~chinese
    /// @brief �����Ƿ���ʾɾ���ߣ�Ĭ��ֵΪ false��
    void SetStrikethrough(bool enable);

    /// \~chinese
    /// @brief �����ı�����
    void SetTextLayout(RefPtr<TextLayout> layout);

    /// \~chinese
    /// @brief ���������ֲ���
    /// @details �������ֲ�����ʱ����
    void UpdateDirtyLayout();

    /// \~chinese
    /// @brief ǿ�Ƹ������ֲ���
    /// @details ���ֲ����������µģ��ֶ��������ֲ����Ը��½ڵ�״̬
    void ForceUpdateLayout();

protected:
    void Update(Duration dt) override;

    void UpdateCachedTexture();

    void ResetLayout(StringView content, const TextStyle& style);

private:
    String    content_;
    TextStyle style_;

    RefPtr<TextRenderAbility> render_comp_;
};

/** @} */

inline StringView TextActor::GetText() const
{
    return content_;
}

inline const Font& TextActor::GetFont() const
{
    return style_.font;
}

inline TextStyle TextActor::GetStyle() const
{
    return style_;
}

inline RefPtr<TextLayout> TextActor::GetLayout() const
{
    return render_comp_->GetTextLayout();
}

inline RefPtr<Brush> TextActor::GetFillBrush() const
{
    return render_comp_->GetFillBrush();
}

inline RefPtr<Brush> TextActor::GetOutlineBrush() const
{
    return render_comp_->GetOutlineBrush();
}

inline RefPtr<StrokeStyle> TextActor::GetOutlineStrokeStyle() const
{
    return render_comp_->GetOutlineStrokeStyle();
}

}  // namespace kiwano
