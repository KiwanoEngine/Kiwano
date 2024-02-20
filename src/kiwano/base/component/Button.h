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
 * @brief ������ť���
 */
class KGE_API ButtonBase : public MouseSensor
{
public:
    /// \~chinese
    /// @brief ��ť�¼�
    enum class Event
    {
        MouseEntered,  ///< ��긡�ڰ�ť��
        MouseExited,   ///< ���Ӱ�ť���Ƴ�
        Pressed,       ///< ��갴��
        Released,      ///< ���̧��
        Clicked,       ///< �����
    };

    ButtonBase();

    virtual ~ButtonBase();

    /// \~chinese
    /// @brief ��ť�¼�����ʱ
    virtual void OnEvent(Event evt) = 0;

protected:
    /// \~chinese
    /// @brief �����ɫ�¼�
    void HandleEvent(kiwano::Event* evt) override;
};

/**
 * \~chinese
 * @brief ��ť���
 */
class KGE_API Button : public ButtonBase
{
public:
    /// \~chinese
    /// @brief ��ť�ص�����
    using Callback = Function<void(Button* /* self */, Event /* evt */)>;

    /// \~chinese
    /// @brief ��ť����ص�����
    using ClickedCallback = Function<void(Button* /* self */)>;

    Button();

    /// \~chinese
    /// @brief ������ť
    /// @param cb ��ť�ص�����
    Button(const Callback& cb);

    /// \~chinese
    /// @brief ���ð�ť�ص�����
    /// @param cb ��ť�ص�����
    void SetCallback(const Callback& cb);

    /// \~chinese
    /// @brief ���ð�ť�ص�����
    /// @param cb ��ť�ص�����
    void SetCallbackOnClicked(const ClickedCallback& cb);

    /// \~chinese
    /// @brief ��ť״̬�仯ʱ
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
