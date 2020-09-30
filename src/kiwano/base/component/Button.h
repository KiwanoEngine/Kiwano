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
 * @brief ������ť���
 */
class KGE_API ButtonBase : public Component
{
public:
    /// \~chinese
    /// @brief ��ť�¼�
    enum class Event
    {
        Click,      ///< �����
        Pressed,    ///< ������
        MouseOver,  ///< ��긡�ڰ�ť��
        MouseOut,   ///< ���Ӱ�ť���Ƴ�
    };

    /// \~chinese
    /// @brief ��ť״̬
    enum class Status
    {
        Normal,   ///< ��ͨ
        Hover,    ///< ����ڰ�ť��
        Pressed,  ///< ������
    };

    ButtonBase();

    virtual ~ButtonBase();

    /// \~chinese
    /// @brief ��ť�¼�����ʱ
    virtual void OnEvent(Event evt) = 0;

    /// \~chinese
    /// @brief ��ȡ��ť״̬
    Status GetStatus() const;

protected:
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
    void HandleEvent(kiwano::Event* evt) override;

private:
    Status status_;
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
    /// @brief ��ť״̬�仯ʱ
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
