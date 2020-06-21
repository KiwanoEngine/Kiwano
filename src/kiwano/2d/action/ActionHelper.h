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
#include <kiwano/2d/action/ActionDelay.h>
#include <kiwano/2d/action/ActionGroup.h>
#include <kiwano/2d/action/ActionTween.h>
#include <kiwano/2d/action/ActionWalk.h>
#include <kiwano/2d/action/Animation.h>

namespace kiwano
{
/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief ����������
struct ActionHelper
{
    /// \~chinese
    /// @brief ����ѭ������
    inline ActionHelper& Loops(int loops)
    {
        ptr->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö����ӳ�
    inline ActionHelper& Delay(Duration delay)
    {
        ptr->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö��������ص�����
    inline ActionHelper& DoneCallback(const Action::DoneCallback& cb)
    {
        ptr->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö���ѭ������ʱ�Ļص�����
    inline ActionHelper& LoopDoneCallback(const Action::DoneCallback& cb)
    {
        ptr->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ��������ʱ�Ƴ�Ŀ���ɫ
    inline ActionHelper& RemoveTargetWhenDone()
    {
        ptr->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief ��������
    inline ActionHelper& Name(const String& name)
    {
        ptr->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief ��ȡָ��
    inline ActionPtr Get() const
    {
        return ptr;
    }

    inline ActionHelper(ActionPtr ptr)
        : ptr(ptr)
    {
    }

    inline operator ActionPtr() const
    {
        return ptr;
    }

    inline ActionPtr operator->() const
    {
        return ptr;
    }

private:
    ActionPtr ptr;
};

/// \~chinese
/// @brief ���䶯��������
struct TweenHelper
{
    /// \~chinese
    /// @brief ���ö�������ʱ��
    inline TweenHelper& Dur(Duration dur)
    {
        ptr->SetDuration(dur);
        return (*this);
    }

    /// \~chinese
    /// @brief ����ѭ������
    inline TweenHelper& Loops(int loops)
    {
        ptr->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief ���û�������
    inline TweenHelper& EaseFunc(EaseFunc ease)
    {
        ptr->SetEaseFunc(ease);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö����ӳ�
    inline TweenHelper& Delay(Duration delay)
    {
        ptr->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö��������ص�����
    inline TweenHelper& DoneCallback(const Action::DoneCallback& cb)
    {
        ptr->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö���ѭ������ʱ�Ļص�����
    inline TweenHelper& LoopDoneCallback(const Action::DoneCallback& cb)
    {
        ptr->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ��������ʱ�Ƴ�Ŀ���ɫ
    inline TweenHelper& RemoveTargetWhenDone()
    {
        ptr->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief ��������
    inline TweenHelper& Name(const String& name)
    {
        ptr->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief ��ȡָ��
    inline ActionTweenPtr Get() const
    {
        return ptr;
    }

    inline TweenHelper(ActionTweenPtr ptr)
        : ptr(ptr)
    {
    }

    inline operator ActionPtr() const
    {
        return ptr;
    }

    inline operator ActionTweenPtr() const
    {
        return ptr;
    }

    inline ActionTweenPtr operator->() const
    {
        return ptr;
    }

private:
    ActionTweenPtr ptr;
};

/// \~chinese
/// @brief ����������
struct Tween
{
public:
    /// \~chinese
    /// @brief �������λ�ƶ���
    /// @param dur ����ʱ��
    /// @param vector �ƶ�����
    static inline TweenHelper MoveBy(Duration dur, const Point& vector)
    {
        return TweenHelper(ActionMoveBy::Create(dur, vector));
    }

    /// \~chinese
    /// @brief ����λ�ƶ���
    /// @param dur ����ʱ��
    /// @param pos Ŀ������
    static inline TweenHelper MoveTo(Duration dur, const Point& pos)
    {
        return TweenHelper(ActionMoveTo::Create(dur, pos));
    }

    /// \~chinese
    /// @brief ���������Ծ����
    /// @param dur ����ʱ��
    /// @param vec ��Ծλ������
    /// @param height ��Ծ�߶�
    /// @param jumps ��Ծ����
    static inline TweenHelper JumpBy(Duration dur, const Vec2& vec, float height, int jumps = 1)
    {
        return TweenHelper(ActionJumpBy::Create(dur, vec, height, jumps));
    }

    /// \~chinese
    /// @brief ������Ծ����
    /// @param dur ����ʱ��
    /// @param pos Ŀ������
    /// @param height ��Ծ�߶�
    /// @param jumps ��Ծ����
    static inline TweenHelper JumpTo(Duration dur, const Point& pos, float height, int jumps = 1)
    {
        return TweenHelper(ActionJumpTo::Create(dur, pos, height, jumps));
    }

    /// \~chinese
    /// @brief ����������Ŷ���
    /// @param dur ����ʱ��
    /// @param scale_x ����������Ա仯ֵ
    /// @param scale_y ����������Ա仯ֵ
    static inline TweenHelper ScaleBy(Duration dur, float scale_x, float scale_y)
    {
        return TweenHelper(ActionScaleBy::Create(dur, scale_x, scale_y));
    }

    /// \~chinese
    /// @brief �������Ŷ���
    /// @param dur ����ʱ��
    /// @param scale_x ��������Ŀ��ֵ
    /// @param scale_y ��������Ŀ��ֵ
    static inline TweenHelper ScaleTo(Duration dur, float scale_x, float scale_y)
    {
        return TweenHelper(ActionScaleTo::Create(dur, scale_x, scale_y));
    }

    /// \~chinese
    /// @brief ����͸���Ƚ��䶯��
    /// @param dur ����ʱ��
    /// @param opacity Ŀ��͸����
    static inline TweenHelper FadeTo(Duration dur, float opacity)
    {
        return TweenHelper(ActionFadeTo::Create(dur, opacity));
    }

    /// \~chinese
    /// @brief ���쵭�붯��
    /// @param dur ����ʱ��
    static inline TweenHelper FadeIn(Duration dur)
    {
        return TweenHelper(ActionFadeIn::Create(dur));
    }

    /// \~chinese
    /// @brief ���쵭������
    /// @param dur ����ʱ��
    static inline TweenHelper FadeOut(Duration dur)
    {
        return TweenHelper(ActionFadeOut::Create(dur));
    }

    /// \~chinese
    /// @brief ���������ת����
    /// @param dur ����ʱ��
    /// @param rotation �Ƕ���Ա仯ֵ
    static inline TweenHelper RotateBy(Duration dur, float rotation)
    {
        return TweenHelper(ActionRotateBy::Create(dur, rotation));
    }

    /// \~chinese
    /// @brief ������ת����
    /// @param dur ����ʱ��
    /// @param rotation Ŀ��Ƕ�
    static inline TweenHelper RotateTo(Duration dur, float rotation)
    {
        return TweenHelper(ActionRotateTo::Create(dur, rotation));
    }

    /// \~chinese
    /// @brief ����·�����߶���
    /// @param dur ����ʱ��
    /// @param path ·����״
    /// @param rotating �Ƿ���·�����߷�����ת
    /// @param start ·����㣨�ٷֱȣ�
    /// @param end ·���յ㣨�ٷֱȣ�
    static inline TweenHelper Walk(Duration dur, ShapePtr path, bool rotating = false, float start = 0.f,
                                   float end = 1.f)
    {
        return TweenHelper(ActionWalk::Create(dur, path, rotating, start, end));
    }

    /// \~chinese
    /// @brief ����֡����
    /// @param dur ����ʱ��
    /// @param[in] frame_seq ����֡
    static inline TweenHelper Animation(Duration dur, FrameSequencePtr frames)
    {
        return TweenHelper(Animation::Create(dur, frames));
    }

    /// \~chinese
    /// @brief �����Զ��嶯��
    /// @param dur ����ʱ��
    /// @param tween_func �����ص�����
    static inline TweenHelper Custom(Duration dur, ActionCustom::TweenFunc tween_func)
    {
        return TweenHelper(ActionCustom::Create(dur, tween_func));
    }

    /// \~chinese
    /// @brief ������ʱ����
    /// @param delay ��ʱʱ��
    static inline ActionHelper Delay(Duration delay)
    {
        return ActionHelper(ActionDelay::Create(delay));
    }

    /// \~chinese
    /// @brief �������
    /// @param actions ��������
    /// @param sync ͬ��ִ��
    static inline ActionHelper Group(const Vector<ActionPtr>& actions, bool sync = false)
    {
        return ActionHelper(ActionGroup::Create(actions, sync));
    }
};

/** @} */
}  // namespace kiwano
