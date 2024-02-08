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

#include <kiwano/2d/animation/CustomAnimation.h>

namespace kiwano
{

CustomAnimation::CustomAnimation(Duration duration)
    : TweenAnimation(duration)
{
}

RefPtr<CustomAnimation> CustomAnimation::Create(Duration duration, Function<void(Actor*, float)> tween_func)
{
    class CallbackCustomAnimation : public CustomAnimation
    {
    public:
        CallbackCustomAnimation(Duration duration, Function<void(Actor*, float)> tween_func)
            : CustomAnimation(duration)
            , tween_func_(tween_func)
        {
        }

        void Init(Actor* target) override
        {
            if (!tween_func_)
                this->Done();
        }

        void OnAnimationUpdate(Actor* target, float frac) override
        {
            if (tween_func_)
                tween_func_(target, frac);
        }

        CallbackCustomAnimation* Clone() const
        {
            CallbackCustomAnimation* ptr = new CallbackCustomAnimation(GetDuration(), tween_func_);
            DoClone(ptr);
            return ptr;
        }

    private:
        Function<void(Actor*, float)> tween_func_;
    };
    RefPtr<CustomAnimation> ptr = new CallbackCustomAnimation(duration, tween_func);
    return ptr;
}

void CustomAnimation::UpdateTween(Actor* target, float frac)
{
    OnAnimationUpdate(target, frac);
}

}  // namespace kiwano
