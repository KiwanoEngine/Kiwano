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
#include <kiwano/render/NativeObject.h>
#include <kiwano/render/Texture.h>

namespace kiwano
{

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

    LinearGradientStyle(const Point& begin, const Point& end, const Vector<GradientStop>& stops,
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

    RadialGradientStyle(const Point& center, const Vec2& offset, const Vec2& radius, const Vector<GradientStop>& stops,
                        GradientExtendMode extend_mode = GradientExtendMode::Clamp);
};

/**
 * \~chinese
 * @brief ��ˢ
 */
class KGE_API Brush : public NativeObject
{
public:
    /// \~chinese
    /// @brief ������ɫ��ˢ
    /// @param color ��ˢ��ɫ
    static BrushPtr Create(const Color& color);

    /// \~chinese
    /// @brief �������Խ�����ʽ
    /// @param style ���Խ�����ʽ
    static BrushPtr Create(const LinearGradientStyle& style);

    /// \~chinese
    /// @brief �������򽥱���ʽ
    /// @param style ���򽥱���ʽ
    static BrushPtr Create(const RadialGradientStyle& style);

    Brush();

    /// \~chinese
    /// @brief ���ô�ɫ��ˢ��ɫ
    void SetColor(const Color& color);

    /// \~chinese
    /// @brief �������Խ�����ʽ
    void SetStyle(const LinearGradientStyle& style);

    /// \~chinese
    /// @brief ���þ��򽥱���ʽ
    void SetStyle(const RadialGradientStyle& style);

    /// \~chinese
    /// @brief ��������
    void SetTexture(TexturePtr texture);

    /// \~chinese
    /// @brief ���ö�ά�任
    void SetTransform(const Transform& transform);

    /// \~chinese
    /// @brief ���ö�ά�任
    void SetTransform(const Matrix3x2& transform);

public:
    /// \~chinese
    /// @brief ��ˢ����
    enum class Type
    {
        None,
        SolidColor,      ///< ��ɫ��仭ˢ
        LinearGradient,  ///< ���Խ��仭ˢ
        RadialGradient,  ///< ���򽥱仭ˢ
        Texture          ///< ����ˢ
    };

    /// \~chinese
    /// @brief ��ȡ��ˢ����
    Type GetType() const;

private:
    Type type_;
};

/** @} */

inline Brush::Type Brush::GetType() const
{
    return type_;
}

}  // namespace kiwano
