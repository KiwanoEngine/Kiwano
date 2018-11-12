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

#include "Resource.h"
#include "logs.h"

namespace easy2d
{
	Resource::Resource(LPCWSTR name, LPCWSTR type)
		: name_(name)
		, type_(type)
	{
	}

	LPCWSTR Resource::GetName() const
	{
		return name_;
	}

	LPCWSTR Resource::GetType() const
	{
		return type_;
	}

	size_t Resource::GetHashCode() const
	{
		return std::hash<LPCWSTR>{}(name_);
	}

	bool Resource::Load(ResourceData* buffer) const
	{
		if (!buffer)
			return false;

		HGLOBAL res_data;
		HRSRC res_info;

		res_info = FindResourceW(nullptr, name_, type_);
		if (res_info == nullptr)
		{
			logs::Trace(L"FindResource");
			return false;
		}

		res_data = LoadResource(nullptr, res_info);
		if (res_data == nullptr)
		{
			logs::Trace(L"LoadResource");
			return false;
		}

		(*buffer).buffer_size = SizeofResource(nullptr, res_info);
		if ((*buffer).buffer_size == 0)
		{
			logs::Trace(L"SizeofResource");
			return false;
		}

		(*buffer).buffer = LockResource(res_data);
		if ((*buffer).buffer == nullptr)
		{
			logs::Trace(L"LockResource");
			return false;
		}
		return true;
	}
}