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
#include <kiwano/2d/action/Action.h>

namespace kiwano
{

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief �����б�
typedef IntrusiveList<ActionPtr> ActionList;

/**
 * \~chinese
 * @brief ����������
 */
class KGE_API ActionScheduler
{
public:
    /// \~chinese
    /// @brief ��Ӷ���
    Action* AddAction(ActionPtr action);

    /// \~chinese
    /// @brief ����������ͣ����
    void ResumeAllActions();

    /// \~chinese
    /// @brief ��ͣ���ж���
    void PauseAllActions();

    /// \~chinese
    /// @brief ֹͣ���ж���
    void StopAllActions();

    /// \~chinese
    /// @brief ��ȡָ�����ƵĶ���
    /// @param name ��������
    ActionPtr GetAction(const String& name);

    /// \~chinese
    /// @brief ��ȡ���ж���
    const ActionList& GetAllActions() const;

    /// \~chinese
    /// @brief ���¶���
    void Update(Actor* target, Duration dt);

private:
    ActionList actions_;
};

/** @} */
}  // namespace kiwano
