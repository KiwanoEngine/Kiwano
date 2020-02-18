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
#include <kiwano/2d/Actor.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Button);

/**
 * \~chinese
 * @brief 按钮
 */
class KGE_API Button : public Component
{
public:
    /// \~chinese
    /// @brief 按钮回调函数
    using Callback = Function<void(Button* /* self */, Actor* /* target */)>;

    /// \~chinese
    /// @brief 创建按钮
    /// @param actor 绑定的角色
    /// @param click 按钮点击回调函数
    static ButtonPtr Create(ActorPtr actor, Callback const& click);

    /// \~chinese
    /// @brief 创建按钮
    /// @param actor 绑定的角色
    /// @param click 按钮点击回调函数
    /// @param pressed 按钮按下回调函数
    /// @param mouse_over 按钮移入回调函数
    /// @param mouse_out 按钮移出回调函数
    static ButtonPtr Create(ActorPtr actor, Callback const& click, Callback const& pressed, Callback const& mouse_over,
                            Callback const& mouse_out);

    Button();

    virtual ~Button();

    /// \~chinese
    /// @brief 获取按钮状态是启用还是禁用
    bool IsEnable() const;

    /// \~chinese
    /// @brief 设置按钮启用或禁用
    void SetEnabled(bool enabled);

    /// \~chinese
    /// @brief 设置按钮点击后的回调函数
    void SetClickCallback(const Callback& func);

    /// \~chinese
    /// @brief 设置按钮被按下时的回调函数
    void SetPressedCallback(const Callback& func);

    /// \~chinese
    /// @brief 设置鼠标移入按钮时的回调函数
    void SetMouseOverCallback(const Callback& func);

    /// \~chinese
    /// @brief 设置鼠标移出按钮时的回调函数
    void SetMouseOutCallback(const Callback& func);

    /// \~chinese
    /// @brief 绑定到角色
    void BindActor(ActorPtr actor);

    /// \~chinese
    /// @brief 绑定到角色
    void BindActor(Actor* actor) override;

protected:
    /// \~chinese
    /// @brief 按钮状态
    enum class Status
    {
        Normal,  ///< 普通
        Hover,   ///< 鼠标在按钮内
        Pressed  ///< 被按下
    };

    /// \~chinese
    /// @brief 获取按钮状态
    Status GetStatus() const;

    /// \~chinese
    /// @brief 设置按钮状态
    void SetStatus(Status status);

    /// \~chinese
    /// @brief 处理角色事件
    void HandleEvent(Event* evt) override;

private:
    bool     enabled_;
    Status   status_;
    Callback click_callback_;
    Callback pressed_callback_;
    Callback mouse_over_callback_;
    Callback mouse_out_callback_;
};

inline void Button::BindActor(ActorPtr actor)
{
    this->BindActor(actor.Get());
}

}  // namespace kiwano
