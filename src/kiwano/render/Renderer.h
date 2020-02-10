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
#include <kiwano/render/Font.h>
#include <kiwano/render/GifImage.h>
#include <kiwano/render/TextStyle.hpp>
#include <kiwano/render/RenderContext.h>
#include <kiwano/render/TextureRenderContext.h>

namespace kiwano
{

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
class KGE_API Renderer : public EventComponent
{
public:
    /// \~chinese
    /// @brief 获取实例
    static Renderer& GetInstance();

    /// \~chinese
    /// @brief 获取清屏颜色
    virtual Color GetClearColor() const;

    /// \~chinese
    /// @brief 设置清屏颜色
    virtual void SetClearColor(Color const& clear_color);

    /// \~chinese
    /// @brief 开启或关闭垂直同步
    virtual void SetVSyncEnabled(bool enabled);

    /// \~chinese
    /// @brief 创建纹理内部资源
    /// @param[out] texture 纹理
    /// @param[in] file_path 图片路径
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateTexture(Texture& texture, String const& file_path) = 0;

    /// \~chinese
    /// @brief 创建纹理内部资源
    /// @param[out] texture 纹理
    /// @param[in] resource 图片资源
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateTexture(Texture& texture, Resource const& resource) = 0;

    /// \~chinese
    /// @brief 创建GIF图像内部资源
    /// @param[out] gif GIF图像
    /// @param[in] file_path 图片路径
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateGifImage(GifImage& gif, String const& file_path) = 0;

    /// \~chinese
    /// @brief 创建GIF图像内部资源
    /// @param[out] gif GIF图像
    /// @param[in] resource 图片资源
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateGifImage(GifImage& gif, Resource const& resource) = 0;

    /// \~chinese
    /// @brief 创建GIF图像帧内部资源
    /// @param[out] frame GIF图像帧
    /// @param[in] gif GIF图像
    /// @param[in] frame_index 帧下标
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateGifImageFrame(GifImage::Frame& frame, GifImage const& gif, size_t frame_index) = 0;

    /// \~chinese
    /// @brief 创建字体集内部资源
    /// @param[out] font 字体
    /// @param[in] file_paths 字体文件路径
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateFontCollection(Font& font, String const& file_path) = 0;

    /// \~chinese
    /// @brief 创建字体集内部资源
    /// @param[out] font 字体
    /// @param[in] res_arr 字体资源
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateFontCollection(Font& font, Resource const& res) = 0;

    /// \~chinese
    /// @brief 创建文字格式内部资源
    /// @param[out] layout 字体布局
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateTextFormat(TextLayout& layout) = 0;

    /// \~chinese
    /// @brief 创建文字布局内部资源
    /// @param[out] layout 字体布局
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateTextLayout(TextLayout& layout) = 0;

    /// \~chinese
    /// @brief 创建线段形状内部资源
    /// @param[out] shape 形状
    /// @param[in] begin_pos 线段起点
    /// @param[in] end_pos 线段终点
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateLineShape(Shape& shape, Point const& begin_pos, Point const& end_pos) = 0;

    /// \~chinese
    /// @brief 创建矩形形状内部资源
    /// @param[out] shape 形状
    /// @param[in] rect 矩形大小
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateRectShape(Shape& shape, Rect const& rect) = 0;

    /// \~chinese
    /// @brief 创建圆角矩形形状内部资源
    /// @param[out] shape 形状
    /// @param[in] rect 矩形大小
    /// @param[in] radius 圆角半径
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateRoundedRectShape(Shape& shape, Rect const& rect, Vec2 const& radius) = 0;

    /// \~chinese
    /// @brief 创建椭圆形状内部资源
    /// @param[out] shape 形状
    /// @param[in] center 椭圆圆心
    /// @param[in] radius 椭圆半径
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateEllipseShape(Shape& shape, Point const& center, Vec2 const& radius) = 0;

    /// \~chinese
    /// @brief 创建几何图形生成器内部资源
    /// @param[out] sink 形状生成器
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateShapeSink(ShapeSink& sink) = 0;

    /// \~chinese
    /// @brief 创建纯色画刷内部资源
    /// @param[out] brush 画刷
    /// @param[in] color 颜色
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateBrush(Brush& brush, Color const& color) = 0;

    /// \~chinese
    /// @brief 创建线性渐变画刷内部资源
    /// @param[out] brush 画刷
    /// @param[in] style 线性渐变样式
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateBrush(Brush& brush, LinearGradientStyle const& style) = 0;

    /// \~chinese
    /// @brief 创建径向渐变画刷内部资源
    /// @param[out] brush 画刷
    /// @param[in] style 径向渐变样式
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateBrush(Brush& brush, RadialGradientStyle const& style) = 0;

    /// \~chinese
    /// @brief 创建线条样式内部资源
    /// @param[out] stroke_style 线条样式
    /// @param[in] cap 线段端点样式
    /// @param[in] line_join 线段相交样式
    /// @param[in] dash_array 虚线长度与间隙数组
    /// @param[in] dash_size 虚线数组大小
    /// @param[in] dash_offset 虚线偏移量
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual void CreateStrokeStyle(StrokeStyle& stroke_style, CapStyle cap, LineJoinStyle line_join,
                                   const float* dash_array, size_t dash_size, float dash_offset) = 0;

    /// \~chinese
    /// @brief 创建纹理渲染上下文
    /// @param[in] desired_size 期望的输出大小
    /// @return 纹理渲染上下文
    /// @throw kiwano::SystemException 创建失败时抛出
    virtual TextureRenderContextPtr CreateTextureRenderContext(const Size* desired_size = nullptr) = 0;

public:
    /// \~chinese
    /// @brief 开始渲染
    virtual void BeginDraw() = 0;

    /// \~chinese
    /// @brief 结束渲染
    virtual void EndDraw() = 0;

    /// \~chinese
    /// @brief 清除绘制内容
    virtual void Clear() = 0;

    /// \~chinese
    /// @brief 将绘制内容呈现至窗口
    /// @throw kiwano::SystemException 呈现失败时抛出
    virtual void Present() = 0;

    /// \~chinese
    /// @brief 获取渲染上下文
    virtual RenderContext& GetContext() = 0;

    /// \~chinese
    /// @brief 获取渲染输出大小
    virtual Size GetOutputSize() const;

protected:
    Renderer();

protected:
    bool         vsync_;
    Color        clear_color_;
    Size         output_size_;
};

/** @} */

inline Size Renderer::GetOutputSize() const
{
    return output_size_;
}

inline Color Renderer::GetClearColor() const
{
    return clear_color_;
}

inline void Renderer::SetVSyncEnabled(bool enabled)
{
    vsync_ = enabled;
}

inline void Renderer::SetClearColor(const Color& color)
{
    clear_color_ = color;
}

}  // namespace kiwano
