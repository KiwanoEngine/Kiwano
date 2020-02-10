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

/**
 * \~chinese
 * @brief 动画管理器
 */
class KGE_API ActionManager
{
public:
    /// \~chinese
    /// @brief 动画列表
    using Actions = IntrusiveList<ActionPtr>;

    /// \~chinese
    /// @brief 添加动画
    Action* AddAction(ActionPtr action);

    /// \~chinese
    /// @brief 添加动画
    Action* AddAction(Action* action);

    /// \~chinese
    /// @brief 继续所有暂停动画
    void ResumeAllActions();

    /// \~chinese
    /// @brief 暂停所有动画
    void PauseAllActions();

    /// \~chinese
    /// @brief 停止所有动画
    void StopAllActions();

    /// \~chinese
    /// @brief 获取指定名称的动画
    /// @param name 动画名称
    ActionPtr GetAction(String const& name);

    /// \~chinese
    /// @brief 获取所有动画
    Actions const& GetAllActions() const;

protected:
    /// \~chinese
    /// @brief 更新动画
    void UpdateActions(Actor* target, Duration dt);

private:
    Actions actions_;
};

/** @} */
}  // namespace kiwano
