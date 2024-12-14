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
#include <kiwano/core/Time.h>
#include <kiwano/render/Brush.h>
#include <kiwano/render/Shape.h>
#include <kiwano/render/Layer.h>
#include <kiwano/render/TextLayout.h>
#include <kiwano/render/Texture.h>

namespace kiwano
{

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
/// @brief ���ģʽ
enum class BlendMode
{
    SourceOver = 0,
    Copy       = 1,
    Min        = 2,
    Add        = 3,
    Max        = 4,
};

/// \~chinese
/// @brief ��Ⱦ������
/// @details ��Ⱦ�����Ľ���ɻ���ͼԪ�Ļ��ƣ��������ƽ��������ض���ƽ����
class KGE_API RenderContext : public NativeObject
{
public:
    /// \~chinese
    /// @brief ����������Ⱦ�����ģ������ƽ�������������
    /// @param texture ������ƽ��������
    /// @param size ��Ⱦ�����С
    static RefPtr<RenderContext> Create(RefPtr<Texture> texture, const PixelSize& size);

    /// \~chinese
    /// @brief ��ʼ��Ⱦ
    virtual void BeginDraw();

    /// \~chinese
    /// @brief ������Ⱦ
    virtual void EndDraw();

    /// \~chinese
    /// @brief ����������
    /// @param[out] texture �������
    /// @param[in] size �������ش�С
    virtual void CreateTexture(Texture& texture, const PixelSize& size) = 0;

    /// \~chinese
    /// @brief ��������
    /// @param texture ����
    /// @param src_rect Դ����ü�����
    /// @param dest_rect ���Ƶ�Ŀ������
    virtual void DrawTexture(const Texture& texture, const Rect* src_rect = nullptr,
                             const Rect* dest_rect = nullptr) = 0;

    /// \~chinese
    /// @brief �����ı�����
    /// @param layout �ı�����
    /// @param offset ƫ����
    /// @param outline_brush ��߻�ˢ
    virtual void DrawTextLayout(const TextLayout& layout, const Point& offset, RefPtr<Brush> outline_brush) = 0;

    /// \~chinese
    /// @brief ������״����
    /// @param shape ��״
    virtual void DrawShape(const Shape& shape) = 0;

    /// \~chinese
    /// @brief �����߶�
    /// @param point1 �߶����
    /// @param point2 �߶��յ�
    virtual void DrawLine(const Point& point1, const Point& point2) = 0;

    /// \~chinese
    /// @brief ���ƾ��α߿�
    /// @param rect ����
    virtual void DrawRectangle(const Rect& rect) = 0;

    /// \~chinese
    /// @brief ����Բ�Ǿ��α߿�
    /// @param rect ����
    /// @param radius Բ�ǰ뾶
    virtual void DrawRoundedRectangle(const Rect& rect, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief ����Բ�α߿�
    /// @param center Բ��
    /// @param radius ��Բ�뾶
    virtual void DrawCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief ������Բ�߿�
    /// @param center Բ��
    /// @param radius ��Բ�뾶
    virtual void DrawEllipse(const Point& center, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief �����״
    /// @param shape ��״
    virtual void FillShape(const Shape& shape) = 0;

    /// \~chinese
    /// @brief ������
    /// @param rect ����
    virtual void FillRectangle(const Rect& rect) = 0;

    /// \~chinese
    /// @brief ���Բ�Ǿ���
    /// @param rect ����
    /// @param radius Բ�ǰ뾶
    virtual void FillRoundedRectangle(const Rect& rect, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief ���Բ��
    /// @param center Բ��
    /// @param radius ��Բ�뾶
    virtual void FillCircle(const Point& center, float radius);

    /// \~chinese
    /// @brief �����Բ
    /// @param center Բ��
    /// @param radius ��Բ�뾶
    virtual void FillEllipse(const Point& center, const Vec2& radius) = 0;

    /// \~chinese
    /// @brief ���û��ƵĲü�����
    /// @param clip_rect �ü�����
    virtual void PushClipRect(const Rect& clip_rect) = 0;

    /// \~chinese
    /// @brief ȡ����һ�����õĻ��Ʋü�����
    virtual void PopClipRect() = 0;

    /// \~chinese
    /// @brief ����ͼ������
    /// @param layer ͼ��
    virtual void PushLayer(Layer& layer) = 0;

    /// \~chinese
    /// @brief ȡ����һ�����õ�ͼ������
    virtual void PopLayer() = 0;

    /// \~chinese
    /// @brief �����Ⱦ����
    virtual void Clear() = 0;

    /// \~chinese
    /// @brief ʹ�ô�ɫ�����Ⱦ����
    /// @param clear_color ������ɫ
    virtual void Clear(const Color& clear_color) = 0;

    /// \~chinese
    /// @brief ��ȡ��Ⱦ�����С
    virtual Size GetSize() const = 0;

    /// \~chinese
    /// @brief ��ȡ��ˢ͸����
    virtual float GetBrushOpacity() const;

    /// \~chinese
    /// @brief ��ȡ��ǰ��ˢ
    virtual RefPtr<Brush> GetCurrentBrush() const;

    /// \~chinese
    /// @brief ��ȡȫ�ֶ�ά�任
    virtual const Matrix3x2& GetGlobalTransform() const;

    /// \~chinese
    /// @brief ���û�ˢ͸����
    virtual void SetBrushOpacity(float opacity);

    /// \~chinese
    /// @brief ���õ�ǰʹ�õĻ�ˢ
    virtual void SetCurrentBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief ���õ�ǰʹ�õ�������ʽ
    virtual void SetCurrentStrokeStyle(RefPtr<StrokeStyle> stroke);

    /// \~chinese
    /// @brief ���û��ģʽ
    virtual void SetBlendMode(BlendMode blend) = 0;

    /// \~chinese
    /// @brief ���ÿ����ģʽ
    virtual void SetAntialiasMode(bool enabled) = 0;

    /// \~chinese
    /// @brief �������ֿ����ģʽ
    virtual void SetTextAntialiasMode(TextAntialiasMode mode) = 0;

    /// \~chinese
    /// @brief ���߽��Ƿ���������
    virtual bool CheckVisibility(const Rect& bounds, const Matrix3x2& transform) = 0;

    /// \~chinese
    /// @brief ������Ⱦ�����Ĵ�С
    virtual void Resize(const Size& size) = 0;

    /// \~chinese
    /// @brief ���������ĵĶ�ά�任
    virtual void SetTransform(const Matrix3x2& matrix) = 0;

    /// \~chinese
    /// @brief ����ȫ�ֶ�ά�任
    virtual void SetGlobalTransform(const Matrix3x2& matrix);

    /// \~chinese
    /// @brief ����ȫ�ֶ�ά�任
    virtual void SetGlobalTransform(const Matrix3x2* matrix);

    /// \~chinese
    /// @brief ��ȡ��ȾĿ��
    virtual RefPtr<Texture> GetTarget() const = 0;

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
    const Status& GetStatus() const;

protected:
    RenderContext();

    /// \~chinese
    /// @brief ������ȾͼԪ����
    void IncreasePrimitivesCount(uint32_t increase = 1) const;

protected:
    bool                antialias_;
    bool                fast_global_transform_;
    mutable bool        collecting_status_;
    float               brush_opacity_;
    TextAntialiasMode   text_antialias_;
    RefPtr<Brush>       current_brush_;
    RefPtr<StrokeStyle> current_stroke_;
    Rect                visible_size_;
    Matrix3x2           global_transform_;
    mutable Status      status_;
};

/** @} */

inline RenderContext::Status::Status()
    : primitives(0)
{
}

inline const RenderContext::Status& RenderContext::GetStatus() const
{
    return status_;
}

}  // namespace kiwano
