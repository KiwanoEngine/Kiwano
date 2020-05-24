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
#include <mutex>
#include <kiwano/core/Common.h>
#include <kiwano/base/Module.h>
#include <kiwano/core/Time.h>
#include <kiwano/core/Singleton.h>
#include <kiwano/event/Event.h>
#include <kiwano/platform/Runner.h>
#include <kiwano/platform/Window.h>
#include <kiwano/utils/Timer.h>

namespace kiwano
{

/**
 * \~chinese
 * @brief ��ȡ�汾��
 */
extern KGE_API int GetVersion();

/**
 * \~chinese
 * @brief Ӧ�ó��򣬿�����Ϸ�������������ڣ�������ʼ���������������Լ��¼��ַ���
 */
class KGE_API Application : public Singleton<Application>
{
    friend Singleton<Application>;

public:
    Application();

    virtual ~Application();

    /**
     * \~chinese
     * @brief ����Ӧ�ó���
     * @param runner ����������
     * @param debug �Ƿ����õ���ģʽ
     * @note �ú����������ģ�Ӧ�ó������ʱ��������
     */
    void Run(RunnerPtr runner, bool debug = false);

    /**
     * \~chinese
     * @brief ��ֹӦ�ó���
     */
    void Quit();

    /**
     * \~chinese
     * @brief ��ȡ����������
     */
    RunnerPtr GetRunner() const;

    /**
     * \~chinese
     * @brief ��ȡ������
     */
    WindowPtr GetMainWindow() const;

    /**
     * \~chinese
     * @brief ��ȡ��ʱ��
     */
    TimerPtr GetTimer() const;

    /**
     * \~chinese
     * @brief �Ƿ���������
     */
    bool IsRunning() const;

    /**
     * \~chinese
     * @brief ���ģ��
     * @param[in] module ģ��
     */
    void Use(Module& module);

    /**
     * \~chinese
     * @brief ����ʱ����������
     * @details ����ʱ���������ӿɵȱ����Ŵ����Сʱ�����
     * @param scale_factor ��������
     * @warning ����Ϊ�������ܵ��¶���ϵͳ����
     */
    void SetTimeScale(float scale_factor);

    /**
     * \~chinese
     * @brief �ַ��¼�
     * @details ���¼��ַ��������¼�����ģ��
     * @param evt �¼�
     */
    void DispatchEvent(EventPtr evt);

    /**
     * \~chinese
     * @brief �ַ��¼�
     * @details ���¼��ַ��������¼�����ģ��
     * @param evt �¼�
     */
    void DispatchEvent(Event* evt);

    /**
     * \~chinese
     * @brief �����߳���ִ�к���
     * @details �ṩ�������̵߳��� Kiwano ����������
     * @param func ��Ҫִ�еĺ���
     */
    void PreformInMainThread(Function<void()> func);

    /**
     * \~chinese
     * @brief ��������ģ��
     * @param dt ʱ����
     */
    void Update(Duration dt);

    /**
     * \~chinese
     * @brief ������Ⱦ�����Ĳ���Ⱦ����
     */
    void Render();

    /**
     * \~chinese
     * @brief ������Ϸ���й����в�����������Դ
     */
    void Destroy();

private:
    bool                    running_;
    float                   time_scale_;
    RunnerPtr               runner_;
    TimerPtr                timer_;
    List<Module*>           modules_;
    std::mutex              perform_mutex_;
    Queue<Function<void()>> functions_to_perform_;
};

inline RunnerPtr Application::GetRunner() const
{
    return runner_;
}

inline WindowPtr Application::GetMainWindow() const
{
    KGE_ASSERT(runner_);
    return runner_->GetMainWindow();
}

inline TimerPtr Application::GetTimer() const
{
    return timer_;
}

inline bool Application::IsRunning() const
{
    return running_;
}

}  // namespace kiwano
