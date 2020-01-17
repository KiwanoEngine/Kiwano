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
#include <kiwano/macros.h>
#include <kiwano/core/Common.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief 资源
	* @details
	*   资源是保存在 exe 中的二进制数据，
	*   例如，一份音频资源的类型为 L"WAVE"，名称标识符为 IDR_WAVE_1，那么可以这样指定该资源: 
	*   @code
	*     Resource(IDR_WAVE_1, L"WAVE");
	*   @endcode
	*   了解资源的更多信息: https://docs.microsoft.com/en-us/windows/desktop/menurc/resources
	*/
	class KGE_API Resource
	{
	public:
		/// \~chinese
		/// @brief 资源的二进制数据
		struct Data
		{
			void* buffer;	///< 资源数据
			uint32_t size;	///< 资源数据大小

			Data();

			operator bool() const;
		};

		/// \~chinese
		/// @brief 构造资源
		Resource();

		/// \~chinese
		/// @brief 构造资源
		/// @param id 资源 ID
		/// @param type 资源类型
		Resource(uint32_t id, const wchar_t* type);

		/// \~chinese
		/// @brief 获取资源的二进制数据
		/// @return 资源数据
		Resource::Data GetData() const;

		/// \~chinese
		/// @brief 获取资源 ID
		uint32_t GetId() const;

		/// \~chinese
		/// @brief 获取资源类型
		const wchar_t* GetType() const;

	private:
		uint32_t		id_;
		const wchar_t*	type_;

		mutable Resource::Data	data_;
	};

	inline Resource::Data::Data()
		: buffer(nullptr)
		, size(0)
	{
	}

	inline Resource::Data::operator bool() const
	{
		return buffer != nullptr && size;
	}

	inline uint32_t Resource::GetId() const
	{
		return id_;
	}

	inline const wchar_t* Resource::GetType() const
	{
		return type_;
	}
}
