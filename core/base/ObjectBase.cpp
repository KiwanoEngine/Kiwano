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

#include "ObjectBase.h"

namespace easy2d
{
	namespace
	{
		bool tracing_leaks = true;
		std::vector<ObjectBase*> tracing_objects;
	}

	ObjectBase::ObjectBase()
		: tracing_leak_(false)
	{
#ifdef E2D_DEBUG

		ObjectBase::__AddObjectToTracingList(this);

#endif
	}

	ObjectBase::~ObjectBase()
	{
#ifdef E2D_DEBUG

		ObjectBase::__RemoveObjectFromTracingList(this);

#endif
	}

	void ObjectBase::StartTracingLeaks()
	{
		tracing_leaks = true;
	}

	void ObjectBase::StopTracingLeaks()
	{
		tracing_leaks = false;
	}

	std::vector<ObjectBase*> const& easy2d::ObjectBase::__GetTracingObjects()
	{
		return tracing_objects;
	}

	void ObjectBase::__AddObjectToTracingList(ObjectBase * obj)
	{
#ifdef E2D_DEBUG

		if (tracing_leaks && !obj->tracing_leak_)
		{
			obj->tracing_leak_ = true;
			tracing_objects.push_back(obj);
		}

#endif
	}

	void ObjectBase::__RemoveObjectFromTracingList(ObjectBase * obj)
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
