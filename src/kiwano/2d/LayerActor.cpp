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

LayerActor::LayerActor(const Rect& bounds, float opacity)
    : swallow_(false)
    , layer_(bounds, opacity)
{
}

LayerActor::LayerActor(RefPtr<Shape> mask, const Matrix3x2& mask_transform, float opacity, const Rect& bounds)
    : swallow_(false)
    , layer_(mask, mask_transform, opacity, bounds)
{
}

LayerActor::~LayerActor() {}

void LayerActor::Update(Duration dt)
{
    layer_.transform = GetTransformMatrix(); // TODO: refactor
    Renderer::GetInstance().PushRenderGroup(layer_);

    Actor::Update(dt);

    Renderer::GetInstance().PopRenderGroup();
}

}  // namespace kiwano
