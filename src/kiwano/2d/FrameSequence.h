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
    /// @brief 创建序列帧
    static FrameSequencePtr Create();

    /// \~chinese
    /// @brief 创建序列帧
    /// @param frames 图像帧集合
    static FrameSequencePtr Create(const Vector<FramePtr>& frames);

    /// \~chinese
    /// @brief 按行列分割图像并创建序列帧
    /// @param frame 图像帧
    /// @param cols 列数
    /// @param rows 行数
    /// @param max_num 最大帧数量，设-1为将分割后的图像全部作为序列帧
    /// @param padding_x X方向间隔
    /// @param padding_y Y方向间隔
    static FrameSequencePtr Create(FramePtr frame, int cols, int rows = 1, int max_num = -1, float padding_x = 0,
                                   float padding_y = 0);

    /// \~chinese
    /// @brief 构建空序列帧
    FrameSequence();

    virtual ~FrameSequence();

    /// \~chinese
    /// @brief 添加关键帧
    /// @param frame 图像帧
    void AddFrame(FramePtr frame);

    /// \~chinese
    /// @brief 添加多个关键帧
    /// @param frames 图像帧集合
    void AddFrames(const Vector<FramePtr>& frames);

    /// \~chinese
    /// @brief 按行列分割图像并添加序列帧
    /// @param frame 图像帧
    /// @param cols 列数
    /// @param rows 行数
    /// @param max_num 最大帧数量，设-1为将分割后的图像全部作为序列帧
    /// @param padding_x X方向间隔
    /// @param padding_y Y方向间隔
    void AddFrames(FramePtr frame, int cols, int rows = 1, int max_num = -1, float padding_x = 0, float padding_y = 0);

    /// \~chinese
    /// @brief 获取关键帧
    /// @param index 图像帧下标
    FramePtr GetFrame(size_t index) const;

    /// \~chinese
    /// @brief 获取所有关键帧
    const Vector<FramePtr>& GetFrames() const;

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
