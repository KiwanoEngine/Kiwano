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
#include <kiwano/render/ShapeSink.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(ActionWalk);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief 路径行走动画
class KGE_API ActionWalk : public ActionTween
{
public:
    /// \~chinese
    /// @brief 创建路径行走动画
    /// @param duration 持续时长
    /// @param path 路径形状
    /// @param rotating 是否沿路径切线方向旋转
    /// @param start 路径起点（百分比）
    /// @param end 路径终点（百分比）
    static ActionWalkPtr Create(Duration duration, ShapePtr path, bool rotating = false, float start = 0.f,
                                float end = 1.f);

    ActionWalk();

    /// \~chinese
    /// @brief 获取路线
    ShapePtr GetPath() const;

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
    void SetPath(ShapePtr path);

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
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

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

inline ShapePtr ActionWalk::GetPath() const
{
    return path_;
}

inline bool ActionWalk::IsRotating() const
{
    return rotating_;
}

inline float ActionWalk::GetStartValue() const
{
    return start_;
}

inline float ActionWalk::GetEndValue() const
{
    return end_;
}

inline void ActionWalk::SetPath(ShapePtr path)
{
    path_ = path;
}

inline void ActionWalk::SetRotating(bool rotating)
{
    rotating_ = rotating;
}

inline void ActionWalk::SetStartValue(float start)
{
    start_ = start;
}

inline void ActionWalk::SetEndValue(float end)
{
    end_ = end;
}

}  // namespace kiwano
