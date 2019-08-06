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

#pragma once
#include "helper.hpp"

namespace easy2d
{
	// ��Դ
	// 
	// ��Դ�������ļ����ͣ�Ҳ�����Ǳ����� exe �еĶ�������Դ
	// ����, һ����Ƶ��Դ������Ϊ L"WAVE", ���Ʊ�ʶ��Ϊ IDR_WAVE_1,
	// ��ô��������ָ������Դ: Resource res(MAKEINTRESOURCE(IDR_WAVE_1), L"WAVE");
	// 
	// �˽���Դ�ĸ�����Ϣ: https://docs.microsoft.com/en-us/windows/desktop/menurc/resources
	//
	class Resource
	{
		enum class Type { File, Binary };

	public:
		Resource(
			LPCWSTR file_name		/* �ļ�·�� */
		);

		Resource(
			String const& file_name	/* �ļ�·�� */
		);

		Resource(
			LPCWSTR name,			/* ��Դ���� */
			LPCWSTR type			/* ��Դ���� */
		);

		Resource(
			Resource const& rhs
		);

		Resource(
			Resource && rhs
		);

		virtual ~Resource();

		inline bool IsFileType() const { return type_ == Type::File; }

		inline String GetFileName() const { if (file_name_) return *file_name_; return String(); }

		bool Load(
			LPVOID& buffer,
			DWORD& buffer_size
		) const;

		size_t GetHashCode() const;

		Resource& operator= (Resource const& rhs);

		Resource& operator= (Resource && rhs);

	private:
		Type type_;
		union
		{
			struct
			{
				String*	file_name_;
			};

			struct
			{
				LPCWSTR	bin_name_;
				LPCWSTR	bin_type_;
			};
		};
	};
}
