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
#include <kiwano/2d/animation/FrameSequence.h>

namespace kiwano
{

/**
 * \addtogroup Animation
 * @{
 */

/// \~chinese
/// @brief 帧动画
class KGE_API FrameAnimation : public TweenAnimation
{
public:
    FrameAnimation();

    /// \~chinese
    /// @brief 创建帧动画
    /// @param dur 动画时长
    /// @param frame_seq 序列帧
    FrameAnimation(Duration dur, RefPtr<FrameSequence> frame_seq);

    virtual ~FrameAnimation();

    /// \~chinese
    /// @brief 获取序列帧
    RefPtr<FrameSequence> GetFrameSequence() const;

    /// \~chinese
    /// @brief 设置序列帧
    /// @param[in] frame_seq 序列帧
    void SetFrameSequence(RefPtr<FrameSequence> frame_seq);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    FrameAnimation* Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    FrameAnimation* Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    size_t                current_index_;
    RefPtr<FrameSequence> frame_seq_;
};

/** @} */

}  // namespace kiwano
