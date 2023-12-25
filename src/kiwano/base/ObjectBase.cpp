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
#include <kiwano/base/ObjectBase.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/utils/Json.h>

namespace kiwano
{
namespace
{

bool                  tracing_leaks = false;
Vector<ObjectBase*>   tracing_objects;
std::atomic<uint64_t> last_object_id = 0;
ObjectPolicyFunc      object_policy_ = ObjectPolicy::ErrorLog();

}  // namespace



ObjectFailException::ObjectFailException(ObjectBase* obj, const ObjectStatus& status)
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
    return [=](ObjectBase* obj, const ObjectStatus& status)
    {
        if (!obj->IsValid() || status.code <= threshold)
        {
            KGE_WARNF("Object operation failed: obj(%p), code(%d), msg(%s)", obj, status.code, status.msg.c_str());
        }
    };
}

ObjectPolicyFunc ObjectPolicy::ErrorLog(int threshold)
{
    return [=](ObjectBase* obj, const ObjectStatus& status)
    {
        if (!obj->IsValid() || status.code <= threshold)
        {
            KGE_ERRORF("Object operation failed: obj(%p), code(%d), msg(%s)", obj, status.code, status.msg.c_str());
        }
    };
}

ObjectPolicyFunc ObjectPolicy::Exception(int threshold)
{
    return [=](ObjectBase* obj, const ObjectStatus& status)
    {
        if (!obj->IsValid() || status.code <= threshold)
        {
            throw ObjectFailException(obj, status);
        }
    };
}

ObjectBase::ObjectBase()
    : tracing_leak_(false)
    , name_(nullptr)
    , user_data_(nullptr)
    , status_(nullptr)
    , holdings_(nullptr)
    , id_(++last_object_id)
{
#ifdef KGE_DEBUG
    ObjectBase::AddObjectToTracingList(this);
#endif
}

ObjectBase::~ObjectBase()
{
    if (name_)
    {
        delete name_;
        name_ = nullptr;
    }

    ClearStatus();

    if (holdings_)
    {
        delete holdings_;
        holdings_ = nullptr;
    }

#ifdef KGE_DEBUG
    ObjectBase::RemoveObjectFromTracingList(this);
#endif
}

void* ObjectBase::GetUserData() const
{
    return user_data_;
}

void ObjectBase::SetUserData(void* data)
{
    user_data_ = data;
}

void ObjectBase::Hold(ObjectBasePtr other)
{
    if (!holdings_)
    {
        holdings_ = new Set<ObjectBasePtr>;
    }
    holdings_->insert(other);
}

void ObjectBase::Unhold(ObjectBasePtr other)
{
    if (holdings_)
    {
        holdings_->erase(other);
    }
}

void ObjectBase::SetName(StringView name)
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

void ObjectBase::DoSerialize(Serializer* serializer) const
{
    (*serializer) << GetName();
}

void ObjectBase::DoDeserialize(Deserializer* deserializer)
{
    String name;
    (*deserializer) >> name;
    SetName(name);
}

bool ObjectBase::IsValid() const
{
    return status_ ? status_->Success() : true;
}

ObjectStatus* ObjectBase::GetStatus() const
{
    return status_;
}

void ObjectBase::SetStatus(const ObjectStatus& status)
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

void ObjectBase::Fail(StringView msg, int code)
{
    SetStatus(ObjectStatus(code, msg));
}

void ObjectBase::ClearStatus()
{
    if (status_)
    {
        delete status_;
        status_ = nullptr;
    }
}

void ObjectBase::SetObjectPolicy(const ObjectPolicyFunc& policy)
{
    object_policy_ = policy;
}

bool ObjectBase::IsTracingLeaks()
{
    return tracing_leaks;
}

void ObjectBase::StartTracingLeaks()
{
    tracing_leaks = true;
}

void ObjectBase::StopTracingLeaks()
{
    tracing_leaks = false;
}

void ObjectBase::DumpTracingObjects()
{
    KGE_DEBUG_LOGF("-------------------------- All Objects --------------------------");
    for (const auto object : tracing_objects)
    {
        KGE_DEBUG_LOGF("{ class=\"%s\" id=%d refcount=%d name=\"%s\" }", typeid(*object).name(), object->GetObjectID(),
                    object->GetRefCount(), object->GetName().data());
    }
    KGE_DEBUG_LOGF("------------------------- Total size: %d -------------------------", tracing_objects.size());
}

Vector<ObjectBase*>& ObjectBase::GetTracingObjects()
{
    return tracing_objects;
}

void ObjectBase::AddObjectToTracingList(ObjectBase* obj)
{
#ifdef KGE_DEBUG
    if (tracing_leaks && !obj->tracing_leak_)
    {
        obj->tracing_leak_ = true;
        tracing_objects.push_back(obj);
    }
#endif
}

void ObjectBase::RemoveObjectFromTracingList(ObjectBase* obj)
{
#ifdef KGE_DEBUG
    if (tracing_leaks && obj->tracing_leak_)
    {
        obj->tracing_leak_ = false;

        auto iter = std::find(tracing_objects.begin(), tracing_objects.end(), obj);
        if (iter != tracing_objects.end())
        {
            tracing_objects.erase(iter);
        }
    }
#endif
}

}  // namespace kiwano
