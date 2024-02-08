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
#include <kiwano/2d/animation/TweenAnimation.h>
#include <kiwano/render/Shape.h>

namespace kiwano
{
/**
 * \addtogroup Animation
 * @{
 */


/// \~chinese
/// @brief 路径行走动画
class KGE_API PathAnimation : public TweenAnimation
{
public:
    /// \~chinese
    /// @brief 创建路径行走动画
    /// @param duration 持续时长
    /// @param path 路径形状
    /// @param rotating 是否沿路径切线方向旋转
    /// @param start 路径起点（百分比）
    /// @param end 路径终点（百分比）
    PathAnimation(Duration duration, RefPtr<Shape> path, bool rotating = false, float start = 0.f, float end = 1.f);

    /// \~chinese
    /// @brief 获取路线
    RefPtr<Shape> GetPath() const;

    /// \~chinese
    /// @brief 是否沿路径切线方向旋转
    bool IsRotating() const;

    /// \~chinese
    /// @brief 获取路径起点（百分比）
    float GetStartValue() const;

    /// \~chinese
    /// @brief 获取路径终点（百分比）
    float GetEndValue() const;

    /// \~chinese
    /// @brief 设置路径形状
    void SetPath(RefPtr<Shape> path);

    /// \~chinese
    /// @brief 设置沿路径切线方向旋转
    void SetRotating(bool rotating);

    /// \~chinese
    /// @brief 设置路径起点（百分比）
    void SetStartValue(float start);

    /// \~chinese
    /// @brief 设置路径终点（百分比）
    void SetEndValue(float end);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    PathAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    PathAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    bool     rotating_;
    float    start_;
    float    end_;
    float    length_;
    Point    start_pos_;
    RefPtr<Shape> path_;
};

/** @} */

inline RefPtr<Shape> PathAnimation::GetPath() const
{
    return path_;
}

inline bool PathAnimation::IsRotating() const
{
    return rotating_;
}

inline float PathAnimation::GetStartValue() const
{
    return start_;
}

inline float PathAnimation::GetEndValue() const
{
    return end_;
}

inline void PathAnimation::SetPath(RefPtr<Shape> path)
{
    path_ = path;
}

inline void PathAnimation::SetRotating(bool rotating)
{
    rotating_ = rotating;
}

inline void PathAnimation::SetStartValue(float start)
{
    start_ = start;
}

inline void PathAnimation::SetEndValue(float end)
{
    end_ = end;
}

}  // namespace kiwano
