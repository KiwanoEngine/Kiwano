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

#include <kiwano-physics/Joint.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
namespace physics
{
//
// Joint
//

Joint::Joint()
    : joint_(nullptr)
    , world_(nullptr)
    , type_(Type::Unknown)
{
}

Joint::~Joint()
{
    Destroy();
}

bool Joint::InitJoint(World* world, b2JointDef* joint_def)
{
    KGE_ASSERT(world);

    Destroy();

    world_ = world;
    if (world_)
    {
        world_->AddJoint(this);

        b2Joint* joint = world_->GetB2World()->CreateJoint(joint_def);
        SetB2Joint(joint);

        return joint != nullptr;
    }
    return false;
}

BodyPtr Joint::GetBodyA() const
{
    KGE_ASSERT(joint_);

    b2Body* body = joint_->GetBodyA();
    return BodyPtr(static_cast<Body*>(body->GetUserData()));
}

BodyPtr Joint::GetBodyB() const
{
    KGE_ASSERT(joint_);

    b2Body* body = joint_->GetBodyB();
    return BodyPtr(static_cast<Body*>(body->GetUserData()));
}

void Joint::SetB2Joint(b2Joint* joint)
{
    joint_ = joint;
    if (joint_)
    {
        type_ = Joint::Type(joint_->GetType());
    }
}

void Joint::Destroy()
{
    if (world_)
    {
        world_->RemoveJoint(this);
    }
}

//
// DistanceJoint
//

DistanceJointPtr DistanceJoint::Create(World* world, Param const& param)
{
    DistanceJointPtr ptr = new (std::nothrow) DistanceJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

DistanceJoint::DistanceJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool DistanceJoint::InitJoint(World* world, DistanceJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2DistanceJointDef def;
    def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor_a),
                   world->Stage2World(param.anchor_b));
    def.frequencyHz  = param.frequency_hz;
    def.dampingRatio = param.damping_ratio;

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2DistanceJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

void DistanceJoint::SetLength(float length)
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    raw_joint_->SetLength(GetWorld()->Stage2World(length));
}

float DistanceJoint::GetLength() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetLength());
}

//
// FrictionJoint
//

FrictionJointPtr FrictionJoint::Create(World* world, Param const& param)
{
    FrictionJointPtr ptr = new (std::nothrow) FrictionJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

FrictionJoint::FrictionJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool FrictionJoint::InitJoint(World* world, FrictionJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2FrictionJointDef def;
    def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor));
    def.maxForce  = param.max_force;
    def.maxTorque = world->Stage2World(param.max_torque);

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2FrictionJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

void FrictionJoint::SetMaxForce(float length)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxForce(length);
}

float FrictionJoint::GetMaxForce() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetMaxForce();
}

void FrictionJoint::SetMaxTorque(float length)
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    raw_joint_->SetMaxTorque(GetWorld()->Stage2World(length));
}

float FrictionJoint::GetMaxTorque() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetMaxTorque());
}

//
// GearJoint
//

GearJointPtr GearJoint::Create(World* world, Param const& param)
{
    GearJointPtr ptr = new (std::nothrow) GearJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

GearJoint::GearJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool GearJoint::InitJoint(World* world, GearJoint::Param const& param)
{
    KGE_ASSERT(param.joint_a && param.joint_b);

    b2GearJointDef def;
    def.joint1 = param.joint_a->GetB2Joint();
    def.joint2 = param.joint_b->GetB2Joint();
    def.ratio  = param.ratio;

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2GearJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

void GearJoint::SetRatio(float ratio)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetRatio(ratio);
}

float GearJoint::GetRatio() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetRatio();
}

//
// MotorJoint
//

MotorJointPtr MotorJoint::Create(World* world, Param const& param)
{
    MotorJointPtr ptr = new (std::nothrow) MotorJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

MotorJoint::MotorJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool MotorJoint::InitJoint(World* world, MotorJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2MotorJointDef def;
    def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body());
    def.maxForce         = param.max_force;
    def.maxTorque        = world->Stage2World(param.max_torque);
    def.correctionFactor = param.correction_factor;

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2MotorJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

void MotorJoint::SetMaxForce(float length)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxForce(length);
}

float MotorJoint::GetMaxForce() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetMaxForce();
}

void MotorJoint::SetMaxTorque(float length)
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    raw_joint_->SetMaxTorque(GetWorld()->Stage2World(length));
}

float MotorJoint::GetMaxTorque() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetMaxTorque());
}

//
// PrismaticJoint
//

PrismaticJointPtr PrismaticJoint::Create(World* world, Param const& param)
{
    PrismaticJointPtr ptr = new (std::nothrow) PrismaticJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

PrismaticJoint::PrismaticJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool PrismaticJoint::InitJoint(World* world, PrismaticJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2PrismaticJointDef def;
    def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor),
                   Stage2World(param.axis));
    def.enableLimit      = param.enable_limit;
    def.lowerTranslation = world->Stage2World(param.lower_translation);
    def.upperTranslation = world->Stage2World(param.upper_translation);
    def.enableMotor      = param.enable_motor;
    def.maxMotorForce    = param.max_motor_force;
    def.motorSpeed       = world->Stage2World(param.motor_speed);

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2PrismaticJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

float PrismaticJoint::GetJointTranslation() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetJointTranslation());
}

float PrismaticJoint::GetJointSpeed() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetJointSpeed());
}

float PrismaticJoint::GetLowerLimit() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetLowerLimit());
}

float PrismaticJoint::GetUpperLimit() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetUpperLimit());
}

void PrismaticJoint::SetLimits(float lower, float upper)
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    raw_joint_->SetLimits(GetWorld()->Stage2World(lower), GetWorld()->Stage2World(upper));
}

//
// PulleyJoint
//

PulleyJointPtr PulleyJoint::Create(World* world, Param const& param)
{
    PulleyJointPtr ptr = new (std::nothrow) PulleyJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

PulleyJoint::PulleyJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool PulleyJoint::InitJoint(World* world, PulleyJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2PulleyJointDef def;
    def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.ground_anchor_a),
                   world->Stage2World(param.ground_anchor_b), world->Stage2World(param.anchor_a),
                   world->Stage2World(param.anchor_b), param.ratio);

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2PulleyJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

Point PulleyJoint::GetGroundAnchorA() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetGroundAnchorA());
}

Point PulleyJoint::GetGroundAnchorB() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetGroundAnchorB());
}

float PulleyJoint::GetRatio() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetRatio();
}

float PulleyJoint::GetLengthA() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetLengthA());
}

float PulleyJoint::GetLengthB() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetLengthB());
}

float PulleyJoint::GetCurrentLengthA() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetCurrentLengthA());
}

float PulleyJoint::GetCurrentLengthB() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetCurrentLengthB());
}

//
// RevoluteJoint
//

RevoluteJointPtr RevoluteJoint::Create(World* world, Param const& param)
{
    RevoluteJointPtr ptr = new (std::nothrow) RevoluteJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

RevoluteJoint::RevoluteJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool RevoluteJoint::InitJoint(World* world, RevoluteJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2RevoluteJointDef def;
    def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor));
    def.enableLimit    = param.enable_limit;
    def.lowerAngle     = math::Degree2Radian(param.lower_angle);
    def.upperAngle     = math::Degree2Radian(param.upper_angle);
    def.enableMotor    = param.enable_motor;
    def.maxMotorTorque = world->Stage2World(param.max_motor_torque);
    def.motorSpeed     = math::Degree2Radian(param.motor_speed);

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2RevoluteJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

float RevoluteJoint::GetJointAngle() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return math::Radian2Degree(raw_joint_->GetJointAngle());
}

float RevoluteJoint::GetJointSpeed() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return math::Radian2Degree(raw_joint_->GetJointSpeed());
}

float RevoluteJoint::GetLowerLimit() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return math::Radian2Degree(raw_joint_->GetLowerLimit());
}

float RevoluteJoint::GetUpperLimit() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return math::Radian2Degree(raw_joint_->GetUpperLimit());
}

void RevoluteJoint::SetLimits(float lower, float upper)
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    raw_joint_->SetLimits(math::Degree2Radian(lower), math::Degree2Radian(upper));
}

void RevoluteJoint::SetMaxMotorTorque(float torque)
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    raw_joint_->SetMaxMotorTorque(GetWorld()->Stage2World(torque));
}

float RevoluteJoint::GetMaxMotorTorque() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetMaxMotorTorque());
}

//
// RopeJoint
//

RopeJointPtr RopeJoint::Create(World* world, Param const& param)
{
    RopeJointPtr ptr = new (std::nothrow) RopeJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

RopeJoint::RopeJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool RopeJoint::InitJoint(World* world, RopeJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2RopeJointDef def;
    def.bodyA        = param.body_a->GetB2Body();
    def.bodyB        = param.body_b->GetB2Body();
    def.localAnchorA = world->Stage2World(param.local_anchor_a);
    def.localAnchorB = world->Stage2World(param.local_anchor_b);
    def.maxLength    = world->Stage2World(param.max_length);

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2RopeJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

void RopeJoint::SetMaxLength(float length)
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    raw_joint_->SetMaxLength(GetWorld()->Stage2World(length));
}

float RopeJoint::GetMaxLength() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetMaxLength());
}

//
// WeldJoint
//

WeldJointPtr WeldJoint::Create(World* world, Param const& param)
{
    WeldJointPtr ptr = new (std::nothrow) WeldJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

WeldJoint::WeldJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool WeldJoint::InitJoint(World* world, WeldJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2WeldJointDef def;
    def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor));
    def.frequencyHz  = param.frequency_hz;
    def.dampingRatio = param.damping_ratio;

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2WeldJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

//
// WheelJoint
//

WheelJointPtr WheelJoint::Create(World* world, Param const& param)
{
    WheelJointPtr ptr = new (std::nothrow) WheelJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

WheelJoint::WheelJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool WheelJoint::InitJoint(World* world, WheelJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2WheelJointDef def;
    def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor),
                   Stage2World(param.axis));
    def.enableMotor    = param.enable_motor;
    def.maxMotorTorque = world->Stage2World(param.max_motor_torque);
    def.motorSpeed     = world->Stage2World(param.motor_speed);
    def.frequencyHz    = param.frequency_hz;
    def.dampingRatio   = param.damping_ratio;

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2WheelJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

float WheelJoint::GetJointTranslation() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetJointTranslation());
}

float WheelJoint::GetJointLinearSpeed() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetJointLinearSpeed());
}

void WheelJoint::SetMaxMotorTorque(float torque)
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    raw_joint_->SetMaxMotorTorque(GetWorld()->Stage2World(torque));
}

float WheelJoint::GetMaxMotorTorque() const
{
    KGE_ASSERT(raw_joint_ && GetWorld());
    return GetWorld()->World2Stage(raw_joint_->GetMaxMotorTorque());
}

//
// MouseJoint
//

MouseJointPtr MouseJoint::Create(World* world, Param const& param)
{
    MouseJointPtr ptr = new (std::nothrow) MouseJoint;
    if (ptr)
    {
        if (!ptr->InitJoint(world, param))
            return nullptr;
    }
    return ptr;
}

MouseJoint::MouseJoint()
    : Joint()
    , raw_joint_(nullptr)
{
}

bool MouseJoint::InitJoint(World* world, MouseJoint::Param const& param)
{
    KGE_ASSERT(param.body_a && param.body_b);

    b2MouseJointDef def;
    def.bodyA        = param.body_a->GetB2Body();
    def.bodyB        = param.body_b->GetB2Body();
    def.target       = world->Stage2World(param.target);
    def.maxForce     = param.max_force;
    def.frequencyHz  = param.frequency_hz;
    def.dampingRatio = param.damping_ratio;

    Joint::InitJoint(world, &def);
    raw_joint_ = static_cast<b2MouseJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

void MouseJoint::SetMaxForce(float length)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxForce(length);
}

float MouseJoint::GetMaxForce() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetMaxForce();
}

}  // namespace physics
}  // namespace kiwano
