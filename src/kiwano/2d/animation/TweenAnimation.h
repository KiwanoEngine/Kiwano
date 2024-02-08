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
#include <kiwano/2d/animation/Animation.h>
#include <kiwano/2d/animation/EaseFunc.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

/**
 * \addtogroup Animation
 * @{
 */

/// \~chinese
/// @brief 补间动画
class KGE_API TweenAnimation : public Animation
{
public:
    /// \~chinese
    /// @brief 获取动画时长
    Duration GetDuration() const;

    /// \~chinese
    /// @brief 设置动画时长
    void SetDuration(Duration duration);

    /// \~chinese
    /// @brief 获取动画速度缓动函数
    const EaseFunc& GetEaseFunc() const;

    /// \~chinese
    /// @brief 设置动画速度缓动函数
    void SetEaseFunc(const EaseFunc& func);

protected:
    TweenAnimation();

    TweenAnimation(Duration duration);

    float Interpolate(float frac);

    void Update(Actor* target, Duration dt) override;

    virtual void UpdateTween(Actor* target, float frac) = 0;

    void DoClone(TweenAnimation* to) const;

private:
    Duration dur_;
    EaseFunc ease_func_;
};


/// \~chinese
/// @brief 相对位移动画
class KGE_API MoveByAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief 创建相对位移动画
    /// @param duration 动画时长
    /// @param displacement 位移向量
    MoveByAnimation(Duration duration, const Vec2& displacement);

    /// \~chinese
    /// @brief 获取位移向量
    Vec2 GetDisplacement() const;

    /// \~chinese
    /// @brief 设置位移向量
    void SetDisplacement(const Vec2& displacement);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    MoveByAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    MoveByAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

protected:
    Point start_pos_;
    Point prev_pos_;
    Vec2  displacement_;
};


/// \~chinese
/// @brief 位移动画
class KGE_API MoveToAnimation : public MoveByAnimation
{
public:
    /// \~chinese
    /// @brief 创建位移动画
    /// @param duration 动画时长
    /// @param distination 目的坐标
    MoveToAnimation(Duration duration, const Point& distination);

    /// \~chinese
    /// @brief 获取目的坐标
    Point GetDistination() const;

    /// \~chinese
    /// @brief 设置目的坐标
    void SetDistination(const Point& distination);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    MoveToAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    MoveToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in MoveToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point distination_;
};


/// \~chinese
/// @brief 相对跳跃动画
class KGE_API JumpByAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief 创建相对跳跃动画
    /// @param duration 动画时长
    /// @param displacement 跳跃位移向量
    /// @param height 跳跃高度
    /// @param count 跳跃次数
    JumpByAnimation(Duration duration, const Vec2& displacement, float height, int count = 1);

    /// \~chinese
    /// @brief 获取跳跃位移
    Vec2 GetDisplacement() const;

    /// \~chinese
    /// @brief 获取跳跃高度
    float GetJumpHeight() const;

    /// \~chinese
    /// @brief 获取跳跃次数
    int GetJumpCount() const;

    /// \~chinese
    /// @brief 设置跳跃位移
    void SetDisplacement(const Vec2& displacement);

    /// \~chinese
    /// @brief 设置跳跃高度
    void SetJumpHeight(float height);

    /// \~chinese
    /// @brief 设置跳跃次数
    void SetJumpCount(int count);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    JumpByAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    JumpByAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

protected:
    float height_;
    int   jump_count_;
    Point start_pos_;
    Point displacement_;
    Point prev_pos_;
};


/// \~chinese
/// @brief 跳跃动画
class KGE_API JumpToAnimation : public JumpByAnimation
{
public:
    /// \~chinese
    /// @brief 创建跳跃动画
    /// @param duration 动画时长
    /// @param distination 目的坐标
    /// @param height 跳跃高度
    /// @param count 跳跃次数
    JumpToAnimation(Duration duration, const Point& distination, float height, int count = 1);

    /// \~chinese
    /// @brief 获取目的坐标
    Point GetDistination() const;

    /// \~chinese
    /// @brief 设置目的坐标
    void SetDistination(const Point& distination);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    JumpToAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    JumpToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in JumpToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point distination_;
};


/// \~chinese
/// @brief 相对缩放动画
class KGE_API ScaleByAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief 创建相对缩放动画
    /// @param duration 动画时长
    /// @param scale 缩放相对变化值
    ScaleByAnimation(Duration duration, const Vec2& scale);

    /// \~chinese
    /// @brief 获取横向缩放相对变化值
    float GetScaleX() const;

    /// \~chinese
    /// @brief 获取横向缩放相对变化值
    float GetScaleY() const;

    /// \~chinese
    /// @brief 设置纵向缩放相对变化值
    void SetScaleX(float scale_x);

    /// \~chinese
    /// @brief 设置纵向缩放相对变化值
    void SetScaleY(float scale_y);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ScaleByAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ScaleByAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

protected:
    Vec2 start_val_;
    Vec2 delta_;
};


/// \~chinese
/// @brief 缩放动画
class KGE_API ScaleToAnimation : public ScaleByAnimation
{
public:
    /// \~chinese
    /// @brief 创建缩放动画
    /// @param duration 动画时长
    /// @param scale 缩放目标值
    ScaleToAnimation(Duration duration, const Vec2& scale);

    /// \~chinese
    /// @brief 获取横向缩放目标值
    float GetTargetScaleX() const;

    /// \~chinese
    /// @brief 获取横向缩放目标值
    float GetTargetScaleY() const;

    /// \~chinese
    /// @brief 设置纵向缩放目标值
    void SetTargetScaleX(float scale_x);

    /// \~chinese
    /// @brief 设置纵向缩放目标值
    void SetTargetScaleY(float scale_y);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ScaleToAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ScaleToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ScaleToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Vec2 end_val_;
};


/// \~chinese
/// @brief 透明度渐变动画
class KGE_API FadeToAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief 创建透明度渐变动画
    /// @param duration 动画时长
    /// @param opacity 目标透明度
    FadeToAnimation(Duration duration, float opacity);

    /// \~chinese
    /// @brief 获取目标透明度
    float GetTargetOpacity() const;

    /// \~chinese
    /// @brief 设置目标透明度
    void SetTargetOpacity(float opacity);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    FadeToAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    FadeToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in FadeToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

private:
    float start_val_;
    float delta_val_;
    float end_val_;
};


/// \~chinese
/// @brief 相对旋转动画
class KGE_API RotateByAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief 创建相对旋转动画
    /// @param duration 动画时长
    /// @param rotation 角度相对变化值
    RotateByAnimation(Duration duration, float rotation);

    /// \~chinese
    /// @brief 获取角度相对变化值
    float GetRotation() const;

    /// \~chinese
    /// @brief 设置角度相对变化值
    void SetRotation(float rotation);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    RotateByAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    RotateByAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float frac) override;

protected:
    float start_val_;
    float delta_val_;
};


/// \~chinese
/// @brief 旋转动画
class KGE_API RotateToAnimation : public RotateByAnimation
{
public:
    /// \~chinese
    /// @brief 创建旋转动画
    /// @param duration 动画时长
    /// @param rotation 目标角度
    RotateToAnimation(Duration duration, float rotation);

    /// \~chinese
    /// @brief 获取目标角度
    float GetTargetRotation() const;

    /// \~chinese
    /// @brief 设置目标角度
    void SetTargetRotation(float rotation);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    RotateToAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    RotateToAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in RotateToAnimation");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    float end_val_;
};

/** @} */

inline const EaseFunc& TweenAnimation::GetEaseFunc() const
{
    return ease_func_;
}

inline Duration TweenAnimation::GetDuration() const
{
    return dur_;
}

inline void TweenAnimation::SetDuration(Duration duration)
{
    dur_ = duration;
}

inline void TweenAnimation::SetEaseFunc(const EaseFunc& func)
{
    ease_func_ = func;
}

inline Vec2 MoveByAnimation::GetDisplacement() const
{
    return displacement_;
}

inline void MoveByAnimation::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline Point MoveToAnimation::GetDistination() const
{
    return distination_;
}

inline void MoveToAnimation::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline Vec2 JumpByAnimation::GetDisplacement() const
{
    return displacement_;
}

inline float JumpByAnimation::GetJumpHeight() const
{
    return height_;
}

inline int JumpByAnimation::GetJumpCount() const
{
    return jump_count_;
}

inline void JumpByAnimation::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline void JumpByAnimation::SetJumpHeight(float height)
{
    height_ = height;
}

inline void JumpByAnimation::SetJumpCount(int count)
{
    jump_count_ = count;
}

inline Point JumpToAnimation::GetDistination() const
{
    return distination_;
}

inline void JumpToAnimation::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline float ScaleByAnimation::GetScaleX() const
{
    return delta_.x;
}

inline float ScaleByAnimation::GetScaleY() const
{
    return delta_.y;
}

inline void ScaleByAnimation::SetScaleX(float scale_x)
{
    delta_.x = scale_x;
}

inline void ScaleByAnimation::SetScaleY(float scale_y)
{
    delta_.y = scale_y;
}

inline float ScaleToAnimation::GetTargetScaleX() const
{
    return end_val_.x;
}

inline float ScaleToAnimation::GetTargetScaleY() const
{
    return end_val_.y;
}

inline void ScaleToAnimation::SetTargetScaleX(float scale_x)
{
    end_val_.x = scale_x;
}

inline void ScaleToAnimation::SetTargetScaleY(float scale_y)
{
    end_val_.y = scale_y;
}

inline float FadeToAnimation::GetTargetOpacity() const
{
    return end_val_;
}

inline void FadeToAnimation::SetTargetOpacity(float opacity)
{
    end_val_ = opacity;
}

inline float RotateByAnimation::GetRotation() const
{
    return delta_val_;
}

inline void RotateByAnimation::SetRotation(float rotation)
{
    delta_val_ = rotation;
}

inline float RotateToAnimation::GetTargetRotation() const
{
    return end_val_;
}

inline void RotateToAnimation::SetTargetRotation(float rotation)
{
    end_val_ = rotation;
}

}  // namespace kiwano
