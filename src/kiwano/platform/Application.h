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
#include <kiwano/core/Time.h>
#include <kiwano/core/Singleton.h>
#include <kiwano/module/Module.h>
#include <kiwano/module/Runner.h>
#include <kiwano/module/Window.h>
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

    /**
     * \~chinese
     * @brief ����Ӧ�ó���
     * @param runner ����������
     * @note �ú����������ģ�Ӧ�ó������ʱ��������
     */
    inline void Run(RefPtr<Runner> runner)
    {
        Run(*runner);
    }

    /**
     * \~chinese
     * @brief ����Ӧ�ó���
     * @param runner ����������
     * @note �ú����������ģ�Ӧ�ó������ʱ��������
     */
    void Run(Runner& runner);

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
     * @param index ����λ��
     */
    void Use(Module& m, int index = -1);

    /**
     * \~chinese
     * @brief ��ȡ����ģ��
     */
    const ModuleList& GetModules() const;

    /**
     * \~chinese
     * @brief ��ȡ����������
     */
    Runner* GetRunner() const;

    /**
     * \~chinese
     * @brief ��ȡ����
     */
    RefPtr<Window> GetWindow() const;

    /**
     * \~chinese
     * @brief �����߳���ִ�к���
     * @details �ṩ�������̵߳��� Kiwano ����������
     * @param func ��Ҫִ�еĺ���
     */
    void PerformInMainThread(Function<void()> func);

private:
    /**
     * \~chinese
     * @brief ��������ģ��
     * @param dt ʱ����
     */
    void Update(Duration dt);

    /**
     * \~chinese
     * @brief ������Ϸ���й����в�����������Դ
     */
    void Destroy();

private:
    bool                    running_;
    bool                    is_paused_;
    Runner*                 runner_;
    RefPtr<Timer>           timer_;
    ModuleList              modules_;
    std::mutex              perform_mutex_;
    Queue<Function<void()>> functions_to_perform_;
};

inline Runner* Application::GetRunner() const
{
    return runner_;
}

inline RefPtr<Window> Application::GetWindow() const
{
    return runner_ ? runner_->GetWindow() : nullptr;
}

inline bool Application::IsPaused() const
{
    return is_paused_;
}

}  // namespace kiwano
