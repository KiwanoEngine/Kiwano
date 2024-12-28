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

#include <kiwano/2d/animation/AnimationGroup.h>

namespace kiwano
{

AnimationGroup::AnimationGroup()
    : parallel_(false)
{
}

AnimationGroup::AnimationGroup(const Vector<RefPtr<Animation>>& animations, bool parallel)
    : parallel_(parallel)
{
    AddAnimation(animations);
}

AnimationGroup::~AnimationGroup() {}

void AnimationGroup::Init(Actor* target)
{
    if (animations_.IsEmpty())
    {
        Done();
        return;
    }

    // reset all animations
    for (auto& anim : animations_)
    {
        anim->Reset();
    }

    if (!parallel_)
    {
        current_ = animations_.GetFirst();
    }
}

void AnimationGroup::Update(Actor* target, Duration dt)
{
    if (!parallel_)
    {
        if (current_)
        {
            current_->UpdateStep(target, dt);

            if (current_->IsDone())
            {
                current_ = current_->GetNext();

                if (!current_)
                    Complete(target);
            }
        }
    }
    else
    {
        bool done = true;
        for (auto& anim : animations_)
        {
            if (!anim->IsDone())
            {
                done = false;
                anim->UpdateStep(target, dt);
            }
        }

        if (done)
        {
            Complete(target);
        }
    }
}

void AnimationGroup::AddAnimation(RefPtr<Animation> animation)
{
    if (animation)
    {
        animations_.PushBack(animation);
    }
}

void AnimationGroup::AddAnimation(const Vector<RefPtr<Animation>>& animations)
{
    for (const auto& animation : animations)
        AddAnimation(animation);
}

AnimationGroup* AnimationGroup::Clone() const
{
    Vector<RefPtr<Animation>> animations;
    if (!animations_.IsEmpty())
    {
        for (auto animation = animations_.GetFirst(); animation; animation = animation->GetNext())
        {
            animations.push_back(animation->Clone());
        }
    }
    AnimationGroup* ptr = new AnimationGroup(animations, parallel_);
    DoClone(ptr);
    return ptr;
}

AnimationGroup* AnimationGroup::Reverse() const
{
    Vector<RefPtr<Animation>> animations;
    if (!animations_.IsEmpty())
    {
        for (auto animation = animations_.GetLast(); animation; animation = animation->GetPrev())
        {
            animations.push_back(animation->Reverse());
        }
    }
    AnimationGroup* ptr = new AnimationGroup(animations, parallel_);
    DoClone(ptr);
    return ptr;
}

}  // namespace kiwano
