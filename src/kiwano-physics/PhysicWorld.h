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
#include <kiwano-physics/PhysicBody.h>
#include <kiwano-physics/Joint.h>

namespace kiwano
{
namespace physics
{

/**
 * \~chinese
 * \defgroup Physics 物理模块
 */

/**
 * \addtogroup Physics
 * @{
 */

/**
 * \~chinese
 * @brief 物理世界
 */
class KGE_API PhysicWorld : public Component
{
    friend class PhysicBody;
    friend class Joint;

public:
    PhysicWorld();

    /// \~chinese
    /// @brief 创建物理世界
    /// @param gravity 重力
    PhysicWorld(const Vec2& gravity);

    virtual ~PhysicWorld();

    /// \~chinese
    /// @brief 添加物体
    void AddBody(PhysicBodyPtr body);

    /// \~chinese
    /// @brief 移除物体
    void RemoveBody(PhysicBodyPtr body);

    /// \~chinese
    /// @brief 移除所有物体
    void RemoveAllBodies();

    /// \~chinese
    /// @brief 获取所有物体
    const List<PhysicBodyPtr>& GetAllBodies() const;

    /// \~chinese
    /// @brief 添加关节
    void AddJoint(JointPtr joint);

    /// \~chinese
    /// @brief 移除关节
    void RemoveJoint(JointPtr joint);

    /// \~chinese
    /// @brief 移除所有关节
    void RemoveAllJoints();

    /// \~chinese
    /// @brief 获取所有关节
    const List<JointPtr>& GetAllJoints() const;

    /// \~chinese
    /// @brief 获取重力 [N]
    Vec2 GetGravity() const;

    /// \~chinese
    /// @brief 设置重力 [N]
    void SetGravity(Vec2 gravity);

    /// \~chinese
    /// @brief 获取物理接触列表
    ContactList GetContactList();

    /// \~chinese
    /// @brief 设置速度迭代次数, 默认为 6
    void SetVelocityIterations(int vel_iter);

    /// \~chinese
    /// @brief 设置位置迭代次数, 默认为 2
    void SetPositionIterations(int pos_iter);

    /// \~chinese
    /// @brief 设置是否绘制调试信息
    void ShowDebugInfo(bool show);

    /// \~chinese
    /// @brief 获取b2World
    b2World* GetB2World();

    /// \~chinese
    /// @brief 获取b2World
    const b2World* GetB2World() const;

protected:
    /// \~chinese
    /// @brief 初始化组件
    void InitComponent(Actor* actor) override;

    /// \~chinese
    /// @brief 更新组件
    void OnUpdate(Duration dt) override;

    /// \~chinese
    /// @brief 渲染组件
    void OnRender(RenderContext& ctx) override;

    /// \~chinese
    /// @brief 分发物理世界事件
    void DispatchEvent(Event* evt);

    /// \~chinese
    /// @brief 关节移除时的回调函数
    void JointRemoved(b2Joint* b2joint);

    /// \~chinese
    /// @brief 更新物理世界前
    void BeforeSimulation(Actor* parent, const Matrix3x2& parent_to_world, float parent_rotation);

    /// \~chinese
    /// @brief 更新物理世界后
    void AfterSimulation(Actor* parent, const Matrix3x2& parent_to_world, float parent_rotation);

private:
    bool    debug_;
    int     vel_iter_;
    int     pos_iter_;
    b2World world_;

    class DebugDrawer;
    std::unique_ptr<DebugDrawer> drawer_;

    List<PhysicBodyPtr> bodies_;
    List<JointPtr>      joints_;

    std::unique_ptr<b2DestructionListener> destroy_listener_;
    std::unique_ptr<b2ContactListener>     contact_listener_;
};

/** @} */

inline void PhysicWorld::SetVelocityIterations(int vel_iter)
{
    vel_iter_ = vel_iter;
}

inline void PhysicWorld::SetPositionIterations(int pos_iter)
{
    pos_iter_ = pos_iter;
}

}  // namespace physics
}  // namespace kiwano
