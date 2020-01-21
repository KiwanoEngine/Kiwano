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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/render/dx/D2DDeviceResources.h>

namespace kiwano
{
class RenderContext;
class Renderer;

KGE_DECLARE_SMART_PTR(Brush);

/**
 * \addtogroup Render
 * @{
 */

/// \~chinese
/// @brief ����ת����
struct GradientStop
{
    float offset;  ///< ƫ�ƾ���
    Color color;   ///< �������ɫ

    GradientStop();

    GradientStop(float offset, Color color);
};

/// \~chinese
/// @brief ��������ģʽ
/// @details ��ģʽ����ָ��������λ�������������Ĳ���
enum class GradientExtendMode
{
    Clamp,  ///< ��ģʽ���ظ����Ʊ߽���ɫ
    Wrap,   ///< ����ģʽ���ظ���������
    Mirror  ///< ����ģʽ����ת��������
};

/// \~chinese
/// @brief ���Խ�����ʽ
struct LinearGradientStyle
{
    Point                begin;        ///< ������ʼ��
    Point                end;          ///< ������ֹ��
    Vector<GradientStop> stops;        ///< ����ת���㼯��
    GradientExtendMode   extend_mode;  ///< ��������ģʽ

    LinearGradientStyle(Point const& begin, Point const& end, Vector<GradientStop> const& stops,
                        GradientExtendMode extend_mode = GradientExtendMode::Clamp);
};

/// \~chinese
/// @brief ���򽥱���ʽ
struct RadialGradientStyle
{
    Point                center;       ///< ���򽥱�Բ��
    Vec2                 offset;       ///< ���򽥱�ƫ��
    Vec2                 radius;       ///< ���򽥱�뾶
    Vector<GradientStop> stops;        ///< ����ת���㼯��
    GradientExtendMode   extend_mode;  ///< ��������ģʽ

    RadialGradientStyle(Point const& center, Vec2 const& offset, Vec2 const& radius, Vector<GradientStop> const& stops,
                        GradientExtendMode extend_mode = GradientExtendMode::Clamp);
};

/**
 * \~chinese
 * @brief ��ˢ
 */
class KGE_API Brush : public virtual ObjectBase
{
    friend class RenderContext;
    friend class Renderer;

public:
    /// \~chinese
    /// @brief ����Ĭ�ϻ�ˢ
    Brush();

    /// \~chinese
    /// @brief �Ƿ���Ч
    bool IsValid() const;

    /// \~chinese
    /// @brief ���ô�ɫ��ˢ��ɫ
    void SetColor(Color const& color);

    /// \~chinese
    /// @brief �������Խ�����ʽ
    void SetStyle(LinearGradientStyle const& style);

    /// \~chinese
    /// @brief ���þ��򽥱���ʽ
    void SetStyle(RadialGradientStyle const& style);

public:
    /// \~chinese
    /// @brief ��ˢ����
    enum class Type
    {
        Unknown,
        SolidColor,      ///< ��ɫ��仭ˢ
        LinearGradient,  ///< ���Խ��仭ˢ
        RadialGradient   ///< ���򽥱仭ˢ
    };

    /// \~chinese
    /// @brief ��ȡ��ˢ����
    Type GetType() const;

private:
    /// \~chinese
    /// @brief ��ȡ͸����
    float GetOpacity() const;

    /// \~chinese
    /// @brief ����͸����
    void SetOpacity(float opacity);

    void SetBrush(ComPtr<ID2D1Brush> brush, Type type);

    ComPtr<ID2D1Brush> GetBrush() const;

private:
    Type               type_;
    float              opacity_;
    ComPtr<ID2D1Brush> raw_;
};

/** @} */

inline Brush::Type Brush::GetType() const
{
    return type_;
}

inline ComPtr<ID2D1Brush> Brush::GetBrush() const
{
    return raw_;
}
}  // namespace kiwano
