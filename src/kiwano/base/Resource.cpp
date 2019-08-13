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

#include "Resource.h"
#include "../base/logs.h"

namespace kiwano
{
	Resource::Resource(LPCWSTR file_name)
		: type_(Type::File)
		, bin_name_(nullptr)
		, bin_type_(nullptr)
	{
		if (file_name)
			file_name_ = new (std::nothrow) String(file_name);
	}

	Resource::Resource(String const& file_name)
		: type_(Type::File)
        , bin_name_(nullptr)
        , bin_type_(nullptr)
	{
		if (!file_name.empty())
			file_name_ = new (std::nothrow) String(file_name);
	}

	Resource::Resource(LPCWSTR name, LPCWSTR type)
		: type_(Type::Binary)
		, bin_name_(name)
		, bin_type_(type)
	{
	}

	Resource::Resource(Resource const & rhs)
	{
		operator=(rhs);
	}

	Resource::~Resource()
	{
		if (IsFileType() && file_name_)
			delete file_name_;
	}

	size_t Resource::GetHashCode() const
	{
		if (type_ == Type::File)
			return GetFileName().hash();
		return std::hash<LPCWSTR>{}(bin_name_);
	}

	Resource & Resource::operator=(Resource const & rhs)
	{
		if (&rhs != this)
		{
			if (IsFileType() && file_name_)
			{
				delete file_name_;
				file_name_ = nullptr;
			}

			type_ = rhs.type_;
			if (IsFileType())
			{
				if (rhs.file_name_)
				{
					file_name_ = new (std::nothrow) String(*rhs.file_name_);
				}
			}
			else
			{
				bin_name_ = rhs.bin_name_;
				bin_type_ = rhs.bin_type_;
			}
		}
		return *this;
	}

	bool Resource::Load(LPVOID& buffer, DWORD& buffer_size) const
	{
		if (type_ != Type::Binary)
		{
			KGE_ERROR_LOG(L"Only binary resource can be loaded");
			return false;
		}

		HGLOBAL res_data;
		HRSRC res_info;

		res_info = FindResourceW(nullptr, bin_name_, bin_type_);
		if (res_info == nullptr)
		{
			KGE_ERROR_LOG(L"FindResource failed");
			return false;
		}

		res_data = LoadResource(nullptr, res_info);
		if (res_data == nullptr)
		{
			KGE_ERROR_LOG(L"LoadResource failed");
			return false;
		}

		buffer_size = SizeofResource(nullptr, res_info);
		if (buffer_size == 0)
		{
			KGE_ERROR_LOG(L"SizeofResource failed");
			return false;
		}

		buffer = LockResource(res_data);
		if (buffer == nullptr)
		{
			KGE_ERROR_LOG(L"LockResource failed");
			return false;
		}
		return true;
	}
}
