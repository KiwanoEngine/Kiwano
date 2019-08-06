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
	// 图片
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
			Rect const& crop_rect	/* 裁剪矩形 */
		);

		explicit Image(
			ComPtr<ID2D1Bitmap> const& bitmap
		);

		virtual ~Image();

		// 加载图片资源
		bool Load(
			Resource const& res
		);

		// 资源是否有效
		bool IsValid() const;

		// 将图片裁剪为矩形
		void Crop(
			Rect const& crop_rect	/* 裁剪矩形 */
		);

		// 获取宽度
		float GetWidth() const;

		// 获取高度
		float GetHeight() const;

		// 获取大小
		Size GetSize() const;

		// 获取源图片宽度
		float GetSourceWidth() const;

		// 获取源图片高度
		float GetSourceHeight() const;

		// 获取源图片大小
		Size GetSourceSize() const;

		// 获取裁剪位置 X 坐标
		float GetCropX() const;

		// 获取裁剪位置 Y 坐标
		float GetCropY() const;

		// 获取裁剪位置
		Point GetCropPos() const;

		// 获取裁剪矩形
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
