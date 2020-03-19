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
#include <kiwano/core/Timer.h>

namespace kiwano
{
/**
 * \~chinese
 * @brief ��ʱ��������
 */
class KGE_API TimerManager
{
public:
    /// \~chinese
    /// @brief ��Ӷ�ʱ��
    /// @param cb �ص�����
    /// @param interval ʱ����
    /// @param times ִ�д������� -1 Ϊ����ִ�У�
    Timer* AddTimer(const Timer::Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief ��Ӷ�ʱ��
    /// @param name ��ʱ������
    /// @param cb �ص�����
    /// @param interval ʱ����
    /// @param times ִ�д������� -1 Ϊ����ִ�У�
    Timer* AddTimer(const String& name, const Timer::Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief ��Ӷ�ʱ��
    Timer* AddTimer(TimerPtr timer);

    /// \~chinese
    /// @brief ������ʱ��
    void StartTimers(const String& timer_name);

    /// \~chinese
    /// @brief ֹͣ��ʱ��
    void StopTimers(const String& timer_name);

    /// \~chinese
    /// @brief �Ƴ���ʱ��
    void RemoveTimers(const String& timer_name);

    /// \~chinese
    /// @brief �������ж�ʱ��
    void StartAllTimers();

    /// \~chinese
    /// @brief ֹͣ���ж�ʱ��
    void StopAllTimers();

    /// \~chinese
    /// @brief �Ƴ����ж�ʱ��
    void RemoveAllTimers();

    /// \~chinese
    /// @brief ��ȡ���ж�ʱ��
    const TimerList& GetAllTimers() const;

protected:
    /// \~chinese
    /// @brief ���¶�ʱ��
    void UpdateTimers(Duration dt);

private:
    TimerList timers_;
};
}  // namespace kiwano
