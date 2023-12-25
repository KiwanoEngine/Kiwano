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
#include <kiwano/core/IntrusiveList.h>
#include <kiwano/base/component/Component.h>

namespace kiwano
{

/**
 * \addtogroup Component
 * @{
 */

/// \~chinese
/// @brief 组件映射
typedef UnorderedMap<size_t, ComponentPtr> ComponentMap;

/**
 * \~chinese
 * @brief 组件管理器
 */
class KGE_API ComponentManager
{
public:
    /// \~chinese
    /// @brief 添加组件
    /// @param component 组件
    Component* AddComponent(ComponentPtr component);

    /// \~chinese
    /// @brief 添加组件
    /// @param index 索引值
    /// @param component 组件
    Component* AddComponent(size_t index, ComponentPtr component);

    /// \~chinese
    /// @brief 获取组件
    Component* GetComponent(StringView name);

    /// \~chinese
    /// @brief 获取组件
    Component* GetComponent(size_t name_hash);

    /// \~chinese
    /// @brief 获取所有组件
    ComponentMap& GetAllComponents();

    /// \~chinese
    /// @brief 获取所有组件
    const ComponentMap& GetAllComponents() const;

    /// \~chinese
    /// @brief 移除组件
    void RemoveComponent(ComponentPtr component);

    /// \~chinese
    /// @brief 移除组件
    /// @param name 组件名称
    void RemoveComponent(StringView name);

    /// \~chinese
    /// @brief 移除组件
    /// @param name_hash 组件名称hash值
    void RemoveComponent(size_t name_hash);

    /// \~chinese
    /// @brief 移除所有组件
    void RemoveAllComponents();

    /// \~chinese
    /// @brief 更新组件
    void Update(Duration dt);

    /// \~chinese
    /// @brief 渲染组件
    void Render(RenderContext& ctx);

    /// \~chinese
    /// @brief 分发事件
    void DispatchToComponents(Event* evt);

protected:
    ComponentManager(Actor* target);

private:
    Actor*       target_;
    ComponentMap components_;
};

/** @} */

}  // namespace kiwano
