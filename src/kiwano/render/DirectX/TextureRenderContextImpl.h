// Copyright (c) 2016-2019 Kiwano - Nomango
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
#include <kiwano/render/TextureRenderContext.h>
#include <kiwano/render/DirectX/RenderContextImpl.h>

namespace kiwano
{

class RendererImpl;

KGE_DECLARE_SMART_PTR(TextureRenderContextImpl);

KGE_SUPPRESS_WARNING_PUSH
KGE_SUPPRESS_WARNING(4250)  // inherits via domainance

class KGE_API TextureRenderContextImpl
    : public RenderContextImpl
    , public TextureRenderContext
{
    friend class RendererImpl;

public:
    bool GetOutput(Texture& texture) override;

    using RenderContextImpl::IsValid;

    using RenderContextImpl::BeginDraw;

    using RenderContextImpl::EndDraw;

    using RenderContextImpl::DrawTexture;

    using RenderContextImpl::DrawTextLayout;

    using RenderContextImpl::DrawShape;

    using RenderContextImpl::DrawLine;

    using RenderContextImpl::DrawRectangle;

    using RenderContextImpl::DrawRoundedRectangle;

    using RenderContextImpl::DrawEllipse;

    using RenderContextImpl::FillShape;

    using RenderContextImpl::FillRectangle;

    using RenderContextImpl::FillRoundedRectangle;

    using RenderContextImpl::FillEllipse;

    using RenderContextImpl::CreateTexture;

    using RenderContextImpl::PushClipRect;

    using RenderContextImpl::PopClipRect;

    using RenderContextImpl::PushLayer;

    using RenderContextImpl::PopLayer;

    using RenderContextImpl::Clear;

    using RenderContextImpl::GetSize;

    using RenderContextImpl::SetCurrentBrush;

    using RenderContextImpl::SetTransform;

    using RenderContextImpl::SetAntialiasMode;

    using RenderContextImpl::SetTextAntialiasMode;

    using RenderContextImpl::CheckVisibility;

    using RenderContextImpl::Resize;

private:
    ComPtr<ID2D1BitmapRenderTarget> bitmap_rt_;
};

KGE_SUPPRESS_WARNING_POP

}  // namespace kiwano
