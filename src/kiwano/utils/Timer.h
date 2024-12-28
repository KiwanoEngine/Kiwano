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
#include <kiwano/core/Time.h>
#include <kiwano/core/BaseObject.h>

namespace kiwano
{

/// \~chinese
/// @brief ��ʱ��
class KGE_API Timer : public BaseObject
{
public:
    Timer();

    virtual ~Timer();

    /// \~chinese
    /// @brief ��ȡʱ������
    Duration GetDeltaTime() const;

    /// \~chinese
    /// @brief ��ȡ��ʱ��
    Duration GetTotalTime() const;

    /// \~chinese
    /// @brief ��ȡ��ͣ״̬
    bool IsPausing() const;

    /// \~chinese
    /// @brief ��ʱ
    void Tick();

    /// \~chinese
    /// @brief ������ʱ
    void Resume();

    /// \~chinese
    /// @brief ��ͣ��ʱ
    void Pause();

    /// \~chinese
    /// @brief ���ü�ʱ��
    void Reset();

private:
    bool     is_paused_;
    Time     start_time_;
    Time     paused_time_;
    Time     current_time_;
    Time     previous_time_;
    Duration delta_time_;
    Duration total_idle_time_;
};

}  // namespace kiwano
