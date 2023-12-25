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
#include <kiwano/2d/animation/Animation.h>

namespace kiwano
{

/**
 * \addtogroup Animation
 * @{
 */

/**
 * \~chinese
 * @brief ����������
 */
class KGE_API Animator
{
public:
    /// \~chinese
    /// @brief ��Ӷ���
    Animation* AddAnimation(AnimationPtr animation);

    /// \~chinese
    /// @brief ��ʼ����
    inline Animation* StartAnimation(AnimationPtr animation)
    {
        return AddAnimation(animation);
    }

    /// \~chinese
    /// @brief ����������ͣ����
    void ResumeAllAnimations();

    /// \~chinese
    /// @brief ��ͣ���ж���
    void PauseAllAnimations();

    /// \~chinese
    /// @brief ֹͣ���ж���
    void StopAllAnimations();

    /// \~chinese
    /// @brief ��ȡָ�����ƵĶ���
    /// @param name ��������
    Animation* GetAnimation(StringView name);

    /// \~chinese
    /// @brief ��ȡ���ж���
    const AnimationList& GetAllAnimations() const;

    /// \~chinese
    /// @brief ���¶���
    void Update(Actor* target, Duration dt);

private:
    AnimationList animations_;
};

/** @} */
}  // namespace kiwano
