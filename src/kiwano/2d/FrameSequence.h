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
#include <kiwano/2d/Frame.h>
#include <kiwano/core/Common.h>
#include <kiwano/core/Time.h>

namespace kiwano
{
KGE_DECLARE_SMART_PTR(FrameSequence);

/**
 * \~chinese
 * @brief 序列帧
 */
class KGE_API FrameSequence : public virtual ObjectBase
{
public:
    /// \~chinese
    /// @brief 构建空序列帧
    FrameSequence();

    /// \~chinese
    /// @brief 构建序列帧
    /// @param frames 图像帧集合
    explicit FrameSequence(Vector<FramePtr> const& frames);

    virtual ~FrameSequence();

    /// \~chinese
    /// @brief 添加关键帧
    /// @param frame 图像帧
    void AddFrame(FramePtr frame);

    /// \~chinese
    /// @brief 添加多个关键帧
    /// @param frames 图像帧集合
    void AddFrames(Vector<FramePtr> const& frames);

    /// \~chinese
    /// @brief 获取关键帧
    /// @param index 图像帧下标
    FramePtr GetFrame(size_t index) const;

    /// \~chinese
    /// @brief 获取所有关键帧
    Vector<FramePtr> const& GetFrames() const;

    /// \~chinese
    /// @brief 获取关键帧数量
    size_t GetFramesCount() const;

    /// \~chinese
    /// @brief 获取序列帧的拷贝对象
    FrameSequencePtr Clone() const;

    /// \~chinese
    /// @brief 获取序列帧的倒转
    FrameSequencePtr Reverse() const;

private:
    Vector<FramePtr> frames_;
};
}  // namespace kiwano
