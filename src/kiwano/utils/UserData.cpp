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

#include <kiwano/utils/UserData.h>

namespace kiwano
{
	Any UserData::Get(String const& key, Any const& default_data) const
	{
		auto iter = data_.find(key);
		if (iter != data_.end())
		{
			return iter->second;
		}
		return default_data;
	}

	void UserData::Set(String const& key, Any const& data)
	{
		data_.insert(std::make_pair(key, data));
	}

	void UserData::Set(DataPair const& pair)
	{
		data_.insert(pair);
	}

	void UserData::Set(std::initializer_list<DataPair> const& list)
	{
		for (const auto& pair : list)
		{
			data_.insert(pair);
		}
	}

	void UserData::Set(DataMap const& map)
	{
		data_ = map;
	}

	bool UserData::Contains(String const& key) const
	{
		return data_.count(key) != 0;
	}

	const UserData::DataMap& UserData::GetDataMap() const
	{
		return data_;
	}

	void UserData::Clear()
	{
		data_.clear();
	}

}
