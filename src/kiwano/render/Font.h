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
#include <kiwano/render/NativeObject.h>
#include <kiwano/core/Resource.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(Font);

class Renderer;

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief �����ϸֵ
 */
struct FontWeight
{
    enum Value : uint32_t
    {
        Thin       = 100U,
        ExtraLight = 200U,
        Light      = 300U,
        Normal     = 400U,  ///< ����
        Medium     = 500U,
        Bold       = 700U,  ///< �Ӵ�
        ExtraBold  = 800U,
        Black      = 900U,
        ExtraBlack = 950U
    };
};

/**
 * \~chinese
 * @brief ������̬
 */
enum class FontPosture
{
    Normal,   ///< ����
    Oblique,  ///< ��б��
    Italic,   ///< б��
};

/**
 * \~chinese
 * @brief ��������
 */
enum class FontStretch
{
    Unknown,
    UltraCondensed,
    ExtraCondensed,
    Condensed,       ///< ѹ��
    SemiCondensed,
    Normal,          ///< ����
    SemiExpanded,
    Expanded,        ///< ����
    ExtraExpanded,
    UltraExpanded,
};

/**
 * \~chinese
 * @brief ����
 */
class Font : public NativeObject
{
public:
    /// \~chinese
    /// @brief Ԥ��������
    /// @param file �����ļ�
    static FontPtr Preload(const String& file);

    /// \~chinese
    /// @brief Ԥ��������
    /// @param resource ������Դ
    static FontPtr Preload(const Resource& resource);

    /// \~chinese
    /// @brief ����ϵͳĬ������
    Font();

    /// \~chinese
    /// @brief ͨ�������崴������
    /// @param family_name ������
    /// @param size �ֺ�
    /// @param weight �����ϸ
    /// @param posture ������̬
    Font(const String& family_name, float size, uint32_t weight = FontWeight::Normal,
         FontPosture posture = FontPosture::Normal, FontStretch stretch = FontStretch::Normal);

    /// \~chinese
    /// @brief ��ȡ������
    String GetFamilyName() const;

    /// \~chinese
    /// @brief ��ȡ�ֺ�
    float GetSize() const;

    /// \~chinese
    /// @brief ��ȡ�����ϸֵ
    uint32_t GetWeight() const;

    /// \~chinese
    /// @brief ��ȡ������̬
    FontPosture GetPosture() const;

    /// \~chinese
    /// @brief ��ȡ��������
    FontStretch GetStretch() const;

protected:
    /// \~chinese
    /// @brief ��ȡ������
    void SetFamilyName(const String& name);

protected:
    float       size_;
    uint32_t    weight_;
    FontPosture posture_;
    FontStretch stretch_;
    String      family_name_;
};

/**
 * \~chinese
 * @brief ������
 */
class KGE_API FontCache final : public Singleton<FontCache>
{
    friend Singleton<FontCache>;

public:
    /// \~chinese
    /// @brief ������建��
    void AddFont(size_t key, FontPtr font);

    /// \~chinese
    /// @brief ���������ӳ�����建��
    void AddFontByFamily(const String& font_family, FontPtr font);

    /// \~chinese
    /// @brief ��ȡ���建��
    FontPtr GetFont(size_t key) const;

    /// \~chinese
    /// @brief ��ȡ������ӳ�����建��
    FontPtr GetFontByFamily(const String& font_family) const;

    /// \~chinese
    /// @brief �Ƴ����建��
    void RemoveFont(size_t key);

    /// \~chinese
    /// @brief �Ƴ�������ӳ�����建��
    void RemoveFontByFamily(const String& font_family);

    /// \~chinese
    /// @brief ��ջ���
    void Clear();

    ~FontCache();

private:
    FontCache();

    String TransformFamily(String family) const;

private:
    using FontMap = UnorderedMap<size_t, FontPtr>;
    FontMap font_cache_;

    using FontFamilyMap = UnorderedMap<String, FontPtr>;
    FontFamilyMap font_family_cache_;
};

/** @} */

inline String Font::GetFamilyName() const
{
    return family_name_;
}

inline float Font::GetSize() const
{
    return size_;
}

inline uint32_t Font::GetWeight() const
{
    return weight_;
}

inline FontPosture Font::GetPosture() const
{
    return posture_;
}

inline FontStretch Font::GetStretch() const
{
    return stretch_;
}

inline void Font::SetFamilyName(const String& name)
{
    family_name_ = name;
}

}  // namespace kiwano
