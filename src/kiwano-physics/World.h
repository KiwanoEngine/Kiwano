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
#include <kiwano-physics/Body.h>
#include <kiwano-physics/Contact.h>

#define KGE_COMP_PHYSIC_WORLD "__KGE_PHYSIC_WORLD__"

namespace kiwano
{
namespace physics
{

/**
 * \~chinese
 * \defgroup Physics ����ģ��
 */

/**
 * \addtogroup Physics
 * @{
 */

/**
 * \~chinese
 * @brief ��������
 */
class KGE_API World : public Component
{
    friend class Body;
    friend class Joint;

public:
    /// \~chinese
    /// @brief ������������
    /// @param gravity ����
    World(const b2Vec2& gravity);

    virtual ~World();

    /// \~chinese
    /// @brief �������
    RefPtr<Body> AddBody(b2BodyDef* def);

    /// \~chinese
    /// @brief ��ӹؽ�
    b2Joint* AddJoint(b2JointDef* def);

    /// \~chinese
    /// @brief ��ȡ����Ӵ��б�
    ContactList GetContactList();

    /// \~chinese
    /// @brief �����ٶȵ�������, Ĭ��Ϊ 6
    void SetVelocityIterations(int vel_iter);

    /// \~chinese
    /// @brief ����λ�õ�������, Ĭ��Ϊ 2
    void SetPositionIterations(int pos_iter);

    /// \~chinese
    /// @brief �����Ƿ���Ƶ�����Ϣ
    void ShowDebugInfo(bool show);

    /// \~chinese
    /// @brief ��ȡb2World
    b2World* GetB2World();

    /// \~chinese
    /// @brief ��ȡb2World
    const b2World* GetB2World() const;

protected:
    /// \~chinese
    /// @brief ��ʼ�����
    void InitComponent(Actor* actor) override;

    /// \~chinese
    /// @brief �������
    void OnUpdate(Duration dt) override;

    /// \~chinese
    /// @brief ��Ⱦ���
    void OnRender(RenderContext& ctx) override;

    /// \~chinese
    /// @brief �ַ����������¼�
    void DispatchEvent(Event* evt);

    /// \~chinese
    /// @brief ������������ǰ
    void BeforeSimulation(Actor* parent, const Matrix3x2& parent_to_world, float parent_rotation);

    /// \~chinese
    /// @brief �������������
    void AfterSimulation(Actor* parent, const Matrix3x2& parent_to_world, float parent_rotation);

private:
    int     vel_iter_;
    int     pos_iter_;
    float   fixed_acc_;
    b2World world_;

    class DebugDrawer;
    std::unique_ptr<DebugDrawer> drawer_;

    std::unique_ptr<b2ContactListener> contact_listener_;
};

/** @} */

inline void World::SetVelocityIterations(int vel_iter)
{
    vel_iter_ = vel_iter;
}

inline void World::SetPositionIterations(int pos_iter)
{
    pos_iter_ = pos_iter;
}

}  // namespace physics
}  // namespace kiwano
