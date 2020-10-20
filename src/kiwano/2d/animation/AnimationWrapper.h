// Copyright (c) 2020-2021 Kiwano - Nomango
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
#include <kiwano/2d/animation/Animation.h>
#include <kiwano/2d/animation/TweenAnimation.h>
#include <kiwano/2d/animation/PathAnimation.h>
#include <kiwano/2d/animation/DelayAnimation.h>
#include <kiwano/2d/animation/FrameAnimation.h>
#include <kiwano/2d/animation/CustomAnimation.h>
#include <kiwano/2d/animation/AnimationGroup.h>

/**
 * \addtogroup Animation
 * @{
 */

namespace kiwano
{

/// \~chinese
/// @brief ������װ��
class KGE_API AnimationWrapper
{
public:
    AnimationWrapper() = default;

    inline AnimationWrapper(AnimationPtr ptr)
        : ptr(ptr)
    {
    }

    /// \~chinese
    /// @brief ����ѭ������
    inline AnimationWrapper& Loops(int loops)
    {
        if (ptr)
            ptr->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö����ӳ�
    inline AnimationWrapper& Delay(Duration delay)
    {
        if (ptr)
            ptr->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö��������ص�����
    inline AnimationWrapper& Handler(AnimationEventHandlerPtr handler)
    {
        if (ptr)
            ptr->SetHandler(handler);
        return (*this);
    }

    /// \~chinese
    /// @brief ��������ʱ�Ƴ�Ŀ���ɫ
    inline AnimationWrapper& RemoveTargetWhenDone()
    {
        if (ptr)
            ptr->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief ��������
    inline AnimationWrapper& Name(const String& name)
    {
        if (ptr)
            ptr->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief ���û�������
    inline AnimationWrapper& Ease(const EaseFunc& ease)
    {
        auto tween = dynamic_cast<TweenAnimation*>(ptr.Get());
        if (tween)
        {
            tween->SetEaseFunc(ease);
        }
        return (*this);
    }

    /// \~chinese
    /// @brief ��¡����
    inline AnimationWrapper Clone() const
    {
        if (ptr)
            return AnimationWrapper(ptr->Clone());
        return AnimationWrapper();
    }

    /// \~chinese
    /// @brief ��ȡ���򶯻�
    inline AnimationWrapper Reverse() const
    {
        if (ptr)
            return AnimationWrapper(ptr->Reverse());
        return AnimationWrapper();
    }

    /// \~chinese
    /// @brief ��ȡָ��
    inline Animation* Get() const
    {
        return const_cast<Animation*>(ptr.Get());
    }

    /// \~chinese
    /// @brief ���ö���
    inline void SetEntity(AnimationPtr ptr)
    {
        this->ptr = ptr;
    }

    inline Animation* operator->() const
    {
        return Get();
    }

    inline operator Animation*() const
    {
        return Get();
    }

    inline operator AnimationPtr() const
    {
        return ptr;
    }

    inline operator bool() const
    {
        return ptr != nullptr;
    }

protected:
    AnimationPtr ptr;
};

namespace animation
{

/// \~chinese
/// @brief �������λ�ƶ���
/// @param duration ����ʱ��
/// @param displacement λ������
inline AnimationWrapper MoveBy(kiwano::Duration duration, const Vec2& displacement)
{
    return AnimationWrapper(new MoveByAnimation(duration, displacement));
}

/// \~chinese
/// @brief ����λ�ƶ���
/// @param duration ����ʱ��
/// @param distination Ŀ������
inline AnimationWrapper MoveTo(kiwano::Duration duration, const Point& distination)
{
    return AnimationWrapper(new MoveToAnimation(duration, distination));
}

/// \~chinese
/// @brief ���������Ծ����
/// @param duration ����ʱ��
/// @param displacement ��Ծλ������
/// @param height ��Ծ�߶�
/// @param count ��Ծ����
inline AnimationWrapper JumpBy(kiwano::Duration duration, const Vec2& displacement, float height, int count = 1)
{
    return AnimationWrapper(new JumpByAnimation(duration, displacement, height, count));
}

/// \~chinese
/// @brief ������Ծ����
/// @param duration ����ʱ��
/// @param distination Ŀ������
/// @param height ��Ծ�߶�
/// @param count ��Ծ����
inline AnimationWrapper JumpTo(kiwano::Duration duration, const Point& distination, float height, int count = 1)
{
    return AnimationWrapper(new JumpToAnimation(duration, distination, height, count));
}

/// \~chinese
/// @brief ����������Ŷ���
/// @param duration ����ʱ��
/// @param scale ������Ա仯ֵ
inline AnimationWrapper ScaleBy(kiwano::Duration duration, Vec2 scale)
{
    return AnimationWrapper(new ScaleByAnimation(duration, scale));
}

/// \~chinese
/// @brief �������Ŷ���
/// @param duration ����ʱ��
/// @param scale ����Ŀ��ֵ
inline AnimationWrapper ScaleTo(kiwano::Duration duration, Vec2 scale)
{
    return AnimationWrapper(new ScaleToAnimation(duration, scale));
}

/// \~chinese
/// @brief ����͸���Ƚ��䶯��
/// @param duration ����ʱ��
/// @param opacity Ŀ��͸����
inline AnimationWrapper FadeTo(kiwano::Duration duration, float opacity)
{
    return AnimationWrapper(new FadeToAnimation(duration, opacity));
}

/// \~chinese
/// @brief �������붯��
/// @param duration ����ʱ��
inline AnimationWrapper FadeIn(kiwano::Duration duration)
{
    return AnimationWrapper(new FadeToAnimation(duration, 1.0f));
}

/// \~chinese
/// @brief ������������
/// @param duration ����ʱ��
inline AnimationWrapper FadeOut(kiwano::Duration duration)
{
    return AnimationWrapper(new FadeToAnimation(duration, 0.0f));
}

/// \~chinese
/// @brief ���������ת����
/// @param duration ����ʱ��
/// @param rotation �Ƕ���Ա仯ֵ
inline AnimationWrapper RotateBy(kiwano::Duration duration, float rotation)
{
    return AnimationWrapper(new RotateByAnimation(duration, rotation));
}

/// \~chinese
/// @brief ������ת����
/// @param duration ����ʱ��
/// @param rotation Ŀ��Ƕ�
inline AnimationWrapper RotateTo(kiwano::Duration duration, float rotation)
{
    return AnimationWrapper(new RotateToAnimation(duration, rotation));
}

/// \~chinese
/// @brief �����Զ��嶯��
/// @param duration ����ʱ��
/// @param tween_func �����ص�����
inline AnimationWrapper Custom(kiwano::Duration duration, Function<void(Actor*, float)> tween_func)
{
    return AnimationWrapper(CustomAnimation::Create(duration, tween_func));
}

/// \~chinese
/// @brief ����·�����߶���
/// @param duration ����ʱ��
/// @param path ·����״
/// @param rotating �Ƿ���·�����߷�����ת
/// @param start ·����㣨�ٷֱȣ�
/// @param end ·���յ㣨�ٷֱȣ�
inline AnimationWrapper Path(kiwano::Duration duration, ShapePtr path, bool rotating = false, float start = 0.f,
                             float end = 1.f)
{
    return AnimationWrapper(new PathAnimation(duration, path, rotating, start, end));
}

/// \~chinese
/// @brief ������ʱ����
/// @param delay ��ʱʱ��
inline AnimationWrapper Delay(kiwano::Duration delay)
{
    return AnimationWrapper(new DelayAnimation(delay));
}

/// \~chinese
/// @brief ����֡����
/// @param duration ����ʱ��
/// @param frame_seq ����֡
inline AnimationWrapper Frames(kiwano::Duration duration, FrameSequencePtr frame_seq)
{
    return AnimationWrapper(new FrameAnimation(duration, frame_seq));
}

/// \~chinese
/// @brief �����������
/// @param actions ��������
/// @param parallel ͬ��ִ��
inline AnimationWrapper Group(const Vector<AnimationPtr>& animations, bool parallel = false)
{
    return AnimationWrapper(new AnimationGroup(animations, parallel));
}

}  // namespace animation
}  // namespace kiwano

/** @} */
