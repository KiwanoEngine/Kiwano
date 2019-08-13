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
	// 图像
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

		// 资源是否有效
		bool IsValid() const;

		// 获取位图宽度
		float GetWidth() const;

		// 获取位图高度
		float GetHeight() const;

		// 获取位图大小
		Size GetSize() const;

		// 获取位图像素宽度
		UINT32 GetWidthInPixels() const;

		// 获取位图像素高度
		UINT32 GetHeightInPixels() const;

		// 获取位图像素大小
		math::Vec2T<UINT32> GetSizeInPixels() const;

		// 获取源位图
		ComPtr<ID2D1Bitmap> GetBitmap() const;

		// 设置源位图
		void SetBitmap(ComPtr<ID2D1Bitmap> bitmap);

	protected:
		ComPtr<ID2D1Bitmap>	bitmap_;
	};
}
