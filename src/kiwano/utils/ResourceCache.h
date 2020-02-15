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
#include <kiwano/2d/Frame.h>
#include <kiwano/2d/FrameSequence.h>
#include <kiwano/core/Resource.h>
#include <kiwano/core/Json.h>
#include <kiwano/core/Xml.h>
#include <kiwano/render/Font.h>
#include <kiwano/render/GifImage.h>

namespace kiwano
{
/// \~chinese
/// @brief 资源缓存
/// @details 资源缓存
class KGE_API ResourceCache : public Singleton<ResourceCache>
{
    friend Singleton<ResourceCache>;

public:
    /// \~chinese
    /// @brief 从 JSON 文件加载资源信息
    /// @param file_path JSON文件路径
    bool LoadFromJsonFile(String const& file_path);

    /// \~chinese
    /// @brief 从 JSON 加载资源信息
    /// @param json_data JSON对象
    bool LoadFromJson(Json const& json_data);

    /// \~chinese
    /// @brief 从 XML 文件加载资源信息
    /// @param file_path XML文件路径
    bool LoadFromXmlFile(String const& file_path);

    /// \~chinese
    /// @brief 从 XML 文档对象加载资源信息
    /// @param doc XML文档对象
    bool LoadFromXml(XmlDocument const& doc);

    /// \~chinese
    /// @brief 获取资源
    /// @param id 对象ID
    ObjectBasePtr Get(String const& id) const;

    /// \~chinese
    /// @brief 获取资源
    /// @tparam _Ty 对象类型
    /// @param id 对象ID
    /// @return 指定对象类型的指针
    template <typename _Ty>
    SmartPtr<_Ty> Get(String const& id) const
    {
        return dynamic_cast<_Ty*>(Get(id).Get());
    }

    /// \~chinese
    /// @brief 将对象放入缓存
    /// @param id 对象ID
    /// @param obj 对象
    bool AddObject(String const& id, ObjectBasePtr obj);

    /// \~chinese
    /// @brief 删除指定资源
    /// @param id 对象ID
    void Remove(String const& id);

    /// \~chinese
    /// @brief 清空所有资源
    void Clear();

private:
    ResourceCache();

    virtual ~ResourceCache();

private:
    UnorderedMap<String, ObjectBasePtr> object_cache_;
};
}  // namespace kiwano
