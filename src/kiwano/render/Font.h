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
 * @brief 字体粗细值
 */
struct FontWeight
{
    enum Value : uint32_t
    {
        Thin       = 100U,
        ExtraLight = 200U,
        Light      = 300U,
        Normal     = 400U,  ///< 正常
        Medium     = 500U,
        Bold       = 700U,  ///< 加粗
        ExtraBold  = 800U,
        Black      = 900U,
        ExtraBlack = 950U
    };
};

/**
 * \~chinese
 * @brief 字体形态
 */
enum class FontPosture
{
    Normal,   ///< 正常
    Oblique,  ///< 倾斜体
    Italic,   ///< 斜体
};

/**
 * \~chinese
 * @brief 字体拉伸
 */
enum class FontStretch
{
    Unknown,
    UltraCondensed,
    ExtraCondensed,
    Condensed,  ///< 压缩
    SemiCondensed,
    Normal,  ///< 正常
    SemiExpanded,
    Expanded,  ///< 扩大
    ExtraExpanded,
    UltraExpanded,
};

/**
 * \~chinese
 * @brief 字体集合
 */
class KGE_API FontCollection : public NativeObject
{
public:
    /// \~chinese
    /// @brief 预加载字体集合
    /// @param files 字体文件列表
    static RefPtr<FontCollection> Preload(const Vector<String>& files);

    /// \~chinese
    /// @brief 预加载字体集合
    /// @param resources 字体资源列表
    static RefPtr<FontCollection> Preload(const Vector<Resource>& resources);

    const Vector<String>& GetFamilyNames() const;

protected:
    Vector<String> family_names_;
};

/**
 * \~chinese
 * @brief 字体
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
    /// @brief 创建字体，根据字体族名称自动搜索已预加载的 FontCollection 缓存，查找失败时使用系统字体
    /// @param family_name 字体族
    /// @param size 字号
    /// @param weight 字体粗细
    /// @param posture 字体形态
    Font(StringView family_name, float size, uint32_t weight = FontWeight::Normal,
         FontPosture posture = FontPosture::Normal, FontStretch stretch = FontStretch::Normal);

    /// \~chinese
    /// @brief 创建字体，使用字体集合中的第一个字体族名称
    /// @param collection 字体集合
    /// @param size 字号
    /// @param weight 字体粗细
    /// @param posture 字体形态
    Font(RefPtr<FontCollection> collection, float size, uint32_t weight = FontWeight::Normal,
         FontPosture posture = FontPosture::Normal, FontStretch stretch = FontStretch::Normal);
};

/**
 * \~chinese
 * @brief 字体缓存
 */
class KGE_API FontCache final : public Singleton<FontCache>
{
    friend Singleton<FontCache>;

public:
    /// \~chinese
    /// @brief 添加字体集合缓存
    void AddFontCollection(RefPtr<FontCollection> collection);

    /// \~chinese
    /// @brief 添加字体集合缓存
    void AddFontCollection(StringView font_family, RefPtr<FontCollection> collection);

    /// \~chinese
    /// @brief 获取字体集合缓存
    RefPtr<FontCollection> GetFontCollection(StringView font_family) const;

    /// \~chinese
    /// @brief 移除字体集合缓存
    void RemoveFontCollection(StringView font_family);

    /// \~chinese
    /// @brief 清空缓存
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
