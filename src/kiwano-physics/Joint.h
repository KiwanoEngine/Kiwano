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
#include <kiwano-physics/helper.h>

namespace kiwano
{
namespace physics
{
KGE_DECLARE_SMART_PTR(Joint);
KGE_DECLARE_SMART_PTR(DistanceJoint);
KGE_DECLARE_SMART_PTR(FrictionJoint);
KGE_DECLARE_SMART_PTR(GearJoint);
KGE_DECLARE_SMART_PTR(MotorJoint);
KGE_DECLARE_SMART_PTR(MouseJoint);
KGE_DECLARE_SMART_PTR(PrismaticJoint);
KGE_DECLARE_SMART_PTR(PulleyJoint);
KGE_DECLARE_SMART_PTR(RevoluteJoint);
KGE_DECLARE_SMART_PTR(RopeJoint);
KGE_DECLARE_SMART_PTR(WeldJoint);
KGE_DECLARE_SMART_PTR(WheelJoint);

/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief �ؽ�
class KGE_API Joint : public virtual ObjectBase
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
        Body* body_a;  ///< �ؽ����ӵ�����A
        Body* body_b;  ///< �ؽ����ӵ�����B

        ParamBase(Body* body_a, Body* body_b)
            : body_a(body_a)
            , body_b(body_b)
        {
        }

        ParamBase(BodyPtr body_a, BodyPtr body_b)
            : body_a(body_a.get())
            , body_b(body_b.get())
        {
        }
    };

    Joint();

    virtual ~Joint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, b2JointDef* joint_def);

    /// \~chinese
    /// @brief ��ȡ�ؽ����ӵ�����A
    BodyPtr GetBodyA() const;

    /// \~chinese
    /// @brief ��ȡ�ؽ����ӵ�����B
    BodyPtr GetBodyB() const;

    /// \~chinese
    /// @brief ��ȡ��������
    World* GetWorld() const;

    /// \~chinese
    /// @brief ���ٹؽ�
    void Destroy();

    b2Joint* GetB2Joint() const;
    void     SetB2Joint(b2Joint* joint);

private:
    b2Joint* joint_;
    World*   world_;
    Type     type_;
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

        Param(Body* body_a, Body* body_b, Point const& anchor_a, Point const& anchor_b, float frequency_hz = 0.f,
              float damping_ratio = 0.f)
            : ParamBase(body_a, body_b)
            , anchor_a(anchor_a)
            , anchor_b(anchor_b)
            , frequency_hz(frequency_hz)
            , damping_ratio(damping_ratio)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& anchor_a, Point const& anchor_b, float frequency_hz = 0.f,
              float damping_ratio = 0.f)
            : Param(body_a.get(), body_b.get(), anchor_a, anchor_b, frequency_hz, damping_ratio)
        {
        }
    };

    /// \~chinese
    /// @brief �����̶�����ؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static DistanceJointPtr Create(World* world, Param const& param);

    DistanceJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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

        Param(Body* body_a, Body* body_b, Point const& anchor, float max_force = 0.f, float max_torque = 0.f)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , max_force(max_force)
            , max_torque(max_torque)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& anchor, float max_force = 0.f, float max_torque = 0.f)
            : Param(body_a.get(), body_b.get(), anchor, max_force, max_torque)
        {
        }
    };

    /// \~chinese
    /// @brief ����Ħ���ؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static FrictionJointPtr Create(World* world, Param const& param);

    FrictionJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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

        Param(Joint* joint_a, Joint* joint_b, float ratio = 1.f)
            : ParamBase(nullptr, nullptr)
            , joint_a(joint_a)
            , joint_b(joint_b)
            , ratio(ratio)
        {
        }

        Param(JointPtr joint_a, JointPtr joint_b, float ratio = 1.f)
            : Param(joint_a.get(), joint_b.get(), ratio)
        {
        }
    };

    /// \~chinese
    /// @brief �������ֹؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static GearJointPtr Create(World* world, Param const& param);

    GearJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

    /// \~chinese
    /// @brief �趨���ִ�����
    void SetRatio(float ratio);

    /// \~chinese
    /// @brief ��ȡ���ִ�����
    float GetRatio() const;

private:
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

        Param(Body* body_a, Body* body_b, float max_force = 1.f, float max_torque = 100.f,
              float correction_factor = 0.3f)
            : ParamBase(body_a, body_b)
            , max_force(max_force)
            , max_torque(max_torque)
            , correction_factor(correction_factor)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, float max_force = 0.f, float max_torque = 0.f,
              float correction_factor = 0.3f)
            : Param(body_a.get(), body_b.get(), max_force, max_torque, correction_factor)
        {
        }
    };

    /// \~chinese
    /// @brief �������ؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static MotorJointPtr Create(World* world, Param const& param);

    MotorJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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

        Param(Body* body_a, Body* body_b, Point const& anchor, Vec2 const& axis, bool enable_limit = false,
              float lower_translation = 0.0f, float upper_translation = 0.0f, bool enable_motor = false,
              float max_motor_force = 0.0f, float motor_speed = 0.0f)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , axis(axis)
            , enable_limit(enable_limit)
            , lower_translation(lower_translation)
            , upper_translation(upper_translation)
            , enable_motor(enable_motor)
            , max_motor_force(max_motor_force)
            , motor_speed(motor_speed)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& anchor, Vec2 const& axis, bool enable_limit = false,
              float lower_translation = 0.0f, float upper_translation = 0.0f, bool enable_motor = false,
              float max_motor_force = 0.0f, float motor_speed = 0.0f)
            : Param(body_a.get(), body_b.get(), anchor, axis, enable_limit, lower_translation, upper_translation,
                    enable_motor, max_motor_force, motor_speed)
        {
        }
    };

    /// \~chinese
    /// @brief ����ƽ�ƹؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static PrismaticJointPtr Create(World* world, Param const& param);

    PrismaticJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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

        Param(Body* body_a, Body* body_b, Point const& anchor_a, Point const& anchor_b, Point const& ground_anchor_a,
              Point const& ground_anchor_b, float ratio = 1.0f)
            : ParamBase(body_a, body_b)
            , anchor_a(anchor_a)
            , anchor_b(anchor_b)
            , ground_anchor_a(ground_anchor_a)
            , ground_anchor_b(ground_anchor_b)
            , ratio(ratio)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& anchor_a, Point const& anchor_b,
              Point const& ground_anchor_a, Point const& ground_anchor_b, float ratio = 1.0f)
            : Param(body_a.get(), body_b.get(), anchor_a, anchor_b, ground_anchor_a, ground_anchor_b, ratio)
        {
        }
    };

    /// \~chinese
    /// @brief �������ֹؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static PulleyJointPtr Create(World* world, Param const& param);

    PulleyJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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

        Param(Body* body_a, Body* body_b, Point const& anchor, bool enable_limit = false, float lower_angle = 0.0f,
              float upper_angle = 0.0f, bool enable_motor = false, float max_motor_torque = 0.0f,
              float motor_speed = 0.0f)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , enable_limit(enable_limit)
            , lower_angle(lower_angle)
            , upper_angle(upper_angle)
            , enable_motor(enable_motor)
            , max_motor_torque(max_motor_torque)
            , motor_speed(motor_speed)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& anchor, bool enable_limit = false, float lower_angle = 0.0f,
              float upper_angle = 0.0f, bool enable_motor = false, float max_motor_torque = 0.0f,
              float motor_speed = 0.0f)
            : Param(body_a.get(), body_b.get(), anchor, enable_limit, lower_angle, upper_angle, enable_motor,
                    max_motor_torque, motor_speed)
        {
        }
    };

    /// \~chinese
    /// @brief ������ת�ؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static RevoluteJointPtr Create(World* world, Param const& param);

    RevoluteJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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

        Param(Body* body_a, Body* body_b, Point const& local_anchor_a, Point const& local_anchor_b,
              float max_length = 0.f)
            : ParamBase(body_a, body_b)
            , local_anchor_a(local_anchor_a)
            , local_anchor_b(local_anchor_b)
            , max_length(max_length)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& local_anchor_a, Point const& local_anchor_b,
              float max_length = 0.f)
            : Param(body_a.get(), body_b.get(), local_anchor_a, local_anchor_b, max_length)
        {
        }
    };

    /// \~chinese
    /// @brief �������ؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static RopeJointPtr Create(World* world, Param const& param);

    RopeJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

    /// \~chinese
    /// @brief ���ùؽ���󳤶�
    void SetMaxLength(float length);

    /// \~chinese
    /// @brief ��ȡ�ؽ���󳤶�
    float GetMaxLength() const;

private:
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

        Param(Body* body_a, Body* body_b, Point const& anchor, float frequency_hz = 0.f, float damping_ratio = 0.f)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , frequency_hz(frequency_hz)
            , damping_ratio(damping_ratio)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& anchor, float frequency_hz = 0.f, float damping_ratio = 0.f)
            : Param(body_a.get(), body_b.get(), anchor, frequency_hz, damping_ratio)
        {
        }
    };

    /// \~chinese
    /// @brief �������ӹؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static WeldJointPtr Create(World* world, Param const& param);

    WeldJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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

        Param(Body* body_a, Body* body_b, Point const& anchor, Vec2 const& axis, float frequency_hz = 2.0f,
              float damping_ratio = 0.7f, bool enable_motor = false, float max_motor_torque = 0.0f,
              float motor_speed = 0.0f)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , axis(axis)
            , enable_motor(enable_motor)
            , max_motor_torque(max_motor_torque)
            , motor_speed(motor_speed)
            , frequency_hz(frequency_hz)
            , damping_ratio(damping_ratio)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& anchor, Vec2 const& axis, float frequency_hz = 2.0f,
              float damping_ratio = 0.7f, bool enable_motor = false, float max_motor_torque = 0.0f,
              float motor_speed = 0.0f)
            : Param(body_a.get(), body_b.get(), anchor, axis, frequency_hz, damping_ratio, enable_motor,
                    max_motor_torque, motor_speed)
        {
        }
    };

    /// \~chinese
    /// @brief �����ֹؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static WheelJointPtr Create(World* world, Param const& param);

    WheelJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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

        Param(Body* body_a, Body* body_b, Point const& target, float max_force, float frequency_hz = 5.0f,
              float damping_ratio = 0.7f)
            : ParamBase(body_a, body_b)
            , target(target)
            , max_force(max_force)
            , frequency_hz(frequency_hz)
            , damping_ratio(damping_ratio)
        {
        }

        Param(BodyPtr body_a, BodyPtr body_b, Point const& target, float max_force, float frequency_hz = 5.0f,
              float damping_ratio = 0.7f)
            : Param(body_a.get(), body_b.get(), target, max_force, frequency_hz, damping_ratio)
        {
        }
    };

    /// \~chinese
    /// @brief �������ؽ�
    /// @param world ��������
    /// @param param �ؽڲ���
    static MouseJointPtr Create(World* world, Param const& param);

    MouseJoint();

    /// \~chinese
    /// @brief ��ʼ���ؽ�
    bool InitJoint(World* world, Param const& param);

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
    b2MouseJoint* raw_joint_;
};

/** @} */

inline b2Joint* Joint::GetB2Joint() const
{
    return joint_;
}
inline World* Joint::GetWorld() const
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
