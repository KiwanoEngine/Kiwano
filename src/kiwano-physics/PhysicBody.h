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
#include <kiwano-physics/Fixture.h>
#include <kiwano-physics/ContactEdge.h>

namespace kiwano
{
namespace physics
{

class PhysicWorld;

/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief 物体
class KGE_API PhysicBody : public Component
{
    friend class PhysicWorld;

public:
    /// \~chinese
    /// @brief 物体类型
    enum class Type
    {
        Static = 0,  ///< 静态物体
        Kinematic,   ///< 动力学物体
        Dynamic,     ///< 动态物体
    };

    /// \~chinese
    /// @brief 初始化物体
    /// @param world 物理世界
    /// @param type 物体类型
    PhysicBody(PhysicWorld* world, Type type);

    /// \~chinese
    /// @brief 初始化物体
    /// @param world 物理世界
    /// @param type 物体类型
    PhysicBody(RefPtr<PhysicWorld> world, Type type);

    virtual ~PhysicBody();

    /// \~chinese
    /// @brief 初始化物体
    /// @param[in] world 物理世界
    bool Init(PhysicWorld* world);

    /// \~chinese
    /// @brief 添加夹具
    void AddFixture(RefPtr<Fixture> fixture);

    /// \~chinese
    /// @brief 添加圆形夹具
    /// @param radius 圆形半径
    /// @param density 物体密度
    /// @param
    Fixture* AddCircleShape(float radius, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief 添加矩形夹具
    /// @param size 矩形大小
    /// @param density 物体密度
    Fixture* AddRectShape(const Vec2& size, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief 添加多边形夹具
    /// @param vertexs 多边形端点
    /// @param density 物体密度
    Fixture* AddPolygonShape(const Vector<Point>& vertexs, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief 添加线段形夹具
    /// @param p1 线段起点
    /// @param p2 线段终点
    /// @param density 物体密度
    Fixture* AddEdgeShape(const Point& p1, const Point& p2, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief 添加链条形夹具
    /// @param vertexs 链条端点
    /// @param loop 是否闭合
    /// @param density 物体密度
    Fixture* AddChainShape(const Vector<Point>& vertices, bool loop, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief 移除夹具
    void RemoveFixture(RefPtr<Fixture> fixture);

    /// \~chinese
    /// @brief 移除所有夹具
    void RemoveAllFixtures();

    /// \~chinese
    /// @brief 获取夹具列表
    FixtureList GetFixtureList() const;

    /// \~chinese
    /// @brief 获取接触边列表
    ContactEdgeList GetContactList() const;

    /// \~chinese
    /// @brief 获取类别码
    uint16_t GetCategoryBits() const;

    /// \~chinese
    /// @brief 设置类别码
    void SetCategoryBits(uint16_t category_bits);

    /// \~chinese
    /// @brief 获取碰撞掩码
    uint16_t GetMaskBits() const;

    /// \~chinese
    /// @brief 设置碰撞掩码
    void SetMaskBits(uint16_t mask_bits);

    /// \~chinese
    /// @brief 获取组索引
    int16_t GetGroupIndex() const;

    /// \~chinese
    /// @brief 设置组索引
    void SetGroupIndex(int16_t index);

    /// \~chinese
    /// @brief 获取旋转角度
    float GetRotation() const;

    /// \~chinese
    /// @brief 设置旋转角度
    void SetRotation(float angle);

    /// \~chinese
    /// @brief 获取物体位置
    Point GetPosition() const;

    /// \~chinese
    /// @brief 设置物体位置
    void SetPosition(const Point& pos);

    /// \~chinese
    /// @brief 位置和旋转变换
    void SetTransform(const Point& pos, float angle);

    /// \~chinese
    /// @brief 获取质量 [kg]
    float GetMass() const;

    /// \~chinese
    /// @brief 获取惯性
    float GetInertia() const;

    /// \~chinese
    /// @brief 获取质量数据
    /// @param[out] mass 物体质量 [kg]
    /// @param[out] center 质心位置
    /// @param[out] inertia 惯性
    void GetMassData(float* mass, Point* center, float* inertia) const;

    /// \~chinese
    /// @brief 设置质量数据
    /// @param mass 物体质量 [kg]
    /// @param center 质心位置
    /// @param inertia 惯性
    void SetMassData(float mass, const Point& center, float inertia);

    /// \~chinese
    /// @brief 重置质量数据
    void ResetMassData();

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

    /// \~chinese
    /// @brief 获取物体质心相对于物体的位置
    Point GetLocalCenter() const;

    /// \~chinese
    /// @brief 获取物体质心位置
    Point GetWorldCenter() const;

    /// \~chinese
    /// @brief 获取物体类型
    Type GetType() const;

    /// \~chinese
    /// @brief 设置物体类型
    void SetType(Type type);

    /// \~chinese
    /// @brief 获取物体受重力的比例
    float GetGravityScale() const;

    /// \~chinese
    /// @brief 设置物体受重力的比例
    void SetGravityScale(float scale);

    /// \~chinese
    /// @brief 获取线性阻尼
    float GetLinearDamping() const;

    /// \~chinese
    /// @brief 设置线性阻尼
    void SetLinearDamping(float damping);

    /// \~chinese
    /// @brief 获取旋转阻尼
    float GetAngularDamping() const;

    /// \~chinese
    /// @brief 设置旋转阻尼
    void SetAngularDamping(float damping);

    /// \~chinese
    /// @brief 施力
    /// @param force 力的大小和方向
    /// @param point 施力点
    /// @param wake 是否唤醒物体
    void ApplyForce(const Vec2& force, const Point& point, bool wake = true);

    /// \~chinese
    /// @brief 给物体中心施力
    /// @param force 力的大小和方向
    /// @param wake 是否唤醒物体
    void ApplyForceToCenter(const Vec2& force, bool wake = true);

    /// \~chinese
    /// @brief 施加扭矩
    /// @param torque 扭矩
    /// @param wake 是否唤醒物体
    void ApplyTorque(float torque, bool wake = false);

    /// \~chinese
    /// @brief 旋转角度是否固定
    bool IsIgnoreRotation() const;

    /// \~chinese
    /// @brief 设置是否固定旋转角度
    void SetIgnoreRotation(bool flag);

    /// \~chinese
    /// @brief 是否是子弹物体
    bool IsBullet() const;

    /// \~chinese
    /// @brief 设置物体是否是子弹物体
    void SetBullet(bool flag);

    /// \~chinese
    /// @brief 是否处于唤醒状态
    bool IsAwake() const;

    /// \~chinese
    /// @brief 设置唤醒状态
    void SetAwake(bool flag);

    /// \~chinese
    /// @brief 是否启用休眠
    bool IsSleepingAllowed() const;

    /// \~chinese
    /// @brief 设置是否允许休眠
    void SetSleepingAllowed(bool flag);

    /// \~chinese
    /// @brief 是否启用
    bool IsActive() const;

    /// \~chinese
    /// @brief 设置启用状态
    void SetActive(bool flag);

    /// \~chinese
    /// @brief 获取物体所在物理世界
    PhysicWorld* GetWorld() const;

    /// \~chinese
    /// @brief 销毁物体
    void Destroy();

    /// \~chinese
    /// @brief 获取b2Body
    b2Body* GetB2Body() const;

    /// \~chinese
    /// @brief 设置b2Body
    void SetB2Body(b2Body* body);

protected:
    /// \~chinese
    /// @brief 初始化组件
    void InitComponent(Actor* actor) override;

    /// \~chinese
    /// @brief 销毁组件
    void DestroyComponent() override;

    /// \~chinese
    /// @brief 更新物体状态
    void UpdateFromActor(Actor* actor);

    /// \~chinese
    /// @brief 更新物体状态
    void UpdateFromActor(Actor* actor, const Matrix3x2& actor_to_world, float parent_rotation);

    /// \~chinese
    /// @brief 更新夹具过滤器
    void UpdateFixtureFilter(b2Fixture* fixture);

    /// \~chinese
    /// @brief 更新物理身体前
    void BeforeSimulation(Actor* actor, const Matrix3x2& parent_to_world, const Matrix3x2& actor_to_world,
                          float parent_rotation);

    /// \~chinese
    /// @brief 更新物理身体后
    void AfterSimulation(Actor* actorconst, const Matrix3x2& parent_to_world, float parent_rotation);

private:
    PhysicWorld* world_;
    b2Body*      body_;
    uint16_t     category_bits_;
    uint16_t     mask_bits_;
    int16_t      group_index_;

    PhysicBody::Type   type_;
    Vector<RefPtr<Fixture>> fixtures_;

    Point offset_;
    Point position_cached_;
};

/** @} */

inline FixtureList PhysicBody::GetFixtureList() const
{
    KGE_ASSERT(body_);

    if (!body_->GetFixtureList())
        return FixtureList();

    Fixture* fixture = static_cast<Fixture*>(body_->GetFixtureList()->GetUserData());
    return FixtureList(fixture);
}

inline ContactEdgeList PhysicBody::GetContactList() const
{
    KGE_ASSERT(body_);
    ContactEdge edge;
    edge.SetB2ContactEdge(body_->GetContactList());
    return ContactEdgeList(edge);
}

inline uint16_t PhysicBody::GetCategoryBits() const
{
    return category_bits_;
}

inline uint16_t PhysicBody::GetMaskBits() const
{
    return mask_bits_;
}

inline int16_t PhysicBody::GetGroupIndex() const
{
    return group_index_;
}

inline float PhysicBody::GetRotation() const
{
    KGE_ASSERT(body_);
    return math::Radian2Degree(body_->GetAngle());
}

inline void PhysicBody::SetRotation(float angle)
{
    SetTransform(GetPosition(), angle);
}

inline void PhysicBody::SetPosition(const Point& pos)
{
    SetTransform(pos, GetRotation());
}

inline float PhysicBody::GetMass() const
{
    KGE_ASSERT(body_);
    return body_->GetMass();
}

inline float PhysicBody::GetInertia() const
{
    KGE_ASSERT(body_);
    return body_->GetInertia();
}

inline PhysicBody::Type PhysicBody::GetType() const
{
    return type_;
}

inline void PhysicBody::SetType(Type type)
{
    if (type_ != type)
    {
        type_ = type;
        if (body_)
        {
            body_->SetType(b2BodyType(type));
        }
    }
}

inline float PhysicBody::GetGravityScale() const
{
    KGE_ASSERT(body_);
    return body_->GetGravityScale();
}

inline void PhysicBody::SetGravityScale(float scale)
{
    KGE_ASSERT(body_);
    body_->SetGravityScale(scale);
}

inline float PhysicBody::GetLinearDamping() const
{
    KGE_ASSERT(body_);
    return body_->GetLinearDamping();
}

inline void PhysicBody::SetLinearDamping(float damping)
{
    KGE_ASSERT(body_);
    body_->SetLinearDamping(damping);
}

inline float PhysicBody::GetAngularDamping() const
{
    KGE_ASSERT(body_);
    return body_->GetAngularDamping();
}

inline void PhysicBody::SetAngularDamping(float damping)
{
    KGE_ASSERT(body_);
    body_->SetAngularDamping(damping);
}

inline bool PhysicBody::IsIgnoreRotation() const
{
    KGE_ASSERT(body_);
    return body_->IsFixedRotation();
}

inline void PhysicBody::SetIgnoreRotation(bool flag)
{
    KGE_ASSERT(body_);
    body_->SetFixedRotation(flag);
}

inline bool PhysicBody::IsBullet() const
{
    KGE_ASSERT(body_);
    return body_->IsBullet();
}

inline void PhysicBody::SetBullet(bool flag)
{
    KGE_ASSERT(body_);
    body_->SetBullet(flag);
}

inline bool PhysicBody::IsAwake() const
{
    KGE_ASSERT(body_);
    return body_->IsAwake();
}

inline void PhysicBody::SetAwake(bool flag)
{
    KGE_ASSERT(body_);
    body_->SetAwake(flag);
}

inline bool PhysicBody::IsSleepingAllowed() const
{
    KGE_ASSERT(body_);
    return body_->IsSleepingAllowed();
}

inline void PhysicBody::SetSleepingAllowed(bool flag)
{
    KGE_ASSERT(body_);
    body_->SetSleepingAllowed(flag);
}

inline bool PhysicBody::IsActive() const
{
    KGE_ASSERT(body_);
    return body_->IsActive();
}

inline void PhysicBody::SetActive(bool flag)
{
    KGE_ASSERT(body_);
    body_->SetActive(flag);
}

inline b2Body* PhysicBody::GetB2Body() const
{
    return body_;
}

inline PhysicWorld* PhysicBody::GetWorld() const
{
    return world_;
}

}  // namespace physics
}  // namespace kiwano
