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
#include <kiwano/core/Common.h>
#include <kiwano/core/Time.h>
#include <kiwano/2d/SpriteFrame.h>

namespace kiwano
{
/**
 * \~chinese
 * @brief 序列帧
 */
class KGE_API FrameSequence : public BaseObject
{
public:
    /// \~chinese
    /// @brief 创建序列帧
    /// @param frames 精灵帧集合
    FrameSequence(const Vector<SpriteFrame>& frames);

    /// \~chinese
    /// @brief 构建空序列帧
    FrameSequence();

    virtual ~FrameSequence();

    /// \~chinese
    /// @brief 添加精灵帧
    /// @param frame 精灵帧
    void AddFrame(const SpriteFrame& frame);

    /// \~chinese
    /// @brief 添加多个精灵帧
    /// @param frames 精灵帧集合
    void AddFrames(const Vector<SpriteFrame>& frames);

    /// \~chinese
    /// @brief 获取精灵帧
    /// @param index 精灵帧下标
    const SpriteFrame& GetFrame(size_t index) const;

    /// \~chinese
    /// @brief 获取所有精灵帧
    const Vector<SpriteFrame>& GetFrames() const;

    /// \~chinese
    /// @brief 获取精灵帧数量
    size_t GetFramesCount() const;

    /// \~chinese
    /// @brief 获取序列帧的拷贝对象
    RefPtr<FrameSequence> Clone() const;

    /// \~chinese
    /// @brief 获取序列帧的倒转
    RefPtr<FrameSequence> Reverse() const;

private:
    Vector<SpriteFrame> frames_;
};

}  // namespace kiwano
