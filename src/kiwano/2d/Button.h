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
 * @brief ��ť
 */
class KGE_API Button : public Component
{
public:
    /// \~chinese
    /// @brief ��ť�ص�����
    using Callback = Function<void(Button* /* self */, Actor* /* target */)>;

    /// \~chinese
    /// @brief ������ť
    /// @param click ��ť����ص�����
    static ButtonPtr Create(const Callback& click);

    /// \~chinese
    /// @brief ������ť
    /// @param click ��ť����ص�����
    /// @param pressed ��ť���»ص�����
    /// @param mouse_over ��ť����ص�����
    /// @param mouse_out ��ť�Ƴ��ص�����
    static ButtonPtr Create(const Callback& click, const Callback& pressed, const Callback& mouse_over,
                            const Callback& mouse_out);

    Button();

    virtual ~Button();

    /// \~chinese
    /// @brief ���ð�ť�����Ļص�����
    void SetClickCallback(const Callback& func);

    /// \~chinese
    /// @brief ���ð�ť������ʱ�Ļص�����
    void SetPressedCallback(const Callback& func);

    /// \~chinese
    /// @brief ����������밴ťʱ�Ļص�����
    void SetMouseOverCallback(const Callback& func);

    /// \~chinese
    /// @brief ��������Ƴ���ťʱ�Ļص�����
    void SetMouseOutCallback(const Callback& func);

protected:
    /// \~chinese
    /// @brief ��ť״̬
    enum class Status
    {
        Normal,  ///< ��ͨ
        Hover,   ///< ����ڰ�ť��
        Pressed  ///< ������
    };

    /// \~chinese
    /// @brief ��ȡ��ť״̬
    Status GetStatus() const;

    /// \~chinese
    /// @brief ���ð�ť״̬
    void SetStatus(Status status);

    /// \~chinese
    /// @brief ��ʼ�����
    void InitComponent(Actor* actor) override;

    /// \~chinese
    /// @brief �������
    void DestroyComponent() override;

    /// \~chinese
    /// @brief �����ɫ�¼�
    void HandleEvent(Event* evt) override;

private:
    Status   status_;
    Callback click_callback_;
    Callback pressed_callback_;
    Callback mouse_over_callback_;
    Callback mouse_out_callback_;
};

inline void Button::SetClickCallback(const Callback& func)
{
    click_callback_ = func;
}

inline void Button::SetPressedCallback(const Callback& func)
{
    pressed_callback_ = func;
}

inline void Button::SetMouseOverCallback(const Callback& func)
{
    mouse_over_callback_ = func;
}

inline void Button::SetMouseOutCallback(const Callback& func)
{
    mouse_out_callback_ = func;
}

inline Button::Status Button::GetStatus() const
{
    return status_;
}

}  // namespace kiwano
