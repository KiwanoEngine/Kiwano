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

#include <kiwano/base/component/ComponentManager.h>
#include <functional>

namespace kiwano
{

ComponentManager::ComponentManager(Actor* target)
    : target_(target)
{
}

Component* ComponentManager::AddComponent(RefPtr<Component> component)
{
    KGE_ASSERT(component && "AddComponent failed, NULL pointer exception");

    if (component)
    {
        size_t hash = std::hash<String>{}(component->GetName());
        AddComponent(hash, component);
    }
    return component.Get();
}

Component* ComponentManager::AddComponent(size_t index, RefPtr<Component> component)
{
    KGE_ASSERT(component && "AddComponent failed, NULL pointer exception");

    if (component)
    {
        component->InitComponent(target_);

        components_[index] = component;
    }
    return component.Get();
}

Component* ComponentManager::GetComponent(StringView name)
{
    size_t hash = std::hash<String>{}(name);
    return GetComponent(hash);
}

Component* ComponentManager::GetComponent(size_t name_hash)
{
    if (!components_.empty())
    {
        auto iter = components_.find(name_hash);
        if (iter != components_.end())
        {
            return iter->second.Get();
        }
    }
    return nullptr;
}

ComponentMap& ComponentManager::GetAllComponents()
{
    return components_;
}

const ComponentMap& ComponentManager::GetAllComponents() const
{
    return components_;
}

void ComponentManager::RemoveComponent(RefPtr<Component> component)
{
    RemoveComponent(component->GetName());
}

void ComponentManager::RemoveComponent(StringView name)
{
    size_t hash = std::hash<String>{}(name);
    RemoveComponent(hash);
}

void ComponentManager::RemoveComponent(size_t name_hash)
{
    if (!components_.empty())
    {
        auto iter = components_.find(name_hash);
        if (iter != components_.end())
        {
            iter->second->DestroyComponent();
            components_.erase(iter);
        }
    }
}

void ComponentManager::RemoveAllComponents()
{
    // Destroy all components
    if (!components_.empty())
    {
        for (auto& p : components_)
        {
            p.second->DestroyComponent();
        }
    }
    components_.clear();
}

void ComponentManager::Update(Duration dt)
{
    if (!components_.empty())
    {
        for (auto& p : components_)
        {
            if (p.second->IsEnable())
            {
                p.second->OnUpdate(dt);
            }
        }
    }
}

void ComponentManager::Render(RenderContext& ctx)
{
    if (!components_.empty())
    {
        for (auto& p : components_)
        {
            if (p.second->IsEnable())
            {
                p.second->OnRender(ctx);
            }
        }
    }
}

void ComponentManager::DispatchToComponents(Event* evt)
{
    if (!components_.empty())
    {
        for (auto& p : components_)
        {
            if (p.second->IsEnable())
            {
                p.second->HandleEvent(evt);
            }
        }
    }
}

}  // namespace kiwano
