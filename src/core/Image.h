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
#include "include-forwards.h"
#include "Resource.h"

namespace easy2d
{
	// ͼƬ
	class Image
		: public virtual Object
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
			D2DBitmapPtr const& bitmap
		);

		virtual ~Image();

		// ����ͼƬ��Դ
		bool Load(
			Resource const& res
		);

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
		Rect const& GetCropRect() const;

		D2DBitmapPtr const& GetBitmap() const;

	protected:
		void SetBitmap(
			D2DBitmapPtr const& bitmap
		);

	protected:
		Rect		crop_rect_;
		D2DBitmapPtr	bitmap_;
	};
}
