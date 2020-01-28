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
    using DoneCallback = Action::DoneCallback;

    /// \~chinese
    /// @brief ����ѭ������
    inline ActionHelper& SetLoops(int loops)
    {
        core->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö����ӳ�
    inline ActionHelper& SetDelay(Duration delay)
    {
        core->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö��������ص�����
    inline ActionHelper& SetDoneCallback(DoneCallback const& cb)
    {
        core->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö���ѭ������ʱ�Ļص�����
    inline ActionHelper& SetLoopDoneCallback(DoneCallback const& cb)
    {
        core->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ��������ʱ�Ƴ�Ŀ���ɫ
    inline ActionHelper& RemoveTargetWhenDone()
    {
        core->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief ��������
    inline ActionHelper& SetName(String const& name)
    {
        core->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief ��ȡָ��
    inline ActionPtr Get() const
    {
        return core;
    }

    inline ActionHelper(ActionPtr core)
        : core(core)
    {
    }

    inline operator ActionPtr() const
    {
        return core;
    }

private:
    ActionPtr core;
};

/// \~chinese
/// @brief ���䶯��������
struct TweenHelper
{
    using DoneCallback = Action::DoneCallback;

    /// \~chinese
    /// @brief ���ö�������ʱ��
    inline TweenHelper& SetDuration(Duration dur)
    {
        core->SetDuration(dur);
        return (*this);
    }

    /// \~chinese
    /// @brief ����ѭ������
    inline TweenHelper& SetLoops(int loops)
    {
        core->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief ���û�������
    inline TweenHelper& SetEaseFunc(EaseFunc ease)
    {
        core->SetEaseFunc(ease);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö����ӳ�
    inline TweenHelper& SetDelay(Duration delay)
    {
        core->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö��������ص�����
    inline TweenHelper& SetDoneCallback(DoneCallback const& cb)
    {
        core->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ���ö���ѭ������ʱ�Ļص�����
    inline TweenHelper& SetLoopDoneCallback(DoneCallback const& cb)
    {
        core->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief ��������ʱ�Ƴ�Ŀ���ɫ
    inline TweenHelper& RemoveTargetWhenDone()
    {
        core->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief ��������
    inline TweenHelper& SetName(String const& name)
    {
        core->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief ��ȡָ��
    inline ActionTweenPtr Get() const
    {
        return core;
    }

    inline TweenHelper(ActionTweenPtr core)
        : core(core)
    {
    }

    inline operator ActionPtr() const
    {
        return core;
    }

    inline operator ActionTweenPtr() const
    {
        return core;
    }

private:
    ActionTweenPtr core;
};

/// \~chinese
/// @brief ����������
struct Tween
{
public:
    /// \~chinese
    /// @brief �������λ�ƶ���
    /// @param duration ����ʱ��
    /// @param vector �ƶ�����
    static inline TweenHelper MoveBy(Duration dur, Point const& vector)
    {
        return TweenHelper(new kiwano::ActionMoveBy(dur, vector));
    }

    /// \~chinese
    /// @brief ����λ�ƶ���
    /// @param duration ����ʱ��
    /// @param pos Ŀ������
    static inline TweenHelper MoveTo(Duration dur, Point const& pos)
    {
        return TweenHelper(new kiwano::ActionMoveTo(dur, pos));
    }

    /// \~chinese
    /// @brief ���������Ծ����
    /// @param duration ����ʱ��
    /// @param vec ��Ծλ������
    /// @param height ��Ծ�߶�
    /// @param jumps ��Ծ����
    static inline TweenHelper JumpBy(Duration duration, Vec2 const& vec, float height, int jumps = 1)
    {
        return TweenHelper(new kiwano::ActionJumpBy(duration, vec, height, jumps));
    }

    /// \~chinese
    /// @brief ������Ծ����
    /// @param duration ����ʱ��
    /// @param pos Ŀ������
    /// @param height ��Ծ�߶�
    /// @param jumps ��Ծ����
    static inline TweenHelper JumpTo(Duration duration, Point const& pos, float height, int jumps = 1)
    {
        return TweenHelper(new kiwano::ActionJumpTo(duration, pos, height, jumps));
    }

    /// \~chinese
    /// @brief ����������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale_x ����������Ա仯ֵ
    /// @param scale_y ����������Ա仯ֵ
    static inline TweenHelper ScaleBy(Duration dur, float scale_x, float scale_y)
    {
        return TweenHelper(new kiwano::ActionScaleBy(dur, scale_x, scale_y));
    }

    /// \~chinese
    /// @brief �������Ŷ���
    /// @param duration ����ʱ��
    /// @param scale_x ��������Ŀ��ֵ
    /// @param scale_y ��������Ŀ��ֵ
    static inline TweenHelper ScaleTo(Duration dur, float scale_x, float scale_y)
    {
        return TweenHelper(new kiwano::ActionScaleTo(dur, scale_x, scale_y));
    }

    /// \~chinese
    /// @brief ����͸���Ƚ��䶯��
    /// @param duration ����ʱ��
    /// @param opacity Ŀ��͸����
    static inline TweenHelper FadeTo(Duration dur, float opacity)
    {
        return TweenHelper(new kiwano::ActionFadeTo(dur, opacity));
    }

    /// \~chinese
    /// @brief ���쵭�붯��
    /// @param duration ����ʱ��
    static inline TweenHelper FadeIn(Duration dur)
    {
        return TweenHelper(new kiwano::ActionFadeIn(dur));
    }

    /// \~chinese
    /// @brief ���쵭������
    /// @param duration ����ʱ��
    static inline TweenHelper FadeOut(Duration dur)
    {
        return TweenHelper(new kiwano::ActionFadeOut(dur));
    }

    /// \~chinese
    /// @brief ���������ת����
    /// @param duration ����ʱ��
    /// @param rotation �Ƕ���Ա仯ֵ
    static inline TweenHelper RotateBy(Duration dur, float rotation)
    {
        return TweenHelper(new kiwano::ActionRotateBy(dur, rotation));
    }

    /// \~chinese
    /// @brief ������ת����
    /// @param duration ����ʱ��
    /// @param rotation Ŀ��Ƕ�
    static inline TweenHelper RotateTo(Duration dur, float rotation)
    {
        return TweenHelper(new kiwano::ActionRotateTo(dur, rotation));
    }

    /// \~chinese
    /// @brief ����·�����߶���
    /// @param duration ����ʱ��
    /// @param path ·��������״
    /// @param rotating �Ƿ���·�����߷�����ת
    /// @param start ·����㣨�ٷֱȣ�
    /// @param end ·���յ㣨�ٷֱȣ�
    static inline TweenHelper Walk(Duration duration, Geometry const& path, bool rotating = false, float start = 0.f,
                                   float end = 1.f)
    {
        return TweenHelper(new kiwano::ActionWalk(duration, path, rotating, start, end));
    }

    /// \~chinese
    /// @brief ����֡����
    /// @param duration ����ʱ��
    /// @param[in] frame_seq ����֡
    static inline TweenHelper Animation(Duration dur, FrameSequencePtr frames)
    {
        return TweenHelper(new kiwano::Animation(dur, frames));
    }

    /// \~chinese
    /// @brief �����Զ��嶯��
    /// @param duration ����ʱ��
    /// @param tween_func �����ص�����
    static inline TweenHelper Custom(Duration dur, ActionCustom::TweenFunc tween_func)
    {
        return TweenHelper(new kiwano::ActionCustom(dur, tween_func));
    }

    /// \~chinese
    /// @brief ������ʱ����
    /// @param delay ��ʱʱ��
    static inline ActionHelper Delay(Duration delay)
    {
        return ActionHelper(new kiwano::ActionDelay(delay));
    }

    /// \~chinese
    /// @brief �������
    /// @param actions ��������
    /// @param sequence ������˳������ִ�л�ͬʱִ��
    static inline ActionHelper Group(Vector<ActionPtr> const& actions, bool sequence = true)
    {
        return ActionHelper(new kiwano::ActionGroup(actions, sequence));
    }

    /// \~chinese
    /// @brief ͬ���������
    /// @param actions ��������
    static inline ActionHelper Multiple(Vector<ActionPtr> const& actions)
    {
        return ActionHelper(new kiwano::ActionGroup(actions, false));
    }
};

/** @} */
}  // namespace kiwano
