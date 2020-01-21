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
 * @brief ����֡
 */
class KGE_API FrameSequence : public virtual ObjectBase
{
public:
    /// \~chinese
    /// @brief ����������֡
    FrameSequence();

    /// \~chinese
    /// @brief ��������֡
    /// @param frames ͼ��֡����
    explicit FrameSequence(Vector<FramePtr> const& frames);

    virtual ~FrameSequence();

    /// \~chinese
    /// @brief ��ӹؼ�֡
    /// @param frame ͼ��֡
    void AddFrame(FramePtr frame);

    /// \~chinese
    /// @brief ��Ӷ���ؼ�֡
    /// @param frames ͼ��֡����
    void AddFrames(Vector<FramePtr> const& frames);

    /// \~chinese
    /// @brief ��ȡ�ؼ�֡
    /// @param index ͼ��֡�±�
    FramePtr GetFrame(size_t index) const;

    /// \~chinese
    /// @brief ��ȡ���йؼ�֡
    Vector<FramePtr> const& GetFrames() const;

    /// \~chinese
    /// @brief ��ȡ�ؼ�֡����
    size_t GetFramesCount() const;

    /// \~chinese
    /// @brief ��ȡ����֡�Ŀ�������
    FrameSequencePtr Clone() const;

    /// \~chinese
    /// @brief ��ȡ����֡�ĵ�ת
    FrameSequencePtr Reverse() const;

private:
    Vector<FramePtr> frames_;
};
}  // namespace kiwano
