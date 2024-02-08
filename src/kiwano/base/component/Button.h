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
#include <kiwano/base/component/MouseSensor.h>

namespace kiwano
{

/**
 * \addtogroup Component
 * @{
 */

/**
 * \~chinese
 * @brief 基础按钮组件
 */
class KGE_API ButtonBase : public MouseSensor
{
public:
    /// \~chinese
    /// @brief 按钮事件
    enum class Event
    {
        MouseEntered,  ///< 鼠标浮于按钮上
        MouseExited,   ///< 鼠标从按钮上移出
        Pressed,       ///< 鼠标按下
        Released,      ///< 鼠标抬起
        Clicked,       ///< 鼠标点击
    };

    ButtonBase();

    virtual ~ButtonBase();

    /// \~chinese
    /// @brief 按钮事件发生时
    virtual void OnEvent(Event evt) = 0;

protected:
    /// \~chinese
    /// @brief 处理角色事件
    void HandleEvent(kiwano::Event* evt) override;
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

    /// \~chinese
    /// @brief 按钮点击回调函数
    using ClickedCallback = Function<void(Button* /* self */)>;

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
    /// @brief 设置按钮回调函数
    /// @param cb 按钮回调函数
    void SetCallbackOnClicked(const ClickedCallback& cb);

    /// \~chinese
    /// @brief 按钮状态变化时
    void OnEvent(Event evt) override;

private:
    Callback        cb_;
    ClickedCallback clicked_cb_;
};

/** @} */

inline void Button::SetCallback(const Button::Callback& cb)
{
    cb_ = cb;
}

inline void Button::SetCallbackOnClicked(const Button::ClickedCallback& cb)
{
    clicked_cb_ = cb;
}

}  // namespace kiwano
