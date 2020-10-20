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
/// @brief 动画包装器
class KGE_API AnimationWrapper
{
public:
    AnimationWrapper() = default;

    inline AnimationWrapper(AnimationPtr ptr)
        : ptr(ptr)
    {
    }

    /// \~chinese
    /// @brief 设置循环次数
    inline AnimationWrapper& Loops(int loops)
    {
        if (ptr)
            ptr->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画延迟
    inline AnimationWrapper& Delay(Duration delay)
    {
        if (ptr)
            ptr->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画结束回调函数
    inline AnimationWrapper& Handler(AnimationEventHandlerPtr handler)
    {
        if (ptr)
            ptr->SetHandler(handler);
        return (*this);
    }

    /// \~chinese
    /// @brief 动画结束时移除目标角色
    inline AnimationWrapper& RemoveTargetWhenDone()
    {
        if (ptr)
            ptr->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief 设置名称
    inline AnimationWrapper& Name(const String& name)
    {
        if (ptr)
            ptr->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置缓动函数
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
    /// @brief 克隆动画
    inline AnimationWrapper Clone() const
    {
        if (ptr)
            return AnimationWrapper(ptr->Clone());
        return AnimationWrapper();
    }

    /// \~chinese
    /// @brief 获取反向动画
    inline AnimationWrapper Reverse() const
    {
        if (ptr)
            return AnimationWrapper(ptr->Reverse());
        return AnimationWrapper();
    }

    /// \~chinese
    /// @brief 获取指针
    inline Animation* Get() const
    {
        return const_cast<Animation*>(ptr.Get());
    }

    /// \~chinese
    /// @brief 设置动画
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
/// @brief 创建相对位移动画
/// @param duration 动画时长
/// @param displacement 位移向量
inline AnimationWrapper MoveBy(kiwano::Duration duration, const Vec2& displacement)
{
    return AnimationWrapper(new MoveByAnimation(duration, displacement));
}

/// \~chinese
/// @brief 创建位移动画
/// @param duration 动画时长
/// @param distination 目的坐标
inline AnimationWrapper MoveTo(kiwano::Duration duration, const Point& distination)
{
    return AnimationWrapper(new MoveToAnimation(duration, distination));
}

/// \~chinese
/// @brief 创建相对跳跃动画
/// @param duration 动画时长
/// @param displacement 跳跃位移向量
/// @param height 跳跃高度
/// @param count 跳跃次数
inline AnimationWrapper JumpBy(kiwano::Duration duration, const Vec2& displacement, float height, int count = 1)
{
    return AnimationWrapper(new JumpByAnimation(duration, displacement, height, count));
}

/// \~chinese
/// @brief 创建跳跃动画
/// @param duration 动画时长
/// @param distination 目的坐标
/// @param height 跳跃高度
/// @param count 跳跃次数
inline AnimationWrapper JumpTo(kiwano::Duration duration, const Point& distination, float height, int count = 1)
{
    return AnimationWrapper(new JumpToAnimation(duration, distination, height, count));
}

/// \~chinese
/// @brief 创建相对缩放动画
/// @param duration 动画时长
/// @param scale 缩放相对变化值
inline AnimationWrapper ScaleBy(kiwano::Duration duration, Vec2 scale)
{
    return AnimationWrapper(new ScaleByAnimation(duration, scale));
}

/// \~chinese
/// @brief 创建缩放动画
/// @param duration 动画时长
/// @param scale 缩放目标值
inline AnimationWrapper ScaleTo(kiwano::Duration duration, Vec2 scale)
{
    return AnimationWrapper(new ScaleToAnimation(duration, scale));
}

/// \~chinese
/// @brief 创建透明度渐变动画
/// @param duration 动画时长
/// @param opacity 目标透明度
inline AnimationWrapper FadeTo(kiwano::Duration duration, float opacity)
{
    return AnimationWrapper(new FadeToAnimation(duration, opacity));
}

/// \~chinese
/// @brief 创建淡入动画
/// @param duration 动画时长
inline AnimationWrapper FadeIn(kiwano::Duration duration)
{
    return AnimationWrapper(new FadeToAnimation(duration, 1.0f));
}

/// \~chinese
/// @brief 创建淡出动画
/// @param duration 动画时长
inline AnimationWrapper FadeOut(kiwano::Duration duration)
{
    return AnimationWrapper(new FadeToAnimation(duration, 0.0f));
}

/// \~chinese
/// @brief 创建相对旋转动画
/// @param duration 动画时长
/// @param rotation 角度相对变化值
inline AnimationWrapper RotateBy(kiwano::Duration duration, float rotation)
{
    return AnimationWrapper(new RotateByAnimation(duration, rotation));
}

/// \~chinese
/// @brief 创建旋转动画
/// @param duration 动画时长
/// @param rotation 目标角度
inline AnimationWrapper RotateTo(kiwano::Duration duration, float rotation)
{
    return AnimationWrapper(new RotateToAnimation(duration, rotation));
}

/// \~chinese
/// @brief 创建自定义动画
/// @param duration 动画时长
/// @param tween_func 动画回调函数
inline AnimationWrapper Custom(kiwano::Duration duration, Function<void(Actor*, float)> tween_func)
{
    return AnimationWrapper(CustomAnimation::Create(duration, tween_func));
}

/// \~chinese
/// @brief 创建路径行走动画
/// @param duration 持续时长
/// @param path 路径形状
/// @param rotating 是否沿路径切线方向旋转
/// @param start 路径起点（百分比）
/// @param end 路径终点（百分比）
inline AnimationWrapper Path(kiwano::Duration duration, ShapePtr path, bool rotating = false, float start = 0.f,
                             float end = 1.f)
{
    return AnimationWrapper(new PathAnimation(duration, path, rotating, start, end));
}

/// \~chinese
/// @brief 创建延时动画
/// @param delay 延时时长
inline AnimationWrapper Delay(kiwano::Duration delay)
{
    return AnimationWrapper(new DelayAnimation(delay));
}

/// \~chinese
/// @brief 创建帧动画
/// @param duration 动画时长
/// @param frame_seq 序列帧
inline AnimationWrapper Frames(kiwano::Duration duration, FrameSequencePtr frame_seq)
{
    return AnimationWrapper(new FrameAnimation(duration, frame_seq));
}

/// \~chinese
/// @brief 创建动画组合
/// @param actions 动画集合
/// @param parallel 同步执行
inline AnimationWrapper Group(const Vector<AnimationPtr>& animations, bool parallel = false)
{
    return AnimationWrapper(new AnimationGroup(animations, parallel));
}

}  // namespace animation
}  // namespace kiwano

/** @} */
