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
 * @brief 获取版本号
 */
extern KGE_API int GetVersion();

/**
 * \~chinese
 * @brief 应用程序，控制游戏的整个生命周期，包括初始化、启动、结束以及事件分发等
 */
class KGE_API Application : public Singleton<Application>
{
    friend Singleton<Application>;

public:
    Application();

    /**
     * \~chinese
     * @brief 启动应用程序
     * @param runner 程序运行器
     * @note 该函数是阻塞的，应用程序结束时函数返回
     */
    inline void Run(RefPtr<Runner> runner)
    {
        Run(*runner);
    }

    /**
     * \~chinese
     * @brief 启动应用程序
     * @param runner 程序运行器
     * @note 该函数是阻塞的，应用程序结束时函数返回
     */
    void Run(Runner& runner);

    /**
     * \~chinese
     * @brief 暂停应用程序
     */
    void Pause();

    /**
     * \~chinese
     * @brief 继续应用程序
     */
    void Resume();

    /**
     * \~chinese
     * @brief 终止应用程序
     */
    void Quit();

    /**
     * \~chinese
     * @brief 获取暂停状态
     */
    bool IsPaused() const;

    /**
     * \~chinese
     * @brief 添加模块
     * @param[in] m 模块
     * @param index 插入位置
     */
    void Use(Module& m, int index = -1);

    /**
     * \~chinese
     * @brief 获取所有模块
     */
    const ModuleList& GetModules() const;

    /**
     * \~chinese
     * @brief 获取程序运行器
     */
    Runner* GetRunner() const;

    /**
     * \~chinese
     * @brief 获取窗口
     */
    RefPtr<Window> GetWindow() const;

    /**
     * \~chinese
     * @brief 在主线程中执行函数
     * @details 提供在其他线程调用 Kiwano 函数的能力
     * @param func 需要执行的函数
     */
    void PerformInMainThread(Function<void()> func);

private:
    /**
     * \~chinese
     * @brief 更新所有模块
     * @param dt 时间间隔
     */
    void Update(Duration dt);

    /**
     * \~chinese
     * @brief 销毁游戏运行过程中产生的所有资源
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
