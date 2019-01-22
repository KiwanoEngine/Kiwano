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

#include "File.h"
#include "../core/modules.h"
#include <cwctype>

namespace easy2d
{
	File::File()
		: file_path_()
	{
	}

	File::File(String const& file_name)
		: file_path_(file_name)
	{
		this->Open(file_name);
	}

	File::~File()
	{
	}

	bool File::Open(String const& file_name)
	{
		if (file_name.empty())
			return false;

		file_path_ = file_name;
		return Exists();
	}

	bool File::Exists() const
	{
		return modules::Shlwapi{}.PathFileExistsW(file_path_.c_str());
	}

	String const& File::GetPath() const
	{
		return file_path_;
	}

	String File::GetExtension() const
	{
		String file_ext;
		size_t pos = file_path_.find_last_of(L'.');
		if (pos != String::npos)
		{
			file_ext = file_path_.substr(pos);
			std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), std::towlower);
		}
		return file_ext;
	}

	bool File::Delete()
	{
		if (::DeleteFile(file_path_.c_str()))
			return true;
		return false;
	}

	File File::Extract(Resource const& res, String const& dest_file_name)
	{
		File file;
		HANDLE file_handle = ::CreateFile(
			dest_file_name.c_str(),
			GENERIC_WRITE,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_TEMPORARY,
			NULL
		);

		if (file_handle == INVALID_HANDLE_VALUE)
			return file;

		LPVOID buffer;
		DWORD buffer_size;
		if (res.Load(buffer, buffer_size))
		{
			// Ð´ÈëÎÄ¼þ
			DWORD written_bytes = 0;
			::WriteFile(file_handle, buffer, buffer_size, &written_bytes, NULL);
			::CloseHandle(file_handle);

			file.Open(dest_file_name);
		}
		else
		{
			::CloseHandle(file_handle);
			::DeleteFile(dest_file_name.c_str());
		}

		return file;
	}
}