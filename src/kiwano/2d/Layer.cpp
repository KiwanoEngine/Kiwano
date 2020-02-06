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
#include <kiwano/2d/Layer.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

LayerPtr Layer::Create()
{
    LayerPtr ptr = new (std::nothrow) Layer;
    return ptr;
}

Layer::Layer()
    : swallow_(false)
{
}

Layer::~Layer() {}

void Layer::SetClipRect(Rect const& clip_rect)
{
    area_.SetAreaRect(clip_rect);
}

void Layer::SetOpacity(float opacity)
{
    // Actor::SetOpacity(opacity);
    area_.SetOpacity(opacity);
}

void Layer::SetMaskShape(ShapePtr mask)
{
    area_.SetMaskShape(mask);
}

void Layer::SetMaskTransform(Matrix3x2 const& transform)
{
    area_.SetMaskTransform(transform);
}

bool Layer::DispatchEvent(Event* evt)
{
    if (!IsVisible())
        return true;

    if (swallow_)
    {
        return EventDispatcher::DispatchEvent(evt);
    }
    return Actor::DispatchEvent(evt);
}

void Layer::Render(RenderContext& ctx)
{
    ctx.PushLayer(area_);

    Actor::Render(ctx);

    ctx.PopLayer();
}

bool Layer::CheckVisibility(RenderContext& ctx) const
{
    // Do not need to render Layer
    return false;
}

}  // namespace kiwano
