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

#pragma once
#include "../macros.h"
#include "../core/core.h"
#include "RefCounter.hpp"
#include "SmartPtr.hpp"

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Object);

	class KGE_API Object
		: public RefCounter
	{
	public:
		Object();

		virtual ~Object();

		void* GetUserData() const;

		void SetUserData(void* data);

		void SetName(String const& name);

		inline String GetName() const					{ if (name_) return *name_; return String(); }

		inline bool IsName(String const& name) const	{ return name_ ? (*name_ == name) : name.empty(); }

		inline unsigned int GetObjectID() const			{ return id_; }

		String DumpObject();

	public:
		static bool IsTracingLeaks();

		static void StartTracingLeaks();

		static void StopTracingLeaks();

		static void DumpTracingObjects();

	public:
		static Vector<Object*>& __GetTracingObjects();

		static void __AddObjectToTracingList(Object*);

		static void __RemoveObjectFromTracingList(Object*);

	private:
		bool tracing_leak_;
		void* user_data_;
		String* name_;

		const unsigned int id_;
		static unsigned int last_object_id;
	};
}
