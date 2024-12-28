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
 * @brief ����֡
 */
class KGE_API FrameSequence : public BaseObject
{
public:
    /// \~chinese
    /// @brief ��������֡
    /// @param frames ����֡����
    FrameSequence(const Vector<SpriteFrame>& frames);

    /// \~chinese
    /// @brief ����������֡
    FrameSequence();

    virtual ~FrameSequence();

    /// \~chinese
    /// @brief ��Ӿ���֡
    /// @param frame ����֡
    void AddFrame(const SpriteFrame& frame);

    /// \~chinese
    /// @brief ��Ӷ������֡
    /// @param frames ����֡����
    void AddFrames(const Vector<SpriteFrame>& frames);

    /// \~chinese
    /// @brief ��ȡ����֡
    /// @param index ����֡�±�
    const SpriteFrame& GetFrame(size_t index) const;

    /// \~chinese
    /// @brief ��ȡ���о���֡
    const Vector<SpriteFrame>& GetFrames() const;

    /// \~chinese
    /// @brief ��ȡ����֡����
    size_t GetFramesCount() const;

    /// \~chinese
    /// @brief ��ȡ����֡�Ŀ�������
    RefPtr<FrameSequence> Clone() const;

    /// \~chinese
    /// @brief ��ȡ����֡�ĵ�ת
    RefPtr<FrameSequence> Reverse() const;

private:
    Vector<SpriteFrame> frames_;
};

}  // namespace kiwano
