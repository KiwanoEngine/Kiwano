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
#include <kiwano/renderer/win32/D2DDeviceResources.h>
#include <kiwano/core/ObjectBase.h>

namespace kiwano
{
	class RenderTarget;
	class TextureRenderTarget;
	class Renderer;

	/**
	* \~chinese
	* @brief 插值模式
	* @details 插值模式指定了位图在缩放和旋转时像素颜色的计算方式
	*/
	enum class InterpolationMode
	{
		Linear,		///< 双线性插值，对周围四个像素进行两次线性插值计算, 在图像放大时可能会模糊
		Nearest,	///< 最邻近插值，取最邻近的像素点的颜色值
	};

	KGE_DECLARE_SMART_PTR(Texture);

	/**
	* \~chinese
	* @brief 纹理
	*/
	class KGE_API Texture
		: public ObjectBase
	{
		friend class RenderTarget;
		friend class TextureRenderTarget;
		friend class Renderer;

	public:
		Texture();

		virtual ~Texture();

		/// \~chinese
		/// @brief 加载本地文件
		bool Load(String const& file_path);

		/// \~chinese
		/// @brief 加载资源
		bool Load(Resource const& res);

		/// \~chinese
		/// @brief 是否有效
		bool IsValid() const;

		/// \~chinese
		/// @brief 获取纹理宽度
		float GetWidth() const;

		/// \~chinese
		/// @brief 获取纹理高度
		float GetHeight() const;

		/// \~chinese
		/// @brief 获取纹理大小
		Size GetSize() const;

		/// \~chinese
		/// @brief 获取像素宽度
		uint32_t GetWidthInPixels() const;

		/// \~chinese
		/// @brief 获取像素高度
		uint32_t GetHeightInPixels() const;

		/// \~chinese
		/// @brief 获取像素大小
		math::Vec2T<uint32_t> GetSizeInPixels() const;

		/// \~chinese
		/// @brief 获取像素插值方式
		InterpolationMode GetBitmapInterpolationMode() const;

		/// \~chinese
		/// @brief 获取像素格式
		D2D1_PIXEL_FORMAT GetPixelFormat() const;

		/// \~chinese
		/// @brief 拷贝纹理
		/// @param copy_from 源纹理
		void CopyFrom(TexturePtr copy_from);

		/// \~chinese
		/// @brief 拷贝纹理
		/// @param copy_from 源纹理
		/// @param src_rect 源纹理裁剪矩形
		/// @param dest_point 拷贝至目标位置
		void CopyFrom(TexturePtr copy_from, Rect const& src_rect, Point const& dest_point);

		/// \~chinese
		/// @brief 设置像素插值方式
		void SetInterpolationMode(InterpolationMode mode);

		/// \~chinese
		/// @brief 设置默认的像素插值方式
		static void SetDefaultInterpolationMode(InterpolationMode mode);

		/// \~chinese
		/// @brief 获取默认的像素插值方式
		static InterpolationMode GetDefaultInterpolationMode();

	private:
		/// \~chinese
		/// @brief 获取源位图
		ComPtr<ID2D1Bitmap> GetBitmap() const;

		/// \~chinese
		/// @brief 设置源位图
		void SetBitmap(ComPtr<ID2D1Bitmap> bitmap);

	private:
		ComPtr<ID2D1Bitmap>	bitmap_;
		InterpolationMode	interpolation_mode_;

		static InterpolationMode default_interpolation_mode_;
	};
}
