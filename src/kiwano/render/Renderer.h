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
class KGE_API Renderer
    : public Singleton<Renderer>
    , public RenderComponent
    , public EventComponent
    , public RenderContext
{
    friend Singleton<Renderer>;

public:
    /// \~chinese
    /// @brief ��ȡ������ɫ
    Color const& GetClearColor() const;

    /// \~chinese
    /// @brief ����������ɫ
    void SetClearColor(Color const& clear_color);

    /// \~chinese
    /// @brief ������رմ�ֱͬ��
    void SetVSyncEnabled(bool enabled);

    /// \~chinese
    /// @brief ����DPI
    void SetDpi(float dpi);

    /// \~chinese
    /// @brief ��������
    /// @param[out] texture ����
    /// @param[in] file_path ͼƬ·��
    void CreateTexture(Texture& texture, String const& file_path);

    /// \~chinese
    /// @brief ��������
    /// @param[out] texture ����
    /// @param[in] resource ͼƬ��Դ
    void CreateTexture(Texture& texture, Resource const& resource);

    /// \~chinese
    /// @brief ����GIFͼ��
    /// @param[out] gif GIFͼ��
    /// @param[in] file_path ͼƬ·��
    void CreateGifImage(GifImage& gif, String const& file_path);

    /// \~chinese
    /// @brief ����GIFͼ��
    /// @param[out] gif GIFͼ��
    /// @param[in] resource ͼƬ��Դ
    void CreateGifImage(GifImage& gif, Resource const& resource);

    /// \~chinese
    /// @brief ����GIFͼ��֡
    /// @param[out] frame GIFͼ��֡
    /// @param[in] gif GIFͼ��
    /// @param[in] frame_index ֡�±�
    void CreateGifImageFrame(GifImage::Frame& frame, GifImage const& gif, size_t frame_index);

    /// \~chinese
    /// @brief �������弯
    /// @param[out] font ����
    /// @param[in] file_paths �����ļ�·��
    void CreateFontCollection(Font& font, Vector<String> const& file_paths);

    /// \~chinese
    /// @brief �������弯
    /// @param[out] font ����
    /// @param[in] res_arr ������Դ
    void CreateFontCollection(Font& font, Vector<Resource> const& res_arr);

    /// \~chinese
    /// @brief �������ָ�ʽ
    /// @param[out] layout ���岼��
    void CreateTextFormat(TextLayout& layout);

    /// \~chinese
    /// @brief �������ֲ���
    /// @param[out] layout ���岼��
    void CreateTextLayout(TextLayout& layout);

    /// \~chinese
    /// @brief �����߶���״
    /// @param[out] geo ��״
    /// @param[in] begin_pos �߶����
    /// @param[in] end_pos �߶��յ�
    void CreateLineGeometry(Geometry& geo, Point const& begin_pos, Point const& end_pos);

    /// \~chinese
    /// @brief ����������״
    /// @param[out] geo ��״
    /// @param[in] rect ���δ�С
    void CreateRectGeometry(Geometry& geo, Rect const& rect);

    /// \~chinese
    /// @brief ����Բ�Ǿ�����״
    /// @param[out] geo ��״
    /// @param[in] rect ���δ�С
    /// @param[in] radius Բ�ǰ뾶
    void CreateRoundedRectGeometry(Geometry& geo, Rect const& rect, Vec2 const& radius);

    /// \~chinese
    /// @brief ������Բ��״
    /// @param[out] geo ��״
    /// @param[in] center ��ԲԲ��
    /// @param[in] radius ��Բ�뾶
    void CreateEllipseGeometry(Geometry& geo, Point const& center, Vec2 const& radius);

    /// \~chinese
    /// @brief ��������ͼ��������
    /// @param[out] sink ��״������
    void CreateGeometrySink(GeometrySink& sink);

    /// \~chinese
    /// @brief ����������Ⱦ������
    /// @param[out] render_context ��Ⱦ������
    void CreateTextureRenderTarget(TextureRenderContextPtr& render_context);

    /// \~chinese
    /// @brief ������ɫ��ˢ
    /// @param[out] brush ��ˢ
    /// @param[in] color ��ɫ
    void CreateSolidBrush(Brush& brush, Color const& color);

    /// \~chinese
    /// @brief �������Խ��仭ˢ
    /// @param[out] brush ��ˢ
    /// @param[in] style ���Խ�����ʽ
    void CreateLinearGradientBrush(Brush& brush, LinearGradientStyle const& style);

    /// \~chinese
    /// @brief �������򽥱仭ˢ
    /// @param[out] brush ��ˢ
    /// @param[in] style ���򽥱���ʽ
    void CreateRadialGradientBrush(Brush& brush, RadialGradientStyle const& style);

    /// \~chinese
    /// @brief ����������ʽ
    /// @param[out] stroke_style ������ʽ
    /// @param[in] cap �߶ζ˵���ʽ
    /// @param[in] line_join �߶��ཻ��ʽ
    /// @param[in] dash_array ���߳������϶����
    /// @param[in] dash_size ���������С
    /// @param[in] dash_offset ����ƫ����
    void CreateStrokeStyle(StrokeStyle& stroke_style, CapStyle cap, LineJoinStyle line_join, const float* dash_array,
                           size_t dash_size, float dash_offset);

public:
    /// \~chinese
    /// @brief ��ȡĿ�괰��
    WindowHandle GetTargetWindow() const;

    /// \~chinese
    /// @brief ��ȡ��Ⱦ�����С
    Size const& GetOutputSize() const;

    /// \~chinese
    /// @brief ��ȡDirect2D�豸��Դ
    ID2DDeviceResources* GetD2DDeviceResources();

    /// \~chinese
    /// @brief ��ȡDirect3D�豸��Դ
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
