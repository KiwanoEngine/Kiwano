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

#include "FileUtil.h"
#include "../platform/modules.h"

namespace kiwano
{

	bool FileUtil::Delete(String const& file_path)
	{
		if (::DeleteFile(file_path.c_str()))
			return true;
		return false;
	}

	bool FileUtil::Extract(Resource const& res, String const& dest_file_name)
	{
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
			return false;

		LPVOID buffer;
		DWORD buffer_size;
		if (res.Load(buffer, buffer_size))
		{
			DWORD written_bytes = 0;
			::WriteFile(file_handle, buffer, buffer_size, &written_bytes, NULL);
			::CloseHandle(file_handle);

			return true;
		}
		else
		{
			::CloseHandle(file_handle);
			::DeleteFile(dest_file_name.c_str());
		}
		return false;
	}

	bool FileUtil::ExistsFile(String const& file_path)
	{
		DWORD dwAttrib = ::GetFileAttributesW(file_path.c_str());

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool FileUtil::ExistsDirectory(String const& dir_path)
	{
		DWORD dwAttrib = ::GetFileAttributesW(dir_path.c_str());

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}
}