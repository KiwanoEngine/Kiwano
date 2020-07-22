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
#include <kiwano/2d/LayerActor.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

LayerActorPtr LayerActor::Create()
{
    LayerActorPtr ptr = new (autogc) LayerActor;
    return ptr;
}

LayerActor::LayerActor()
    : swallow_(false)
{
}

LayerActor::~LayerActor() {}

void LayerActor::SetOpacity(float opacity)
{
    if (layer_)
    {
        layer_->SetOpacity(opacity);
    }
    else
    {
        Actor::SetOpacity(opacity);
    }
}

bool LayerActor::DispatchEvent(Event* evt)
{
    if (!IsVisible())
        return true;

    if (swallow_)
    {
        return EventDispatcher::DispatchEvent(evt);
    }
    return Actor::DispatchEvent(evt);
}

void LayerActor::Render(RenderContext& ctx)
{
    if (layer_)
    {
        ctx.PushLayer(*layer_);
        Actor::Render(ctx);
        ctx.PopLayer();
    }
    else
    {
        Actor::Render(ctx);
    }
}

bool LayerActor::CheckVisibility(RenderContext& ctx) const
{
    // Do not need to render LayerActor
    return false;
}

}  // namespace kiwano
