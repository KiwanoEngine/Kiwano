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
/// @brief ����
class KGE_API PhysicBody : public Component
{
    friend class PhysicWorld;

public:
    /// \~chinese
    /// @brief ��������
    enum class Type
    {
        Static = 0,  ///< ��̬����
        Kinematic,   ///< ����ѧ����
        Dynamic,     ///< ��̬����
    };

    /// \~chinese
    /// @brief ��ʼ������
    /// @param world ��������
    /// @param type ��������
    PhysicBody(PhysicWorld* world, Type type);

    /// \~chinese
    /// @brief ��ʼ������
    /// @param world ��������
    /// @param type ��������
    PhysicBody(RefPtr<PhysicWorld> world, Type type);

    virtual ~PhysicBody();

    /// \~chinese
    /// @brief ��ʼ������
    /// @param[in] world ��������
    bool Init(PhysicWorld* world);

    /// \~chinese
    /// @brief ��Ӽо�
    void AddFixture(RefPtr<Fixture> fixture);

    /// \~chinese
    /// @brief ���Բ�μо�
    /// @param radius Բ�ΰ뾶
    /// @param density �����ܶ�
    /// @param
    Fixture* AddCircleShape(float radius, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief ��Ӿ��μо�
    /// @param size ���δ�С
    /// @param density �����ܶ�
    Fixture* AddRectShape(const Vec2& size, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief ��Ӷ���μо�
    /// @param vertexs ����ζ˵�
    /// @param density �����ܶ�
    Fixture* AddPolygonShape(const Vector<Point>& vertexs, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief ����߶��μо�
    /// @param p1 �߶����
    /// @param p2 �߶��յ�
    /// @param density �����ܶ�
    Fixture* AddEdgeShape(const Point& p1, const Point& p2, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief ��������μо�
    /// @param vertexs �����˵�
    /// @param loop �Ƿ�պ�
    /// @param density �����ܶ�
    Fixture* AddChainShape(const Vector<Point>& vertices, bool loop, float density, float friction = 0.2f);

    /// \~chinese
    /// @brief �Ƴ��о�
    void RemoveFixture(RefPtr<Fixture> fixture);

    /// \~chinese
    /// @brief �Ƴ����мо�
    void RemoveAllFixtures();

    /// \~chinese
    /// @brief ��ȡ�о��б�
    FixtureList GetFixtureList() const;

    /// \~chinese
    /// @brief ��ȡ�Ӵ����б�
    ContactEdgeList GetContactList() const;

    /// \~chinese
    /// @brief ��ȡ�����
    uint16_t GetCategoryBits() const;

    /// \~chinese
    /// @brief ���������
    void SetCategoryBits(uint16_t category_bits);

    /// \~chinese
    /// @brief ��ȡ��ײ����
    uint16_t GetMaskBits() const;

    /// \~chinese
    /// @brief ������ײ����
    void SetMaskBits(uint16_t mask_bits);

    /// \~chinese
    /// @brief ��ȡ������
    int16_t GetGroupIndex() const;

    /// \~chinese
    /// @brief ����������
    void SetGroupIndex(int16_t index);

    /// \~chinese
    /// @brief ��ȡ��ת�Ƕ�
    float GetRotation() const;

    /// \~chinese
    /// @brief ������ת�Ƕ�
    void SetRotation(float angle);

    /// \~chinese
    /// @brief ��ȡ����λ��
    Point GetPosition() const;

    /// \~chinese
    /// @brief ��������λ��
    void SetPosition(const Point& pos);

    /// \~chinese
    /// @brief λ�ú���ת�任
    void SetTransform(const Point& pos, float angle);

    /// \~chinese
    /// @brief ��ȡ���� [kg]
    float GetMass() const;

    /// \~chinese
    /// @brief ��ȡ����
    float GetInertia() const;

    /// \~chinese
    /// @brief ��ȡ��������
    /// @param[out] mass �������� [kg]
    /// @param[out] center ����λ��
    /// @param[out] inertia ����
    void GetMassData(float* mass, Point* center, float* inertia) const;

    /// \~chinese
    /// @brief ������������
    /// @param mass �������� [kg]
    /// @param center ����λ��
    /// @param inertia ����
    void SetMassData(float mass, const Point& center, float inertia);

    /// \~chinese
    /// @brief ������������
    void ResetMassData();

    /// \~chinese
    /// @brief ��ȡ��������ϵ�ϵĵ��������ϵ�λ��
    Point GetLocalPoint(const Point& world) const;

    /// \~chinese
    /// @brief ����������ϵ�е�����ת������������ϵ��
    Vec2 GetLocalVector(const Vec2& world) const;

    /// \~chinese
    /// @brief ��ȡ�����ϵĵ�����������ϵ��λ��
    Point GetWorldPoint(const Point& local) const;

    /// \~chinese
    /// @brief ����������ϵ�е�����ת������������ϵ��
    Vec2 GetWorldVector(const Vec2& local) const;

    /// \~chinese
    /// @brief ��ȡ������������������λ��
    Point GetLocalCenter() const;

    /// \~chinese
    /// @brief ��ȡ��������λ��
    Point GetWorldCenter() const;

    /// \~chinese
    /// @brief ��ȡ��������
    Type GetType() const;

    /// \~chinese
    /// @brief ������������
    void SetType(Type type);

    /// \~chinese
    /// @brief ��ȡ�����������ı���
    float GetGravityScale() const;

    /// \~chinese
    /// @brief ���������������ı���
    void SetGravityScale(float scale);

    /// \~chinese
    /// @brief ��ȡ��������
    float GetLinearDamping() const;

    /// \~chinese
    /// @brief ������������
    void SetLinearDamping(float damping);

    /// \~chinese
    /// @brief ��ȡ��ת����
    float GetAngularDamping() const;

    /// \~chinese
    /// @brief ������ת����
    void SetAngularDamping(float damping);

    /// \~chinese
    /// @brief ʩ��
    /// @param force ���Ĵ�С�ͷ���
    /// @param point ʩ����
    /// @param wake �Ƿ�������
    void ApplyForce(const Vec2& force, const Point& point, bool wake = true);

    /// \~chinese
    /// @brief ����������ʩ��
    /// @param force ���Ĵ�С�ͷ���
    /// @param wake �Ƿ�������
    void ApplyForceToCenter(const Vec2& force, bool wake = true);

    /// \~chinese
    /// @brief ʩ��Ť��
    /// @param torque Ť��
    /// @param wake �Ƿ�������
    void ApplyTorque(float torque, bool wake = false);

    /// \~chinese
    /// @brief ��ת�Ƕ��Ƿ�̶�
    bool IsIgnoreRotation() const;

    /// \~chinese
    /// @brief �����Ƿ�̶���ת�Ƕ�
    void SetIgnoreRotation(bool flag);

    /// \~chinese
    /// @brief �Ƿ����ӵ�����
    bool IsBullet() const;

    /// \~chinese
    /// @brief ���������Ƿ����ӵ�����
    void SetBullet(bool flag);

    /// \~chinese
    /// @brief �Ƿ��ڻ���״̬
    bool IsAwake() const;

    /// \~chinese
    /// @brief ���û���״̬
    void SetAwake(bool flag);

    /// \~chinese
    /// @brief �Ƿ���������
    bool IsSleepingAllowed() const;

    /// \~chinese
    /// @brief �����Ƿ���������
    void SetSleepingAllowed(bool flag);

    /// \~chinese
    /// @brief �Ƿ�����
    bool IsActive() const;

    /// \~chinese
    /// @brief ��������״̬
    void SetActive(bool flag);

    /// \~chinese
    /// @brief ��ȡ����������������
    PhysicWorld* GetWorld() const;

    /// \~chinese
    /// @brief ��������
    void Destroy();

    /// \~chinese
    /// @brief ��ȡb2Body
    b2Body* GetB2Body() const;

    /// \~chinese
    /// @brief ����b2Body
    void SetB2Body(b2Body* body);

protected:
    /// \~chinese
    /// @brief ��ʼ�����
    void InitComponent(Actor* actor) override;

    /// \~chinese
    /// @brief �������
    void DestroyComponent() override;

    /// \~chinese
    /// @brief ��������״̬
    void UpdateFromActor(Actor* actor);

    /// \~chinese
    /// @brief ��������״̬
    void UpdateFromActor(Actor* actor, const Matrix3x2& actor_to_world, float parent_rotation);

    /// \~chinese
    /// @brief ���¼о߹�����
    void UpdateFixtureFilter(b2Fixture* fixture);

    /// \~chinese
    /// @brief ������������ǰ
    void BeforeSimulation(Actor* actor, const Matrix3x2& parent_to_world, const Matrix3x2& actor_to_world,
                          float parent_rotation);

    /// \~chinese
    /// @brief �������������
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
