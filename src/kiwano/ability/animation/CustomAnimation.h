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
#include <kiwano/ability/animation/TweenAnimation.h>

namespace kiwano
{
/**
 * \addtogroup Animation
 * @{
 */

/// \~chinese
/// @brief 自定义动画
class KGE_API CustomAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief 创建自定义动画
    /// @param duration 动画时长
    CustomAnimation(Duration duration);

    /// \~chinese
    /// @brief 创建自定义动画
    /// @param duration 动画时长
    /// @param tween_func 动画回调函数
    static RefPtr<CustomAnimation> Create(Duration duration, Function<void(Actor*, float)> tween_func);

    /// \~chinese
    /// @brief 动画更新时
    /// @param target 执行动画的目标
    /// @param frac 动画进度（0.0 - 1.0）
    virtual void OnAnimationUpdate(Actor* target, float frac) = 0;

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    CustomAnimation* Clone() const override
    {
        KGE_ERRORF("Clone() not supported in CustomAnimation");
        return nullptr;
    }

    /// \~chinese
    /// @brief 获取该动画的倒转
    CustomAnimation* Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in CustomAnimation");
        return nullptr;
    }

protected:
    void UpdateTween(Actor* target, float frac) override;
};

/** @} */

}  // namespace kiwano
