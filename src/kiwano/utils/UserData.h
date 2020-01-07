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
	/// @brief �û�����
	/// @details UserData��һ�����׵�����ʱ���ݿ⣬��ţ��ַ���-ֵ���ļ�ֵ�ԣ��޳־û�
	class KGE_API UserData
		: public Singleton<UserData>
	{
		friend Singleton<UserData>;

	public:
		/// \~chinese
		/// @brief �����ֵ�
		using DataMap = UnorderedMap<String, Any>;

		/// \~chinese
		/// @brief ��ֵ��
		using DataPair = Pair<const String, Any>;

		/// \~chinese
		/// @brief ȡ����
		/// @param key ��
		/// @param default_data ���ݲ�����ʱ���ص�Ĭ��ֵ
		/// @return ����Ӧ��ֵ����
		Any Get(String const& key, Any const& default_data = Any()) const;

		/// \~chinese
		/// @brief ������
		/// @param key ��
		/// @param data ֵ
		void Set(String const& key, Any const& data);

		/// \~chinese
		/// @brief ������
		/// @param pair ��ֵ��
		void Set(DataPair const& pair);

		/// \~chinese
		/// @brief ������
		/// @param list ��ֵ���б�
		void Set(std::initializer_list<DataPair> const& list);

		/// \~chinese
		/// @brief ������
		/// @param map �����ֵ�
		void Set(DataMap const& map);

		/// \~chinese
		/// @brief �ж��Ƿ��������Ӧ������
		/// @param key ��
		bool Contains(String const& key) const;

		/// \~chinese
		/// @brief ��ȡ�����ֵ�
		const DataMap& GetDataMap() const;

		/// \~chinese
		/// @brief �����������
		void Clear();

	private:
		UserData() = default;
		UserData(const UserData&) = default;
		UserData& operator=(const UserData&) = default;

	private:
		DataMap data_;
	};
}
