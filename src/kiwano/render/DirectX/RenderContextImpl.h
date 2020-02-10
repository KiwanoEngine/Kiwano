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
#include <kiwano/render/RenderContext.h>

namespace kiwano
{

class RendererImpl;

KGE_DECLARE_SMART_PTR(RenderContextImpl);

class KGE_API RenderContextImpl : public virtual RenderContext
{
    friend class RendererImpl;

public:
    bool IsValid() const override;

    void BeginDraw() override;

    void EndDraw() override;

    void DrawTexture(Texture const& texture, const Rect* src_rect = nullptr, const Rect* dest_rect = nullptr) override;

    void DrawTextLayout(TextLayout const& layout, Point const& offset = Point()) override;

    void DrawShape(Shape const& shape, StrokeStylePtr stroke = nullptr, float stroke_width = 1.0f) override;

    void DrawLine(Point const& point1, Point const& point2, StrokeStylePtr stroke = nullptr,
                  float stroke_width = 1.0f) override;

    void DrawRectangle(Rect const& rect, StrokeStylePtr stroke = nullptr, float stroke_width = 1.0f) override;

    void DrawRoundedRectangle(Rect const& rect, Vec2 const& radius, StrokeStylePtr stroke = nullptr,
                              float stroke_width = 1.0f) override;

    void DrawEllipse(Point const& center, Vec2 const& radius, StrokeStylePtr stroke = nullptr,
                     float stroke_width = 1.0f) override;

    void FillShape(Shape const& shape) override;

    void FillRectangle(Rect const& rect) override;

    void FillRoundedRectangle(Rect const& rect, Vec2 const& radius) override;

    void FillEllipse(Point const& center, Vec2 const& radius) override;

    void CreateTexture(Texture& texture, math::Vec2T<uint32_t> size) override;

    void PushClipRect(Rect const& clip_rect) override;

    void PopClipRect() override;

    void PushLayer(Layer& layer) override;

    void PopLayer() override;

    void Clear() override;

    void Clear(Color const& clear_color) override;

    Size GetSize() const override;

    void SetCurrentBrush(BrushPtr brush) override;

    void SetTransform(const Matrix3x2& matrix) override;

    void SetAntialiasMode(bool enabled) override;

    void SetTextAntialiasMode(TextAntialiasMode mode) override;

    bool CheckVisibility(Rect const& bounds, Matrix3x2 const& transform) override;

    void Resize(Size const& size) override;

protected:
    RenderContextImpl();

    virtual ~RenderContextImpl();

    HRESULT CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1RenderTarget> ctx);

    void DiscardDeviceResources();

    void SaveDrawingState();

    void RestoreDrawingState();

protected:
    ComPtr<ITextRenderer>          text_renderer_;
    ComPtr<ID2D1RenderTarget>      render_target_;
    ComPtr<ID2D1DrawingStateBlock> drawing_state_;
};

}  // namespace kiwano
