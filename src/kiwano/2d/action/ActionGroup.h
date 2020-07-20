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
KGE_DECLARE_SMART_PTR(ActionGroupEntity);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief �������
class KGE_API ActionGroup : public Action
{
public:
    /// \~chinese
    /// @brief �����������
    /// @param actions ��������
    /// @param parallel ͬ��ִ��
    ActionGroup(const Vector<ActionEntityPtr>& actions, bool parallel = false);
};

/// \~chinese
/// @brief �������ʵ��
class KGE_API ActionGroupEntity : public ActionEntity
{
public:
    /// \~chinese
    /// @brief �����������
    /// @param actions ��������
    /// @param parallel ͬ��ִ��
    static ActionGroupEntityPtr Create(const Vector<ActionEntityPtr>& actions, bool parallel = false);

    ActionGroupEntity();

    ActionGroupEntity(bool parallel);

    virtual ~ActionGroupEntity();

    /// \~chinese
    /// @brief ��Ӷ���
    /// @param action ����
    void AddAction(ActionEntityPtr action);

    /// \~chinese
    /// @brief ��Ӷ������
    /// @param actions ��������
    void AddActions(const Vector<ActionEntityPtr>& actions);

    /// \~chinese
    /// @brief ��ȡ���ж���
    const ActionList& GetActions() const;

    /// \~chinese
    /// @brief ��ȡ�ö����Ŀ�������
    ActionEntityPtr Clone() const override;

    /// \~chinese
    /// @brief ��ȡ�ö����ĵ�ת
    ActionEntityPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void Update(Actor* target, Duration dt) override;

private:
    bool            parallel_;
    ActionEntityPtr current_;
    ActionList      actions_;
};

/** @} */

inline const ActionList& ActionGroupEntity::GetActions() const
{
    return actions_;
}

}  // namespace kiwano
