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
#include <kiwano/render/Renderer.h>
#include <kiwano/render/DirectX/D3DDeviceResources.h>
#include <kiwano/render/DirectX/RenderContextImpl.h>

namespace kiwano
{

class KGE_API RendererImpl
    : public Renderer
{
public:
    static RendererImpl& GetInstance();

    void CreateTexture(Texture& texture, const String& file_path) override;

    void CreateTexture(Texture& texture, const BinaryData& data) override;

    void CreateTexture(Texture& texture, const PixelSize& size, const BinaryData& data, PixelFormat format) override;

    void CreateGifImage(GifImage& gif, const String& file_path) override;

    void CreateGifImage(GifImage& gif, const BinaryData& data) override;

    void CreateGifImageFrame(GifImage::Frame& frame, const GifImage& gif, size_t frame_index) override;

    void CreateFontCollection(Font& font, Vector<String>& family_names, const String& file_path) override;

    void CreateFontCollection(Font& font, Vector<String>& family_names, const BinaryData& data) override;

    void CreateTextLayout(TextLayout& layout, const String& content, const TextStyle& style) override;

    void CreateLineShape(Shape& shape, const Point& begin_pos, const Point& end_pos) override;

    void CreateRectShape(Shape& shape, const Rect& rect) override;

    void CreateRoundedRectShape(Shape& shape, const Rect& rect, const Vec2& radius) override;

    void CreateEllipseShape(Shape& shape, const Point& center, const Vec2& radius) override;

    void CreateShapeSink(ShapeMaker& maker) override;

    void CreateBrush(Brush& brush, const Color& color) override;

    void CreateBrush(Brush& brush, const LinearGradientStyle& style) override;

    void CreateBrush(Brush& brush, const RadialGradientStyle& style) override;

    void CreateBrush(Brush& brush, TexturePtr texture) override;

    void CreateStrokeStyle(StrokeStyle& stroke_style) override;

    RenderContextPtr CreateTextureRenderContext(TexturePtr texture, const PixelSize& desired_size) override;

public:
    void Clear() override;

    void Present() override;

    void Resize(uint32_t width, uint32_t height) override;

    void MakeContextForWindow(WindowPtr window) override;

    void Destroy() override;

    void HandleEvent(EventModuleContext& ctx) override;

protected:
    RendererImpl();

private:
    using ID2DDeviceResources = kiwano::graphics::directx::ID2DDeviceResources;
    using ID3DDeviceResources = kiwano::graphics::directx::ID3DDeviceResources;

    ComPtr<ID2DDeviceResources> d2d_res_;
    ComPtr<ID3DDeviceResources> d3d_res_;

    HMONITOR monitor_;
};


/** @} */

}  // namespace kiwano
