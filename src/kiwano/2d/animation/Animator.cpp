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

#include <kiwano/2d/Actor.h>
#include <kiwano/2d/animation/Animator.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{

void Animator::Update(Actor* target, Duration dt)
{
    if (animations_.IsEmpty() || !target)
        return;

    for (auto iter = animations_.begin(); iter != animations_.end();)
    {
        RefPtr<Animation> animation = *(iter++);

        if (animation->IsRunning())
            animation->UpdateStep(target, dt);

        if (animation->IsRemoveable())
            animations_.Remove(animation);
    }
}

Animation* Animator::AddAnimation(RefPtr<Animation> animation)
{
    KGE_ASSERT(animation && "AddAnimation failed, NULL pointer exception");

    if (animation)
    {
        animations_.PushBack(animation);
    }
    return animation.Get();
}

void Animator::ResumeAllAnimations()
{
    if (animations_.IsEmpty())
        return;

    for (auto& animation : animations_)
    {
        animation->Resume();
    }
}

void Animator::PauseAllAnimations()
{
    if (animations_.IsEmpty())
        return;

    for (auto& animation : animations_)
    {
        animation->Pause();
    }
}

void Animator::StopAllAnimations()
{
    if (animations_.IsEmpty())
        return;

    for (auto& animation : animations_)
    {
        animation->Stop();
    }
}

Animation* Animator::GetAnimation(StringView name)
{
    if (animations_.IsEmpty())
        return nullptr;

    for (auto& animation : animations_)
        if (animation->IsName(name))
            return animation.Get();
    return nullptr;
}

const AnimationList& Animator::GetAllAnimations() const
{
    return animations_;
}
}  // namespace kiwano
