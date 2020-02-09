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
#include <kiwano/render/TextStyle.hpp>
#include <kiwano/render/RenderContext.h>
#include <kiwano/render/TextureRenderContext.h>

namespace kiwano
{

/**
 * \~chinese
 * \defgroup Render ��Ⱦ����
 */

/**
 * \addtogroup Render
 * @{
 */

/**
 * \~chinese
 * @brief ��Ⱦ��
 */
class KGE_API Renderer : public EventComponent
{
public:
    /// \~chinese
    /// @brief ��ȡʵ��
    static Renderer& GetInstance();

    /// \~chinese
    /// @brief ��ȡ������ɫ
    virtual Color GetClearColor() const;

    /// \~chinese
    /// @brief ����������ɫ
    virtual void SetClearColor(Color const& clear_color);

    /// \~chinese
    /// @brief ������رմ�ֱͬ��
    virtual void SetVSyncEnabled(bool enabled);

    /// \~chinese
    /// @brief ��������
    /// @param[out] texture ����
    /// @param[in] file_path ͼƬ·��
    virtual void CreateTexture(Texture& texture, String const& file_path) = 0;

    /// \~chinese
    /// @brief ��������
    /// @param[out] texture ����
    /// @param[in] resource ͼƬ��Դ
    virtual void CreateTexture(Texture& texture, Resource const& resource) = 0;

    /// \~chinese
    /// @brief ����GIFͼ��
    /// @param[out] gif GIFͼ��
    /// @param[in] file_path ͼƬ·��
    virtual void CreateGifImage(GifImage& gif, String const& file_path) = 0;

    /// \~chinese
    /// @brief ����GIFͼ��
    /// @param[out] gif GIFͼ��
    /// @param[in] resource ͼƬ��Դ
    virtual void CreateGifImage(GifImage& gif, Resource const& resource) = 0;

    /// \~chinese
    /// @brief ����GIFͼ��֡
    /// @param[out] frame GIFͼ��֡
    /// @param[in] gif GIFͼ��
    /// @param[in] frame_index ֡�±�
    virtual void CreateGifImageFrame(GifImage::Frame& frame, GifImage const& gif, size_t frame_index) = 0;

    /// \~chinese
    /// @brief �������弯
    /// @param[out] font ����
    /// @param[in] file_paths �����ļ�·��
    virtual void CreateFontCollection(Font& font, String const& file_path) = 0;

    /// \~chinese
    /// @brief �������弯
    /// @param[out] font ����
    /// @param[in] res_arr ������Դ
    virtual void CreateFontCollection(Font& font, Resource const& res) = 0;

    /// \~chinese
    /// @brief �������ָ�ʽ
    /// @param[out] layout ���岼��
    virtual void CreateTextFormat(TextLayout& layout) = 0;

    /// \~chinese
    /// @brief �������ֲ���
    /// @param[out] layout ���岼��
    virtual void CreateTextLayout(TextLayout& layout) = 0;

    /// \~chinese
    /// @brief �����߶���״
    /// @param[out] geo ��״
    /// @param[in] begin_pos �߶����
    /// @param[in] end_pos �߶��յ�
    virtual void CreateLineShape(Shape& shape, Point const& begin_pos, Point const& end_pos) = 0;

    /// \~chinese
    /// @brief ����������״
    /// @param[out] geo ��״
    /// @param[in] rect ���δ�С
    virtual void CreateRectShape(Shape& shape, Rect const& rect) = 0;

    /// \~chinese
    /// @brief ����Բ�Ǿ�����״
    /// @param[out] geo ��״
    /// @param[in] rect ���δ�С
    /// @param[in] radius Բ�ǰ뾶
    virtual void CreateRoundedRectShape(Shape& shape, Rect const& rect, Vec2 const& radius) = 0;

    /// \~chinese
    /// @brief ������Բ��״
    /// @param[out] geo ��״
    /// @param[in] center ��ԲԲ��
    /// @param[in] radius ��Բ�뾶
    virtual void CreateEllipseShape(Shape& shape, Point const& center, Vec2 const& radius) = 0;

    /// \~chinese
    /// @brief ��������ͼ��������
    /// @param[out] sink ��״������
    virtual void CreateShapeSink(ShapeSink& sink) = 0;

    /// \~chinese
    /// @brief ������ɫ��ˢ
    /// @param[out] brush ��ˢ
    /// @param[in] color ��ɫ
    virtual void CreateBrush(Brush& brush, Color const& color) = 0;

    /// \~chinese
    /// @brief �������Խ��仭ˢ
    /// @param[out] brush ��ˢ
    /// @param[in] style ���Խ�����ʽ
    virtual void CreateBrush(Brush& brush, LinearGradientStyle const& style) = 0;

    /// \~chinese
    /// @brief �������򽥱仭ˢ
    /// @param[out] brush ��ˢ
    /// @param[in] style ���򽥱���ʽ
    virtual void CreateBrush(Brush& brush, RadialGradientStyle const& style) = 0;

    /// \~chinese
    /// @brief ����������ʽ
    /// @param[out] stroke_style ������ʽ
    /// @param[in] cap �߶ζ˵���ʽ
    /// @param[in] line_join �߶��ཻ��ʽ
    /// @param[in] dash_array ���߳������϶����
    /// @param[in] dash_size ���������С
    /// @param[in] dash_offset ����ƫ����
    virtual void CreateStrokeStyle(StrokeStyle& stroke_style, CapStyle cap, LineJoinStyle line_join,
                                   const float* dash_array, size_t dash_size, float dash_offset) = 0;

    /// \~chinese
    /// @brief ����������Ⱦ������
    /// @param[in] desired_size �����������С
    /// @return ������Ⱦ������
    virtual TextureRenderContextPtr CreateTextureRenderContext(const Size* desired_size = nullptr) = 0;

public:
    /// \~chinese
    /// @brief ��ʼ��Ⱦ
    virtual void BeginDraw() = 0;

    /// \~chinese
    /// @brief ������Ⱦ
    virtual void EndDraw() = 0;

    /// \~chinese
    /// @brief �����������
    virtual void Clear() = 0;

    /// \~chinese
    /// @brief ���������ݳ���������
    virtual void Present() = 0;

    /// \~chinese
    /// @brief ��ȡ��Ⱦ������
    virtual RenderContext& GetContext() = 0;

    /// \~chinese
    /// @brief ��ȡĿ�괰��
    virtual WindowHandle GetTargetWindow() const;

    /// \~chinese
    /// @brief ��ȡ��Ⱦ�����С
    virtual Size GetOutputSize() const;

protected:
    Renderer();

protected:
    bool         vsync_;
    WindowHandle target_window_;
    Color        clear_color_;
    Size         output_size_;
};

/** @} */

inline WindowHandle Renderer::GetTargetWindow() const
{
    return target_window_;
}

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
