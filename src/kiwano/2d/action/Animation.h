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
#include <kiwano/render/FrameSequence.h>
#include <kiwano/2d/action/ActionTween.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(AnimationEntity);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief ֡����
class KGE_API Animation : public ActionTween
{
public:
    /// \~chinese
    /// @brief ����֡����
    /// @param dur ����ʱ��
    /// @param frame_seq ����֡
    Animation(Duration dur, FrameSequencePtr frame_seq);
};

/// \~chinese
/// @brief ֡����ʵ��
class KGE_API AnimationEntity : public ActionTweenEntity
{
public:
    /// \~chinese
    /// @brief ����֡����
    /// @param dur ����ʱ��
    /// @param frame_seq ����֡
    static AnimationEntityPtr Create(Duration dur, FrameSequencePtr frame_seq);

    AnimationEntity();

    virtual ~AnimationEntity();

    /// \~chinese
    /// @brief ��ȡ����֡
    FrameSequencePtr GetFrameSequence() const;

    /// \~chinese
    /// @brief ��������֡
    /// @param[in] frame_seq ����֡
    void SetFrameSequence(FrameSequencePtr frame_seq);

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionEntityPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionEntityPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    FrameSequencePtr frame_seq_;
};

/** @} */
}  // namespace kiwano
