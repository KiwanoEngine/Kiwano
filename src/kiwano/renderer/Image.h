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
#include "../base/Resource.h"
#include "render.h"  // ID2D1Bitmap

namespace kiwano
{
	// ͼ��
	KGE_DECLARE_SMART_PTR(Image);
	class KGE_API Image
		: public Object
	{
	public:
		Image();

		explicit Image(
			ComPtr<ID2D1Bitmap> const& bitmap
		);

		virtual ~Image();

		// ��Դ�Ƿ���Ч
		bool IsValid() const;

		// ��ȡλͼ���
		float GetWidth() const;

		// ��ȡλͼ�߶�
		float GetHeight() const;

		// ��ȡλͼ��С
		Size GetSize() const;

		// ��ȡλͼ���ؿ��
		UINT32 GetWidthInPixels() const;

		// ��ȡλͼ���ظ߶�
		UINT32 GetHeightInPixels() const;

		// ��ȡλͼ���ش�С
		math::Vec2T<UINT32> GetSizeInPixels() const;

		// ��ȡԴλͼ
		ComPtr<ID2D1Bitmap> GetBitmap() const;

		// ����Դλͼ
		void SetBitmap(ComPtr<ID2D1Bitmap> bitmap);

	protected:
		ComPtr<ID2D1Bitmap>	bitmap_;
	};
}
