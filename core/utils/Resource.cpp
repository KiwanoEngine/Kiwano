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

#include "..\e2dtool.h"


easy2d::Resource::Resource(LPCWSTR name, LPCWSTR type)
	: name_(name)
	, type_(type)
	, data_(nullptr)
	, data_size_(0)
	, loaded_(false)
{
}

LPCWSTR easy2d::Resource::GetName() const
{
	return name_;
}

LPCWSTR easy2d::Resource::GetType() const
{
	return type_;
}

LPVOID easy2d::Resource::GetData() const
{
	return data_;
}

DWORD easy2d::Resource::GetDataSize() const
{
	return data_size_;
}

size_t easy2d::Resource::GetHashCode() const
{
	return std::hash<LPCWSTR>{}(name_);
}

bool easy2d::Resource::Load()
{
	if (!loaded_)
	{
		HRSRC res_info;
		HGLOBAL res_data;
		HINSTANCE hinstance = GetModuleHandle(NULL);

		res_info = FindResourceW(hinstance, name_, type_);
		if (res_info == nullptr)
		{
			E2D_WARNING("FindResource");
			return false;
		}

		res_data = LoadResource(hinstance, res_info);
		if (res_data == nullptr)
		{
			E2D_WARNING("LoadResource");
			return false;
		}

		data_size_ = SizeofResource(hinstance, res_info);
		if (data_size_ == 0)
		{
			E2D_WARNING("SizeofResource");
			return false;
		}

		data_ = LockResource(res_data);
		if (data_ == nullptr)
		{
			E2D_WARNING("LockResource");
			return false;
		}

		loaded_ = true;
	}
	return true;
}
