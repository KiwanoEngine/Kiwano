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
	//
	// ���ݴ�ȡ���� (.ini ��ʽ)
	// һ�� DataUtil �����ʾһ�����ݴ���ʵ��, ���ڴ�ȡ�򵥸�ʽ (bool | int | float | double | String) ������
	// ���ݶ����� key-value (��-ֵ) �ķ�ʽ��ȡ
	// ����, ����һ����Ϸ��߷�, �Ա��´ν�����Ϸʱ��ȡ:
	// DataUtil data;                          // �������ݶ���
	// data.SaveInt(L"best score", 20);        // ������߷� 20
	// int best = data.GetInt(L"best score");  // ��ȡ֮ǰ�������߷�
	//

	class KGE_API DataUtil
	{
	public:
		DataUtil(
			String const& file_path = L"./data.ini",	// �ļ�·��
			String const& field = L"defalut"			// �ֶ���
		);

		// ��ȡ���ݴ��·��
		inline String const& GetFilePath() const { return file_path_; }

		// �������ݴ��·��
		void SetFilePath(
			String const& file_path
		);

		// ��ȡ�ֶ���
		inline String const& GetFieldName() const { return field_name_; }

		// �����ֶ���
		void SetFieldName(
			String const& field
		);

		// �ж������Ƿ����
		bool Exists(
			String const& key
		) const;

		// ���� int ���͵�ֵ
		bool SaveInt(
			String const& key,
			int val
		) const;

		// ���� float ���͵�ֵ
		bool SaveFloat(
			String const& key,
			float val
		) const;

		// ���� double ���͵�ֵ
		bool SaveDouble(
			String const& key,
			double val
		) const;

		// ���� bool ���͵�ֵ
		bool SaveBool(
			String const& key,
			bool val
		) const;

		// ���� String ���͵�ֵ
		bool SaveString(
			String const& key,
			String const& val
		) const;

		// ��ȡ int ���͵�ֵ
		int GetInt(
			String const& key,
			int default_value = 0
		) const;

		// ��ȡ float ���͵�ֵ
		float GetFloat(
			String const& key,
			float default_value = 0.0f
		) const;

		// ��ȡ double ���͵�ֵ
		double GetDouble(
			String const& key,
			double default_value = 0.0
		) const;

		// ��ȡ bool ���͵�ֵ
		bool GetBool(
			String const& key,
			bool default_value = false
		) const;

		// ��ȡ �ַ��� ���͵�ֵ
		String GetString(
			String const& key,
			String const& default_value = L""
		) const;

	protected:
		String file_path_;
		String field_name_;
	};
}
