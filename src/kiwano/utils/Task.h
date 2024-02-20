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
#include <kiwano/utils/Ticker.h>
#include <kiwano/core/IntrusiveList.h>

namespace kiwano
{
class TaskScheduler;

/// \~chinese
/// @brief ����
/// @details ��������ÿ��һ��ʱ��ִ��һ�λص��������ҿ���ָ��ִ���ܴ���
class KGE_API Task
    : public ObjectBase
    , protected IntrusiveListValue<RefPtr<Task>>
{
    friend class TaskScheduler;
    friend IntrusiveList<RefPtr<Task>>;

public:
    /// \~chinese
    /// @brief ����ص�����
    /// @details
    /// �ص�������һ�����������������ڶ���������ʱ������
    using Callback = Function<void(Task* /* self */, Duration /* dt */)>;

    /// \~chinese
    /// @brief ��������
    /// @param cb �ص�����
    /// @param ��ʱ��
    Task(const Callback& cb, RefPtr<Ticker> ticker);

    /// \~chinese
    /// @brief ��������
    /// @param name ����
    /// @param cb �ص�����
    /// @param ��ʱ��
    Task(StringView name, const Callback& cb, RefPtr<Ticker> ticker);

    /// \~chinese
    /// @brief ��������
    /// @param cb �ص�����
    /// @param interval ʱ����
    /// @param times ִ�д������� -1 Ϊ����ִ�У�
    Task(const Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief ��������
    /// @param name ����
    /// @param cb �ص�����
    /// @param interval ʱ����
    /// @param times ִ�д������� -1 Ϊ����ִ�У�
    Task(StringView name, const Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief ���������
    Task();

    /// \~chinese
    /// @brief ��������
    void Start();

    /// \~chinese
    /// @brief ֹͣ����
    void Stop();

    /// \~chinese
    /// @brief �Ƴ�����
    void Remove();

    /// \~chinese
    /// @brief �����Ƿ�������
    bool IsRunning() const;

    /// \~chinese
    /// @brief �����Ƿ���Ƴ�
    bool IsRemoveable() const;

    /// \~chinese
    /// @brief ��ȡ����ص�����
    Callback GetCallback() const;

    /// \~chinese
    /// @brief ��������ص�����
    void SetCallback(const Callback& callback);

    /// \~chinese
    /// @brief ��ȡ����ı�ʱ��
    RefPtr<Ticker> GetTicker() const;

    /// \~chinese
    /// @brief ��������ı�ʱ��
    void SetTicker(RefPtr<Ticker> ticker);

private:
    /// \~chinese
    /// @brief ��������
    void Update(Duration dt);

    /// \~chinese
    /// @brief ��������
    void Reset();

private:
    bool      running_;
    bool      removeable_;
    RefPtr<Ticker> ticker_;
    Callback  callback_;
};

inline void Task::Remove()
{
    removeable_ = true;
}

inline bool Task::IsRunning() const
{
    return running_;
}

inline bool Task::IsRemoveable() const
{
    return removeable_;
}

inline RefPtr<Ticker> Task::GetTicker() const
{
    return ticker_;
}

inline void Task::SetTicker(RefPtr<Ticker> ticker)
{
    ticker_ = ticker;
    if (ticker_)
    {
        if (running_)
            ticker_->Resume();
        else
            ticker_->Pause();
    }
}

inline Task::Callback Task::GetCallback() const
{
    return callback_;
}

inline void Task::SetCallback(const Task::Callback& callback)
{
    callback_ = callback;
}
}  // namespace kiwano
