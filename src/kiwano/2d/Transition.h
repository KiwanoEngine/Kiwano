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

KGE_DECLARE_SMART_PTR(Transition);
KGE_DECLARE_SMART_PTR(FadeTransition);
KGE_DECLARE_SMART_PTR(EmergeTransition);
KGE_DECLARE_SMART_PTR(BoxTransition);
KGE_DECLARE_SMART_PTR(MoveTransition);
KGE_DECLARE_SMART_PTR(RotationTransition);

/**
 * \~chinese
 * @brief 舞台过渡动画
 */
class KGE_API Transition : public virtual ObjectBase
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
    virtual void Init(StagePtr prev, StagePtr next);

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
    bool      done_;
    float     process_;
    Duration  duration_;
    Duration  delta_;
    Size      window_size_;
    StagePtr  out_stage_;
    StagePtr  in_stage_;
    Layer out_layer_;
    Layer in_layer_;
};

/**
 * \~chinese
 * @brief 淡入淡出过渡动画
 * @details 前一场景淡出动画结束后，后一场景淡入
 */
class FadeTransition : public Transition
{
public:
    /**
     * \~chinese
     * @brief 创建淡入淡出过渡动画
     * @param duration 动画时长
     */
    static FadeTransitionPtr Create(Duration duration);

    FadeTransition();

protected:
    void Update(Duration dt) override;

    virtual void Init(StagePtr prev, StagePtr next) override;
};

/**
 * \~chinese
 * @brief 渐变过渡动画
 * @details 前一场景淡出动画的同时，后一场景淡入
 */
class EmergeTransition : public Transition
{
public:
    /**
     * \~chinese
     * @brief 创建渐变过渡动画
     * @param duration 动画时长
     */
    static EmergeTransitionPtr Create(Duration duration);

    EmergeTransition();

protected:
    void Update(Duration dt) override;

    virtual void Init(StagePtr prev, StagePtr next) override;
};

/**
 * \~chinese
 * @brief 盒状过渡动画
 * @details 前一场景以盒状收缩至消失，后一场景以盒状扩大
 */
class BoxTransition : public Transition
{
public:
    /**
     * \~chinese
     * @brief 创建盒状过渡动画
     * @param duration 动画时长
     */
    static BoxTransitionPtr Create(Duration duration);

    BoxTransition();

protected:
    void Update(Duration dt) override;

    virtual void Init(StagePtr prev, StagePtr next) override;
};

/**
 * \~chinese
 * @brief 位移过渡动画
 * @details 两场景以位移的方式切换
 */
class MoveTransition : public Transition
{
public:
    /**
     * \~chinese
     * @brief 位移方式
     */
    enum class Type : int
    {
        Up,    ///< 上移
        Down,  ///< 下移
        Left,  ///< 左移
        Right  ///< 右移
    };

    /**
     * \~chinese
     * @brief 创建位移过渡动画
     * @param duration 动画时长
     * @param type 位移方式
     */
    static MoveTransitionPtr Create(Duration duration, Type type);

    MoveTransition();

protected:
    void Update(Duration dt) override;

    virtual void Init(StagePtr prev, StagePtr next) override;

    void Reset() override;

private:
    Type  type_;
    Point pos_delta_;
    Point start_pos_;
};

/**
 * \~chinese
 * @brief 旋转过渡动画
 * @details 前一场景以旋转方式收缩至消失，后一场景以旋转方式扩大
 */
class RotationTransition : public Transition
{
public:
    /**
     * \~chinese
     * @brief 创建旋转过渡动画
     * @param duration 动画时长
     * @param rotation 旋转度数
     */
    static RotationTransitionPtr Create(Duration duration, float rotation = 360.0f);

    RotationTransition();

protected:
    void Update(Duration dt) override;

    virtual void Init(StagePtr prev, StagePtr next) override;

    void Reset() override;

private:
    float rotation_;
};


inline void Transition::SetDuration(Duration dt)
{
    duration_ = dt;
}

}  // namespace kiwano
