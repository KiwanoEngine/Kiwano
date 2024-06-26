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
#include <kiwano/2d/Stage.h>
#include <kiwano/render/Layer.h>

namespace kiwano
{
class Director;
class RenderContext;

/**
 * \~chinese
 * \defgroup Transition 舞台过渡动画
 */

/**
 * \addtogroup Transition
 * @{
 */

/**
 * \~chinese
 * @brief 舞台过渡动画
 */
class KGE_API Transition : public ObjectBase
{
    friend class Director;

public:
    Transition();

    virtual ~Transition();

    /**
     * \~chinese
     * @brief 设置动画时长
     */
    void SetDuration(Duration dt);

    /**
     * \~chinese
     * @brief 场景过渡动画是否已结束
     */
    bool IsDone();

protected:
    /**
     * \~chinese
     * @brief 初始化场景过渡动画
     * @param[in] prev 转出场景
     * @param[in] next 转入场景
     */
    virtual void Init(Stage* prev, Stage* next);

    /**
     * \~chinese
     * @brief 更新过渡动画
     * @param dt 距上一次更新的时间间隔
     */
    virtual void Update(Duration dt);

    /**
     * \~chinese
     * @brief 渲染过度动画
     * @param[in] ctx 渲染上下文
     */
    virtual void Render(RenderContext& ctx);

    /**
     * \~chinese
     * @brief 停止动画
     */
    virtual void Stop();

    /**
     * \~chinese
     * @brief 重置动画
     */
    virtual void Reset() {}

protected:
    bool          done_;
    float         process_;
    Duration      duration_;
    Duration      delta_;
    Size          window_size_;
    RefPtr<Stage> out_stage_;
    RefPtr<Stage> in_stage_;
    Layer         out_layer_;
    Layer         in_layer_;
};

/** @} */

inline void Transition::SetDuration(Duration dt)
{
    duration_ = dt;
}

}  // namespace kiwano
