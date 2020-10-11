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
 * \defgroup Render ��Ⱦģ��
 */

/**
 * \addtogroup Render
 * @{
 */


/**
 * \~chinese
 * @brief ��Ⱦ��
 */
class KGE_API Renderer : public Module
{
public:
    /// \~chinese
    /// @brief ��ȡʵ��
    static Renderer& GetInstance();

    /// \~chinese
    /// @brief ��ȡ������ɫ
    virtual Color GetClearColor() const;

    /// \~chinese
    /// @brief ��ȡ��Ⱦ�����С
    Size GetOutputSize() const;

    /// \~chinese
    /// @brief ��ȡ��Ⱦ������
    RenderContext& GetContext();

    /// \~chinese
    /// @brief ������Ⱦ�����С
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    /// \~chinese
    /// @brief ����������ɫ
    void SetClearColor(const Color& clear_color);

    /// \~chinese
    /// @brief ������رմ�ֱͬ��
    void SetVSyncEnabled(bool enabled);

    /// \~chinese
    /// @brief ���ڴ�С�仯ʱ�Զ������ֱ���
    void ResetResolutionWhenWindowResized(bool enabled);

    /// \~chinese
    /// @brief ���������ڲ���Դ
    /// @param[out] texture ����
    /// @param[in] file_path ͼƬ·��
    virtual void CreateTexture(Texture& texture, const String& file_path) = 0;

    /// \~chinese
    /// @brief ���������ڲ���Դ
    /// @param[out] texture ����
    /// @param[in] data ͼƬ����������
    virtual void CreateTexture(Texture& texture, const BinaryData& data) = 0;

    /// \~chinese
    /// @brief ����GIFͼ���ڲ���Դ
    /// @param[out] gif GIFͼ��
    /// @param[in] file_path ͼƬ·��
    virtual void CreateGifImage(GifImage& gif, const String& file_path) = 0;

    /// \~chinese
    /// @brief ����GIFͼ���ڲ���Դ
    /// @param[out] gif GIFͼ��
    /// @param[in] data ͼƬ����������
    virtual void CreateGifImage(GifImage& gif, const BinaryData& data) = 0;

    /// \~chinese
    /// @brief ����GIF�ؼ�֡�ڲ���Դ
    /// @param[out] frame GIF�ؼ�֡
    /// @param[in] gif GIFͼ��
    /// @param[in] frame_index ֡�±�
    virtual void CreateGifImageFrame(GifImage::Frame& frame, const GifImage& gif, size_t frame_index) = 0;

    /// \~chinese
    /// @brief �������弯�ڲ���Դ
    /// @param[out] font ����
    /// @param[out] family_names ���������������
    /// @param[in] file_paths �����ļ�·��
    virtual void CreateFontCollection(Font& font, Vector<String>& family_names, const String& file_path) = 0;

    /// \~chinese
    /// @brief �������弯�ڲ���Դ
    /// @param[out] font ����
    /// @param[out] family_names ���������������
    /// @param[in] data �����������Դ
    virtual void CreateFontCollection(Font& font, Vector<String>& family_names, const BinaryData& data) = 0;

    /// \~chinese
    /// @brief �������ֲ����ڲ���Դ
    /// @param[out] layout ���岼��
    /// @param text ��������
    /// @param style �ı���ʽ
    virtual void CreateTextLayout(TextLayout& layout, const String& content, const TextStyle& style) = 0;

    /// \~chinese
    /// @brief �����߶���״�ڲ���Դ
    /// @param[out] shape ��״
    /// @param[in] begin_pos �߶����
    /// @param[in] end_pos �߶��յ�
    virtual void CreateLineShape(Shape& shape, const Point& begin_pos, const Point& end_pos) = 0;

    /// \~chinese
    /// @brief ����������״�ڲ���Դ
    /// @param[out] shape ��״
    /// @param[in] rect ���δ�С
    virtual void CreateRectShape(Shape& shape, const Rect& rect) = 0;

    /// \~chinese
    /// @brief ����Բ�Ǿ�����״�ڲ���Դ
    /// @param[out] shape ��״
    /// @param[in] rect ���δ�С
    /// @param[in] radius Բ�ǰ뾶
    virtual void CreateRoundedRectShape(Shape& shape, const Rect& rect, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief ������Բ��״�ڲ���Դ
    /// @param[out] shape ��״
    /// @param[in] center ��ԲԲ��
    /// @param[in] radius ��Բ�뾶
    virtual void CreateEllipseShape(Shape& shape, const Point& center, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief ��������ͼ���������ڲ���Դ
    /// @param[out] maker ��״������
    virtual void CreateShapeSink(ShapeMaker& maker) = 0;

    /// \~chinese
    /// @brief ������ɫ��ˢ�ڲ���Դ
    /// @param[out] brush ��ˢ
    /// @param[in] color ��ɫ
    virtual void CreateBrush(Brush& brush, const Color& color) = 0;

    /// \~chinese
    /// @brief �������Խ��仭ˢ�ڲ���Դ
    /// @param[out] brush ��ˢ
    /// @param[in] style ���Խ�����ʽ
    virtual void CreateBrush(Brush& brush, const LinearGradientStyle& style) = 0;

    /// \~chinese
    /// @brief �������򽥱仭ˢ�ڲ���Դ
    /// @param[out] brush ��ˢ
    /// @param[in] style ���򽥱���ʽ
    virtual void CreateBrush(Brush& brush, const RadialGradientStyle& style) = 0;

    /// \~chinese
    /// @brief ��������ˢ�ڲ���Դ
    /// @param[out] brush ��ˢ
    /// @param[in] texture ����
    virtual void CreateBrush(Brush& brush, TexturePtr texture) = 0;

    /// \~chinese
    /// @brief ����������ʽ�ڲ���Դ
    /// @param[out] stroke_style ������ʽ
    /// @param[in] cap �߶ζ˵���ʽ
    /// @param[in] line_join �߶��ཻ��ʽ
    /// @param[in] dash_array ���߳������϶����
    /// @param[in] dash_size ���������С
    /// @param[in] dash_offset ����ƫ����
    virtual void CreateStrokeStyle(StrokeStyle& stroke_style) = 0;

    /// \~chinese
    /// @brief ����������Ⱦ�����ģ��������ĵ���Ⱦ�����������
    /// @param[in,out] texture ��Ⱦ���������
    /// @param[in] desired_size �����������С
    /// @return ������Ⱦ������
    virtual RenderContextPtr CreateTextureRenderContext(Texture& texture, const Size* desired_size = nullptr) = 0;

public:
    /// \~chinese
    /// @brief �����������
    virtual void Clear() = 0;

    /// \~chinese
    /// @brief ���������ݳ���������
    /// @throw kiwano::SystemError ����ʧ��ʱ�׳�
    virtual void Present() = 0;

    /// \~chinese
    /// @brief Ϊ���ڴ�����Ⱦ������
    /// @throw kiwano::SystemError ����������ʧ��ʱ�׳�
    virtual void MakeContextForWindow(WindowPtr window) = 0;

    /// \~chinese
    /// @brief ������Ⱦ����Դ
    virtual void Destroy();

    /// \~chinese
    /// @brief �����¼�
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
