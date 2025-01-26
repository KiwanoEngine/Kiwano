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
#include <kiwano/ability/Ability.h>
#include <kiwano/event/MouseEvent.h>
#include <kiwano/event/listener/EventListener.h>

namespace kiwano
{

/**
 * \addtogroup Ability
 * @{
 */

/**
 * \~chinese
 * @brief 二维变换能力
 */
class KGE_API TransformAbility : public Ability
{
public:
    TransformAbility();

    /// \~chinese
    /// @brief 获取二维变换
    const Transform& GetTransform() const;

    /// \~chinese
    /// @brief 获取二维变换
    Transform& GetTransform();

    /// \~chinese
    /// @brief 设置二维变换
    void SetTransform(const Transform& transform);

    /// \~chinese
    /// @brief 获取父变换
    TransformAbility* GetParent() const;

    /// \~chinese
    /// @brief 设置父变换
    void SetParent(TransformAbility* parent);

    /// \~chinese
    /// @brief 获取子变换
    const UnorderedSet<TransformAbility*>& GetChildren() const;

    /// \~chinese
    /// @brief 获取二维变换矩阵
    const Matrix3x2& GetMatrix() const;

    /// \~chinese
    /// @brief 获取二维变换的逆矩阵
    const Matrix3x2& GetInverseMatrix() const;

private:
    void UpdateDirtyMatrix() const;

    void UpdateDirtyMatrixUpwards() const;

    void OnDetached() override;

private:
    mutable bool                    dirty_flag_;
    TransformAbility*               parent_;
    mutable Matrix3x2               matrix_;
    Transform                       transform_;
    UnorderedSet<TransformAbility*> children_;
};

/** @} */

inline const Transform& TransformAbility::GetTransform() const
{
    return transform_;
}

inline TransformAbility* TransformAbility::GetParent() const
{
    return parent_;
}

inline const UnorderedSet<TransformAbility*>& TransformAbility::GetChildren() const
{
    return children_;
}

}  // namespace kiwano
