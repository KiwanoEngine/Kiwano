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

#include <kiwano/ability/TransformAbility.h>

namespace kiwano
{

TransformAbility::TransformAbility()
    : dirty_flag_(false)
    , parent_(nullptr)
    , transform_()
    , matrix_()
    , children_()
{
}

void TransformAbility::OnDetached()
{
    SetParent(nullptr);
    Ability::OnDetached();
}

Transform& TransformAbility::GetTransform()
{
    dirty_flag_ = true;
    return transform_;
}

void TransformAbility::SetTransform(const Transform& transform)
{
    transform_  = transform;
    dirty_flag_ = true;
}

void TransformAbility::SetParent(TransformAbility* parent)
{
    if (parent_)
    {
        parent_->children_.erase(this);
    }

    parent_ = parent;
    if (parent_)
    {
        parent_->children_.insert(this); // TODO resize
        dirty_flag_ = true;
    }
}

const Matrix3x2& TransformAbility::GetMatrix() const
{
    UpdateDirtyMatrixUpwards();
    return matrix_;
}

const Matrix3x2& TransformAbility::GetInverseMatrix() const
{
    UpdateDirtyMatrixUpwards();
    return matrix_.Invert();
}

void TransformAbility::UpdateDirtyMatrix() const
{
    if (!dirty_flag_)
        return;

    matrix_ = transform_.ToMatrix();
    if (parent_)
        matrix_ *= parent_->GetMatrix();

    dirty_flag_ = false;
    for (const auto& child : children_)
        child->dirty_flag_ = true;
}

void TransformAbility::UpdateDirtyMatrixUpwards() const
{
    if (parent_)
        parent_->UpdateDirtyMatrixUpwards();

    UpdateDirtyMatrix();
}

}  // namespace kiwano
