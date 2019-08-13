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
	// 数据存取工具 (.ini 格式)
	// 一个 DataUtil 对象表示一个数据储存实体, 用于存取简单格式 (bool | int | float | double | String) 的数据
	// 数据都采用 key-value (键-值) 的方式存取
	// 例如, 保存一份游戏最高分, 以便下次进行游戏时读取:
	// DataUtil data;                          // 创建数据对象
	// data.SaveInt(L"best score", 20);        // 保存最高分 20
	// int best = data.GetInt(L"best score");  // 读取之前储存的最高分
	//

	class KGE_API DataUtil
	{
	public:
		DataUtil(
			String const& file_path = L"./data.ini",	// 文件路径
			String const& field = L"defalut"			// 字段名
		);

		// 获取数据存放路径
		inline String const& GetFilePath() const { return file_path_; }

		// 设置数据存放路径
		void SetFilePath(
			String const& file_path
		);

		// 获取字段名
		inline String const& GetFieldName() const { return field_name_; }

		// 设置字段名
		void SetFieldName(
			String const& field
		);

		// 判断数据是否存在
		bool Exists(
			String const& key
		) const;

		// 保存 int 类型的值
		bool SaveInt(
			String const& key,
			int val
		) const;

		// 保存 float 类型的值
		bool SaveFloat(
			String const& key,
			float val
		) const;

		// 保存 double 类型的值
		bool SaveDouble(
			String const& key,
			double val
		) const;

		// 保存 bool 类型的值
		bool SaveBool(
			String const& key,
			bool val
		) const;

		// 保存 String 类型的值
		bool SaveString(
			String const& key,
			String const& val
		) const;

		// 获取 int 类型的值
		int GetInt(
			String const& key,
			int default_value = 0
		) const;

		// 获取 float 类型的值
		float GetFloat(
			String const& key,
			float default_value = 0.0f
		) const;

		// 获取 double 类型的值
		double GetDouble(
			String const& key,
			double default_value = 0.0
		) const;

		// 获取 bool 类型的值
		bool GetBool(
			String const& key,
			bool default_value = false
		) const;

		// 获取 字符串 类型的值
		String GetString(
			String const& key,
			String const& default_value = L""
		) const;

	protected:
		String file_path_;
		String field_name_;
	};
}
