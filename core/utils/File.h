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
#include "../base/Resource.h"
#include <string>

namespace easy2d
{
    // �ļ�
	class File
	{
	public:
		File();

		File(
			std::wstring const& file_name
		);

		virtual ~File();

		// ���ļ�
		bool Open(
			std::wstring const& file_name
		);

		// �ļ��Ƿ����
		bool Exists() const;

		// ɾ���ļ�
		bool Delete();

		// ��ȡ�ļ�·��
		std::wstring const& GetPath() const;

		// ��ȡ�ļ���չ��
		std::wstring GetExtension() const;

		// �ͷ���Դ����ʱ�ļ�Ŀ¼
		static File Extract(
			Resource& res,						/* ��Դ */
			std::wstring const& dest_file_name	/* Ŀ���ļ��� */
		);

		// ����ļ�����·��
		static void AddSearchPath(
			std::wstring const& path
		);

	protected:
		std::wstring file_path_;

		static std::list<std::wstring> search_paths_;
	};
}
