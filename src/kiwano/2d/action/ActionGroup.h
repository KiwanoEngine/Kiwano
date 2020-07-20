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
/// @brief 动画组合
class KGE_API ActionGroup : public Action
{
public:
    /// \~chinese
    /// @brief 创建动画组合
    /// @param actions 动画集合
    /// @param parallel 同步执行
    ActionGroup(const Vector<ActionEntityPtr>& actions, bool parallel = false);
};

/// \~chinese
/// @brief 动画组合实体
class KGE_API ActionGroupEntity : public ActionEntity
{
public:
    /// \~chinese
    /// @brief 创建动画组合
    /// @param actions 动画集合
    /// @param parallel 同步执行
    static ActionGroupEntityPtr Create(const Vector<ActionEntityPtr>& actions, bool parallel = false);

    ActionGroupEntity();

    ActionGroupEntity(bool parallel);

    virtual ~ActionGroupEntity();

    /// \~chinese
    /// @brief 添加动画
    /// @param action 动画
    void AddAction(ActionEntityPtr action);

    /// \~chinese
    /// @brief 添加多个动画
    /// @param actions 动画集合
    void AddActions(const Vector<ActionEntityPtr>& actions);

    /// \~chinese
    /// @brief 获取所有动画
    const ActionList& GetActions() const;

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionEntityPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
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
