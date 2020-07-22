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
#include <kiwano/render/ShapeMaker.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(ActionWalkEntity);

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
    /// @param path ·����״
    /// @param rotating �Ƿ���·�����߷�����ת
    /// @param start ·����㣨�ٷֱȣ�
    /// @param end ·���յ㣨�ٷֱȣ�
    ActionWalk(Duration duration, ShapePtr path, bool rotating = false, float start = 0.f, float end = 1.f);
};

/// \~chinese
/// @brief ·�����߶���ʵ��
class KGE_API ActionWalkEntity : public ActionTweenEntity
{
public:
    /// \~chinese
    /// @brief ����·�����߶���
    /// @param duration ����ʱ��
    /// @param path ·����״
    /// @param rotating �Ƿ���·�����߷�����ת
    /// @param start ·����㣨�ٷֱȣ�
    /// @param end ·���յ㣨�ٷֱȣ�
    ActionWalkEntity(Duration duration, ShapePtr path, bool rotating = false, float start = 0.f, float end = 1.f);

    /// \~chinese
    /// @brief ��ȡ·��
    ShapePtr GetPath() const;

    /// \~chinese
    /// @brief �Ƿ���·�����߷�����ת
    bool IsRotating() const;

    /// \~chinese
    /// @brief ��ȡ·����㣨�ٷֱȣ�
    float GetStartValue() const;

    /// \~chinese
    /// @brief ��ȡ·���յ㣨�ٷֱȣ�
    float GetEndValue() const;

    /// \~chinese
    /// @brief ����·����״
    void SetPath(ShapePtr path);

    /// \~chinese
    /// @brief ������·�����߷�����ת
    void SetRotating(bool rotating);

    /// \~chinese
    /// @brief ����·����㣨�ٷֱȣ�
    void SetStartValue(float start);

    /// \~chinese
    /// @brief ����·���յ㣨�ٷֱȣ�
    void SetEndValue(float end);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionWalkEntity* Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionWalkEntity* Reverse() const override;

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

inline ShapePtr ActionWalkEntity::GetPath() const
{
    return path_;
}

inline bool ActionWalkEntity::IsRotating() const
{
    return rotating_;
}

inline float ActionWalkEntity::GetStartValue() const
{
    return start_;
}

inline float ActionWalkEntity::GetEndValue() const
{
    return end_;
}

inline void ActionWalkEntity::SetPath(ShapePtr path)
{
    path_ = path;
}

inline void ActionWalkEntity::SetRotating(bool rotating)
{
    rotating_ = rotating;
}

inline void ActionWalkEntity::SetStartValue(float start)
{
    start_ = start;
}

inline void ActionWalkEntity::SetEndValue(float end)
{
    end_ = end;
}

}  // namespace kiwano
