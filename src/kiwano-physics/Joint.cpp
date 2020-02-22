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
#include <kiwano-physics/PhysicWorld.h>

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
}

bool Joint::Init(PhysicWorld* world)
{
    return false;
}

bool Joint::Init(PhysicWorld* world, b2JointDef* joint_def)
{
    KGE_ASSERT(joint_ == nullptr);
    KGE_ASSERT(world);

    Destroy();

    world_ = world;

    b2World* b2world = world_->GetB2World();
    b2Joint* joint   = b2world->CreateJoint(joint_def);
    if (joint)
    {
        SetB2Joint(joint);
        return true;
    }
    return false;
}

PhysicBodyPtr Joint::GetBodyA() const
{
    KGE_ASSERT(joint_);

    b2Body* body = joint_->GetBodyA();
    return PhysicBodyPtr(static_cast<PhysicBody*>(body->GetUserData()));
}

PhysicBodyPtr Joint::GetBodyB() const
{
    KGE_ASSERT(joint_);

    b2Body* body = joint_->GetBodyB();
    return PhysicBodyPtr(static_cast<PhysicBody*>(body->GetUserData()));
}

void Joint::SetB2Joint(b2Joint* joint)
{
    joint_ = joint;
    if (joint_)
    {
        joint_->SetUserData(this);
        type_ = Joint::Type(joint_->GetType());
    }
}

void Joint::Destroy()
{
    if (joint_ && world_)
    {
        b2World* b2world = world_->GetB2World();
        if (b2world)
        {
            b2world->DestroyJoint(joint_);
        }
    }

    joint_ = nullptr;
    world_ = nullptr;
}

//
// DistanceJoint
//

DistanceJointPtr DistanceJoint::Create(const Param& param)
{
    DistanceJointPtr ptr = new (std::nothrow) DistanceJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

DistanceJoint::DistanceJoint()
    : raw_joint_(nullptr)
{
}

bool DistanceJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2DistanceJointDef def;
    def.Initialize(param_.body_a->GetB2Body(), param_.body_b->GetB2Body(), global::LocalToWorld(param_.anchor_a),
                   global::LocalToWorld(param_.anchor_b));
    def.frequencyHz  = param_.frequency_hz;
    def.dampingRatio = param_.damping_ratio;

    Joint::Init(world, &def);
    raw_joint_ = static_cast<b2DistanceJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

void DistanceJoint::SetLength(float length)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetLength(global::LocalToWorld(length));
}

float DistanceJoint::GetLength() const
{
    KGE_ASSERT(raw_joint_);
    return global::LocalToWorld(raw_joint_->GetLength());
}

//
// FrictionJoint
//

FrictionJointPtr FrictionJoint::Create(const Param& param)
{
    FrictionJointPtr ptr = new (std::nothrow) FrictionJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

FrictionJoint::FrictionJoint()
    : raw_joint_(nullptr)
{
}

bool FrictionJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2FrictionJointDef def;
    def.Initialize(param_.body_a->GetB2Body(), param_.body_b->GetB2Body(), global::LocalToWorld(param_.anchor));
    def.maxForce  = param_.max_force;
    def.maxTorque = global::LocalToWorld(param_.max_torque);

    Joint::Init(world, &def);
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
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxTorque(global::LocalToWorld(length));
}

float FrictionJoint::GetMaxTorque() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetMaxTorque());
}

//
// GearJoint
//

GearJointPtr GearJoint::Create(const Param& param)
{
    GearJointPtr ptr = new (std::nothrow) GearJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

GearJoint::GearJoint()
    : raw_joint_(nullptr)
{
}

bool GearJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.joint_a && param_.joint_b);

    b2GearJointDef def;
    def.joint1 = param_.joint_a->GetB2Joint();
    def.joint2 = param_.joint_b->GetB2Joint();
    def.ratio  = param_.ratio;

    Joint::Init(world, &def);
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

MotorJointPtr MotorJoint::Create(const Param& param)
{
    MotorJointPtr ptr = new (std::nothrow) MotorJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

MotorJoint::MotorJoint()
    : raw_joint_(nullptr)
{
}

bool MotorJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2MotorJointDef def;
    def.Initialize(param_.body_a->GetB2Body(), param_.body_b->GetB2Body());
    def.maxForce         = param_.max_force;
    def.maxTorque        = global::LocalToWorld(param_.max_torque);
    def.correctionFactor = param_.correction_factor;

    Joint::Init(world, &def);
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
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxTorque(global::LocalToWorld(length));
}

float MotorJoint::GetMaxTorque() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetMaxTorque());
}

//
// PrismaticJoint
//

PrismaticJointPtr PrismaticJoint::Create(const Param& param)
{
    PrismaticJointPtr ptr = new (std::nothrow) PrismaticJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

PrismaticJoint::PrismaticJoint()
    : raw_joint_(nullptr)
{
}

bool PrismaticJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2PrismaticJointDef def;
    def.Initialize(param_.body_a->GetB2Body(), param_.body_b->GetB2Body(), global::LocalToWorld(param_.anchor),
                   b2Vec2(param_.axis.x, param_.axis.y));
    def.enableLimit      = param_.enable_limit;
    def.lowerTranslation = global::LocalToWorld(param_.lower_translation);
    def.upperTranslation = global::LocalToWorld(param_.upper_translation);
    def.enableMotor      = param_.enable_motor;
    def.maxMotorForce    = param_.max_motor_force;
    def.motorSpeed       = global::LocalToWorld(param_.motor_speed);

    Joint::Init(world, &def);
    raw_joint_ = static_cast<b2PrismaticJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

float PrismaticJoint::GetJointTranslation() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetJointTranslation());
}

float PrismaticJoint::GetJointSpeed() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetJointSpeed());
}

float PrismaticJoint::GetLowerLimit() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetLowerLimit());
}

float PrismaticJoint::GetUpperLimit() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetUpperLimit());
}

void PrismaticJoint::SetLimits(float lower, float upper)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetLimits(global::LocalToWorld(lower), global::LocalToWorld(upper));
}

//
// PulleyJoint
//

PulleyJointPtr PulleyJoint::Create(const Param& param)
{
    PulleyJointPtr ptr = new (std::nothrow) PulleyJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

PulleyJoint::PulleyJoint()
    : raw_joint_(nullptr)
{
}

bool PulleyJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2PulleyJointDef def;
    def.Initialize(param_.body_a->GetB2Body(), param_.body_b->GetB2Body(), global::LocalToWorld(param_.ground_anchor_a),
                   global::LocalToWorld(param_.ground_anchor_b), global::LocalToWorld(param_.anchor_a),
                   global::LocalToWorld(param_.anchor_b), param_.ratio);

    Joint::Init(world, &def);
    raw_joint_ = static_cast<b2PulleyJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

Point PulleyJoint::GetGroundAnchorA() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetGroundAnchorA());
}

Point PulleyJoint::GetGroundAnchorB() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetGroundAnchorB());
}

float PulleyJoint::GetRatio() const
{
    KGE_ASSERT(raw_joint_);
    return raw_joint_->GetRatio();
}

float PulleyJoint::GetLengthA() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetLengthA());
}

float PulleyJoint::GetLengthB() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetLengthB());
}

float PulleyJoint::GetCurrentLengthA() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetCurrentLengthA());
}

float PulleyJoint::GetCurrentLengthB() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetCurrentLengthB());
}

//
// RevoluteJoint
//

RevoluteJointPtr RevoluteJoint::Create(const Param& param)
{
    RevoluteJointPtr ptr = new (std::nothrow) RevoluteJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

RevoluteJoint::RevoluteJoint()
    : raw_joint_(nullptr)
{
}

bool RevoluteJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2RevoluteJointDef def;
    def.Initialize(param_.body_a->GetB2Body(), param_.body_b->GetB2Body(), global::LocalToWorld(param_.anchor));
    def.enableLimit    = param_.enable_limit;
    def.lowerAngle     = math::Degree2Radian(param_.lower_angle);
    def.upperAngle     = math::Degree2Radian(param_.upper_angle);
    def.enableMotor    = param_.enable_motor;
    def.maxMotorTorque = global::LocalToWorld(param_.max_motor_torque);
    def.motorSpeed     = math::Degree2Radian(param_.motor_speed);

    Joint::Init(world, &def);
    raw_joint_ = static_cast<b2RevoluteJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

float RevoluteJoint::GetJointAngle() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetJointAngle());
}

float RevoluteJoint::GetJointSpeed() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetJointSpeed());
}

float RevoluteJoint::GetLowerLimit() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetLowerLimit());
}

float RevoluteJoint::GetUpperLimit() const
{
    KGE_ASSERT(raw_joint_);
    return math::Radian2Degree(raw_joint_->GetUpperLimit());
}

void RevoluteJoint::SetLimits(float lower, float upper)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetLimits(math::Degree2Radian(lower), math::Degree2Radian(upper));
}

void RevoluteJoint::SetMaxMotorTorque(float torque)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxMotorTorque(global::LocalToWorld(torque));
}

float RevoluteJoint::GetMaxMotorTorque() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetMaxMotorTorque());
}

//
// RopeJoint
//

RopeJointPtr RopeJoint::Create(const Param& param)
{
    RopeJointPtr ptr = new (std::nothrow) RopeJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

RopeJoint::RopeJoint()
    : raw_joint_(nullptr)
{
}

bool RopeJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2RopeJointDef def;
    def.bodyA        = param_.body_a->GetB2Body();
    def.bodyB        = param_.body_b->GetB2Body();
    def.localAnchorA = global::LocalToWorld(param_.local_anchor_a);
    def.localAnchorB = global::LocalToWorld(param_.local_anchor_b);
    def.maxLength    = global::LocalToWorld(param_.max_length);

    Joint::Init(world, &def);
    raw_joint_ = static_cast<b2RopeJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

void RopeJoint::SetMaxLength(float length)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxLength(global::LocalToWorld(length));
}

float RopeJoint::GetMaxLength() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetMaxLength());
}

//
// WeldJoint
//

WeldJointPtr WeldJoint::Create(const Param& param)
{
    WeldJointPtr ptr = new (std::nothrow) WeldJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

WeldJoint::WeldJoint()
    : raw_joint_(nullptr)
{
}

bool WeldJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2WeldJointDef def;
    def.Initialize(param_.body_a->GetB2Body(), param_.body_b->GetB2Body(), global::LocalToWorld(param_.anchor));
    def.frequencyHz  = param_.frequency_hz;
    def.dampingRatio = param_.damping_ratio;

    Joint::Init(world, &def);
    raw_joint_ = static_cast<b2WeldJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

//
// WheelJoint
//

WheelJointPtr WheelJoint::Create(const Param& param)
{
    WheelJointPtr ptr = new (std::nothrow) WheelJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

WheelJoint::WheelJoint()
    : raw_joint_(nullptr)
{
}

bool WheelJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2WheelJointDef def;
    def.Initialize(param_.body_a->GetB2Body(), param_.body_b->GetB2Body(), global::LocalToWorld(param_.anchor),
                   b2Vec2(param_.axis.x, param_.axis.y));
    def.enableMotor    = param_.enable_motor;
    def.maxMotorTorque = global::LocalToWorld(param_.max_motor_torque);
    def.motorSpeed     = global::LocalToWorld(param_.motor_speed);
    def.frequencyHz    = param_.frequency_hz;
    def.dampingRatio   = param_.damping_ratio;

    Joint::Init(world, &def);
    raw_joint_ = static_cast<b2WheelJoint*>(GetB2Joint());
    return raw_joint_ != nullptr;
}

float WheelJoint::GetJointTranslation() const
{
    KGE_ASSERT(raw_joint_);
    return global::LocalToWorld(raw_joint_->GetJointTranslation());
}

float WheelJoint::GetJointLinearSpeed() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetJointLinearSpeed());
}

void WheelJoint::SetMaxMotorTorque(float torque)
{
    KGE_ASSERT(raw_joint_);
    raw_joint_->SetMaxMotorTorque(global::LocalToWorld(torque));
}

float WheelJoint::GetMaxMotorTorque() const
{
    KGE_ASSERT(raw_joint_);
    return global::WorldToLocal(raw_joint_->GetMaxMotorTorque());
}

//
// MouseJoint
//

MouseJointPtr MouseJoint::Create(const Param& param)
{
    MouseJointPtr ptr = new (std::nothrow) MouseJoint;
    if (ptr)
    {
        ptr->param_ = param;
    }
    return ptr;
}

MouseJoint::MouseJoint()
    : raw_joint_(nullptr)
{
}

bool MouseJoint::Init(PhysicWorld* world)
{
    KGE_ASSERT(param_.body_a && param_.body_b);

    b2MouseJointDef def;
    def.bodyA        = param_.body_a->GetB2Body();
    def.bodyB        = param_.body_b->GetB2Body();
    def.target       = global::LocalToWorld(param_.target);
    def.maxForce     = param_.max_force;
    def.frequencyHz  = param_.frequency_hz;
    def.dampingRatio = param_.damping_ratio;

    Joint::Init(world, &def);
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
