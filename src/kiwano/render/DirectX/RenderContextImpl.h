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
#include <kiwano/render/DirectX/TextRenderer.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{

class KGE_API RenderContextImpl : public RenderContext
{
public:
    RenderContextImpl();

    virtual ~RenderContextImpl();

    virtual HRESULT CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1DeviceContext> ctx);

    void BeginDraw() override;

    void EndDraw() override;

    void DrawImage(const Image& image, const Rect* src_rect) override;

    void CreateBitmap(Bitmap& bitmap, const PixelSize& size) override;

    void DrawBitmap(const Bitmap& bitmap, const Rect* src_rect, const Rect* dest_rect) override;

    void DrawTextLayout(const TextLayout& layout, const Point& offset, RefPtr<Brush> outline_brush) override;

    void DrawShape(const Shape& shape) override;

    void DrawLine(const Point& point1, const Point& point2) override;

    void DrawRectangle(const Rect& rect) override;

    void DrawRoundedRectangle(const Rect& rect, const Vec2& radius) override;

    void DrawEllipse(const Point& center, const Vec2& radius) override;

    void FillShape(const Shape& shape) override;

    void FillRectangle(const Rect& rect) override;

    void FillRoundedRectangle(const Rect& rect, const Vec2& radius) override;

    void FillEllipse(const Point& center, const Vec2& radius) override;

    void PushClipRect(const Rect& clip_rect) override;

    void PopClipRect() override;

    void PushLayer(Layer& layer) override;

    void PopLayer() override;

    void Clear() override;

    void Clear(const Color& clear_color) override;

    Size GetSize() const override;

    void SetCurrentBrush(RefPtr<Brush> brush) override;

    void SetCurrentStrokeStyle(RefPtr<StrokeStyle> stroke_style) override;

    Matrix3x2 GetTransform() const override;

    void SetTransform(const Matrix3x2& matrix) override;

    void SetBlendMode(BlendMode blend) override;

    void SetAntialiasMode(bool enabled) override;

    void SetTextAntialiasMode(TextAntialiasMode mode) override;

    bool CheckVisibility(const Rect& bounds, const Matrix3x2& transform) override;

    void Resize(const Size& size) override;

    RefPtr<Image> GetTarget() const override;

    void SetTarget(const Image& target) override;

protected:
    void DiscardDeviceResources();

    void SaveDrawingState();

    void RestoreDrawingState();

protected:
    ComPtr<ITextRenderer>          text_renderer_;
    ComPtr<ID2D1DeviceContext>     device_ctx_;
    ComPtr<ID2D1DrawingStateBlock> drawing_state_;
};

class KGE_API CommandListRenderContextImpl : public RenderContextImpl
{
public:
    HRESULT CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1DeviceContext> ctx) override;

    RefPtr<Image> GetTarget() const override;

    void SetTarget(const Image& target) override;

    void BeginDraw() override;

    void EndDraw() override;

private:
    ComPtr<ID2D1CommandList> cmd_list_;
    ComPtr<ID2D1Image>       original_target_;
};

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano
