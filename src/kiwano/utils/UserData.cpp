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
Any UserData::Get(const String& key, const Any& default_data) const
{
    auto iter = data_.find(key);
    if (iter != data_.end())
    {
        return iter->second;
    }
    return default_data;
}

void UserData::Set(const String& key, const Any& data)
{
    data_[key] = data;
}

void UserData::Set(const DataPair& pair)
{
    data_[pair.first] = pair.second;
}

void UserData::Set(const std::initializer_list<DataPair>& list)
{
    for (const auto& pair : list)
    {
        data_[pair.first] = pair.second;
    }
}

void UserData::Set(const DataMap& map)
{
    data_ = map;
}

bool UserData::Contains(const String& key) const
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

}  // namespace kiwano
