// Copyright (c) 2018-2019 Kiwano - Nomango
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
#include <kiwano/core/common.h>

namespace kiwano
{
	/// \~chinese
	/// @brief 用户数据
	/// @details UserData是一个简易的运行时数据库，存放（字符串-值）的键值对，无持久化
	class KGE_API UserData
		: public Singleton<UserData>
	{
		friend Singleton<UserData>;

	public:
		/// \~chinese
		/// @brief 数据字典
		using DataMap = UnorderedMap<String, Any>;

		/// \~chinese
		/// @brief 键值对
		using DataPair = Pair<const String, Any>;

		/// \~chinese
		/// @brief 取数据
		/// @param key 键
		/// @param default_data 数据不存在时返回的默认值
		/// @return 键对应的值数据
		Any Get(String const& key, Any const& default_data = Any()) const;

		/// \~chinese
		/// @brief 存数据
		/// @param key 键
		/// @param data 值
		void Set(String const& key, Any const& data);

		/// \~chinese
		/// @brief 存数据
		/// @param pair 键值对
		void Set(DataPair const& pair);

		/// \~chinese
		/// @brief 存数据
		/// @param list 键值对列表
		void Set(std::initializer_list<DataPair> const& list);

		/// \~chinese
		/// @brief 存数据
		/// @param map 数据字典
		void Set(DataMap const& map);

		/// \~chinese
		/// @brief 判断是否包含键对应的数据
		/// @param key 键
		bool Contains(String const& key) const;

		/// \~chinese
		/// @brief 获取数据字典
		const DataMap& GetDataMap() const;

		/// \~chinese
		/// @brief 清空所有数据
		void Clear();

	private:
		UserData() = default;
		UserData(const UserData&) = default;
		UserData& operator=(const UserData&) = default;

	private:
		DataMap data_;
	};
}
