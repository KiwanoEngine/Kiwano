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
#include <kiwano/math/Math.h>
#include <kiwano/render/NativeObject.h>
#include <kiwano/render/TextStyle.hpp>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(TextLayout);

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief �ı�����
class KGE_API TextLayout : public NativeObject
{
public:
    /// \~chinese
    /// @brief �����ı�����
    static TextLayoutPtr Create();

    /// \~chinese
    /// @brief �����ı�����
    /// @param content ��������
    /// @param style �ı���ʽ
    static TextLayoutPtr Create(const String& content, const TextStyle& style);

    /// \~chinese
    /// @brief ����յ��ı�����
    TextLayout();

    /// \~chinese
    /// @brief �ı������Ƿ�¾�
    bool IsDirty() const;

    /// \~chinese
    /// @brief ����ı�����
    void Clear();

    /// \~chinese
    /// @brief �����ı�����
    /// @param style �ı���ʽ
    void Reset(const TextStyle& style);

    /// \~chinese
    /// @brief �����ı�����
    /// @param content ��������
    /// @param style �ı���ʽ
    void Reset(const String& content, const TextStyle& style);

    /// \~chinese
    /// @brief ��ȡ��������
    String GetContent() const;

    /// \~chinese
    /// @brief ��ȡ�������ݵĳ���
    uint32_t GetContentLength() const;

    /// \~chinese
    /// @brief ��ȡ�ı����ִ�С
    Size GetSize() const;

    /// \~chinese
    /// @brief ��ȡ�ı�����
    uint32_t GetLineCount() const;

    /// \~chinese
    /// @brief ��ȡĬ����仭ˢ
    BrushPtr GetDefaultFillBrush() const;

    /// \~chinese
    /// @brief ��ȡĬ����߻�ˢ
    BrushPtr GetDefaultOutlineBrush() const;

    /// \~chinese
    /// @brief ��ȡĬ�����������ʽ
    StrokeStylePtr GetDefaultOutlineStrokeStyle() const;

    /// \~chinese
    /// @brief ���ַ�Χ
    struct TextRange
    {
        uint32_t start;   ///< ��ʼλ��
        uint32_t length;  ///< ����
    };

    /// \~chinese
    /// @brief ��������
    /// @param font ����
    /// @param range ���ַ�Χ
    void SetFont(FontPtr font, TextRange range);

    /// \~chinese
    /// @brief ����������
    /// @param family ������
    /// @param range ���ַ�Χ
    void SetFontFamily(const String& family, TextRange range);

    /// \~chinese
    /// @brief �����ֺţ�Ĭ��ֵΪ 18��
    /// @param size �ֺ�
    /// @param range ���ַ�Χ
    void SetFontSize(float size, TextRange range);

    /// \~chinese
    /// @brief ���������ϸֵ��Ĭ��ֵΪ FontWeight::Normal��
    /// @param weight ��ϸֵ
    /// @param range ���ַ�Χ
    void SetFontWeight(uint32_t weight, TextRange range);

    /// \~chinese
    /// @brief ��������б�壨Ĭ��ֵΪ false��
    /// @param italic �Ƿ���б��
    /// @param range ���ַ�Χ
    void SetItalic(bool italic, TextRange range);

    /// \~chinese
    /// @brief �����»���
    /// @param enable �Ƿ���ʾ�»���
    /// @param range ���ַ�Χ
    void SetUnderline(bool enable, TextRange range);

    /// \~chinese
    /// @brief ����ɾ����
    /// @param enable �Ƿ���ʾɾ����
    /// @param range ���ַ�Χ
    void SetStrikethrough(bool enable, TextRange range);

    /// \~chinese
    /// @brief ����������仭ˢ����߻�ˢ������߿�
    /// @param brush ��ˢ
    /// @param range ���ַ�Χ
    void SetFillBrush(BrushPtr brush, TextRange range);

    /// \~chinese
    /// @brief ����������߻�ˢ
    /// @param brush ��ˢ
    /// @param range ���ַ�Χ
    void SetOutlineBrush(BrushPtr brush, TextRange range);

    /// \~chinese
    /// @brief �������������ʽ
    /// @param stroke ������ʽ
    /// @param range ���ַ�Χ
    void SetOutlineStrokeStyle(StrokeStylePtr stroke, TextRange range);

    /// \~chinese
    /// @brief ���ö��뷽ʽ
    /// @param align ���뷽ʽ
    void SetAlignment(TextAlign align);

    /// \~chinese
    /// @brief �����ı��Զ����еĿ��
    void SetWrapWidth(float wrap_width);

    /// \~chinese
    /// @brief �����м�ࣨĬ��Ϊ 0��
    void SetLineSpacing(float line_spacing);

    /// \~chinese
    /// @brief ����Ĭ��������仭ˢ
    /// @param brush ��ˢ
    void SetDefaultFillBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ����Ĭ��������߻�ˢ
    /// @param brush ��ˢ
    void SetDefaultOutlineBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ����Ĭ�����������ʽ
    /// @param stroke ������ʽ
    void SetDefaultOutlineStrokeStyle(StrokeStylePtr stroke);

    /// \~chinese
    /// @brief �಼�ֱ�־
    enum class DirtyFlag : uint8_t
    {
        Clean = 0,      ///< �ɾ�����
        Dirty = 1 << 0  ///< �಼��
    };

    /// \~chinese
    /// @brief ��ȡ�಼�ֱ�־
    DirtyFlag GetDirtyFlag() const;

    /// \~chinese
    /// @brief �����಼�ֱ�־
    void SetDirtyFlag(DirtyFlag flag);

    /// \~chinese
    /// @brief �����಼��
    /// @return �Ƿ���Ҫ����
    bool UpdateWhenDirty();

private:
    DirtyFlag      dirty_flag_;
    uint32_t       line_count_;
    Size           size_;
    BrushPtr       default_fill_brush_;
    BrushPtr       default_outline_brush_;
    StrokeStylePtr default_outline_stroke_;
    String         content_;
};

/** @} */

inline bool TextLayout::IsDirty() const
{
    return dirty_flag_ != DirtyFlag::Clean;
}

inline void TextLayout::Clear()
{
    ResetNativePointer();
}

inline void TextLayout::Reset(const TextStyle& style)
{
    this->Reset(content_, style);
}

inline String TextLayout::GetContent() const
{
    return content_;
}

inline uint32_t TextLayout::GetContentLength() const
{
    return uint32_t(content_.size());
}

inline TextLayout::DirtyFlag TextLayout::GetDirtyFlag() const
{
    return dirty_flag_;
}

inline void TextLayout::SetDirtyFlag(TextLayout::DirtyFlag flag)
{
    dirty_flag_ = flag;
}

inline BrushPtr TextLayout::GetDefaultFillBrush() const
{
    return default_fill_brush_;
}

inline BrushPtr TextLayout::GetDefaultOutlineBrush() const
{
    return default_outline_brush_;
}

inline StrokeStylePtr TextLayout::GetDefaultOutlineStrokeStyle() const
{
    return default_outline_stroke_;
}

inline void TextLayout::SetDefaultFillBrush(BrushPtr brush)
{
    default_fill_brush_ = brush;
}

inline void TextLayout::SetDefaultOutlineBrush(BrushPtr brush)
{
    default_outline_brush_ = brush;
}

inline void TextLayout::SetDefaultOutlineStrokeStyle(StrokeStylePtr stroke)
{
    default_outline_stroke_ = stroke;
}

}  // namespace kiwano
