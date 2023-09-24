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
class KGE_API Application
    : public Singleton<Application>
{
    friend Singleton<Application>;

public:
    Application();

    virtual ~Application();

    /**
     * \~chinese
     * @brief ����Ӧ�ó���
     * @param settings ��Ϸ����
     * @param setup ��������
     * @note �ú����������ģ�Ӧ�ó������ʱ��������
     */
    void Run(const Settings& settings, const Function<void()>& setup, std::initializer_list<Module*> modules = {});

    /**
     * \~chinese
     * @brief ����Ӧ�ó���
     * @param runner ����������
     * @note �ú����������ģ�Ӧ�ó������ʱ��������
     */
    void Run(RunnerPtr runner);

    /**
     * \~chinese
     * @brief ��ͣӦ�ó���
     */
    void Pause();

    /**
     * \~chinese
     * @brief ����Ӧ�ó���
     */
    void Resume();

    /**
     * \~chinese
     * @brief ��ֹӦ�ó���
     */
    void Quit();

    /**
     * \~chinese
     * @brief ��ȡ��ͣ״̬
     */
    bool IsPaused() const;

    /**
     * \~chinese
     * @brief ���ģ��
     * @param[in] m ģ��
     */
    void Use(Module& m);

    /**
     * \~chinese
     * @brief ��ȡ����������
     */
    RunnerPtr GetRunner() const;

    /**
     * \~chinese
     * @brief ��ȡ����
     */
    WindowPtr GetWindow() const;

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
    void PerformInMainThread(Function<void()> func);

    /**
     * \~chinese
     * @brief ����һ֡
     * @param dt ʱ����
     */
    void UpdateFrame(Duration dt);

    /**
     * \~chinese
     * @brief ������Ϸ���й����в�����������Դ
     */
    void Destroy();

private:
    /**
     * \~chinese
     * @brief ��������ģ��
     * @param dt ʱ����
     */
    void Update(Duration dt);

    /**
     * \~chinese
     * @brief ��Ⱦ����
     */
    void Render();

private:
    bool                    running_;
    bool                    is_paused_;
    float                   time_scale_;
    RunnerPtr               runner_;
    TimerPtr                timer_;
    ModuleList              modules_;
    std::mutex              perform_mutex_;
    Queue<Function<void()>> functions_to_perform_;
};

inline RunnerPtr Application::GetRunner() const
{
    return runner_;
}

inline WindowPtr Application::GetWindow() const
{
    if (runner_)
        return runner_->GetWindow();
    return nullptr;
}

inline bool Application::IsPaused() const
{
    return is_paused_;
}

}  // namespace kiwano
