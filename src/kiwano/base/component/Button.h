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
 * \addtogroup Component
 * @{
 */

/**
 * \~chinese
 * @brief 基础按钮组件
 */
class KGE_API ButtonBase : public Component
{
public:
    /// \~chinese
    /// @brief 按钮事件
    enum class Event
    {
        Click,      ///< 被点击
        Pressed,    ///< 被按下
        MouseOver,  ///< 鼠标浮于按钮上
        MouseOut,   ///< 鼠标从按钮上移出
    };

    /// \~chinese
    /// @brief 按钮状态
    enum class Status
    {
        Normal,   ///< 普通
        Hover,    ///< 鼠标在按钮内
        Pressed,  ///< 被按下
    };

    ButtonBase();

    virtual ~ButtonBase();

    /// \~chinese
    /// @brief 按钮事件发生时
    virtual void OnEvent(Event evt) = 0;

    /// \~chinese
    /// @brief 获取按钮状态
    Status GetStatus() const;

protected:
    /// \~chinese
    /// @brief 设置按钮状态
    void SetStatus(Status status);

    /// \~chinese
    /// @brief 初始化组件
    void InitComponent(Actor* actor) override;

    /// \~chinese
    /// @brief 销毁组件
    void DestroyComponent() override;

    /// \~chinese
    /// @brief 处理角色事件
    void HandleEvent(kiwano::Event* evt) override;

private:
    Status status_;
};

/**
 * \~chinese
 * @brief 按钮组件
 */
class KGE_API Button : public ButtonBase
{
public:
    /// \~chinese
    /// @brief 按钮回调函数
    using Callback = Function<void(Button* /* self */, Event /* evt */)>;

    Button();

    /// \~chinese
    /// @brief 创建按钮
    /// @param cb 按钮回调函数
    Button(const Callback& cb);

    /// \~chinese
    /// @brief 设置按钮回调函数
    /// @param cb 按钮回调函数
    void SetCallback(const Callback& cb);

    /// \~chinese
    /// @brief 按钮状态变化时
    void OnEvent(Event evt) override;

private:
    Callback cb_;
};

/** @} */

inline void Button::SetCallback(const Callback& cb)
{
    cb_ = cb;
}

inline ButtonBase::Status ButtonBase::GetStatus() const
{
    return status_;
}

}  // namespace kiwano
