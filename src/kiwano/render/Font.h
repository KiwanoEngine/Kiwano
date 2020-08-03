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
 * @brief 字体
 */
class Font : public NativeObject
{
public:
    /// \~chinese
    /// @brief 创建字体
    /// @param file 字体文件
    static FontPtr Preload(const String& file);

    /// \~chinese
    /// @brief 创建字体
    /// @param resource 字体资源
    static FontPtr Preload(const Resource& resource);

    Font();

    /// \~chinese
    /// @brief 创建字体
    /// @param family_name 字体族
    Font(const String& family_name);

    /// \~chinese
    /// @brief 获取字体族
    String GetFamilyName() const;

    /// \~chinese
    /// @brief 获取字体族
    void SetFamilyName(const String& name);

protected:
    String family_name_;
};

/** @} */

inline String Font::GetFamilyName() const
{
    return family_name_;
}

inline void Font::SetFamilyName(const String& name)
{
    family_name_ = name;
}

}  // namespace kiwano
