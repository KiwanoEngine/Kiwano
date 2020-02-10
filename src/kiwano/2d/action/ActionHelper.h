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
/// @brief 动画辅助类
struct ActionHelper
{
    using DoneCallback = Action::DoneCallback;

    /// \~chinese
    /// @brief 设置循环次数
    inline ActionHelper& SetLoops(int loops)
    {
        core->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画延迟
    inline ActionHelper& SetDelay(Duration delay)
    {
        core->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画结束回调函数
    inline ActionHelper& SetDoneCallback(DoneCallback const& cb)
    {
        core->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画循环结束时的回调函数
    inline ActionHelper& SetLoopDoneCallback(DoneCallback const& cb)
    {
        core->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 动画结束时移除目标角色
    inline ActionHelper& RemoveTargetWhenDone()
    {
        core->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief 设置名称
    inline ActionHelper& SetName(String const& name)
    {
        core->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief 获取指针
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
/// @brief 补间动画辅助类
struct TweenHelper
{
    using DoneCallback = Action::DoneCallback;

    /// \~chinese
    /// @brief 设置动画持续时长
    inline TweenHelper& SetDuration(Duration dur)
    {
        core->SetDuration(dur);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置循环次数
    inline TweenHelper& SetLoops(int loops)
    {
        core->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置缓动函数
    inline TweenHelper& SetEaseFunc(EaseFunc ease)
    {
        core->SetEaseFunc(ease);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画延迟
    inline TweenHelper& SetDelay(Duration delay)
    {
        core->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画结束回调函数
    inline TweenHelper& SetDoneCallback(DoneCallback const& cb)
    {
        core->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画循环结束时的回调函数
    inline TweenHelper& SetLoopDoneCallback(DoneCallback const& cb)
    {
        core->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 动画结束时移除目标角色
    inline TweenHelper& RemoveTargetWhenDone()
    {
        core->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief 设置名称
    inline TweenHelper& SetName(String const& name)
    {
        core->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief 获取指针
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
/// @brief 动画构造器
struct Tween
{
public:
    /// \~chinese
    /// @brief 构造相对位移动画
    /// @param duration 动画时长
    /// @param vector 移动向量
    static inline TweenHelper MoveBy(Duration dur, Point const& vector)
    {
        return TweenHelper(new kiwano::ActionMoveBy(dur, vector));
    }

    /// \~chinese
    /// @brief 构造位移动画
    /// @param duration 动画时长
    /// @param pos 目的坐标
    static inline TweenHelper MoveTo(Duration dur, Point const& pos)
    {
        return TweenHelper(new kiwano::ActionMoveTo(dur, pos));
    }

    /// \~chinese
    /// @brief 构造相对跳跃动画
    /// @param duration 动画时长
    /// @param vec 跳跃位移向量
    /// @param height 跳跃高度
    /// @param jumps 跳跃次数
    static inline TweenHelper JumpBy(Duration duration, Vec2 const& vec, float height, int jumps = 1)
    {
        return TweenHelper(new kiwano::ActionJumpBy(duration, vec, height, jumps));
    }

    /// \~chinese
    /// @brief 构造跳跃动画
    /// @param duration 动画时长
    /// @param pos 目的坐标
    /// @param height 跳跃高度
    /// @param jumps 跳跃次数
    static inline TweenHelper JumpTo(Duration duration, Point const& pos, float height, int jumps = 1)
    {
        return TweenHelper(new kiwano::ActionJumpTo(duration, pos, height, jumps));
    }

    /// \~chinese
    /// @brief 构造相对缩放动画
    /// @param duration 动画时长
    /// @param scale_x 横向缩放相对变化值
    /// @param scale_y 纵向缩放相对变化值
    static inline TweenHelper ScaleBy(Duration dur, float scale_x, float scale_y)
    {
        return TweenHelper(new kiwano::ActionScaleBy(dur, scale_x, scale_y));
    }

    /// \~chinese
    /// @brief 构造缩放动画
    /// @param duration 动画时长
    /// @param scale_x 横向缩放目标值
    /// @param scale_y 纵向缩放目标值
    static inline TweenHelper ScaleTo(Duration dur, float scale_x, float scale_y)
    {
        return TweenHelper(new kiwano::ActionScaleTo(dur, scale_x, scale_y));
    }

    /// \~chinese
    /// @brief 构造透明度渐变动画
    /// @param duration 动画时长
    /// @param opacity 目标透明度
    static inline TweenHelper FadeTo(Duration dur, float opacity)
    {
        return TweenHelper(new kiwano::ActionFadeTo(dur, opacity));
    }

    /// \~chinese
    /// @brief 构造淡入动画
    /// @param duration 动画时长
    static inline TweenHelper FadeIn(Duration dur)
    {
        return TweenHelper(new kiwano::ActionFadeIn(dur));
    }

    /// \~chinese
    /// @brief 构造淡出动画
    /// @param duration 动画时长
    static inline TweenHelper FadeOut(Duration dur)
    {
        return TweenHelper(new kiwano::ActionFadeOut(dur));
    }

    /// \~chinese
    /// @brief 构造相对旋转动画
    /// @param duration 动画时长
    /// @param rotation 角度相对变化值
    static inline TweenHelper RotateBy(Duration dur, float rotation)
    {
        return TweenHelper(new kiwano::ActionRotateBy(dur, rotation));
    }

    /// \~chinese
    /// @brief 构造旋转动画
    /// @param duration 动画时长
    /// @param rotation 目标角度
    static inline TweenHelper RotateTo(Duration dur, float rotation)
    {
        return TweenHelper(new kiwano::ActionRotateTo(dur, rotation));
    }

    /// \~chinese
    /// @brief 构造路径行走动画
    /// @param duration 持续时长
    /// @param path 路径形状
    /// @param rotating 是否沿路径切线方向旋转
    /// @param start 路径起点（百分比）
    /// @param end 路径终点（百分比）
    static inline TweenHelper Walk(Duration duration, ShapePtr path, bool rotating = false, float start = 0.f,
                                   float end = 1.f)
    {
        return TweenHelper(new kiwano::ActionWalk(duration, path, rotating, start, end));
    }

    /// \~chinese
    /// @brief 构建帧动画
    /// @param duration 动画时长
    /// @param[in] frame_seq 序列帧
    static inline TweenHelper Animation(Duration dur, FrameSequencePtr frames)
    {
        return TweenHelper(new kiwano::Animation(dur, frames));
    }

    /// \~chinese
    /// @brief 构造自定义动画
    /// @param duration 动画时长
    /// @param tween_func 动画回调函数
    static inline TweenHelper Custom(Duration dur, ActionCustom::TweenFunc tween_func)
    {
        return TweenHelper(new kiwano::ActionCustom(dur, tween_func));
    }

    /// \~chinese
    /// @brief 构建延时动画
    /// @param delay 延时时长
    static inline ActionHelper Delay(Duration delay)
    {
        return ActionHelper(new kiwano::ActionDelay(delay));
    }

    /// \~chinese
    /// @brief 动画组合
    /// @param actions 动画集合
    /// @param sync 同步执行
    static inline ActionHelper Group(Vector<ActionPtr> const& actions, bool sync = false)
    {
        return ActionHelper(new kiwano::ActionGroup(actions, sync));
    }
};

/** @} */
}  // namespace kiwano
