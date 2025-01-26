// Copyright (c) 2018-2019 Kiwano - Nomango
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
#include <kiwano-physics/Global.h>

#define KGE_COMP_PHYSIC_BODY "__KGE_PHYSIC_BODY__"

namespace kiwano
{
namespace physics
{

class World;

/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief 物理身体
class KGE_API Body : public Ability
{
    friend class World;

public:
    /// \~chinese
    /// @brief 初始化物体
    /// @param world 物理世界
    /// @param type 物体类型
    Body(b2Body* body, b2World* world = nullptr);

    virtual ~Body();

    /// \~chinese
    /// @brief 获取b2Body
    b2Body* GetB2Body() const;

    /// \~chinese
    /// @brief 获取世界坐标系上的点在物体上的位置
    Point GetLocalPoint(const Point& world) const;

    /// \~chinese
    /// @brief 将世界坐标系中的向量转换到物体坐标系下
    Vec2 GetLocalVector(const Vec2& world) const;

    /// \~chinese
    /// @brief 获取物体上的点在世界坐标系的位置
    Point GetWorldPoint(const Point& local) const;

    /// \~chinese
    /// @brief 将物体坐标系中的向量转换到世界坐标系下
    Vec2 GetWorldVector(const Vec2& local) const;

protected:
    void OnAttached(Actor* actor) override;

    void OnDetached() override;

    /// \~chinese
    /// @brief 更新物体状态
    void UpdateFromActor(Actor* actor);

    /// \~chinese
    /// @brief 更新物体状态
    void UpdateFromActor(Actor* actor, const Matrix3x2& actor_to_world, float parent_rotation);

    /// \~chinese
    /// @brief 更新物理身体前
    void BeforeSimulation(Actor* actor, const Matrix3x2& parent_to_world, const Matrix3x2& actor_to_world,
                          float parent_rotation);

    /// \~chinese
    /// @brief 更新物理身体后
    void AfterSimulation(Actor* actorconst, const Matrix3x2& parent_to_world, float parent_rotation);

private:
    b2World* b2world_;
    b2Body*  b2body_;

    // Point offset_;
    Point position_cached_;
};

/** @} */

inline b2Body* Body::GetB2Body() const
{
    return b2body_;
}

}  // namespace physics
}  // namespace kiwano
