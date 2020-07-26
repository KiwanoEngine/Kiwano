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
#include <kiwano/utils/ResourceCache.h>
#include <kiwano/utils/Json.h>
#include <kiwano/utils/Xml.h>

namespace kiwano
{

/// \~chinese
/// @brief 资源加载器
class KGE_API ResourceLoader final : Noncopyable
{
public:
    /// \~chinese
    /// @brief 从 JSON 文件加载资源信息
    /// @param file_path JSON文件路径
    static bool LoadFromJsonFile(ResourceCachePtr cache, const String& file_path);

    /// \~chinese
    /// @brief 从 JSON 加载资源信息
    /// @param json_data JSON对象
    static bool LoadFromJson(ResourceCachePtr cache, const Json& json_data);

    /// \~chinese
    /// @brief 从 XML 文件加载资源信息
    /// @param file_path XML文件路径
    static bool LoadFromXmlFile(ResourceCachePtr cache, const String& file_path);

    /// \~chinese
    /// @brief 从 XML 文档对象加载资源信息
    /// @param doc XML文档对象
    static bool LoadFromXml(ResourceCachePtr cache, const XmlDocument& doc);
};

}  // namespace kiwano
