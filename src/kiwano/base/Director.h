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
#include <kiwano/2d/Actor.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/2d/transition/Transition.h>
#include <kiwano/base/Module.h>

namespace kiwano
{
/**
 * \~chinese
 * @brief 导演
 * @details 导演完成舞台的渲染、更新、事件分发以及控制舞台间跳转
 * @see kiwano::Stage
 */
class KGE_API Director
    : public Singleton<Director>
    , public Module
{
    friend Singleton<Director>;

public:
    /**
     * \~chinese
     * @brief 切换舞台
     * @param[in] stage 舞台
     * @param[in] transition 过渡动画
     */
    void EnterStage(RefPtr<Stage> stage, RefPtr<Transition> transition = nullptr);

    /**
     * \~chinese
     * @brief 切换舞台，并将当前舞台储存到栈中
     * @param[in] stage 舞台
     * @param[in] transition 过渡动画
     */
    void PushStage(RefPtr<Stage> stage, RefPtr<Transition> transition = nullptr);

    /**
     * \~chinese
     * @brief 退出当前舞台，并切换到上一个舞台
     * @param[in] transition 过渡动画
     */
    void PopStage(RefPtr<Transition> transition = nullptr);

    /**
     * \~chinese
     * @brief 获取当前舞台
     * @return 返回当前舞台的指针
     */
    RefPtr<Stage> GetCurrentStage();

    /**
     * \~chinese
     * @brief 启用或禁用角色边界渲染功能
     * @param enabled 是否启用
     */
    void SetRenderBorderEnabled(bool enabled);

    /**
     * \~chinese
     * @brief 显示或隐藏调试信息
     * @param show 是否显示
     */
    void ShowDebugInfo(bool show = true);

    /**
     * \~chinese
     * @brief 退出当前舞台并清空舞台栈
     */
    void ClearStages();

public:
    void OnUpdate(UpdateModuleContext& ctx) override;

    void OnRender(RenderModuleContext& ctx) override;

    void HandleEvent(EventModuleContext& ctx) override;

    virtual ~Director();

private:
    Director();

private:
    bool            render_border_enabled_;
    Stack<RefPtr<Stage>> stages_;
    RefPtr<Stage>        current_stage_;
    RefPtr<Stage>        next_stage_;
    RefPtr<Actor>        debug_actor_;
    RefPtr<Transition>   transition_;
};
}  // namespace kiwano
