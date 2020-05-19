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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Time.h>
#include <kiwano/core/IntrusiveList.h>

namespace kiwano
{
class TaskManager;

KGE_DECLARE_SMART_PTR(Task);

/// \~chinese
/// @brief �����б�
typedef IntrusiveList<TaskPtr> TaskList;

/// \~chinese
/// @brief ����
/// @details ��������ÿ��һ��ʱ��ִ��һ�λص��������ҿ���ָ��ִ���ܴ���
class KGE_API Task
    : public ObjectBase
    , protected IntrusiveListValue<TaskPtr>
{
    friend class TaskManager;
    friend IntrusiveList<TaskPtr>;

public:
    /// \~chinese
    /// @brief ����ص�����
    /// @details
    /// �ص�������һ�����������������ڶ��������Ǿ����ϴ�ִ�������ʱ����
    using Callback = Function<void(Task* /* self */, Duration /* dt */)>;

    /// \~chinese
    /// @brief ��������
    /// @param cb �ص�����
    /// @param interval ʱ����
    /// @param times ִ�д������� -1 Ϊ����ִ�У�
    static TaskPtr Create(const Callback& cb, Duration interval, int times = -1);

    /// \~chinese
    /// @brief ��������
    /// @param name ����
    /// @param cb �ص�����
    /// @param interval ʱ����
    /// @param times ִ�д������� -1 Ϊ����ִ�У�
    static TaskPtr Create(const String& name, const Callback& cb, Duration interval, int times = -1);

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
    /// @brief ��ȡ����ִ�й��ص������Ĵ���
    int GetRunTimes() const;

    /// \~chinese
    /// @brief ��ȡ����ִ�лص��������ܴ���
    int GetTotalRunTimes() const;

    /// \~chinese
    /// @brief ��������ִ�лص��������ܴ���
    void SetTotalRunTimes(int times);

    /// \~chinese
    /// @brief ��ȡ����ִ��ʱ����
    Duration GetInterval() const;

    /// \~chinese
    /// @brief ��������ִ��ʱ����
    void SetInterval(Duration interval);

    /// \~chinese
    /// @brief ��ȡ����ص�����
    Callback GetCallback() const;

    /// \~chinese
    /// @brief ��������ص�����
    void SetCallback(const Callback& callback);

private:
    /// \~chinese
    /// @brief ��������
    void Update(Duration dt);

    /// \~chinese
    /// @brief ��������
    void Reset();

private:
    bool     running_;
    bool     removeable_;
    int      run_times_;
    int      total_times_;
    Duration interval_;
    Duration elapsed_;
    Callback callback_;
};

inline void Task::Start()
{
    running_ = true;
}

inline void Task::Stop()
{
    running_ = false;
}

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

inline int Task::GetRunTimes() const
{
    return run_times_;
}

inline int Task::GetTotalRunTimes() const
{
    return total_times_;
}

inline void Task::SetTotalRunTimes(int times)
{
    total_times_ = times;
}

inline Duration Task::GetInterval() const
{
    return interval_;
}

inline void Task::SetInterval(Duration interval)
{
    interval_ = interval;
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
