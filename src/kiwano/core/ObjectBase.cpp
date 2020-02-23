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

#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Logger.h>
#include <kiwano/core/Json.h>
#include <typeinfo>

namespace kiwano
{
namespace
{
bool                tracing_leaks = false;
Vector<ObjectBase*> tracing_objects;
uint32_t            last_object_id = 0;
}  // namespace

ObjectBase::ObjectBase()
    : tracing_leak_(false)
    , name_(nullptr)
    , user_data_()
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

#ifdef KGE_DEBUG
    ObjectBase::RemoveObjectFromTracingList(this);
#endif
}

const Any& ObjectBase::GetUserData() const
{
    return user_data_;
}

void ObjectBase::SetUserData(const Any& data)
{
    user_data_ = data;
}

void ObjectBase::SetName(const String& name)
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

String ObjectBase::DumpObject()
{
    return strings::Format("{ class=\"%s\" id=%d refcount=%d name=\"%s\" }", typeid(*this).name(), GetObjectID(),
                          GetRefCount(), GetName().c_str());
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
    KGE_SYS_LOG("-------------------------- All Objects --------------------------");
    for (const auto object : tracing_objects)
    {
        KGE_SYS_LOG("%s", object->DumpObject().c_str());
    }
    KGE_SYS_LOG("------------------------- Total size: %d -------------------------", tracing_objects.size());
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
