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
#include "include-forwards.h"
#include "../base/Resource.h"
#include "../renderer/render.h"

namespace kiwano
{
	// ͼƬ
	class KGE_API Image
		: public Object
	{
	public:
		Image();

		explicit Image(
			Resource const& res
		);

		explicit Image(
			Resource const& res,
			Rect const& crop_rect	/* �ü����� */
		);

		explicit Image(
			ComPtr<ID2D1Bitmap> const& bitmap
		);

		virtual ~Image();

		// ����ͼƬ��Դ
		bool Load(
			Resource const& res
		);

		// ��Դ�Ƿ���Ч
		bool IsValid() const;

		// ��ͼƬ�ü�Ϊ����
		void Crop(
			Rect const& crop_rect	/* �ü����� */
		);

		// ��ȡ���
		float GetWidth() const;

		// ��ȡ�߶�
		float GetHeight() const;

		// ��ȡ��С
		Size GetSize() const;

		// ��ȡԴͼƬ���
		float GetSourceWidth() const;

		// ��ȡԴͼƬ�߶�
		float GetSourceHeight() const;

		// ��ȡԴͼƬ��С
		Size GetSourceSize() const;

		// ��ȡ�ü�λ�� X ����
		float GetCropX() const;

		// ��ȡ�ü�λ�� Y ����
		float GetCropY() const;

		// ��ȡ�ü�λ��
		Point GetCropPos() const;

		// ��ȡ�ü�����
		Rect GetCropRect() const;

		ComPtr<ID2D1Bitmap> const& GetBitmap() const;

	protected:
		void SetBitmap(
			ComPtr<ID2D1Bitmap> const& bitmap
		);

	protected:
		Rect crop_rect_;
		ComPtr<ID2D1Bitmap>	bitmap_;
	};
}
