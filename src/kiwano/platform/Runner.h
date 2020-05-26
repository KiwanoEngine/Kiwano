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
#include <kiwano/core/Common.h>
#include <kiwano/core/Time.h>
#include <kiwano/platform/Window.h>

namespace kiwano
{

class Application;

KGE_DECLARE_SMART_PTR(Runner);

/**
 * \~chinese
 * @brief ����������
 */
class KGE_API Runner : public ObjectBase
{
    friend class Application;

public:
    /// \~chinese
    /// @brief ��������������
    /// @param main_window ������
    static RunnerPtr Create(WindowPtr main_window);

    /// \~chinese
    /// @brief ��������������
    /// @param main_window ������
    /// @param on_ready Ӧ�ó����ʼ����ɺ�ִ�еĻص�����
    /// @param on_destroy Ӧ�ó�������ʱִ�еĻص�����
    static RunnerPtr Create(WindowPtr main_window, Function<void()> on_ready, Function<void()> on_destroy = nullptr);

    Runner();

    virtual ~Runner();

    /// \~chinese
    /// @brief ��ʼ����ɴ���
    /// @details ���ظú�������Ӧ�ó����ʼ����ɺ��Զ�ִ��
    virtual void OnReady();

    /// \~chinese
    /// @brief Ӧ�ó������ٴ���
    /// @details ���ظú����Դ���Ӧ�ó�������ʱ����Ϊ���������Դ���յ�
    virtual void OnDestroy();

    /// \~chinese
    /// @brief Ӧ�ó���رմ���
    /// @details ���ظú����Դ����û��ر�Ӧ�ó���ʱ����Ϊ���籣���û����ݵ�
    /// @return ����true�����û��رճ��򣬷�����ֹ����ر�
    virtual bool OnClosing();

    /// \~chinese
    /// @brief Ӧ�ó�����ѭ��
    /// @param dt ʱ����
    /// @details ���ظú����Կ��Ƴ�����ѭ��
    /// @return ����false�˳���ѭ�����������������ѭ��
    virtual bool MainLoop(Duration dt);

    /// \~chinese
    /// @brief ��ȡ������
    WindowPtr GetMainWindow() const;

    /// \~chinese
    /// @brief ����������
    void SetMainWindow(WindowPtr window);

private:
    WindowPtr main_window_;
};

inline void Runner::OnReady() {}

inline void Runner::OnDestroy() {}

inline bool Runner::OnClosing()
{
    return true;
}

inline WindowPtr Runner::GetMainWindow() const
{
    return main_window_;
}

inline void Runner::SetMainWindow(WindowPtr window)
{
    main_window_ = window;
}

}  // namespace kiwano
