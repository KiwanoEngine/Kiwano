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
#include <kiwano/core/Resource.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

namespace kiwano
{
	namespace audio
	{
		class Sound;

		/**
		* \addtogroup Audio
		* @{
		*/

		/**
		* \~chinese
		* @brief ��Ƶ������
		*/
		class KGE_API Transcoder
		{
			friend class Sound;

		public:
			/**
			* \~chinese
			* @brief ��Ƶ���ݻ���
			*/
			struct Buffer
			{
				BYTE* data;					///< ��Ƶ����
				uint32_t size;				///< ��Ƶ���ݴ�С
				const WAVEFORMATEX* format;	///< ��Ƶ���ݸ�ʽ
			};

			Transcoder();

			~Transcoder();

			/// \~chinese
			/// @brief ��ȡ���ݻ���
			Buffer GetBuffer() const;

			/// \~chinese
			/// @brief ������ݻ���
			void ClearBuffer();

		private:
			/// \~chinese
			/// @brief ���뱾����Ƶ�ļ�
			HRESULT LoadMediaFile(String const& file_path);

			/// \~chinese
			/// @brief ������Ƶ��Դ
			HRESULT LoadMediaResource(Resource const& res);

			/// \~chinese
			/// @brief ��ȡ��ƵԴ����
			HRESULT ReadSource(IMFSourceReader* reader);

		private:
			BYTE* wave_data_;
			uint32_t wave_size_;
			WAVEFORMATEX* wave_format_;
		};

		/** @} */
	}
}
