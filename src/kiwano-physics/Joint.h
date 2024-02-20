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

namespace kiwano
{
namespace physics
{

/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief �ؽ�
class KGE_API Joint : public ObjectBase
{
public:
    /// \~chinese
    /// @brief �ؽ�����
    enum class Type
    {
        Unknown = 0,  ///< δ֪
        Revolute,     ///< ��ת�ؽ�
        Prismatic,    ///< ƽ�ƹؽ�
        Distance,     ///< �̶�����ؽ�
        Pulley,       ///< ���ֹؽ�
        Mouse,        ///< ���ؽ�
        Gear,         ///< ���ֹؽ�
        Wheel,        ///< �ֹؽ�
        Weld,         ///< ���ӹؽ�
        Friction,     ///< Ħ���ؽ�
        Rope,         ///< ���ؽ�
        Motor         ///< ���ؽ�
    };

    /// \~chinese
    /// @brief �ؽڻ�������
    struct ParamBase
    {
        PhysicBody* body_a;             ///< �ؽ����ӵ�����A
        PhysicBody* body_b;             ///< �ؽ����ӵ�����B
        bool        collide_connected;  // �ؽ����ӵ������Ƿ�������ײ

        ParamBase(PhysicBody* body_a, PhysicBody* body_b)
            : body_a(body_a)
            , body_b(body_b)
            , collide_connected(false)
        {
        }

        ParamBase(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b)
            : ParamBase(body_a.Get(), body_b.Get())
        {
        }
    };

    Joint();

    virtual ~Joint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    virtual bool Init(PhysicWorld* world);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world, b2JointDef* joint_def);

    /// \~chinese
    /// @brief ��ȡ�ؽ����ӵ�����A
    RefPtr<PhysicBody> GetBodyA() const;

    /// \~chinese
    /// @brief ��ȡ�ؽ����ӵ�����B
    RefPtr<PhysicBody> GetBodyB() const;

    /// \~chinese
    /// @brief ��ȡ��������
    PhysicWorld* GetWorld() const;

    /// \~chinese
    /// @brief ���ٹؽ�
    void Destroy();

    /// \~chinese
    /// @brief ��ȡb2Joint
    b2Joint* GetB2Joint() const;

    /// \~chinese
    /// @brief ����b2Joint
    void SetB2Joint(b2Joint* joint);

private:
    b2Joint*     joint_;
    PhysicWorld* world_;
    Type         type_;
};

/// \~chinese
/// @brief �̶�����ؽ�
class KGE_API DistanceJoint : public Joint
{
public:
    /// \~chinese
    /// @brief �̶�����ؽڲ���
    struct Param : public Joint::ParamBase
    {
        Point anchor_a;       ///< �ؽ�������A�ϵ����ӵ�
        Point anchor_b;       ///< �ؽ�������B�ϵ����ӵ�
        float frequency_hz;   ///< ��Ӧ�ٶȣ���ֵԽ�߹ؽ���Ӧ���ٶ�Խ�죬����ȥԽ���
        float damping_ratio;  ///< �����ʣ�ֵԽ��ؽ��˶�����Խ��

        Param()
            : Param(nullptr, nullptr, Point(), Point())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& anchor_a, const Point& anchor_b)
            : ParamBase(body_a, body_b)
            , anchor_a(anchor_a)
            , anchor_b(anchor_b)
            , frequency_hz(0.0f)
            , damping_ratio(0.0f)
        {
        }
    };

    /// \~chinese
    /// @brief �����̶�����ؽ�
    /// @param param �ؽڲ���
    DistanceJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief ���ùؽڳ���
    void SetLength(float length);

    /// \~chinese
    /// @brief ��ȡ�ؽڳ���
    float GetLength() const;

    /// \~chinese
    /// @brief ���õ�����Ӧ�ٶ� [����]
    void SetFrequency(float hz);

    /// \~chinese
    /// @brief ��ȡ������Ӧ�ٶ� [����]
    float GetFrequency() const;

    /// \~chinese
    /// @brief ����������
    void SetDampingRatio(float ratio);

    /// \~chinese
    /// @brief ��ȡ������
    float GetDampingRatio() const;

private:
    Param            param_;
    b2DistanceJoint* raw_joint_;
};

/// \~chinese
/// @brief Ħ���ؽ�
class KGE_API FrictionJoint : public Joint
{
public:
    struct Param : public Joint::ParamBase
    {
        Point anchor;      ///< Ħ�����õ�
        float max_force;   ///< ���Ħ����
        float max_torque;  ///< ���Ť��

        Param()
            : Param(nullptr, nullptr, Point())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& anchor, float max_force = 0.f,
              float max_torque = 0.f)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , max_force(max_force)
            , max_torque(max_torque)
        {
        }
    };

    /// \~chinese
    /// @brief ����Ħ���ؽ�
    /// @param param �ؽڲ���
    FrictionJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief �������Ħ����
    void SetMaxForce(float force);

    /// \~chinese
    /// @brief ��ȡ���Ħ����
    float GetMaxForce() const;

    /// \~chinese
    /// @brief �������ת��
    void SetMaxTorque(float torque);

    /// \~chinese
    /// @brief ��ȡ���ת��
    float GetMaxTorque() const;

private:
    Param            param_;
    b2FrictionJoint* raw_joint_;
};

/// \~chinese
/// @brief ���ֹؽ�
class KGE_API GearJoint : public Joint
{
public:
    /// \~chinese
    /// @brief ���ֹؽڲ���
    struct Param : public Joint::ParamBase
    {
        Joint* joint_a;  ///< �ؽ�A����ת�ؽ�/ƽ�ƹؽڣ�
        Joint* joint_b;  ///< �ؽ�B����ת�ؽ�/ƽ�ƹؽڣ�
        float  ratio;    ///< ���ִ�����

        Param()
            : Param(nullptr, nullptr)
        {
        }

        Param(RefPtr<Joint> joint_a, RefPtr<Joint> joint_b, float ratio = 1.f)
            : ParamBase(nullptr, nullptr)
            , joint_a(joint_a.Get())
            , joint_b(joint_b.Get())
            , ratio(ratio)
        {
        }
    };

    /// \~chinese
    /// @brief �������ֹؽ�
    /// @param param �ؽڲ���
    GearJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief �趨���ִ�����
    void SetRatio(float ratio);

    /// \~chinese
    /// @brief ��ȡ���ִ�����
    float GetRatio() const;

private:
    Param        param_;
    b2GearJoint* raw_joint_;
};

/// \~chinese
/// @brief ���ؽ�
class KGE_API MotorJoint : public Joint
{
public:
    /// \~chinese
    /// @brief ���ؽڲ���
    struct Param : public Joint::ParamBase
    {
        float max_force;          ///< ���Ħ����
        float max_torque;         ///< ���ת��
        float correction_factor;  ///< λ�ý������ӣ���Χ 0-1��

        Param()
            : Param(nullptr, nullptr)
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, float max_force = 0.f, float max_torque = 0.f)
            : ParamBase(body_a, body_b)
            , max_force(max_force)
            , max_torque(max_torque)
            , correction_factor(0.3f)
        {
        }
    };

    /// \~chinese
    /// @brief �������ؽ�
    /// @param param �ؽڲ���
    MotorJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief �������Ħ����
    void SetMaxForce(float force);

    /// \~chinese
    /// @brief ��ȡ���Ħ����
    float GetMaxForce() const;

    /// \~chinese
    /// @brief �������ת��
    void SetMaxTorque(float torque);

    /// \~chinese
    /// @brief ��ȡ���ת��
    float GetMaxTorque() const;

private:
    Param         param_;
    b2MotorJoint* raw_joint_;
};

/// \~chinese
/// @brief ƽ�ƹؽ�
class KGE_API PrismaticJoint : public Joint
{
public:
    /// \~chinese
    /// @brief ƽ�ƹؽڲ���
    struct Param : public Joint::ParamBase
    {
        Point anchor;             ///< �ؽ�λ��
        Vec2  axis;               ///< ����A�����ķ���
        bool  enable_limit;       ///< �Ƿ���������
        float lower_translation;  ///< �ƶ�����С���ƣ��뷽��ͬ��Ϊ��������Ϊ�����������ƺ����Ч��
        float upper_translation;  ///< �ƶ���������ƣ��뷽��ͬ��Ϊ��������Ϊ�����������ƺ����Ч��
        bool  enable_motor;       ///< �Ƿ��������
        float max_motor_force;    ///< �������� [N]
        float motor_speed;        ///< ���ת�� [degree/s]

        Param()
            : Param(nullptr, nullptr, Point(), Vec2())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& anchor, const Vec2& axis)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , axis(axis)
            , enable_limit(false)
            , lower_translation(0.0f)
            , upper_translation(0.0f)
            , enable_motor(false)
            , max_motor_force(0.0f)
            , motor_speed(0.0f)
        {
        }
    };

    /// \~chinese
    /// @brief ����ƽ�ƹؽ�
    /// @param param �ؽڲ���
    PrismaticJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief ��ȡ�ο���
    float GetReferenceAngle() const;

    /// \~chinese
    /// @brief ��ȡ�ؽ�ת��
    float GetJointTranslation() const;

    /// \~chinese
    /// @brief ��ȡ�ؽ��ٶ�
    float GetJointSpeed() const;

    /// \~chinese
    /// @brief �Ƿ����ùؽ�����
    bool IsLimitEnabled() const;

    /// \~chinese
    /// @brief �����Ƿ����ùؽ�����
    void EnableLimit(bool flag);

    /// \~chinese
    /// @brief ��ȡƽ����С����
    float GetLowerLimit() const;

    /// \~chinese
    /// @brief ��ȡƽ���������
    float GetUpperLimit() const;

    /// \~chinese
    /// @brief ���ùؽ�����
    void SetLimits(float lower, float upper);

    /// \~chinese
    /// @brief �Ƿ��������
    bool IsMotorEnabled() const;

    /// \~chinese
    /// @brief �����Ƿ��������
    void EnableMotor(bool flag);

    /// \~chinese
    /// @brief �������ת�� [degree/s]
    void SetMotorSpeed(float speed);

    /// \~chinese
    /// @brief ��ȡ���ת�� [degree/s]
    float GetMotorSpeed() const;

    /// \~chinese
    /// @brief ������������ [N]
    void SetMaxMotorForce(float force);

    /// \~chinese
    /// @brief ��ȡ�������� [N]
    float GetMaxMotorForce() const;

private:
    Param             param_;
    b2PrismaticJoint* raw_joint_;
};

/// \~chinese
/// @brief ���ֹؽ�
class KGE_API PulleyJoint : public Joint
{
public:
    /// \~chinese
    /// @brief ���ֹؽڲ���
    struct Param : public Joint::ParamBase
    {
        Point anchor_a;         ///< �ؽ�������A�ϵ����õ�
        Point anchor_b;         ///< �ؽ�������B�ϵ����õ�
        Point ground_anchor_a;  ///< ����A��Ӧ�Ļ��ֵ�λ��
        Point ground_anchor_b;  ///< ����B��Ӧ�Ļ��ֵ�λ��
        float ratio;            ///< ���ֱȣ��ؽڴ���ʱ�������������½�����ͷ��λ�Ʊ���

        Param()
            : Param(nullptr, nullptr, Point(), Point(), Point(), Point())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& anchor_a, const Point& anchor_b,
              const Point& ground_anchor_a, const Point& ground_anchor_b, float ratio = 1.0f)
            : ParamBase(body_a, body_b)
            , anchor_a(anchor_a)
            , anchor_b(anchor_b)
            , ground_anchor_a(ground_anchor_a)
            , ground_anchor_b(ground_anchor_b)
            , ratio(ratio)
        {
        }
    };

    /// \~chinese
    /// @brief �������ֹؽ�
    /// @param param �ؽڲ���
    PulleyJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief ����A��Ӧ�Ļ��ֵ�λ��
    Point GetGroundAnchorA() const;

    /// \~chinese
    /// @brief ����B��Ӧ�Ļ��ֵ�λ��
    Point GetGroundAnchorB() const;

    /// \~chinese
    /// @brief ��ȡ���ִ�����
    float GetRatio() const;

    /// \~chinese
    /// @brief ��ȡ����A�뻬�ֵľ���
    float GetLengthA() const;

    /// \~chinese
    /// @brief ��ȡ����B�뻬�ֵľ���
    float GetLengthB() const;

    /// \~chinese
    /// @brief ��ȡ����A�뻬�ֵĵ�ǰ����
    float GetCurrentLengthA() const;

    /// \~chinese
    /// @brief ��ȡ����B�뻬�ֵĵ�ǰ����
    float GetCurrentLengthB() const;

private:
    Param          param_;
    b2PulleyJoint* raw_joint_;
};

/// \~chinese
/// @brief ��ת�ؽ�
class KGE_API RevoluteJoint : public Joint
{
public:
    /// \~chinese
    /// @brief ��ת�ؽڲ���
    struct Param : public Joint::ParamBase
    {
        Point anchor;            ///< �ؽ�λ��
        bool  enable_limit;      ///< �Ƿ���������
        float lower_angle;       ///< �ƶ�����С���ƣ��뷽��ͬ��Ϊ��������Ϊ�����������ƺ����Ч��
        float upper_angle;       ///< �ƶ���������ƣ��뷽��ͬ��Ϊ��������Ϊ�����������ƺ����Ч��
        bool  enable_motor;      ///< �Ƿ��������
        float max_motor_torque;  ///< �������� [N]
        float motor_speed;       ///< ���ת�� [degree/s]

        Param()
            : Param(nullptr, nullptr, Point())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& anchor)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , enable_limit(false)
            , lower_angle(0.0f)
            , upper_angle(0.0f)
            , enable_motor(false)
            , max_motor_torque(0.0f)
            , motor_speed(0.0f)
        {
        }
    };

    /// \~chinese
    /// @brief ������ת�ؽ�
    /// @param param �ؽڲ���
    RevoluteJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief ��ȡ�ο���
    float GetReferenceAngle() const;

    /// \~chinese
    /// @brief ��ȡ�ؽڽǶ�
    float GetJointAngle() const;

    /// \~chinese
    /// @brief ��ȡ�ؽ��ٶ�
    float GetJointSpeed() const;

    /// \~chinese
    /// @brief �Ƿ����ùؽ�����
    bool IsLimitEnabled() const;

    /// \~chinese
    /// @brief �����Ƿ����ùؽ�����
    void EnableLimit(bool flag);

    /// \~chinese
    /// @brief ��ȡƽ����С����
    float GetLowerLimit() const;

    /// \~chinese
    /// @brief ��ȡƽ���������
    float GetUpperLimit() const;

    /// \~chinese
    /// @brief ���ùؽ�����
    void SetLimits(float lower, float upper);

    /// \~chinese
    /// @brief �Ƿ��������
    bool IsMotorEnabled() const;

    /// \~chinese
    /// @brief �����Ƿ��������
    void EnableMotor(bool flag);

    /// \~chinese
    /// @brief �������ת�� [degree/s]
    void SetMotorSpeed(float speed);

    /// \~chinese
    /// @brief ��ȡ���ת�� [degree/s]
    float GetMotorSpeed() const;

    /// \~chinese
    /// @brief ����������ת�� [N/m]
    void SetMaxMotorTorque(float torque);

    /// \~chinese
    /// @brief ��ȡ������ת�� [N/m]
    float GetMaxMotorTorque() const;

private:
    Param            param_;
    b2RevoluteJoint* raw_joint_;
};

/// \~chinese
/// @brief ���ؽ�
class KGE_API RopeJoint : public Joint
{
public:
    /// \~chinese
    /// @brief ���ؽڲ���
    struct Param : public Joint::ParamBase
    {
        Point local_anchor_a;  ///< �ؽ�������A�ϵ����ӵ�
        Point local_anchor_b;  ///< �ؽ�������B�ϵ����ӵ�
        float max_length;      ///< ������󳤶�

        Param()
            : Param(nullptr, nullptr, Point(), Point())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& local_anchor_a,
              const Point& local_anchor_b)
            : ParamBase(body_a, body_b)
            , local_anchor_a(local_anchor_a)
            , local_anchor_b(local_anchor_b)
            , max_length(0.0f)
        {
        }
    };

    /// \~chinese
    /// @brief �������ؽ�
    /// @param param �ؽڲ���
    RopeJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief ���ùؽ���󳤶�
    void SetMaxLength(float length);

    /// \~chinese
    /// @brief ��ȡ�ؽ���󳤶�
    float GetMaxLength() const;

private:
    Param        param_;
    b2RopeJoint* raw_joint_;
};

/// \~chinese
/// @brief ���ӹؽ�
class KGE_API WeldJoint : public Joint
{
public:
    /// \~chinese
    /// @brief ���ӹؽڲ���
    struct Param : public Joint::ParamBase
    {
        Point anchor;         ///< ����λ��
        float frequency_hz;   ///< ��Ӧ�ٶȣ���ֵԽ�߹ؽ���Ӧ���ٶ�Խ�죬����ȥԽ���
        float damping_ratio;  ///< �����ʣ�ֵԽ��ؽ��˶�����Խ��

        Param()
            : Param(nullptr, nullptr, Point())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& anchor)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , frequency_hz(0.0f)
            , damping_ratio(0.0f)
        {
        }
    };

    /// \~chinese
    /// @brief �������ӹؽ�
    /// @param param �ؽڲ���
    WeldJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief ��ȡ����B���������A�ĽǶ�
    float GetReferenceAngle() const;

    /// \~chinese
    /// @brief ���õ�����Ӧ�ٶ� [����]
    void SetFrequency(float hz);

    /// \~chinese
    /// @brief ��ȡ������Ӧ�ٶ� [����]
    float GetFrequency() const;

    /// \~chinese
    /// @brief ����������
    void SetDampingRatio(float ratio);

    /// \~chinese
    /// @brief ��ȡ������
    float GetDampingRatio() const;

private:
    Param        param_;
    b2WeldJoint* raw_joint_;
};

/// \~chinese
/// @brief �ֹؽ�
class KGE_API WheelJoint : public Joint
{
public:
    /// \~chinese
    /// @brief �ֹؽڲ���
    struct Param : public Joint::ParamBase
    {
        Point anchor;            ///< �ֹؽ�λ��
        Vec2  axis;              ///< ����A��������
        bool  enable_motor;      ///< �Ƿ��������
        float max_motor_torque;  ///< �������� [N]
        float motor_speed;       ///< ���ת�� [degree/s]
        float frequency_hz;      ///< ��Ӧ�ٶȣ���ֵԽ�߹ؽ���Ӧ���ٶ�Խ�죬����ȥԽ���
        float damping_ratio;     ///< ���������ʣ�ֵԽ��ؽ��˶�����Խ��

        Param()
            : Param(nullptr, nullptr, Point(), Vec2())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& anchor, const Vec2& axis)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , axis(axis)
            , enable_motor(false)
            , max_motor_torque(0.0f)
            , motor_speed(0.0f)
            , frequency_hz(2.0f)
            , damping_ratio(0.7f)
        {
        }
    };

    /// \~chinese
    /// @brief �����ֹؽ�
    /// @param param �ؽڲ���
    WheelJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief ��ȡ�ؽڵ�ǰ��ƽ�ƾ���
    float GetJointTranslation() const;

    /// \~chinese
    /// @brief ��ȡ�ؽڵ�ǰ�������ٶ�
    float GetJointLinearSpeed() const;

    /// \~chinese
    /// @brief ��ȡ�ؽڵ�ǰ�ĽǶ�
    float GetJointAngle() const;

    /// \~chinese
    /// @brief ��ȡ�ؽڵ�ǰ����ת�ٶ�
    float GetJointAngularSpeed() const;

    /// \~chinese
    /// @brief �Ƿ��������
    bool IsMotorEnabled() const;

    /// \~chinese
    /// @brief �����Ƿ��������
    void EnableMotor(bool flag);

    /// \~chinese
    /// @brief �������ת�� [degree/s]
    void SetMotorSpeed(float speed);

    /// \~chinese
    /// @brief ��ȡ���ת�� [degree/s]
    float GetMotorSpeed() const;

    /// \~chinese
    /// @brief ����������ת�� [N/m]
    void SetMaxMotorTorque(float torque);

    /// \~chinese
    /// @brief ��ȡ������ת�� [N/m]
    float GetMaxMotorTorque() const;

    /// \~chinese
    /// @brief ���õ�����Ӧ�ٶ�
    void SetSpringFrequencyHz(float hz);

    /// \~chinese
    /// @brief ��ȡ������Ӧ�ٶ�
    float GetSpringFrequencyHz() const;

    /// \~chinese
    /// @brief ���õ���������
    void SetSpringDampingRatio(float ratio);

    /// \~chinese
    /// @brief ��ȡ����������
    float GetSpringDampingRatio() const;

private:
    Param         param_;
    b2WheelJoint* raw_joint_;
};

/// \~chinese
/// @brief ���ؽ�
/// @details ����ʹ�����ĳ����׷�������ϵ�ָ���㣬����������׷�����λ��
class KGE_API MouseJoint : public Joint
{
public:
    /// \~chinese
    /// @brief ���ؽڲ���
    struct Param : public Joint::ParamBase
    {
        Point target;         ///< �ؽ�����Ŀ��λ��
        float max_force;      ///< ����������A�ϵ������
        float frequency_hz;   ///< ��Ӧ�ٶȣ���ֵԽ�߹ؽ���Ӧ���ٶ�Խ�죬����ȥԽ���
        float damping_ratio;  ///< �����ʣ�ֵԽ��ؽ��˶�����Խ��

        Param()
            : Param(nullptr, nullptr, Point())
        {
        }

        Param(RefPtr<PhysicBody> body_a, RefPtr<PhysicBody> body_b, const Point& target)
            : ParamBase(body_a, body_b)
            , target(target)
            , max_force(0.0f)
            , frequency_hz(5.0f)
            , damping_ratio(0.7f)
        {
        }
    };

    /// \~chinese
    /// @brief �������ؽ�
    /// @param param �ؽڲ���
    MouseJoint(const Param& param);

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief �趨���Ħ���� [N]
    void SetMaxForce(float force);

    /// \~chinese
    /// @brief ��ȡ���Ħ���� [N]
    float GetMaxForce() const;

    /// \~chinese
    /// @brief ������Ӧ�ٶ� [hz]
    void SetFrequency(float hz);

    /// \~chinese
    /// @brief ��ȡ��Ӧ�ٶ� [hz]
    float GetFrequency() const;

    /// \~chinese
    /// @brief ����������
    void SetDampingRatio(float ratio);

    /// \~chinese
    /// @brief ��ȡ������
    float GetDampingRatio() const;

private:
    Param         param_;
    b2MouseJoint* raw_joint_;
};

/** @} */

inline b2Joint* Joint::GetB2Joint() const
{
    return joint_;
}
inline PhysicWorld* Joint::GetWorld() const
{
    return world_;
}

inline void DistanceJoint::SetFrequency(float hz)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetFrequency(hz);
}
inline float DistanceJoint::GetFrequency() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetFrequency();
}
inline void DistanceJoint::SetDampingRatio(float ratio)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetDampingRatio(ratio);
}
inline float DistanceJoint::GetDampingRatio() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetDampingRatio();
}

inline float PrismaticJoint::GetReferenceAngle() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetReferenceAngle());
}
inline bool PrismaticJoint::IsLimitEnabled() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->IsLimitEnabled();
}
inline void PrismaticJoint::EnableLimit(bool flag)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->EnableLimit(flag);
}
inline bool PrismaticJoint::IsMotorEnabled() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->IsMotorEnabled();
}
inline void PrismaticJoint::EnableMotor(bool flag)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->EnableMotor(flag);
}
inline void PrismaticJoint::SetMotorSpeed(float speed)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMotorSpeed(math::Degree2Radian(speed));
}
inline float PrismaticJoint::GetMotorSpeed() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetMotorSpeed());
}
inline void PrismaticJoint::SetMaxMotorForce(float force)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxMotorForce(force);
}
inline float PrismaticJoint::GetMaxMotorForce() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetMaxMotorForce();
}

inline float RevoluteJoint::GetReferenceAngle() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetReferenceAngle());
}
inline bool RevoluteJoint::IsLimitEnabled() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->IsLimitEnabled();
}
inline void RevoluteJoint::EnableLimit(bool flag)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->EnableLimit(flag);
}
inline bool RevoluteJoint::IsMotorEnabled() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->IsMotorEnabled();
}
inline void RevoluteJoint::EnableMotor(bool flag)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->EnableMotor(flag);
}
inline void RevoluteJoint::SetMotorSpeed(float speed)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMotorSpeed(math::Degree2Radian(speed));
}
inline float RevoluteJoint::GetMotorSpeed() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetMotorSpeed());
}

inline float WeldJoint::GetReferenceAngle() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetReferenceAngle());
}
inline void WeldJoint::SetFrequency(float hz)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetFrequency(hz);
}
inline float WeldJoint::GetFrequency() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetFrequency();
}
inline void WeldJoint::SetDampingRatio(float ratio)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetDampingRatio(ratio);
}
inline float WeldJoint::GetDampingRatio() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetDampingRatio();
}

inline float WheelJoint::GetJointAngle() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetJointAngle());
}
inline float WheelJoint::GetJointAngularSpeed() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetJointAngularSpeed());
}
inline bool WheelJoint::IsMotorEnabled() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->IsMotorEnabled();
}
inline void WheelJoint::EnableMotor(bool flag)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->EnableMotor(flag);
}
inline void WheelJoint::SetMotorSpeed(float speed)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMotorSpeed(math::Degree2Radian(speed));
}
inline float WheelJoint::GetMotorSpeed() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetMotorSpeed());
}
inline void WheelJoint::SetSpringFrequencyHz(float hz)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetSpringFrequencyHz(hz);
}
inline float WheelJoint::GetSpringFrequencyHz() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetSpringFrequencyHz();
}
inline void WheelJoint::SetSpringDampingRatio(float ratio)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetSpringDampingRatio(ratio);
}
inline float WheelJoint::GetSpringDampingRatio() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetSpringDampingRatio();
}

inline void MouseJoint::SetFrequency(float hz)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetFrequency(hz);
}
inline float MouseJoint::GetFrequency() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetFrequency();
}
inline void MouseJoint::SetDampingRatio(float ratio)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetDampingRatio(ratio);
}
inline float MouseJoint::GetDampingRatio() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetDampingRatio();
}
}  // namespace physics
}  // namespace kiwano
