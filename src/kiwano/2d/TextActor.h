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
    TextActor();

    /// \~chinese
    /// @brief �����ı���ɫ
    /// @param text ��������
    TextActor(const String& text);

    /// \~chinese
    /// @brief �����ı���ɫ
    /// @param text ��������
    /// @param style �ı���ʽ
    TextActor(const String& text, const TextStyle& style);

    virtual ~TextActor();

    /// \~chinese
    /// @brief ��ȡ�ı�
    String GetText() const;

    /// \~chinese
    /// @brief ��ȡ�ı���ʽ
    TextStyle GetStyle() const;

    /// \~chinese
    /// @brief ��ȡ�ı�����
    TextLayoutPtr GetLayout() const;

    /// \~chinese
    /// @brief ��ȡ��С
    Size GetSize() const override;

    /// \~chinese
    /// @brief ��ȡ��仭ˢ
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief ��ȡ��߻�ˢ
    BrushPtr GetOutlineBrush() const;

    /// \~chinese
    /// @brief ��ȡ���������ʽ
    StrokeStylePtr GetOutlineStrokeStyle() const;

    /// \~chinese
    /// @brief ��ȡ����
    FontPtr GetFont() const;

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
    /// @brief ����������仭ˢ
    void SetFillBrush(BrushPtr brush);

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
    void SetOutlineBrush(BrushPtr brush);

    /// \~chinese
    /// @brief �������������ɫ
    void SetOutlineColor(const Color& outline_color);

    /// \~chinese
    /// @brief �������������ʽ
    void SetOutlineStrokeStyle(StrokeStylePtr stroke);

    /// \~chinese
    /// @brief �����Ƿ���ʾ�»��ߣ�Ĭ��ֵΪ false��
    void SetUnderline(bool enable);

    /// \~chinese
    /// @brief �����Ƿ���ʾɾ���ߣ�Ĭ��ֵΪ false��
    void SetStrikethrough(bool enable);

    /// \~chinese
    /// @brief �����ı�����
    void SetTextLayout(TextLayoutPtr layout);

    /// \~chinese
    /// @brief ���������ֲ���
    /// @details �������ֲ�����ʱ����
    void UpdateDirtyLayout();

    /// \~chinese
    /// @brief ǿ�Ƹ������ֲ���
    /// @details ���ֲ����������µģ��ֶ��������ֲ����Ը��½ڵ�״̬
    void ForceUpdateLayout();

    void OnRender(RenderContext& ctx) override;

protected:
    void Update(Duration dt) override;

    bool CheckVisibility(RenderContext& ctx) const override;

private:
    String        content_;
    TextStyle     style_;
    TextLayoutPtr layout_;
};

/** @} */

inline String TextActor::GetText() const
{
    return content_;
}

inline FontPtr TextActor::GetFont() const
{
    return style_.font;
}

inline TextStyle TextActor::GetStyle() const
{
    return style_;
}

inline TextLayoutPtr TextActor::GetLayout() const
{
    return layout_;
}

inline BrushPtr TextActor::GetFillBrush() const
{
    return style_.fill_brush;
}

inline BrushPtr TextActor::GetOutlineBrush() const
{
    return style_.outline_brush;
}

inline StrokeStylePtr TextActor::GetOutlineStrokeStyle() const
{
    return style_.outline_stroke;
}

}  // namespace kiwano
