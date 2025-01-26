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
#include <kiwano/transition/Transition.h>

namespace kiwano
{

/**
 * \addtogroup Transition
 * @{
 */

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
     * @param parallel 淡入和淡出同时进行
     */
    FadeTransition(Duration duration, bool parallel = false);

protected:
    void UpdateSelf(Duration dt) override;

    virtual void Init(Stage* prev, Stage* next) override;

private:
    bool parallel_;
};

/** @} */

}  // namespace kiwano
