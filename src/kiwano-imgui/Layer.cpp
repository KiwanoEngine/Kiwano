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

#include <kiwano-imgui/Layer.h>

namespace kiwano
{
namespace imgui
{

Layer::Layer()
    : LayerActor(nullptr)
{
}

Layer::Layer(StringView name, const Pipeline& item)
    : Layer()
{
    AddItem(name, item);
}

Layer::~Layer() {}

void Layer::OnRender(RenderContext& ctx)
{
    for (const auto& pipeline : pipelines_)
    {
        pipeline.second();
    }
}

bool Layer::CheckVisibility(RenderContext& ctx) const
{
    return true;
}

void Layer::AddItem(StringView name, const Pipeline& item)
{
    pipelines_[name] = item;
}

void Layer::RemoveItem(StringView name)
{
    auto iter = pipelines_.find(name);
    if (iter != pipelines_.end())
    {
        pipelines_.erase(iter);
    }
}

void Layer::RemoveAllItems()
{
    pipelines_.clear();
}
}  // namespace imgui
}  // namespace kiwano
