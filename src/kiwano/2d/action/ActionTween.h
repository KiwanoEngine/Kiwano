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
#include <kiwano/2d/action/Action.h>
#include <kiwano/core/Logger.h>

namespace kiwano
{
/// \~chinese
/// @brief 缓动函数
using EaseFunc = Function<float(float)>;

/// \~chinese
/// @brief 缓动函数枚举
/// @details 查看 https://easings.net 获取更多信息
struct Ease
{
    static KGE_API EaseFunc Linear;        ///< 线性
    static KGE_API EaseFunc EaseIn;        ///< 由慢变快
    static KGE_API EaseFunc EaseOut;       ///< 由快变慢
    static KGE_API EaseFunc EaseInOut;     ///< 由慢变快, 再由快变慢
    static KGE_API EaseFunc ExpoIn;        ///< 由慢变极快
    static KGE_API EaseFunc ExpoOut;       ///< 由极快变慢
    static KGE_API EaseFunc ExpoInOut;     ///< 由慢至极快, 再由极快边慢
    static KGE_API EaseFunc ElasticIn;     ///< 自起点赋予弹性
    static KGE_API EaseFunc ElasticOut;    ///< 自终点赋予弹性
    static KGE_API EaseFunc ElasticInOut;  ///< 再起点和终点赋予弹性
    static KGE_API EaseFunc BounceIn;      ///< 自起点赋予反弹力
    static KGE_API EaseFunc BounceOut;     ///< 自终点赋予反弹力
    static KGE_API EaseFunc BounceInOut;   ///< 在起点和终点赋予反弹力
    static KGE_API EaseFunc BackIn;
    static KGE_API EaseFunc BackOut;
    static KGE_API EaseFunc BackInOut;
    static KGE_API EaseFunc QuadIn;
    static KGE_API EaseFunc QuadOut;
    static KGE_API EaseFunc QuadInOut;
    static KGE_API EaseFunc CubicIn;
    static KGE_API EaseFunc CubicOut;
    static KGE_API EaseFunc CubicInOut;
    static KGE_API EaseFunc QuartIn;
    static KGE_API EaseFunc QuartOut;
    static KGE_API EaseFunc QuartInOut;
    static KGE_API EaseFunc QuintIn;
    static KGE_API EaseFunc QuintOut;
    static KGE_API EaseFunc QuintInOut;
    static KGE_API EaseFunc SineIn;
    static KGE_API EaseFunc SineOut;
    static KGE_API EaseFunc SineInOut;
};

KGE_DECLARE_SMART_PTR(ActionTween);
KGE_DECLARE_SMART_PTR(ActionMoveBy);
KGE_DECLARE_SMART_PTR(ActionMoveTo);
KGE_DECLARE_SMART_PTR(ActionJumpBy);
KGE_DECLARE_SMART_PTR(ActionJumpTo);
KGE_DECLARE_SMART_PTR(ActionScaleBy);
KGE_DECLARE_SMART_PTR(ActionScaleTo);
KGE_DECLARE_SMART_PTR(ActionFadeTo);
KGE_DECLARE_SMART_PTR(ActionFadeIn);
KGE_DECLARE_SMART_PTR(ActionFadeOut);
KGE_DECLARE_SMART_PTR(ActionRotateBy);
KGE_DECLARE_SMART_PTR(ActionRotateTo);
KGE_DECLARE_SMART_PTR(ActionCustom);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief 补间动画
class KGE_API ActionTween : public Action
{
public:
    ActionTween();

    /// \~chinese
    /// @brief 补间动画
    /// @param duration 动画时长
    /// @param func 动画速度缓动函数
    ActionTween(Duration duration, EaseFunc func);

    /// \~chinese
    /// @brief 获取动画时长
    Duration GetDuration() const;

    /// \~chinese
    /// @brief 设置动画时长
    void SetDuration(Duration duration);

    /// \~chinese
    /// @brief 获取动画速度缓动函数
    EaseFunc const& GetEaseFunc() const;

    /// \~chinese
    /// @brief 设置动画速度缓动函数
    void SetEaseFunc(EaseFunc const& func);

protected:
    void Update(Actor* target, Duration dt) override;

    virtual void UpdateTween(Actor* target, float percent) = 0;

private:
    Duration dur_;
    EaseFunc ease_func_;
};

/// \~chinese
/// @brief 相对位移动画
class KGE_API ActionMoveBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief 构造相对位移动画
    /// @param duration 动画时长
    /// @param vector 移动向量
    /// @param func 动画速度缓动函数
    ActionMoveBy(Duration duration, Vec2 const& vector, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    Point start_pos_;
    Point prev_pos_;
    Vec2  delta_pos_;
};

/// \~chinese
/// @brief 位移动画
class KGE_API ActionMoveTo : public ActionMoveBy
{
public:
    /// \~chinese
    /// @brief 构造位移动画
    /// @param duration 动画时长
    /// @param pos 目的坐标
    /// @param func 动画速度缓动函数
    ActionMoveTo(Duration duration, Point const& pos, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionMoveTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point end_pos_;
};

/// \~chinese
/// @brief 相对跳跃动画
class KGE_API ActionJumpBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief 构造相对跳跃动画
    /// @param duration 动画时长
    /// @param vec 跳跃位移向量
    /// @param height 跳跃高度
    /// @param jumps 跳跃次数
    /// @param func 动画速度缓动函数
    ActionJumpBy(Duration duration, Vec2 const& vec, float height, int jumps = 1, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    Point start_pos_;
    Point delta_pos_;
    float height_;
    int   jumps_;
    Point prev_pos_;
};

/// \~chinese
/// @brief 跳跃动画
class KGE_API ActionJumpTo : public ActionJumpBy
{
public:
    /// \~chinese
    /// @brief 构造跳跃动画
    /// @param duration 动画时长
    /// @param pos 目的坐标
    /// @param height 跳跃高度
    /// @param jumps 跳跃次数
    /// @param func 动画速度缓动函数
    ActionJumpTo(Duration duration, Point const& pos, float height, int jumps = 1, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionJumpTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point end_pos_;
};

/// \~chinese
/// @brief 相对缩放动画
class KGE_API ActionScaleBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief 构造相对缩放动画
    /// @param duration 动画时长
    /// @param scale_x 横向缩放相对变化值
    /// @param scale_y 纵向缩放相对变化值
    /// @param func 动画速度缓动函数
    ActionScaleBy(Duration duration, float scale_x, float scale_y, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    float start_scale_x_;
    float start_scale_y_;
    float delta_x_;
    float delta_y_;
};

/// \~chinese
/// @brief 缩放动画
class KGE_API ActionScaleTo : public ActionScaleBy
{
public:
    /// \~chinese
    /// @brief 构造缩放动画
    /// @param duration 动画时长
    /// @param scale_x 横向缩放目标值
    /// @param scale_y 纵向缩放目标值
    /// @param func 动画速度缓动函数
    ActionScaleTo(Duration duration, float scale_x, float scale_y, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionScaleTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    float end_scale_x_;
    float end_scale_y_;
};

/// \~chinese
/// @brief 透明度渐变动画
class KGE_API ActionFadeTo : public ActionTween
{
public:
    /// \~chinese
    /// @brief 构造透明度渐变动画
    /// @param duration 动画时长
    /// @param opacity 目标透明度
    /// @param func 动画速度缓动函数
    ActionFadeTo(Duration duration, float opacity, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionFadeTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    float start_val_;
    float delta_val_;
    float end_val_;
};

/// \~chinese
/// @brief 淡入动画
class KGE_API ActionFadeIn : public ActionFadeTo
{
public:
    /// \~chinese
    /// @brief 构造淡入动画
    /// @param duration 动画时长
    /// @param func 动画速度缓动函数
    explicit ActionFadeIn(Duration duration, EaseFunc func = nullptr);
};

/// \~chinese
/// @brief 淡出动画
class KGE_API ActionFadeOut : public ActionFadeTo
{
public:
    /// \~chinese
    /// @brief 构造淡出动画
    /// @param duration 动画时长
    /// @param func 动画速度缓动函数
    explicit ActionFadeOut(Duration duration, EaseFunc func = nullptr);
};

/// \~chinese
/// @brief 相对旋转动画
class KGE_API ActionRotateBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief 构造相对旋转动画
    /// @param duration 动画时长
    /// @param rotation 角度相对变化值
    /// @param func 动画速度缓动函数
    ActionRotateBy(Duration duration, float rotation, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    float start_val_;
    float delta_val_;
};

/// \~chinese
/// @brief 旋转动画
class KGE_API ActionRotateTo : public ActionRotateBy
{
public:
    /// \~chinese
    /// @brief 构造旋转动画
    /// @param duration 动画时长
    /// @param rotation 目标角度
    /// @param func 动画速度缓动函数
    ActionRotateTo(Duration duration, float rotation, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionRotateTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    float end_val_;
};

/// \~chinese
/// @brief 自定义动画
class KGE_API ActionCustom : public ActionTween
{
public:
    /// \~chinese
    /// @brief 动画回调函数
    /// @details 在动画更新时回调该函数，第一个参数是执行动画的目标，第二个参数是动画进度（0.0 - 1.0）
    using TweenFunc = Function<void(Actor* /* target */, float /* percent */)>;

    /// \~chinese
    /// @brief 构造自定义动画
    /// @param duration 动画时长
    /// @param tween_func 动画回调函数
    /// @param func 动画速度缓动函数
    ActionCustom(Duration duration, TweenFunc tween_func, EaseFunc func = nullptr);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override
    {
        KGE_ERROR("Reverse() not supported in ActionCustom");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    TweenFunc tween_func_;
};

/** @} */

}  // namespace kiwano
