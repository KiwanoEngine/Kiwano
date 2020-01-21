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
#include <kiwano/core/Component.h>
#include <kiwano/platform/Window.h>
#include <kiwano/render/Font.h>
#include <kiwano/render/GifImage.h>
#include <kiwano/render/RenderContext.h>
#include <kiwano/render/TextStyle.hpp>
#include <kiwano/render/DirectX/FontCollectionLoader.h>

#if defined(KGE_USE_DIRECTX10)
#include <kiwano/render/DirectX/D3D10DeviceResources.h>
#else
#include <kiwano/render/DirectX/D3D11DeviceResources.h>
#endif

namespace kiwano
{

#if defined(KGE_USE_DIRECTX10)
typedef ID3D10DeviceResources ID3DDeviceResources;
#else
typedef ID3D11DeviceResources ID3DDeviceResources;
#endif

/**
 * \~chinese
 * \defgroup Render 渲染引擎
 */

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief 渲染器
 */
class KGE_API Renderer
    : public Singleton<Renderer>
    , public RenderComponent
    , public EventComponent
    , public RenderContext
{
    friend Singleton<Renderer>;

public:
    /// \~chinese
    /// @brief 获取清屏颜色
    Color const& GetClearColor() const;

    /// \~chinese
    /// @brief 设置清屏颜色
    void SetClearColor(Color const& clear_color);

    /// \~chinese
    /// @brief 开启或关闭垂直同步
    void SetVSyncEnabled(bool enabled);

    /// \~chinese
    /// @brief 设置DPI
    void SetDpi(float dpi);

    /// \~chinese
    /// @brief 创建纹理
    /// @param[out] texture 纹理
    /// @param[in] file_path 图片路径
    void CreateTexture(Texture& texture, String const& file_path);

    /// \~chinese
    /// @brief 创建纹理
    /// @param[out] texture 纹理
    /// @param[in] resource 图片资源
    void CreateTexture(Texture& texture, Resource const& resource);

    /// \~chinese
    /// @brief 创建GIF图像
    /// @param[out] gif GIF图像
    /// @param[in] file_path 图片路径
    void CreateGifImage(GifImage& gif, String const& file_path);

    /// \~chinese
    /// @brief 创建GIF图像
    /// @param[out] gif GIF图像
    /// @param[in] resource 图片资源
    void CreateGifImage(GifImage& gif, Resource const& resource);

    /// \~chinese
    /// @brief 创建GIF图像帧
    /// @param[out] frame GIF图像帧
    /// @param[in] gif GIF图像
    /// @param[in] frame_index 帧下标
    void CreateGifImageFrame(GifImage::Frame& frame, GifImage const& gif, size_t frame_index);

    /// \~chinese
    /// @brief 创建字体集
    /// @param[out] font 字体
    /// @param[in] file_paths 字体文件路径
    void CreateFontCollection(Font& font, Vector<String> const& file_paths);

    /// \~chinese
    /// @brief 创建字体集
    /// @param[out] font 字体
    /// @param[in] res_arr 字体资源
    void CreateFontCollection(Font& font, Vector<Resource> const& res_arr);

    /// \~chinese
    /// @brief 创建文字格式
    /// @param[out] layout 字体布局
    void CreateTextFormat(TextLayout& layout);

    /// \~chinese
    /// @brief 创建文字布局
    /// @param[out] layout 字体布局
    void CreateTextLayout(TextLayout& layout);

    /// \~chinese
    /// @brief 创建线段形状
    /// @param[out] geo 形状
    /// @param[in] begin_pos 线段起点
    /// @param[in] end_pos 线段终点
    void CreateLineGeometry(Geometry& geo, Point const& begin_pos, Point const& end_pos);

    /// \~chinese
    /// @brief 创建矩形形状
    /// @param[out] geo 形状
    /// @param[in] rect 矩形大小
    void CreateRectGeometry(Geometry& geo, Rect const& rect);

    /// \~chinese
    /// @brief 创建圆角矩形形状
    /// @param[out] geo 形状
    /// @param[in] rect 矩形大小
    /// @param[in] radius 圆角半径
    void CreateRoundedRectGeometry(Geometry& geo, Rect const& rect, Vec2 const& radius);

    /// \~chinese
    /// @brief 创建椭圆形状
    /// @param[out] geo 形状
    /// @param[in] center 椭圆圆心
    /// @param[in] radius 椭圆半径
    void CreateEllipseGeometry(Geometry& geo, Point const& center, Vec2 const& radius);

    /// \~chinese
    /// @brief 创建几何图形生成器
    /// @param[out] sink 形状生成器
    void CreateGeometrySink(GeometrySink& sink);

    /// \~chinese
    /// @brief 创建纹理渲染上下文
    /// @param[out] render_context 渲染上下文
    void CreateTextureRenderTarget(TextureRenderContextPtr& render_context);

    /// \~chinese
    /// @brief 创建纯色画刷
    /// @param[out] brush 画刷
    /// @param[in] color 颜色
    void CreateSolidBrush(Brush& brush, Color const& color);

    /// \~chinese
    /// @brief 创建线性渐变画刷
    /// @param[out] brush 画刷
    /// @param[in] style 线性渐变样式
    void CreateLinearGradientBrush(Brush& brush, LinearGradientStyle const& style);

    /// \~chinese
    /// @brief 创建径向渐变画刷
    /// @param[out] brush 画刷
    /// @param[in] style 径向渐变样式
    void CreateRadialGradientBrush(Brush& brush, RadialGradientStyle const& style);

    /// \~chinese
    /// @brief 创建线条样式
    /// @param[out] stroke_style 线条样式
    /// @param[in] cap 线段端点样式
    /// @param[in] line_join 线段相交样式
    /// @param[in] dash_array 虚线长度与间隙数组
    /// @param[in] dash_size 虚线数组大小
    /// @param[in] dash_offset 虚线偏移量
    void CreateStrokeStyle(StrokeStyle& stroke_style, CapStyle cap, LineJoinStyle line_join, const float* dash_array,
                           size_t dash_size, float dash_offset);

public:
    /// \~chinese
    /// @brief 获取目标窗口
    WindowHandle GetTargetWindow() const;

    /// \~chinese
    /// @brief 获取渲染输出大小
    Size const& GetOutputSize() const;

    /// \~chinese
    /// @brief 获取Direct2D设备资源
    ID2DDeviceResources* GetD2DDeviceResources();

    /// \~chinese
    /// @brief 获取Direct3D设备资源
    ID3DDeviceResources* GetD3DDeviceResources();

public:
    void SetupComponent() override;

    void DestroyComponent() override;

    void BeforeRender() override;

    void AfterRender() override;

    void HandleEvent(Event* evt) override;

private:
    Renderer();

    ~Renderer();

    HRESULT HandleDeviceLost();

    void ResizeTarget(uint32_t width, uint32_t height);

private:
    bool         vsync_;
    WindowHandle target_window_;
    Color        clear_color_;
    Size         output_size_;

    ComPtr<ID2DDeviceResources>           d2d_res_;
    ComPtr<ID3DDeviceResources>           d3d_res_;
    ComPtr<ID2D1DrawingStateBlock>        drawing_state_block_;
    ComPtr<IFontCollectionLoader>         font_collection_loader_;
    ComPtr<IResourceFontFileLoader>       res_font_file_loader_;
    ComPtr<IResourceFontCollectionLoader> res_font_collection_loader_;
};

/** @} */

inline WindowHandle Renderer::GetTargetWindow() const
{
    return target_window_;
}

inline Size const& Renderer::GetOutputSize() const
{
    return output_size_;
}

inline Color const& Renderer::GetClearColor() const
{
    return clear_color_;
}

inline ID2DDeviceResources* Renderer::GetD2DDeviceResources()
{
    KGE_ASSERT(d2d_res_);
    return d2d_res_.get();
}

inline ID3DDeviceResources* Renderer::GetD3DDeviceResources()
{
    KGE_ASSERT(d3d_res_);
    return d3d_res_.get();
}
}  // namespace kiwano
