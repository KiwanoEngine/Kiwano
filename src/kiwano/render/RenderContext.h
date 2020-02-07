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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Time.h>
#include <kiwano/render/Brush.h>
#include <kiwano/render/Shape.h>
#include <kiwano/render/LayerArea.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/render/Texture.h>
#include <kiwano/render/DirectX/TextRenderer.h>

namespace kiwano
{
class Renderer;

KGE_DECLARE_SMART_PTR(RenderContext);

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief ���ֿ����ģʽ
enum class TextAntialiasMode
{
    Default,    ///< ϵͳĬ��
    ClearType,  ///< ClearType �����
    GrayScale,  ///< �Ҷȿ����
    None        ///< �����ÿ����
};

/// \~chinese
/// @brief ��Ⱦ������
/// @details
/// ��Ⱦ�����Ľ���ɻ���ͼԪ�Ļ��ƣ��������ƽ��������ض���Ŀ���У��細�ڻ�����
class KGE_API RenderContext : public virtual ObjectBase
{
    friend class Renderer;

public:
    /// \~chinese
    /// @brief �Ƿ���Ч
    bool IsValid() const;

    /// \~chinese
    /// @brief ��ʼ��Ⱦ
    void BeginDraw();

    /// \~chinese
    /// @brief ������Ⱦ
    void EndDraw();

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    /// @param src_rect Դ����ü�����
    /// @param dest_rect ���Ƶ�Ŀ������
    void DrawTexture(Texture const& texture, Rect const& src_rect, Rect const& dest_rect);

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    /// @param src_rect Դ����ü�����
    /// @param dest_rect ���Ƶ�Ŀ������
    void DrawTexture(Texture const& texture, const Rect* src_rect = nullptr, const Rect* dest_rect = nullptr);

    /// \~chinese
    /// @brief �����ı�����
    /// @param layout �ı�����
    /// @param offset ƫ����
    void DrawTextLayout(TextLayout const& layout, Point const& offset = Point());

    /// \~chinese
    /// @brief ������״����
    /// @param shape ��״
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    void DrawShape(Shape const& shape, StrokeStylePtr stroke = nullptr, float stroke_width = 1.0f);

    /// \~chinese
    /// @brief �����߶�
    /// @param point1 �߶����
    /// @param point2 �߶��յ�
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    void DrawLine(Point const& point1, Point const& point2, StrokeStylePtr stroke = nullptr, float stroke_width = 1.0f);

    /// \~chinese
    /// @brief ���ƾ��α߿�
    /// @param rect ����
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    void DrawRectangle(Rect const& rect, StrokeStylePtr stroke = nullptr, float stroke_width = 1.0f);

    /// \~chinese
    /// @brief ����Բ�Ǿ��α߿�
    /// @param rect ����
    /// @param radius Բ�ǰ뾶
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    void DrawRoundedRectangle(Rect const& rect, Vec2 const& radius, StrokeStylePtr stroke = nullptr,
                              float stroke_width = 1.0f);

    /// \~chinese
    /// @brief ������Բ�߿�
    /// @param center Բ��
    /// @param radius ��Բ�뾶
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    void DrawEllipse(Point const& center, Vec2 const& radius, StrokeStylePtr stroke = nullptr,
                     float stroke_width = 1.0f);

    /// \~chinese
    /// @brief �����״
    /// @param shape ��״
    void FillShape(Shape const& shape);

    /// \~chinese
    /// @brief ������
    /// @param rect ����
    void FillRectangle(Rect const& rect);

    /// \~chinese
    /// @brief ���Բ�Ǿ���
    /// @param rect ����
    /// @param radius Բ�ǰ뾶
    void FillRoundedRectangle(Rect const& rect, Vec2 const& radius);

    /// \~chinese
    /// @brief �����Բ
    /// @param center Բ��
    /// @param radius ��Բ�뾶
    void FillEllipse(Point const& center, Vec2 const& radius);

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    /// @param size �������ش�С
    void CreateTexture(Texture& texture, math::Vec2T<uint32_t> size);

    /// \~chinese
    /// @brief ���û��ƵĲü�����
    /// @param clip_rect �ü�����
    void PushClipRect(Rect const& clip_rect);

    /// \~chinese
    /// @brief ȡ����һ�����õĻ��Ʋü�����
    void PopClipRect();

    /// \~chinese
    /// @brief ����ͼ������
    /// @param layer ͼ������
    void PushLayer(LayerArea& layer);

    /// \~chinese
    /// @brief ȡ����һ�����õ�ͼ������
    void PopLayer();

    /// \~chinese
    /// @brief �����Ⱦ����
    void Clear();

    /// \~chinese
    /// @brief ʹ�ô�ɫ�����Ⱦ����
    /// @param clear_color ������ɫ
    void Clear(Color const& clear_color);

    /// \~chinese
    /// @brief ��ȡ��Ⱦ�����С
    Size GetSize() const;

    /// \~chinese
    /// @brief ��ȡ��ˢ͸����
    float GetBrushOpacity() const;

    /// \~chinese
    /// @brief ��ȡ��ǰ��ˢ
    BrushPtr GetCurrentBrush() const;

    /// \~chinese
    /// @brief ��ȡȫ�ֶ�ά�任
    Matrix3x2 GetGlobalTransform() const;

    /// \~chinese
    /// @brief ���û�ˢ͸����
    void SetBrushOpacity(float opacity);

    /// \~chinese
    /// @brief ���õ�ǰ��ˢ
    void SetCurrentBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ���������ĵĶ�ά�任
    void SetTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief ����ȫ�ֶ�ά�任
    void SetGlobalTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief ����ȫ�ֶ�ά�任
    void SetGlobalTransform(const Matrix3x2* matrix);

    /// \~chinese
    /// @brief ���ÿ����ģʽ
    void SetAntialiasMode(bool enabled);

    /// \~chinese
    /// @brief �������ֿ����ģʽ
    void SetTextAntialiasMode(TextAntialiasMode mode);

    /// \~chinese
    /// @brief ���߽��Ƿ���������
    bool CheckVisibility(Rect const& bounds, Matrix3x2 const& transform);

    /// \~chinese
    /// @brief ������Ⱦ�����Ĵ�С
    void Resize(Size const& size);

public:
    /// \~chinese
    /// @brief ��Ⱦ������״̬
    struct Status
    {
        uint32_t primitives;  ///< ��ȾͼԪ����
        Time     start;       ///< ��Ⱦ��ʼʱ��
        Duration duration;    ///< ��Ⱦʱ��

        Status();
    };

    /// \~chinese
    /// @brief ���û����״̬�ռ�����
    void SetCollectingStatus(bool enable);

    /// \~chinese
    /// @brief ��ȡ��Ⱦ������״̬
    Status const& GetStatus() const;

protected:
    RenderContext();

    ComPtr<ID2D1RenderTarget> GetRenderTarget() const;

    ComPtr<ITextRenderer> GetTextRenderer() const;

private:
    /// \~chinese
    /// @brief �����豸������Դ
    HRESULT CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1RenderTarget> ctx);

    /// \~chinese
    /// @brief �����豸������Դ
    void DiscardDeviceResources();

    /// \~chinese
    /// @brief ������ȾͼԪ����
    void IncreasePrimitivesCount(uint32_t increase = 1) const;

    /// \~chinese
    /// @brief �������״̬
    void SaveDrawingState();

    /// \~chinese
    /// @brief �ָ�����״̬
    void RestoreDrawingState();

private:
    bool              antialias_;
    bool              fast_global_transform_;
    mutable bool      collecting_status_;
    float             brush_opacity_;
    TextAntialiasMode text_antialias_;
    BrushPtr          current_brush_;
    Rect              visible_size_;
    Matrix3x2         global_transform_;
    mutable Status    status_;

    ComPtr<ITextRenderer>          text_renderer_;
    ComPtr<ID2D1RenderTarget>      render_target_;
    ComPtr<ID2D1DrawingStateBlock> drawing_state_;
};

/** @} */

inline RenderContext::Status::Status()
    : primitives(0)
{
}

inline RenderContext::Status const& RenderContext::GetStatus() const
{
    return status_;
}

inline ComPtr<ID2D1RenderTarget> RenderContext::GetRenderTarget() const
{
    KGE_ASSERT(render_target_);
    return render_target_;
}

inline ComPtr<ITextRenderer> RenderContext::GetTextRenderer() const
{
    KGE_ASSERT(text_renderer_);
    return text_renderer_;
}

inline float RenderContext::GetBrushOpacity() const
{
    return brush_opacity_;
}

inline BrushPtr RenderContext::GetCurrentBrush() const
{
    return current_brush_;
}

inline Matrix3x2 RenderContext::GetGlobalTransform() const
{
    return global_transform_;
}

inline void RenderContext::SetBrushOpacity(float opacity)
{
    brush_opacity_ = opacity;
}

inline void RenderContext::SetGlobalTransform(const Matrix3x2& matrix)
{
    SetGlobalTransform(&matrix);
}

inline void RenderContext::SetCurrentBrush(BrushPtr brush)
{
    current_brush_ = brush;
    if (current_brush_)
    {
        current_brush_->SetOpacity(brush_opacity_);
    }
}

}  // namespace kiwano
