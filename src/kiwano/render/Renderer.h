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
#include <kiwano/base/Module.h>
#include <kiwano/render/Font.h>
#include <kiwano/render/GifImage.h>
#include <kiwano/render/TextStyle.h>
#include <kiwano/render/RenderContext.h>
#include <kiwano/platform/Window.h>

namespace kiwano
{

/**
 * \~chinese
 * \defgroup Render 渲染模块
 */

/**
 * \addtogroup Render
 * @{
 */


/**
 * \~chinese
 * @brief 渲染器
 */
class KGE_API Renderer : public Module
{
public:
    /// \~chinese
    /// @brief 获取实例
    static Renderer& GetInstance();

    /// \~chinese
    /// @brief 获取清屏颜色
    virtual Color GetClearColor() const;

    /// \~chinese
    /// @brief 获取渲染输出大小
    Size GetOutputSize() const;

    /// \~chinese
    /// @brief 获取渲染上下文
    RenderContext& GetContext();

    /// \~chinese
    /// @brief 重设渲染输出大小
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    /// \~chinese
    /// @brief 设置清屏颜色
    void SetClearColor(const Color& clear_color);

    /// \~chinese
    /// @brief 开启或关闭垂直同步
    void SetVSyncEnabled(bool enabled);

    /// \~chinese
    /// @brief 窗口大小变化时自动调整分辨率
    void ResetResolutionWhenWindowResized(bool enabled);

    /// \~chinese
    /// @brief 创建纹理内部资源
    /// @param[out] texture 纹理
    /// @param[in] file_path 图片路径
    virtual void CreateTexture(Texture& texture, const String& file_path) = 0;

    /// \~chinese
    /// @brief 创建纹理内部资源
    /// @param[out] texture 纹理
    /// @param[in] data 图片二进制数据
    virtual void CreateTexture(Texture& texture, const BinaryData& data) = 0;

    /// \~chinese
    /// @brief 创建GIF图像内部资源
    /// @param[out] gif GIF图像
    /// @param[in] file_path 图片路径
    virtual void CreateGifImage(GifImage& gif, const String& file_path) = 0;

    /// \~chinese
    /// @brief 创建GIF图像内部资源
    /// @param[out] gif GIF图像
    /// @param[in] data 图片二进制数据
    virtual void CreateGifImage(GifImage& gif, const BinaryData& data) = 0;

    /// \~chinese
    /// @brief 创建GIF关键帧内部资源
    /// @param[out] frame GIF关键帧
    /// @param[in] gif GIF图像
    /// @param[in] frame_index 帧下标
    virtual void CreateGifImageFrame(GifImage::Frame& frame, const GifImage& gif, size_t frame_index) = 0;

    /// \~chinese
    /// @brief 创建字体集内部资源
    /// @param[out] font 字体
    /// @param[out] family_names 字体包含的字体族
    /// @param[in] file_paths 字体文件路径
    virtual void CreateFontCollection(Font& font, Vector<String>& family_names, const String& file_path) = 0;

    /// \~chinese
    /// @brief 创建字体集内部资源
    /// @param[out] font 字体
    /// @param[out] family_names 字体包含的字体族
    /// @param[in] data 字体二进制资源
    virtual void CreateFontCollection(Font& font, Vector<String>& family_names, const BinaryData& data) = 0;

    /// \~chinese
    /// @brief 创建文字布局内部资源
    /// @param[out] layout 字体布局
    /// @param text 文字内容
    /// @param style 文本样式
    virtual void CreateTextLayout(TextLayout& layout, const String& content, const TextStyle& style) = 0;

    /// \~chinese
    /// @brief 创建线段形状内部资源
    /// @param[out] shape 形状
    /// @param[in] begin_pos 线段起点
    /// @param[in] end_pos 线段终点
    virtual void CreateLineShape(Shape& shape, const Point& begin_pos, const Point& end_pos) = 0;

    /// \~chinese
    /// @brief 创建矩形形状内部资源
    /// @param[out] shape 形状
    /// @param[in] rect 矩形大小
    virtual void CreateRectShape(Shape& shape, const Rect& rect) = 0;

    /// \~chinese
    /// @brief 创建圆角矩形形状内部资源
    /// @param[out] shape 形状
    /// @param[in] rect 矩形大小
    /// @param[in] radius 圆角半径
    virtual void CreateRoundedRectShape(Shape& shape, const Rect& rect, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief 创建椭圆形状内部资源
    /// @param[out] shape 形状
    /// @param[in] center 椭圆圆心
    /// @param[in] radius 椭圆半径
    virtual void CreateEllipseShape(Shape& shape, const Point& center, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief 创建几何图形生成器内部资源
    /// @param[out] maker 形状生成器
    virtual void CreateShapeSink(ShapeMaker& maker) = 0;

    /// \~chinese
    /// @brief 创建纯色画刷内部资源
    /// @param[out] brush 画刷
    /// @param[in] color 颜色
    virtual void CreateBrush(Brush& brush, const Color& color) = 0;

    /// \~chinese
    /// @brief 创建线性渐变画刷内部资源
    /// @param[out] brush 画刷
    /// @param[in] style 线性渐变样式
    virtual void CreateBrush(Brush& brush, const LinearGradientStyle& style) = 0;

    /// \~chinese
    /// @brief 创建径向渐变画刷内部资源
    /// @param[out] brush 画刷
    /// @param[in] style 径向渐变样式
    virtual void CreateBrush(Brush& brush, const RadialGradientStyle& style) = 0;

    /// \~chinese
    /// @brief 创建纹理画刷内部资源
    /// @param[out] brush 画刷
    /// @param[in] texture 纹理
    virtual void CreateBrush(Brush& brush, TexturePtr texture) = 0;

    /// \~chinese
    /// @brief 创建线条样式内部资源
    /// @param[out] stroke_style 线条样式
    /// @param[in] cap 线段端点样式
    /// @param[in] line_join 线段相交样式
    /// @param[in] dash_array 虚线长度与间隙数组
    /// @param[in] dash_size 虚线数组大小
    /// @param[in] dash_offset 虚线偏移量
    virtual void CreateStrokeStyle(StrokeStyle& stroke_style) = 0;

    /// \~chinese
    /// @brief 创建纹理渲染上下文，将上下文的渲染输出到纹理中
    /// @param[in,out] texture 渲染输出的纹理
    /// @param[in] desired_size 期望的输出大小
    /// @return 纹理渲染上下文
    virtual RenderContextPtr CreateTextureRenderContext(Texture& texture, const Size* desired_size = nullptr) = 0;

public:
    /// \~chinese
    /// @brief 清除绘制内容
    virtual void Clear() = 0;

    /// \~chinese
    /// @brief 将绘制内容呈现至窗口
    /// @throw kiwano::SystemError 呈现失败时抛出
    virtual void Present() = 0;

    /// \~chinese
    /// @brief 为窗口创建渲染上下文
    /// @throw kiwano::SystemError 创建上下文失败时抛出
    virtual void MakeContextForWindow(WindowPtr window) = 0;

    /// \~chinese
    /// @brief 销毁渲染器资源
    virtual void Destroy();

    /// \~chinese
    /// @brief 处理事件
    void HandleEvent(EventModuleContext& ctx) override;

protected:
    Renderer();

protected:
    bool             vsync_;
    bool             auto_reset_resolution_;
    Color            clear_color_;
    Size             output_size_;
    RenderContextPtr render_ctx_;
};

/** @} */

inline RenderContext& Renderer::GetContext()
{
    return *render_ctx_;
}

inline Size Renderer::GetOutputSize() const
{
    return output_size_;
}

inline Color Renderer::GetClearColor() const
{
    return clear_color_;
}

}  // namespace kiwano
