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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/core/Logger.h>

namespace kiwano
{
	namespace
	{
		bool tracing_leaks = false;
		Vector<ObjectBase*> tracing_objects;
		uint32_t last_object_id = 0;
	}

	ObjectBase::ObjectBase()
		: tracing_leak_(false)
		, user_data_()
		, name_(nullptr)
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

	void ObjectBase::SetUserData(Any const& data)
	{
		user_data_ = data;
	}

	void ObjectBase::SetName(String const & name)
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
			name_ = new (std::nothrow) String(name);
			return;
		}

		*name_ = name;
	}

	String ObjectBase::DumpObject()
	{
		String name = oc::string_to_wide(typeid(*this).name());
		return String::format(L"{ class=\"%s\" id=%d refcount=%d name=\"%s\" }",
			name.c_str(), GetObjectID(), GetRefCount(), GetName().c_str());
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
		KGE_SYS_LOG(L"-------------------------- All Objects --------------------------");
		for (const auto object : tracing_objects)
		{
			KGE_SYS_LOG(L"%s", object->DumpObject().c_str());
		}
		KGE_SYS_LOG(L"------------------------- Total size: %d -------------------------", tracing_objects.size());
	}

	Vector<ObjectBase*>& ObjectBase::GetTracingObjects()
	{
		return tracing_objects;
	}

	void ObjectBase::AddObjectToTracingList(ObjectBase * obj)
	{
#ifdef KGE_DEBUG
		if (tracing_leaks && !obj->tracing_leak_)
		{
			obj->tracing_leak_ = true;
			tracing_objects.push_back(obj);
		}
#endif
	}

	void ObjectBase::RemoveObjectFromTracingList(ObjectBase * obj)
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

}
