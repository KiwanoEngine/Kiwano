// Copyright (c) 2016-2018 Kiwano - Nomango
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

#include <typeinfo>
#include <atomic>
#include <kiwano/core/BaseObject.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/utils/Json.h>

namespace kiwano
{
namespace
{

#ifdef KGE_DEBUG
bool                      tracing_leaks = false;
UnorderedSet<BaseObject*> tracing_objects;
#endif

ObjectPolicyFunc object_policy_ = ObjectPolicy::ErrorLog();

}  // namespace

ObjectFailException::ObjectFailException(BaseObject* obj, const ObjectStatus& status)
    : obj_(obj)
    , status_(status)
{
}

char const* ObjectFailException::what() const
{
    return status_.msg.empty() ? "Object operation failed" : status_.msg.c_str();
}

ObjectPolicyFunc ObjectPolicy::WarnLog(int threshold)
{
    return [=](BaseObject* obj, const ObjectStatus& status)
    {
        if (!obj->IsValid() || status.code <= threshold)
        {
            KGE_WARNF("Object operation failed: obj(%p), code(%d), msg(%s)", obj, status.code, status.msg.c_str());
        }
    };
}

ObjectPolicyFunc ObjectPolicy::ErrorLog(int threshold)
{
    return [=](BaseObject* obj, const ObjectStatus& status)
    {
        if (!obj->IsValid() || status.code <= threshold)
        {
            KGE_ERRORF("Object operation failed: obj(%p), code(%d), msg(%s)", obj, status.code, status.msg.c_str());
        }
    };
}

ObjectPolicyFunc ObjectPolicy::Exception(int threshold)
{
    return [=](BaseObject* obj, const ObjectStatus& status)
    {
        if (!obj->IsValid() || status.code <= threshold)
        {
            throw ObjectFailException(obj, status);
        }
    };
}

BaseObject::BaseObject()
    : name_(nullptr)
    , status_(nullptr)
    , user_data_(nullptr)
{
#ifdef KGE_DEBUG
    BaseObject::AddObjectToTracingList(this);
#endif
}

BaseObject::~BaseObject()
{
    if (name_)
    {
        delete name_;
        name_ = nullptr;
    }

    ClearStatus();

    user_data_ = nullptr;

#ifdef KGE_DEBUG
    BaseObject::RemoveObjectFromTracingList(this);
#endif
}

void* BaseObject::GetUserData() const
{
    return user_data_;
}

void BaseObject::SetUserData(void* data)
{
    user_data_ = data;
}

void BaseObject::SetName(StringView name)
{
    if (IsName(name))
        return;

    if (name.empty())
    {
        if (name_)
            name_->clear();
        return;
    }

    if (!name_)
    {
        name_ = new String(name);
        return;
    }

    *name_ = name;
}

void BaseObject::OnSerialize(Serializer* serializer) const
{
    (*serializer) << GetName();
}

void BaseObject::OnDeserialize(Deserializer* deserializer)
{
    String name;
    (*deserializer) >> name;
    SetName(name);
}

bool BaseObject::IsValid() const
{
    return status_ ? status_->Success() : true;
}

ObjectStatus* BaseObject::GetStatus() const
{
    return status_;
}

void BaseObject::SetStatus(const ObjectStatus& status)
{
    if (!status_)
    {
        status_ = new ObjectStatus;
    }

    status_->msg = status.msg;
    if (status_->code != status.code)
    {
        status_->code = status.code;

        if (object_policy_)
        {
            object_policy_(this, *status_);
        }
    }
}

void BaseObject::Fail(StringView msg, int code)
{
    SetStatus(ObjectStatus(code, msg));
}

void BaseObject::ClearStatus()
{
    if (status_)
    {
        delete status_;
        status_ = nullptr;
    }
}

void BaseObject::SetObjectPolicy(const ObjectPolicyFunc& policy)
{
    object_policy_ = policy;
}

#ifdef KGE_DEBUG

bool BaseObject::IsTracingLeaks()
{
    return tracing_leaks;
}

void BaseObject::StartTracingLeaks()
{
    tracing_leaks = true;
}

void BaseObject::StopTracingLeaks()
{
    tracing_leaks = false;
}

void BaseObject::DumpTracingObjects()
{
    KGE_DEBUG_LOGF("-------------------------- All Objects --------------------------");
    for (const auto object : tracing_objects)
    {
        KGE_DEBUG_LOGF("{ class=\"%s\" addr=%p refcount=%d name=\"%s\" }", typeid(*object).name(), object,
                       object->GetRefCount(), object->GetName().data());
    }
    KGE_DEBUG_LOGF("------------------------- Total size: %d -------------------------", tracing_objects.size());
}

const UnorderedSet<BaseObject*>& BaseObject::GetTracingObjects()
{
    return tracing_objects;
}

void BaseObject::AddObjectToTracingList(BaseObject* obj)
{
    if (tracing_leaks)
    {
        tracing_objects.insert(obj);
    }
}

void BaseObject::RemoveObjectFromTracingList(BaseObject* obj)
{
    tracing_objects.erase(obj);
}

#endif  // KGE_DEBUG

}  // namespace kiwano
