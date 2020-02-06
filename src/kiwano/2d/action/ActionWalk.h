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
#include <kiwano/2d/action/ActionTween.h>
#include <kiwano/render/Shape.h>
#include <kiwano/render/ShapeSink.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(ActionWalk);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief ·�����߶���
class KGE_API ActionWalk : public ActionTween
{
public:
    /// \~chinese
    /// @brief ����·�����߶���
    /// @param duration ����ʱ��
    /// @param rotating �Ƿ���·�����߷�����ת
    /// @param start ·����㣨�ٷֱȣ�
    /// @param end ·���յ㣨�ٷֱȣ�
    /// @param func �����ٶȻ�������
    ActionWalk(Duration duration, bool rotating = false, float start = 0.f, float end = 1.f, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ����·�����߶���
    /// @param duration ����ʱ��
    /// @param path ·����״
    /// @param rotating �Ƿ���·�����߷�����ת
    /// @param start ·����㣨�ٷֱȣ�
    /// @param end ·���յ㣨�ٷֱȣ�
    /// @param func �����ٶȻ�������
    ActionWalk(Duration duration, ShapePtr path, bool rotating = false, float start = 0.f, float end = 1.f,
               EaseFunc func = nullptr);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionPtr Reverse() const override;

    /// \~chinese
    /// @brief ��ȡ·��
    ShapePtr const& GetPath() const;

    /// \~chinese
    /// @brief ����·����״
    void SetPath(ShapePtr path);

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    bool     rotating_;
    float    start_;
    float    end_;
    float    length_;
    Point    start_pos_;
    ShapePtr path_;
};

/** @} */

inline ShapePtr const& ActionWalk::GetPath() const
{
    return path_;
}

inline void ActionWalk::SetPath(ShapePtr path)
{
    path_ = path;
}
}  // namespace kiwano
