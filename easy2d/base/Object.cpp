// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "Object.h"
#include "logs.h"
#include <typeinfo>

namespace easy2d
{
	namespace
	{
		bool tracing_leaks = false;
		Array<Object*> tracing_objects;
	}

	unsigned int Object::last_object_id = 0;

	Object::Object()
		: tracing_leak_(false)
		, user_data_(nullptr)
		, name_(nullptr)
		, id_(++last_object_id)
	{
#ifdef E2D_DEBUG

		Object::__AddObjectToTracingList(this);

#endif
	}

	Object::~Object()
	{
		if (name_)
			delete name_;

#ifdef E2D_DEBUG

		Object::__RemoveObjectFromTracingList(this);

#endif
	}

	void * Object::GetUserData() const
	{
		return user_data_;
	}

	void Object::SetUserData(void * data)
	{
		user_data_ = data;
	}

	void Object::SetName(String const & name)
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

	String Object::DumpObject()
	{
		String name = typeid(*this).name();
		return String::format(L"{ class=\"%s\" id=%d refcount=%d name=\"%s\" }",
			name.c_str(), GetObjectID(), GetRefCount(), GetName().c_str());
	}

	void Object::StartTracingLeaks()
	{
		tracing_leaks = true;
	}

	void Object::StopTracingLeaks()
	{
		tracing_leaks = false;
	}

	void Object::DumpTracingObjects()
	{
		E2D_LOG(L"-------------------------- All Objects --------------------------");
		for (const auto object : tracing_objects)
		{
			E2D_LOG(object->DumpObject().c_str());
		}
		E2D_LOG(L"------------------------- Total size: %d -------------------------", tracing_objects.size());
	}

	Array<Object*>& easy2d::Object::__GetTracingObjects()
	{
		return tracing_objects;
	}

	void Object::__AddObjectToTracingList(Object * obj)
	{
#ifdef E2D_DEBUG

		if (tracing_leaks && !obj->tracing_leak_)
		{
			obj->tracing_leak_ = true;
			tracing_objects.push_back(obj);
		}

#endif
	}

	void Object::__RemoveObjectFromTracingList(Object * obj)
	{
#ifdef E2D_DEBUG

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
