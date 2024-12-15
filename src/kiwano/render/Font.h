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
#include <kiwano/core/Resource.h>
#include <kiwano/platform/NativeObject.hpp>

namespace kiwano
{

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
    Condensed,  ///< ѹ��
    SemiCondensed,
    Normal,  ///< ����
    SemiExpanded,
    Expanded,  ///< ����
    ExtraExpanded,
    UltraExpanded,
};

/**
 * \~chinese
 * @brief ���弯��
 */
class KGE_API FontCollection : public NativeObject
{
public:
    /// \~chinese
    /// @brief Ԥ�������弯��
    /// @param files �����ļ��б�
    static RefPtr<FontCollection> Preload(const Vector<String>& files);

    /// \~chinese
    /// @brief Ԥ�������弯��
    /// @param resources ������Դ�б�
    static RefPtr<FontCollection> Preload(const Vector<Resource>& resources);

    const Vector<String>& GetFamilyNames() const;

protected:
    Vector<String> family_names_;
};

/**
 * \~chinese
 * @brief ����
 */
class KGE_API Font
{
public:
    float                  size;
    uint32_t               weight;
    FontPosture            posture;
    FontStretch            stretch;
    String                 family_name;
    RefPtr<FontCollection> collection;

    /// \~chinese
    /// @brief �������壬���������������Զ�������Ԥ���ص� FontCollection ���棬����ʧ��ʱʹ��ϵͳ����
    /// @param family_name ������
    /// @param size �ֺ�
    /// @param weight �����ϸ
    /// @param posture ������̬
    Font(StringView family_name, float size, uint32_t weight = FontWeight::Normal,
         FontPosture posture = FontPosture::Normal, FontStretch stretch = FontStretch::Normal);

    /// \~chinese
    /// @brief �������壬ʹ�����弯���еĵ�һ������������
    /// @param collection ���弯��
    /// @param size �ֺ�
    /// @param weight �����ϸ
    /// @param posture ������̬
    Font(RefPtr<FontCollection> collection, float size, uint32_t weight = FontWeight::Normal,
         FontPosture posture = FontPosture::Normal, FontStretch stretch = FontStretch::Normal);
};

/**
 * \~chinese
 * @brief ���建��
 */
class KGE_API FontCache final : public Singleton<FontCache>
{
    friend Singleton<FontCache>;

public:
    /// \~chinese
    /// @brief ������弯�ϻ���
    void AddFontCollection(RefPtr<FontCollection> collection);

    /// \~chinese
    /// @brief ������弯�ϻ���
    void AddFontCollection(StringView font_family, RefPtr<FontCollection> collection);

    /// \~chinese
    /// @brief ��ȡ���弯�ϻ���
    RefPtr<FontCollection> GetFontCollection(StringView font_family) const;

    /// \~chinese
    /// @brief �Ƴ����弯�ϻ���
    void RemoveFontCollection(StringView font_family);

    /// \~chinese
    /// @brief ��ջ���
    void Clear();

    ~FontCache();

private:
    FontCache();

    String TransformFamily(String family) const;

private:
    UnorderedMap<String, RefPtr<FontCollection>> collection_cache_;
};

/** @} */

inline const Vector<String>& FontCollection::GetFamilyNames() const
{
    return family_names_;
}

}  // namespace kiwano
