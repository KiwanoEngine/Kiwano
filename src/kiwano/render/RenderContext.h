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
    virtual bool IsValid() const = 0;

    /// \~chinese
    /// @brief ��ʼ��Ⱦ
    virtual void BeginDraw();

    /// \~chinese
    /// @brief ������Ⱦ
    virtual void EndDraw();

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    /// @param src_rect Դ����ü�����
    /// @param dest_rect ���Ƶ�Ŀ������
    virtual void DrawTexture(Texture const& texture, const Rect* src_rect = nullptr,
                             const Rect* dest_rect = nullptr) = 0;

    /// \~chinese
    /// @brief �����ı�����
    /// @param layout �ı�����
    /// @param offset ƫ����
    virtual void DrawTextLayout(TextLayout const& layout, Point const& offset = Point()) = 0;

    /// \~chinese
    /// @brief ������״����
    /// @param shape ��״
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    virtual void DrawShape(Shape const& shape, StrokeStylePtr stroke = nullptr, float stroke_width = 1.0f) = 0;

    /// \~chinese
    /// @brief �����߶�
    /// @param point1 �߶����
    /// @param point2 �߶��յ�
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    virtual void DrawLine(Point const& point1, Point const& point2, StrokeStylePtr stroke = nullptr,
                          float stroke_width = 1.0f) = 0;

    /// \~chinese
    /// @brief ���ƾ��α߿�
    /// @param rect ����
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    virtual void DrawRectangle(Rect const& rect, StrokeStylePtr stroke = nullptr, float stroke_width = 1.0f) = 0;

    /// \~chinese
    /// @brief ����Բ�Ǿ��α߿�
    /// @param rect ����
    /// @param radius Բ�ǰ뾶
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    virtual void DrawRoundedRectangle(Rect const& rect, Vec2 const& radius, StrokeStylePtr stroke = nullptr,
                                      float stroke_width = 1.0f) = 0;

    /// \~chinese
    /// @brief ������Բ�߿�
    /// @param center Բ��
    /// @param radius ��Բ�뾶
    /// @param stroke ������ʽ
    /// @param stroke_width �������
    virtual void DrawEllipse(Point const& center, Vec2 const& radius, StrokeStylePtr stroke = nullptr,
                             float stroke_width = 1.0f) = 0;

    /// \~chinese
    /// @brief �����״
    /// @param shape ��״
    virtual void FillShape(Shape const& shape) = 0;

    /// \~chinese
    /// @brief ������
    /// @param rect ����
    virtual void FillRectangle(Rect const& rect) = 0;

    /// \~chinese
    /// @brief ���Բ�Ǿ���
    /// @param rect ����
    /// @param radius Բ�ǰ뾶
    virtual void FillRoundedRectangle(Rect const& rect, Vec2 const& radius) = 0;

    /// \~chinese
    /// @brief �����Բ
    /// @param center Բ��
    /// @param radius ��Բ�뾶
    virtual void FillEllipse(Point const& center, Vec2 const& radius) = 0;

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    /// @param size �������ش�С
    virtual void CreateTexture(Texture& texture, math::Vec2T<uint32_t> size) = 0;

    /// \~chinese
    /// @brief ���û��ƵĲü�����
    /// @param clip_rect �ü�����
    virtual void PushClipRect(Rect const& clip_rect) = 0;

    /// \~chinese
    /// @brief ȡ����һ�����õĻ��Ʋü�����
    virtual void PopClipRect() = 0;

    /// \~chinese
    /// @brief ����ͼ������
    /// @param layer ͼ������
    virtual void PushLayer(LayerArea& layer) = 0;

    /// \~chinese
    /// @brief ȡ����һ�����õ�ͼ������
    virtual void PopLayer() = 0;

    /// \~chinese
    /// @brief �����Ⱦ����
    virtual void Clear() = 0;

    /// \~chinese
    /// @brief ʹ�ô�ɫ�����Ⱦ����
    /// @param clear_color ������ɫ
    virtual void Clear(Color const& clear_color) = 0;

    /// \~chinese
    /// @brief ��ȡ��Ⱦ�����С
    virtual Size GetSize() const = 0;

    /// \~chinese
    /// @brief ��ȡ��ˢ͸����
    virtual float GetBrushOpacity() const;

    /// \~chinese
    /// @brief ��ȡ��ǰ��ˢ
    virtual BrushPtr GetCurrentBrush() const;

    /// \~chinese
    /// @brief ��ȡȫ�ֶ�ά�任
    virtual const Matrix3x2& GetGlobalTransform() const;

    /// \~chinese
    /// @brief ���û�ˢ͸����
    virtual void SetBrushOpacity(float opacity);

    /// \~chinese
    /// @brief ���õ�ǰ��ˢ
    virtual void SetCurrentBrush(BrushPtr brush);

    /// \~chinese
    /// @brief ���ÿ����ģʽ
    virtual void SetAntialiasMode(bool enabled) = 0;

    /// \~chinese
    /// @brief �������ֿ����ģʽ
    virtual void SetTextAntialiasMode(TextAntialiasMode mode) = 0;

    /// \~chinese
    /// @brief ���߽��Ƿ���������
    virtual bool CheckVisibility(Rect const& bounds, Matrix3x2 const& transform) = 0;

    /// \~chinese
    /// @brief ������Ⱦ�����Ĵ�С
    virtual void Resize(Size const& size) = 0;

    /// \~chinese
    /// @brief ���������ĵĶ�ά�任
    virtual void SetTransform(const Matrix3x2& matrix) = 0;

    /// \~chinese
    /// @brief ����ȫ�ֶ�ά�任
    virtual void SetGlobalTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief ����ȫ�ֶ�ά�任
    virtual void SetGlobalTransform(const Matrix3x2* matrix);

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

    /// \~chinese
    /// @brief ������ȾͼԪ����
    void IncreasePrimitivesCount(uint32_t increase = 1) const;

protected:
    bool              antialias_;
    bool              fast_global_transform_;
    mutable bool      collecting_status_;
    float             brush_opacity_;
    TextAntialiasMode text_antialias_;
    BrushPtr          current_brush_;
    Rect              visible_size_;
    Matrix3x2         global_transform_;
    mutable Status    status_;
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

}  // namespace kiwano
