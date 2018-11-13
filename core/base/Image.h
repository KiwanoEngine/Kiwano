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
#include "base.h"
#include "Resource.h"

namespace easy2d
{
	// 图片
	class Image
		: public RefCounter
	{
		E2D_DISABLE_COPY(Image);

	public:
		Image();

		explicit Image(
			Resource& res
		);

		explicit Image(
			Resource& res,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		explicit Image(
			const String& file_name
		);

		explicit Image(
			const String& file_name,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		virtual ~Image();

		// 加载图片资源
		bool Load(
			Resource& res
		);

		// 加载图片资源
		bool Load(
			const String& file_name
		);

		// 将图片裁剪为矩形
		void Crop(
			const Rect& crop_rect	/* 裁剪矩形 */
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
		const Rect& GetCropRect() const;

		// 获取 ID2D1Bitmap 对象
		ID2D1Bitmap * GetBitmap() const;

	private:
		// 设置 Bitmap
		void SetBitmap(
			ID2D1Bitmap * bitmap
		);

	private:
		Rect			crop_rect_;
		ID2D1Bitmap*	bitmap_;
	};
}
