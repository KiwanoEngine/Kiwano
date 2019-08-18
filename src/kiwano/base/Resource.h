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

namespace kiwano
{
	// ��Դ
	// 
	// ��Դ�Ǳ����� exe �еĶ���������
	// ����, һ����Ƶ��Դ������Ϊ L"WAVE", ���Ʊ�ʶ��Ϊ IDR_WAVE_1,
	// ��ô��������ָ������Դ: Resource(IDR_WAVE_1, L"WAVE");
	// 
	// �˽���Դ�ĸ�����Ϣ: https://docs.microsoft.com/en-us/windows/desktop/menurc/resources
	//
	class KGE_API Resource
	{
	public:
		// ����������
		struct Data
		{
			void* buffer;
			UINT32 size;

			inline Data() : buffer(nullptr), size(0) {}

			inline operator bool() const { return buffer && size; }
		};

		Resource();

		Resource(
			UINT id,		/* ��Դ���� */
			LPCWSTR type	/* ��Դ���� */
		);

		// ��ȡ����������
		Resource::Data GetData() const;

		inline UINT GetId() const		{ return id_; }

		inline LPCWSTR GetType() const	{ return type_; }

	private:
		UINT	id_;
		LPCWSTR	type_;
		mutable Resource::Data	data_;
	};
}
