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
#include <kiwano/core/common.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief DLL��
	*/
	class KGE_API Library
	{
	public:
		/// \~chinese
		/// @brief ����DLL��
		Library();

		/// \~chinese
		/// @brief ����DLL��
		/// @param lib DLL�ļ�·��
		Library(String const& lib);

		virtual ~Library();

		/// \~chinese
		/// @brief ����DLL
		/// @param lib DLL�ļ�·��
		bool Load(String const& lib);

		/// \~chinese
		/// @brief ����DLL
		/// @param lib DLL�ļ�·��
		bool Load(ByteString const& lib);

		/// \~chinese
		/// @brief �Ƿ���Ч
		bool IsValid() const;

		/// \~chinese
		/// @brief �ͷ�DLL
		void Free();

		/// \~chinese
		/// @brief ����ָ����DLL�е�����⺯����ַ
		/// @param proc_name ������
		FARPROC GetProcess(ByteString const& proc_name);

		/// \~chinese
		/// @brief ����ָ����DLL�е�����⺯����ַ
		/// @param proc_name ������
		template <typename _Proc>
		inline _Proc GetProcess(ByteString const& proc_name)
		{
			return reinterpret_cast<_Proc>(GetProcess(proc_name));
		}

	private:
		HMODULE instance_;
	};

}
