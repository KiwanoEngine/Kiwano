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
#include "win32/D2DDeviceResources.h"

namespace kiwano
{
	// 插值模式
	// 插值模式指定了位图在缩放和旋转时像素颜色的计算方式
	// Linear (双线性插值): 对周围四个像素进行两次线性插值计算, 在图像放大时可能会模糊（默认）
	// Nearest (最邻近插值): 取最邻近的像素点的颜色值
	enum class InterpolationMode
	{
		Linear,		// 双线性插值
		Nearest,	// 最邻近插值
	};


	// 纹理
	class KGE_API Texture
	{
	public:
		Texture();

		explicit Texture(
			String const& file_path
		);

		explicit Texture(
			Resource const& res
		);

		explicit Texture(
			ComPtr<ID2D1Bitmap> const& bitmap
		);

		virtual ~Texture();

		// 加载本地文件
		bool Load(
			String const& file_path
		);

		// 加载资源
		bool Load(
			Resource const& res
		);

		// 资源是否有效
		bool IsValid() const;

		// 获取宽度
		float GetWidth() const;

		// 获取高度
		float GetHeight() const;

		// 获取大小
		Size GetSize() const;

		// 获取像素宽度
		std::uint32_t GetWidthInPixels() const;

		// 获取像素高度
		std::uint32_t GetHeightInPixels() const;

		// 获取像素大小
		math::Vec2T<std::uint32_t> GetSizeInPixels() const;

		// 获取像素插值方式
		InterpolationMode GetBitmapInterpolationMode() const;

		// 拷贝位图内存
		void CopyFrom(Texture const& copy_from);

		// 拷贝位图内存
		void CopyFrom(Texture const& copy_from, Rect const& src_rect, Point const& dest_point);

		// 设置像素插值方式
		void SetInterpolationMode(InterpolationMode mode);

		// 设置默认的像素插值方式
		static void SetDefaultInterpolationMode(InterpolationMode mode);

	public:
		// 获取源位图
		ComPtr<ID2D1Bitmap> GetBitmap() const;

		// 设置源位图
		void SetBitmap(ComPtr<ID2D1Bitmap> bitmap);

		// 获取像素格式
		D2D1_PIXEL_FORMAT GetPixelFormat() const;

	protected:
		ComPtr<ID2D1Bitmap>	bitmap_;
		InterpolationMode	interpolation_mode_;

		static InterpolationMode default_interpolation_mode_;
	};
}
