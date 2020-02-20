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
/// @brief 关节
class KGE_API Joint : public ObjectBase
{
public:
    /// \~chinese
    /// @brief 关节类型
    enum class Type
    {
        Unknown = 0,  ///< 未知
        Revolute,     ///< 旋转关节
        Prismatic,    ///< 平移关节
        Distance,     ///< 固定距离关节
        Pulley,       ///< 滑轮关节
        Mouse,        ///< 鼠标关节
        Gear,         ///< 齿轮关节
        Wheel,        ///< 轮关节
        Weld,         ///< 焊接关节
        Friction,     ///< 摩擦关节
        Rope,         ///< 绳关节
        Motor         ///< 马达关节
    };

    /// \~chinese
    /// @brief 关节基础参数
    struct ParamBase
    {
        PhysicBody* body_a;  ///< 关节连接的物体A
        PhysicBody* body_b;  ///< 关节连接的物体B

        ParamBase(PhysicBody* body_a, PhysicBody* body_b)
            : body_a(body_a)
            , body_b(body_b)
        {
        }

        ParamBase(PhysicBodyPtr body_a, PhysicBodyPtr body_b)
            : body_a(body_a.Get())
            , body_b(body_b.Get())
        {
        }
    };

    Joint();

    virtual ~Joint();

    /// \~chinese
    /// @brief 初始化关节
    virtual bool Init(PhysicWorld* world);

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world, b2JointDef* joint_def);

    /// \~chinese
    /// @brief 获取关节连接的物体A
    PhysicBodyPtr GetBodyA() const;

    /// \~chinese
    /// @brief 获取关节连接的物体B
    PhysicBodyPtr GetBodyB() const;

    /// \~chinese
    /// @brief 获取物理世界
    PhysicWorld* GetWorld() const;

    /// \~chinese
    /// @brief 销毁关节
    void Destroy();

    /// \~chinese
    /// @brief 获取b2Joint
    b2Joint* GetB2Joint() const;

    /// \~chinese
    /// @brief 设置b2Joint
    void SetB2Joint(b2Joint* joint);

private:
    b2Joint*     joint_;
    PhysicWorld* world_;
    Type         type_;
};

/// \~chinese
/// @brief 固定距离关节
class KGE_API DistanceJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 固定距离关节参数
    struct Param : public Joint::ParamBase
    {
        Point anchor_a;       ///< 关节在物体A上的连接点
        Point anchor_b;       ///< 关节在物体B上的连接点
        float frequency_hz;   ///< 响应速度，数值越高关节响应的速度越快，看上去越坚固
        float damping_ratio;  ///< 阻尼率，值越大关节运动阻尼越大

        Param()
            : Param(nullptr, nullptr, Point(), Point())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& anchor_a, const Point& anchor_b)
            : ParamBase(body_a, body_b)
            , anchor_a(anchor_a)
            , anchor_b(anchor_b)
            , frequency_hz(0.0f)
            , damping_ratio(0.0f)
        {
        }
    };

    /// \~chinese
    /// @brief 创建固定距离关节
    /// @param param 关节参数
    static DistanceJointPtr Create(const Param& param);

    DistanceJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 设置关节长度
    void SetLength(float length);

    /// \~chinese
    /// @brief 获取关节长度
    float GetLength() const;

    /// \~chinese
    /// @brief 设置弹簧响应速度 [赫兹]
    void SetFrequency(float hz);

    /// \~chinese
    /// @brief 获取弹簧响应速度 [赫兹]
    float GetFrequency() const;

    /// \~chinese
    /// @brief 设置阻尼率
    void SetDampingRatio(float ratio);

    /// \~chinese
    /// @brief 获取阻尼率
    float GetDampingRatio() const;

private:
    Param            param_;
    b2DistanceJoint* raw_joint_;
};

/// \~chinese
/// @brief 摩擦关节
class KGE_API FrictionJoint : public Joint
{
public:
    struct Param : public Joint::ParamBase
    {
        Point anchor;      ///< 摩擦作用点
        float max_force;   ///< 最大摩擦力
        float max_torque;  ///< 最大扭力

        Param()
            : Param(nullptr, nullptr, Point())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& anchor, float max_force = 0.f, float max_torque = 0.f)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , max_force(max_force)
            , max_torque(max_torque)
        {
        }
    };

    /// \~chinese
    /// @brief 创建摩擦关节
    /// @param param 关节参数
    static FrictionJointPtr Create(const Param& param);

    FrictionJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 设置最大摩擦力
    void SetMaxForce(float force);

    /// \~chinese
    /// @brief 获取最大摩擦力
    float GetMaxForce() const;

    /// \~chinese
    /// @brief 设置最大转矩
    void SetMaxTorque(float torque);

    /// \~chinese
    /// @brief 获取最大转矩
    float GetMaxTorque() const;

private:
    Param            param_;
    b2FrictionJoint* raw_joint_;
};

/// \~chinese
/// @brief 齿轮关节
class KGE_API GearJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 齿轮关节参数
    struct Param : public Joint::ParamBase
    {
        Joint* joint_a;  ///< 关节A（旋转关节/平移关节）
        Joint* joint_b;  ///< 关节B（旋转关节/平移关节）
        float  ratio;    ///< 齿轮传动比

        Param()
            : Param(nullptr, nullptr)
        {
        }

        Param(JointPtr joint_a, JointPtr joint_b, float ratio = 1.f)
            : ParamBase(nullptr, nullptr)
            , joint_a(joint_a.Get())
            , joint_b(joint_b.Get())
            , ratio(ratio)
        {
        }
    };

    /// \~chinese
    /// @brief 创建齿轮关节
    /// @param param 关节参数
    static GearJointPtr Create(const Param& param);

    GearJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 设定齿轮传动比
    void SetRatio(float ratio);

    /// \~chinese
    /// @brief 获取齿轮传动比
    float GetRatio() const;

private:
    Param        param_;
    b2GearJoint* raw_joint_;
};

/// \~chinese
/// @brief 马达关节
class KGE_API MotorJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 马达关节参数
    struct Param : public Joint::ParamBase
    {
        float max_force;          ///< 最大摩擦力
        float max_torque;         ///< 最大转矩
        float correction_factor;  ///< 位置矫正因子（范围 0-1）

        Param()
            : Param(nullptr, nullptr)
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, float max_force = 0.f, float max_torque = 0.f)
            : ParamBase(body_a, body_b)
            , max_force(max_force)
            , max_torque(max_torque)
            , correction_factor(0.3f)
        {
        }
    };

    /// \~chinese
    /// @brief 创建马达关节
    /// @param param 关节参数
    static MotorJointPtr Create(const Param& param);

    MotorJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 设置最大摩擦力
    void SetMaxForce(float force);

    /// \~chinese
    /// @brief 获取最大摩擦力
    float GetMaxForce() const;

    /// \~chinese
    /// @brief 设置最大转矩
    void SetMaxTorque(float torque);

    /// \~chinese
    /// @brief 获取最大转矩
    float GetMaxTorque() const;

private:
    Param         param_;
    b2MotorJoint* raw_joint_;
};

/// \~chinese
/// @brief 平移关节
class KGE_API PrismaticJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 平移关节参数
    struct Param : public Joint::ParamBase
    {
        Point anchor;        ///< 关节位置
        Vec2  axis;          ///< 物体A滑动的方向
        bool  enable_limit;  ///< 是否启用限制
        float lower_translation;  ///< 移动的最小限制，与方向同向为正，反向为负，启用限制后才有效果
        float upper_translation;  ///< 移动的最大限制，与方向同向为正，反向为负，启用限制后才有效果
        bool  enable_motor;     ///< 是否启用马达
        float max_motor_force;  ///< 最大马达力 [N]
        float motor_speed;      ///< 马达转速 [degree/s]

        Param()
            : Param(nullptr, nullptr, Point(), Vec2())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& anchor, const Vec2& axis)
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
    /// @brief 创建平移关节
    /// @param param 关节参数
    static PrismaticJointPtr Create(const Param& param);

    PrismaticJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 获取参考角
    float GetReferenceAngle() const;

    /// \~chinese
    /// @brief 获取关节转换
    float GetJointTranslation() const;

    /// \~chinese
    /// @brief 获取关节速度
    float GetJointSpeed() const;

    /// \~chinese
    /// @brief 是否启用关节限制
    bool IsLimitEnabled() const;

    /// \~chinese
    /// @brief 设置是否启用关节限制
    void EnableLimit(bool flag);

    /// \~chinese
    /// @brief 获取平移最小限制
    float GetLowerLimit() const;

    /// \~chinese
    /// @brief 获取平移最大限制
    float GetUpperLimit() const;

    /// \~chinese
    /// @brief 设置关节限制
    void SetLimits(float lower, float upper);

    /// \~chinese
    /// @brief 是否启用马达
    bool IsMotorEnabled() const;

    /// \~chinese
    /// @brief 设置是否启用马达
    void EnableMotor(bool flag);

    /// \~chinese
    /// @brief 设置马达转速 [degree/s]
    void SetMotorSpeed(float speed);

    /// \~chinese
    /// @brief 获取马达转速 [degree/s]
    float GetMotorSpeed() const;

    /// \~chinese
    /// @brief 设置最大马达力 [N]
    void SetMaxMotorForce(float force);

    /// \~chinese
    /// @brief 获取最大马达力 [N]
    float GetMaxMotorForce() const;

private:
    Param             param_;
    b2PrismaticJoint* raw_joint_;
};

/// \~chinese
/// @brief 滑轮关节
class KGE_API PulleyJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 滑轮关节参数
    struct Param : public Joint::ParamBase
    {
        Point anchor_a;         ///< 关节在物体A上的作用点
        Point anchor_b;         ///< 关节在物体B上的作用点
        Point ground_anchor_a;  ///< 物体A对应的滑轮的位置
        Point ground_anchor_b;  ///< 物体B对应的滑轮的位置
        float ratio;            ///< 滑轮比，关节传动时，滑轮上升和下降的两头的位移比例

        Param()
            : Param(nullptr, nullptr, Point(), Point(), Point(), Point())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& anchor_a, const Point& anchor_b,
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
    /// @brief 创建滑轮关节
    /// @param param 关节参数
    static PulleyJointPtr Create(const Param& param);

    PulleyJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 物体A对应的滑轮的位置
    Point GetGroundAnchorA() const;

    /// \~chinese
    /// @brief 物体B对应的滑轮的位置
    Point GetGroundAnchorB() const;

    /// \~chinese
    /// @brief 获取滑轮传动比
    float GetRatio() const;

    /// \~chinese
    /// @brief 获取物体A与滑轮的距离
    float GetLengthA() const;

    /// \~chinese
    /// @brief 获取物体B与滑轮的距离
    float GetLengthB() const;

    /// \~chinese
    /// @brief 获取物体A与滑轮的当前距离
    float GetCurrentLengthA() const;

    /// \~chinese
    /// @brief 获取物体B与滑轮的当前距离
    float GetCurrentLengthB() const;

private:
    Param          param_;
    b2PulleyJoint* raw_joint_;
};

/// \~chinese
/// @brief 旋转关节
class KGE_API RevoluteJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 旋转关节参数
    struct Param : public Joint::ParamBase
    {
        Point anchor;        ///< 关节位置
        bool  enable_limit;  ///< 是否启用限制
        float lower_angle;  ///< 移动的最小限制，与方向同向为正，反向为负，启用限制后才有效果
        float upper_angle;  ///< 移动的最大限制，与方向同向为正，反向为负，启用限制后才有效果
        bool  enable_motor;      ///< 是否启用马达
        float max_motor_torque;  ///< 最大马达力 [N]
        float motor_speed;       ///< 马达转速 [degree/s]

        Param()
            : Param(nullptr, nullptr, Point())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& anchor)
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
    /// @brief 创建旋转关节
    /// @param param 关节参数
    static RevoluteJointPtr Create(const Param& param);

    RevoluteJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 获取参考角
    float GetReferenceAngle() const;

    /// \~chinese
    /// @brief 获取关节角度
    float GetJointAngle() const;

    /// \~chinese
    /// @brief 获取关节速度
    float GetJointSpeed() const;

    /// \~chinese
    /// @brief 是否启用关节限制
    bool IsLimitEnabled() const;

    /// \~chinese
    /// @brief 设置是否启用关节限制
    void EnableLimit(bool flag);

    /// \~chinese
    /// @brief 获取平移最小限制
    float GetLowerLimit() const;

    /// \~chinese
    /// @brief 获取平移最大限制
    float GetUpperLimit() const;

    /// \~chinese
    /// @brief 设置关节限制
    void SetLimits(float lower, float upper);

    /// \~chinese
    /// @brief 是否启用马达
    bool IsMotorEnabled() const;

    /// \~chinese
    /// @brief 设置是否启用马达
    void EnableMotor(bool flag);

    /// \~chinese
    /// @brief 设置马达转速 [degree/s]
    void SetMotorSpeed(float speed);

    /// \~chinese
    /// @brief 获取马达转速 [degree/s]
    float GetMotorSpeed() const;

    /// \~chinese
    /// @brief 设置最大马达转矩 [N/m]
    void SetMaxMotorTorque(float torque);

    /// \~chinese
    /// @brief 获取最大马达转矩 [N/m]
    float GetMaxMotorTorque() const;

private:
    Param            param_;
    b2RevoluteJoint* raw_joint_;
};

/// \~chinese
/// @brief 绳关节
class KGE_API RopeJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 绳关节参数
    struct Param : public Joint::ParamBase
    {
        Point local_anchor_a;  ///< 关节在物体A上的连接点
        Point local_anchor_b;  ///< 关节在物体B上的连接点
        float max_length;      ///< 绳索最大长度

        Param()
            : Param(nullptr, nullptr, Point(), Point())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& local_anchor_a, const Point& local_anchor_b)
            : ParamBase(body_a, body_b)
            , local_anchor_a(local_anchor_a)
            , local_anchor_b(local_anchor_b)
            , max_length(0.0f)
        {
        }
    };

    /// \~chinese
    /// @brief 创建绳关节
    /// @param param 关节参数
    static RopeJointPtr Create(const Param& param);

    RopeJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 设置关节最大长度
    void SetMaxLength(float length);

    /// \~chinese
    /// @brief 获取关节最大长度
    float GetMaxLength() const;

private:
    Param        param_;
    b2RopeJoint* raw_joint_;
};

/// \~chinese
/// @brief 焊接关节
class KGE_API WeldJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 焊接关节参数
    struct Param : public Joint::ParamBase
    {
        Point anchor;         ///< 焊接位置
        float frequency_hz;   ///< 响应速度，数值越高关节响应的速度越快，看上去越坚固
        float damping_ratio;  ///< 阻尼率，值越大关节运动阻尼越大

        Param()
            : Param(nullptr, nullptr, Point())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& anchor)
            : ParamBase(body_a, body_b)
            , anchor(anchor)
            , frequency_hz(0.0f)
            , damping_ratio(0.0f)
        {
        }
    };

    /// \~chinese
    /// @brief 创建焊接关节
    /// @param param 关节参数
    static WeldJointPtr Create(const Param& param);

    WeldJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 获取物体B相对于物体A的角度
    float GetReferenceAngle() const;

    /// \~chinese
    /// @brief 设置弹簧响应速度 [赫兹]
    void SetFrequency(float hz);

    /// \~chinese
    /// @brief 获取弹簧响应速度 [赫兹]
    float GetFrequency() const;

    /// \~chinese
    /// @brief 设置阻尼率
    void SetDampingRatio(float ratio);

    /// \~chinese
    /// @brief 获取阻尼率
    float GetDampingRatio() const;

private:
    Param        param_;
    b2WeldJoint* raw_joint_;
};

/// \~chinese
/// @brief 轮关节
class KGE_API WheelJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 轮关节参数
    struct Param : public Joint::ParamBase
    {
        Point anchor;            ///< 轮关节位置
        Vec2  axis;              ///< 物体A滑动方向
        bool  enable_motor;      ///< 是否启用马达
        float max_motor_torque;  ///< 最大马达力 [N]
        float motor_speed;       ///< 马达转速 [degree/s]
        float frequency_hz;      ///< 响应速度，数值越高关节响应的速度越快，看上去越坚固
        float damping_ratio;     ///< 弹簧阻尼率，值越大关节运动阻尼越大

        Param()
            : Param(nullptr, nullptr, Point(), Vec2())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& anchor, const Vec2& axis)
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
    /// @brief 创建轮关节
    /// @param param 关节参数
    static WheelJointPtr Create(const Param& param);

    WheelJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 获取关节当前的平移距离
    float GetJointTranslation() const;

    /// \~chinese
    /// @brief 获取关节当前的线性速度
    float GetJointLinearSpeed() const;

    /// \~chinese
    /// @brief 获取关节当前的角度
    float GetJointAngle() const;

    /// \~chinese
    /// @brief 获取关节当前的旋转速度
    float GetJointAngularSpeed() const;

    /// \~chinese
    /// @brief 是否启用马达
    bool IsMotorEnabled() const;

    /// \~chinese
    /// @brief 设置是否启用马达
    void EnableMotor(bool flag);

    /// \~chinese
    /// @brief 设置马达转速 [degree/s]
    void SetMotorSpeed(float speed);

    /// \~chinese
    /// @brief 获取马达转速 [degree/s]
    float GetMotorSpeed() const;

    /// \~chinese
    /// @brief 设置最大马达转矩 [N/m]
    void SetMaxMotorTorque(float torque);

    /// \~chinese
    /// @brief 获取最大马达转矩 [N/m]
    float GetMaxMotorTorque() const;

    /// \~chinese
    /// @brief 设置弹簧响应速度
    void SetSpringFrequencyHz(float hz);

    /// \~chinese
    /// @brief 获取弹簧响应速度
    float GetSpringFrequencyHz() const;

    /// \~chinese
    /// @brief 设置弹簧阻尼率
    void SetSpringDampingRatio(float ratio);

    /// \~chinese
    /// @brief 获取弹簧阻尼率
    float GetSpringDampingRatio() const;

private:
    Param         param_;
    b2WheelJoint* raw_joint_;
};

/// \~chinese
/// @brief 鼠标关节
/// @details 用于使身体的某个点追踪世界上的指定点，例如让物体追踪鼠标位置
class KGE_API MouseJoint : public Joint
{
public:
    /// \~chinese
    /// @brief 鼠标关节参数
    struct Param : public Joint::ParamBase
    {
        Point target;         ///< 关节作用目标位置
        float max_force;      ///< 作用在物体A上的最大力
        float frequency_hz;   ///< 响应速度，数值越高关节响应的速度越快，看上去越坚固
        float damping_ratio;  ///< 阻尼率，值越大关节运动阻尼越大

        Param()
            : Param(nullptr, nullptr, Point())
        {
        }

        Param(PhysicBodyPtr body_a, PhysicBodyPtr body_b, const Point& target)
            : ParamBase(body_a, body_b)
            , target(target)
            , max_force(0.0f)
            , frequency_hz(5.0f)
            , damping_ratio(0.7f)
        {
        }
    };

    /// \~chinese
    /// @brief 创建鼠标关节
    /// @param param 关节参数
    static MouseJointPtr Create(const Param& param);

    MouseJoint();

    /// \~chinese
    /// @brief 初始化关节
    bool Init(PhysicWorld* world) override;

    /// \~chinese
    /// @brief 设定最大摩擦力 [N]
    void SetMaxForce(float force);

    /// \~chinese
    /// @brief 获取最大摩擦力 [N]
    float GetMaxForce() const;

    /// \~chinese
    /// @brief 设置响应速度 [hz]
    void SetFrequency(float hz);

    /// \~chinese
    /// @brief 获取响应速度 [hz]
    float GetFrequency() const;

    /// \~chinese
    /// @brief 设置阻尼率
    void SetDampingRatio(float ratio);

    /// \~chinese
    /// @brief 获取阻尼率
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
