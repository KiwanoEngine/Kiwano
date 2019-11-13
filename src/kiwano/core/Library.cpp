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

#include <kiwano/core/Library.h>

namespace kiwano
{

	Library::Library()
		: instance_(nullptr)
	{
	}

	Library::Library(String const& lib)
		: instance_(nullptr)
	{
		Load(lib);
	}

	Library::~Library()
	{
		Free();
	}

	bool Library::Load(String const& lib)
	{
		instance_ = ::LoadLibraryW(lib.c_str());
		return IsValid();
	}

	bool Library::IsValid() const
	{
		return instance_ != nullptr;
	}

	void Library::Free()
	{
		if (instance_)
		{
			::FreeLibrary(instance_);
			instance_ = nullptr;
		}
	}

	FARPROC Library::GetProcess(String const& proc_name)
	{
		KGE_ASSERT(instance_ != nullptr);
		return GetProcAddress(instance_, wide_to_string(proc_name).c_str());
	}

}
