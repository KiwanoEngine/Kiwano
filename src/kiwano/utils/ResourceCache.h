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
#include <kiwano/base/ObjectBase.h>

namespace kiwano
{

/// \~chinese
/// @brief 资源缓存
class KGE_API ResourceCache final : public ObjectBase
{
public:
    ResourceCache();

    virtual ~ResourceCache();

    /// \~chinese
    /// @brief 从 JSON 文件加载资源信息
    /// @param file_path JSON文件路径
    bool LoadFromJsonFile(StringView file_path);

    /// \~chinese
    /// @brief 从 XML 文件加载资源信息
    /// @param file_path XML文件路径
    bool LoadFromXmlFile(StringView file_path);

    /// \~chinese
    /// @brief 获取资源
    /// @param id 对象ID
    RefPtr<ObjectBase> Get(StringView id) const;

    /// \~chinese
    /// @brief 获取资源
    /// @tparam _Ty 对象类型
    /// @param id 对象ID
    /// @return 指定对象类型的指针
    template <typename _Ty>
    RefPtr<_Ty> Get(StringView id) const
    {
        return dynamic_cast<_Ty*>(Get(id).Get());
    }

    /// \~chinese
    /// @brief 将对象放入缓存
    /// @param id 对象ID
    /// @param obj 对象
    void AddObject(StringView id, RefPtr<ObjectBase> obj);

    /// \~chinese
    /// @brief 删除指定资源
    /// @param id 对象ID
    void Remove(StringView id);

    /// \~chinese
    /// @brief 清空所有资源
    void Clear();

private:
    UnorderedMap<String, RefPtr<ObjectBase>> object_cache_;
};

}  // namespace kiwano
