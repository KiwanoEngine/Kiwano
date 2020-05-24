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

    virtual ~Application();

    /**
     * \~chinese
     * @brief 启动应用程序
     * @param runner 程序运行器
     * @param debug 是否启用调试模式
     * @note 该函数是阻塞的，应用程序结束时函数返回
     */
    void Run(RunnerPtr runner, bool debug = false);

    /**
     * \~chinese
     * @brief 终止应用程序
     */
    void Quit();

    /**
     * \~chinese
     * @brief 获取程序运行器
     */
    RunnerPtr GetRunner() const;

    /**
     * \~chinese
     * @brief 获取主窗口
     */
    WindowPtr GetMainWindow() const;

    /**
     * \~chinese
     * @brief 获取计时器
     */
    TimerPtr GetTimer() const;

    /**
     * \~chinese
     * @brief 是否正在运行
     */
    bool IsRunning() const;

    /**
     * \~chinese
     * @brief 添加模块
     * @param[in] module 模块
     */
    void Use(Module& module);

    /**
     * \~chinese
     * @brief 设置时间缩放因子
     * @details 设置时间缩放因子可等比例放大或缩小时间进度
     * @param scale_factor 缩放因子
     * @warning 设置为负数可能导致动画系统紊乱
     */
    void SetTimeScale(float scale_factor);

    /**
     * \~chinese
     * @brief 分发事件
     * @details 将事件分发给所有事件功能模块
     * @param evt 事件
     */
    void DispatchEvent(EventPtr evt);

    /**
     * \~chinese
     * @brief 分发事件
     * @details 将事件分发给所有事件功能模块
     * @param evt 事件
     */
    void DispatchEvent(Event* evt);

    /**
     * \~chinese
     * @brief 在主线程中执行函数
     * @details 提供在其他线程调用 Kiwano 函数的能力
     * @param func 需要执行的函数
     */
    void PreformInMainThread(Function<void()> func);

    /**
     * \~chinese
     * @brief 更新所有模块
     * @param dt 时间间隔
     */
    void Update(Duration dt);

    /**
     * \~chinese
     * @brief 创建渲染上下文并渲染画面
     */
    void Render();

    /**
     * \~chinese
     * @brief 销毁游戏运行过程中产生的所有资源
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
