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

namespace kiwano
{

ComponentManager::ComponentManager(Actor* target)
    : target_(target)
{
}

Component* ComponentManager::AddComponent(ComponentPtr component)
{
    KGE_ASSERT(component && "AddComponent failed, NULL pointer exception");

    if (component)
    {
        component->InitComponent(target_);
        components_.PushBack(component);
    }
    return component.Get();
}

ComponentList& ComponentManager::GetAllComponents()
{
    return components_;
}

const ComponentList& ComponentManager::GetAllComponents() const
{
    return components_;
}

void ComponentManager::RemoveComponent(ComponentPtr component)
{
    auto iter = std::find(components_.begin(), components_.end(), component);
    if (iter != components_.end())
    {
        component->DestroyComponent();
        components_.Remove(component);
    }
}

void ComponentManager::RemoveComponents(const String& name)
{
    if (!components_.IsEmpty())
    {
        ComponentPtr next;
        for (auto component = components_.GetFirst(); component; component = next)
        {
            next = component->GetNext();

            if (component->IsName(name))
            {
                component->DestroyComponent();
                components_.Remove(component);
            }
        }
    }
}

void ComponentManager::RemoveAllComponents()
{
    // Destroy all components
    if (!components_.IsEmpty())
    {
        ComponentPtr next;
        for (auto component = components_.GetFirst(); component; component = next)
        {
            next = component->GetNext();

            component->DestroyComponent();
        }
    }
    components_.Clear();
}

void ComponentManager::Update(Duration dt)
{
    if (!components_.IsEmpty())
    {
        ComponentPtr next;
        for (auto component = components_.GetFirst(); component; component = next)
        {
            next = component->GetNext();

            if (component->IsEnable())
            {
                component->OnUpdate(dt);
            }
        }
    }
}

void ComponentManager::Render(RenderContext& ctx)
{
    if (!components_.IsEmpty())
    {
        ComponentPtr next;
        for (auto component = components_.GetFirst(); component; component = next)
        {
            next = component->GetNext();

            if (component->IsEnable())
            {
                component->OnRender(ctx);
            }
        }
    }
}

void ComponentManager::DispatchToComponents(Event* evt)
{
    if (!components_.IsEmpty())
    {
        ComponentPtr next;
        for (auto component = components_.GetFirst(); component; component = next)
        {
            next = component->GetNext();

            if (component->IsEnable())
            {
                component->HandleEvent(evt);
            }
        }
    }
}

}  // namespace kiwano
