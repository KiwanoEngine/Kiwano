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
#include <kiwano/utils/Timer.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Ticker);

/// \~chinese
/// @brief ��ʱ��
class KGE_API Ticker
    : public ObjectBase
{
public:
    /// \~chinese
    /// @brief ������ʱ��
    /// @param interval ��ʱ���
    /// @param times ��ʱ�������� -1 Ϊ���ã�
    static TickerPtr Create(Duration interval, int times = -1);

    Ticker();

    /// \~chinese
    /// @brief ��ʱ
    /// @return �Ƿ�ﵽ��ʱʱ��
    virtual bool Tick();

    /// \~chinese
    /// @brief ��ʱ
    /// @param dt ʱ������
    /// @return �Ƿ�ﵽ��ʱʱ��
    virtual bool Tick(Duration dt);

    /// \~chinese
    /// @brief ��ȡʱ������
    Duration GetDeltaTime();

    /// \~chinese
    /// @brief ��ȡ��ͣ״̬
    bool IsPausing() const;

    /// \~chinese
    /// @brief ��ͣ��ʱ��
    void Pause();

    /// \~chinese
    /// @brief ������ʱ��
    void Resume();

    /// \~chinese
    /// @brief ��ȡ��ʱ����ʱ����
    int GetTickedTimes() const;

    /// \~chinese
    /// @brief ��ȡ��ʱ���ܱ�ʱ����
    int GetTotalTickTimes() const;

    /// \~chinese
    /// @brief ���ñ�ʱ���ܱ�ʱ����
    void SetTotalTickTimes(int times);

    /// \~chinese
    /// @brief ��ȡ��ʱ���
    Duration GetInterval() const;

    /// \~chinese
    /// @brief ���ñ�ʱ���
    void SetInterval(Duration interval);

    /// \~chinese
    /// @brief ��ȡ��ʱ��
    TimerPtr GetTimer();

    /// \~chinese
    /// @brief ���ü�ʱ��
    void SetTimer(TimerPtr timer);

    /// \~chinese
    /// @brief ���ñ�ʱ��
    void Reset();

private:
    bool     is_paused_;
    int      ticked_times_;
    int      total_times_;
    Duration interval_;
    Duration elapsed_time_;
    Duration delta_time_;
    Duration error_time_;
    TimerPtr timer_;
};

inline bool Ticker::IsPausing() const
{
    return is_paused_;
}

inline int Ticker::GetTickedTimes() const
{
    return ticked_times_;
}

inline int Ticker::GetTotalTickTimes() const
{
    return total_times_;
}

inline void Ticker::SetTotalTickTimes(int times)
{
    total_times_ = times;
}

inline Duration Ticker::GetInterval() const
{
    return interval_;
}

inline void Ticker::SetInterval(Duration interval)
{
    interval_ = interval;
}

}  // namespace kiwano
