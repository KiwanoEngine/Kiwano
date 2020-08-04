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
    Condensed,       ///< 压缩
    SemiCondensed,
    Normal,          ///< 正常
    SemiExpanded,
    Expanded,        ///< 扩大
    ExtraExpanded,
    UltraExpanded,
};

/**
 * \~chinese
 * @brief 字体
 */
class Font : public NativeObject
{
public:
    /// \~chinese
    /// @brief 预加载字体
    /// @param file 字体文件
    static FontPtr Preload(const String& file);

    /// \~chinese
    /// @brief 预加载字体
    /// @param resource 字体资源
    static FontPtr Preload(const Resource& resource);

    /// \~chinese
    /// @brief 创建系统默认字体
    Font();

    /// \~chinese
    /// @brief 通过字体族创建字体
    /// @param family_name 字体族
    /// @param size 字号
    /// @param weight 字体粗细
    /// @param posture 字体形态
    Font(const String& family_name, float size, uint32_t weight = FontWeight::Normal,
         FontPosture posture = FontPosture::Normal, FontStretch stretch = FontStretch::Normal);

    /// \~chinese
    /// @brief 获取字体族
    String GetFamilyName() const;

    /// \~chinese
    /// @brief 获取字号
    float GetSize() const;

    /// \~chinese
    /// @brief 获取字体粗细值
    uint32_t GetWeight() const;

    /// \~chinese
    /// @brief 获取字体形态
    FontPosture GetPosture() const;

    /// \~chinese
    /// @brief 获取字体拉伸
    FontStretch GetStretch() const;

protected:
    /// \~chinese
    /// @brief 获取字体族
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
 * @brief 纹理缓存
 */
class KGE_API FontCache final : public Singleton<FontCache>
{
    friend Singleton<FontCache>;

public:
    /// \~chinese
    /// @brief 添加字体缓存
    void AddFont(size_t key, FontPtr font);

    /// \~chinese
    /// @brief 添加字体族映射字体缓存
    void AddFontByFamily(const String& font_family, FontPtr font);

    /// \~chinese
    /// @brief 获取字体缓存
    FontPtr GetFont(size_t key) const;

    /// \~chinese
    /// @brief 获取字体族映射字体缓存
    FontPtr GetFontByFamily(const String& font_family) const;

    /// \~chinese
    /// @brief 移除字体缓存
    void RemoveFont(size_t key);

    /// \~chinese
    /// @brief 移除字体族映射字体缓存
    void RemoveFontByFamily(const String& font_family);

    /// \~chinese
    /// @brief 清空缓存
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
