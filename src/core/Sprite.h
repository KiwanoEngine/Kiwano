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
			ImagePtr const& image
		);

		explicit Sprite(
			Resource const& res
		);

		explicit Sprite(
			Resource const& res,
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		virtual ~Sprite();

		// 加载图片文件
		bool Load(
			Resource const& res
		);

		// 加载图片
		bool Load(
			ImagePtr const& image
		);

		// 将图片裁剪为矩形
		void Crop(
			const Rect& crop_rect	/* 裁剪矩形 */
		);

		// 获取 Image 对象
		ImagePtr const& GetImage() const;

		// 渲染精灵
		virtual void OnRender() override;

	protected:
		ImagePtr image_;
	};
}
