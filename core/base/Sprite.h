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
#include "Node.h"
#include "Image.h"

namespace easy2d
{
	// 精灵
	class Sprite
		: public Node
	{
	public:
		Sprite();

		explicit Sprite(
			Image * image
		);

		explicit Sprite(
			Resource& res
		);

		explicit Sprite(
			Resource& res,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		explicit Sprite(
			const String& file_name
		);

		explicit Sprite(
			const String& file_name,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		virtual ~Sprite();

		// 加载图片文件
		bool Load(
			Resource& res
		);

		// 加载图片文件
		bool Load(
			const String& file_name
		);

		// 加载图片
		bool Load(
			Image * image
		);

		// 将图片裁剪为矩形
		void Crop(
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		// 获取 Image 对象
		Image * GetImage() const;

		// 渲染精灵
		virtual void OnDraw() const override;

	private:
		E2D_DISABLE_COPY(Sprite);

	private:
		Image* image_;
	};
}
