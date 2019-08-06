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
#include "macros.h"

namespace easy2d
{
	// ��Դ
	// 
	// Usage:
	//     Resource ���ڻ�ȡ��ִ���ļ� (exe) �е���Դ, �����ڹ��캯����ָ������
	//     ��Դ���ͺ����Ʊ�ʶ����
	//     ����, һ����Ƶ��Դ������Ϊ L"WAVE", ���Ʊ�ʶ��Ϊ IDR_WAVE_1, ��ô��������ָ������Դ:
	//     Resource res(MAKEINTRESOURCE(IDR_WAVE_1), L"WAVE");
	//     �����Ҫ�ֶ����������Դ, ����ͨ�� Load ������ȡ��Դ����
	//     if (res.Load()) {
	//         LPVOID data = res.GetData();
	//         DWORD size = res.GetDataSize();
	//     }
	//     �˽���Դ�ĸ�����Ϣ: https://docs.microsoft.com/en-us/windows/desktop/menurc/resources
	//
	class Resource
	{
	public:
		Resource(
			LPCWSTR name,	/* ��Դ���� */
			LPCWSTR type	/* ��Դ���� */
		);

		bool Load();

		LPCWSTR	GetName() const;

		LPCWSTR	GetType() const;

		LPVOID GetData() const;

		DWORD GetDataSize() const;

		size_t GetHashCode() const;

	private:
		bool	loaded_;
		LPCWSTR	name_;
		LPCWSTR	type_;
		LPVOID	data_;
		DWORD	data_size_;
	};
}
