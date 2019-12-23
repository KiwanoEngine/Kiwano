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
	// UserData is a simple database for user
	class KGE_API UserData
		: public Singleton<UserData>
	{
		friend Singleton<UserData>;

	public:
		using DataMap = UnorderedMap<String, Any>;

		Any Get(String const& key, Any const& default_data = Any()) const;

		void Set(String const& key, Any const& data);

		void Set(Pair<String, Any> const& pair);

		void Set(std::initializer_list<Pair<String, Any>> const& list);

		void Set(DataMap const& map);

		bool Contains(String const& key) const;

		const DataMap& GetData() const;

		void Clear();

	private:
		UserData() = default;
		UserData(const UserData&) = default;
		UserData& operator=(const UserData&) = default;

	private:
		DataMap data_;
	};
}
