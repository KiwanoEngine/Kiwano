// Copyright (c) 2016-2019 Kiwano - Nomango
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
#include <kiwano/renderer/Geometry.h>

namespace kiwano
{
	class RenderTarget;

	/**
	* \addtogroup Render
	* @{
	*/

	/**
	* \~chinese
	* @brief 图层区域
	*/
	class KGE_API LayerArea
	{
		friend class RenderTarget;

	public:
		LayerArea();

		/// \~chinese
		/// @brief 是否有效
		bool IsValid() const;

		/// \~chinese
		/// @brief 获取图层区域
		Rect const& GetAreaRect() const;

		/// \~chinese
		/// @brief 获取图层透明度
		float GetOpacity() const;

		/// \~chinese
		/// @brief 获取几何蒙层
		Geometry const& GetMaskGeometry() const;

		/// \~chinese
		/// @brief 获取几何蒙层变换
		Matrix3x2 const& GetMaskTransform() const;

		/// \~chinese
		/// @brief 设置图层区域
		void SetAreaRect(Rect const& area);

		/// \~chinese
		/// @brief 设置图层透明度
		void SetOpacity(float opacity);

		/// \~chinese
		/// @brief 设置几何蒙层
		void SetMaskGeometry(Geometry const& mask);

		/// \~chinese
		/// @brief 设置几何蒙层变换
		void SetMaskTransform(Matrix3x2 const& matrix);

	private:
		ComPtr<ID2D1Layer> GetLayer() const;

		void SetLayer(ComPtr<ID2D1Layer> layer);

	private:
		Rect area_;
		float opacity_;
		Geometry mask_;
		Matrix3x2 mask_transform_;
		ComPtr<ID2D1Layer> layer_;
	};

	/** @} */

	inline bool					LayerArea::IsValid() const								{ return layer_ != nullptr; }

	inline Rect const&			LayerArea::GetAreaRect() const							{ return area_; }

	inline float				LayerArea::GetOpacity() const							{ return opacity_; }

	inline Geometry const&		LayerArea::GetMaskGeometry() const						{ return mask_; }

	inline Matrix3x2 const&		LayerArea::GetMaskTransform() const						{ return mask_transform_; }

	inline void					LayerArea::SetAreaRect(Rect const& area)				{ area_ = area; }

	inline void					LayerArea::SetOpacity(float opacity)					{ opacity_ = opacity; }

	inline void					LayerArea::SetMaskGeometry(Geometry const& mask)		{ mask_ = mask; }

	inline void					LayerArea::SetMaskTransform(Matrix3x2 const& matrix)	{ mask_transform_ = matrix; }

	inline ComPtr<ID2D1Layer>	LayerArea::GetLayer() const								{ return layer_; }

	inline void					LayerArea::SetLayer(ComPtr<ID2D1Layer> layer)			{ layer_ = layer; }
}
