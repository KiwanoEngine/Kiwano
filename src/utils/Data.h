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
#include "../core/helper.hpp"

namespace easy2d
{
	// 数据管理工具
	class Data
	{
	public:
		Data(
			String const& key,					/* 键值 */
			String const& field = L"Defalut"	/* 字段名称 */
		);

		// 该数据是否存在
		bool Exists() const;

		// 保存 int 类型的值
		bool SaveInt(
			int val
		);

		// 保存 float 类型的值
		bool SaveFloat(
			float val
		);

		// 保存 double 类型的值
		bool SaveDouble(
			double val
		);

		// 保存 bool 类型的值
		bool SaveBool(
			bool val
		);

		// 保存 String 类型的值
		bool SaveString(
			String const& val
		);

		// 获取 int 类型的值
		int GetInt() const;

		// 获取 float 类型的值
		float GetFloat() const;

		// 获取 double 类型的值
		double GetDouble() const;

		// 获取 bool 类型的值
		bool GetBool() const;

		// 获取 字符串 类型的值
		String GetString();

	protected:
		String key_;
		String field_;
		String const& data_path_;
	};
}
