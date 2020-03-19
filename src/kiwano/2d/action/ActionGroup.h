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
KGE_DECLARE_SMART_PTR(ActionGroup);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief 动画组合
class KGE_API ActionGroup : public Action
{
public:
    using ActionList = IntrusiveList<ActionPtr>;

    /// \~chinese
    /// @brief 创建动画组合
    /// @param actions 动画集合
    /// @param sync 同步执行
    static ActionGroupPtr Create(const Vector<ActionPtr>& actions, bool sync = false);

    ActionGroup();

    ActionGroup(bool sync);

    virtual ~ActionGroup();

    /// \~chinese
    /// @brief 添加动画
    /// @param action 动画
    void AddAction(ActionPtr action);

    /// \~chinese
    /// @brief 添加多个动画
    /// @param actions 动画集合
    void AddActions(const Vector<ActionPtr>& actions);

    /// \~chinese
    /// @brief 获取所有动画
    const ActionList& GetActions() const;

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void Update(Actor* target, Duration dt) override;

private:
    bool       sync_;
    ActionPtr  current_;
    ActionList actions_;
};

/** @} */

inline const ActionGroup::ActionList& ActionGroup::GetActions() const
{
    return actions_;
}

}  // namespace kiwano
