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
#include <kiwano/platform/NativeObject.hpp>
#include <kiwano/render/TextStyle.h>

namespace kiwano
{

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
    TextLayout(StringView content, const TextStyle& style);

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
    void Reset(StringView content, const TextStyle& style);

    /// \~chinese
    /// @brief ��ȡ�ı����ִ�С
    Size GetSize() const;

    /// \~chinese
    /// @brief ��ȡ�ı�����
    uint32_t GetLineCount() const;

    /// \~chinese
    /// @brief ��ȡ�ı�����
    uint32_t GetContentLength() const;

    /// \~chinese
    /// @brief ��������
    /// @param font ����
    void SetFont(const Font& font);

    /// \~chinese
    /// @brief �����»���
    /// @param enable �Ƿ���ʾ�»���
    void SetUnderline(bool enable);

    /// \~chinese
    /// @brief ����ɾ����
    /// @param enable �Ƿ���ʾɾ����
    void SetStrikethrough(bool enable);

    /// \~chinese
    /// @brief ���ö��뷽ʽ
    /// @param align ���뷽ʽ
    void SetAlignment(TextAlign align);

    /// \~chinese
    /// @brief �����ı��Զ����еĿ��
    void SetWrapWidth(float wrap_width);

    /// \~chinese
    /// @brief �����ı��Զ����е�ģʽ
    void SetWordWrapping(TextWordWrapping wrap_width);

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
    DirtyFlag dirty_flag_;
    uint32_t  line_count_;
    uint32_t  content_length_;
    Size      size_;
};

/** @} */

inline bool TextLayout::IsDirty() const
{
    return dirty_flag_ != DirtyFlag::Clean;
}

inline uint32_t TextLayout::GetContentLength() const
{
    return content_length_;
}

inline TextLayout::DirtyFlag TextLayout::GetDirtyFlag() const
{
    return dirty_flag_;
}

inline void TextLayout::SetDirtyFlag(TextLayout::DirtyFlag flag)
{
    dirty_flag_ = flag;
}

}  // namespace kiwano
