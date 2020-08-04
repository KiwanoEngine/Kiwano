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
#include <kiwano/render/TextStyle.h>

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
    TextLayout();

    /// \~chinese
    /// @brief �����ı�����
    /// @param content ��������
    /// @param style �ı���ʽ
    TextLayout(const String& content, const TextStyle& style);

    /// \~chinese
    /// @brief �ı������Ƿ�¾�
    bool IsDirty() const;

    /// \~chinese
    /// @brief ����ı�����
    void Clear();

    /// \~chinese
    /// @brief �����ı�����
    /// @param content ��������
    /// @param style �ı���ʽ
    void Reset(const String& content, const TextStyle& style);

    /// \~chinese
    /// @brief ��ȡ�ı����ִ�С
    Size GetSize() const;

    /// \~chinese
    /// @brief ��ȡ�ı�����
    uint32_t GetLineCount() const;

    /// \~chinese
    /// @brief ��ȡĬ����仭ˢ
    BrushPtr GetFillBrush() const;

    /// \~chinese
    /// @brief ��ȡĬ����߻�ˢ
    BrushPtr GetOutlineBrush() const;

    /// \~chinese
    /// @brief ��ȡĬ�����������ʽ
    StrokeStylePtr GetOutlineStrokeStyle() const;

    /// \~chinese
    /// @brief ��������
    /// @param font ����
    void SetFont(FontPtr font);

    /// \~chinese
    /// @brief �����»���
    /// @param enable �Ƿ���ʾ�»���
    void SetUnderline(bool enable);

    /// \~chinese
    /// @brief ����ɾ����
    /// @param enable �Ƿ���ʾɾ����
    void SetStrikethrough(bool enable);

    /// \~chinese
    /// @brief ����������仭ˢ����߻�ˢ������߿�
    /// @param brush ��ˢ
    void SetFillBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ����������߻�ˢ
    /// @param brush ��ˢ
    void SetOutlineBrush(BrushPtr brush);

    /// \~chinese
    /// @brief �������������ʽ
    /// @param stroke ������ʽ
    void SetOutlineStrokeStyle(StrokeStylePtr stroke);

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
    bool UpdateIfDirty();

private:
    DirtyFlag      dirty_flag_;
    uint32_t       line_count_;
    uint32_t       content_length_;
    Size           size_;
    BrushPtr       fill_brush_;
    BrushPtr       outline_brush_;
    StrokeStylePtr outline_stroke_;
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

inline TextLayout::DirtyFlag TextLayout::GetDirtyFlag() const
{
    return dirty_flag_;
}

inline void TextLayout::SetDirtyFlag(TextLayout::DirtyFlag flag)
{
    dirty_flag_ = flag;
}

inline BrushPtr TextLayout::GetFillBrush() const
{
    return fill_brush_;
}

inline BrushPtr TextLayout::GetOutlineBrush() const
{
    return outline_brush_;
}

inline StrokeStylePtr TextLayout::GetOutlineStrokeStyle() const
{
    return outline_stroke_;
}

inline void TextLayout::SetFillBrush(BrushPtr brush)
{
    fill_brush_ = brush;
}

inline void TextLayout::SetOutlineBrush(BrushPtr brush)
{
    outline_brush_ = brush;
}

inline void TextLayout::SetOutlineStrokeStyle(StrokeStylePtr stroke)
{
    outline_stroke_ = stroke;
}

}  // namespace kiwano
