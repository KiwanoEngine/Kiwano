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
#include "Geometry.h"

namespace kiwano
{
	// 图层
	class KGE_API LayerArea
	{
	public:
		LayerArea();

		bool IsValid() const;

		Size GetSize() const;

		inline Rect const& GetAreaRect() const					{ return area_; }

		inline Float32 GetOpacity() const						{ return opacity_; }

		inline Geometry const& GetMaskGeometry() const			{ return mask_; }

		inline Matrix3x2 const& GetMaskTransform() const		{ return mask_transform_; }

		// 设置图层区域
		inline void SetAreaRect(Rect const& area)				{ area_ = area; }

		// 设置图层透明度
		inline void SetOpacity(Float32 opacity)					{ opacity_ = opacity; }

		// 设置几何蒙层
		inline void SetMaskGeometry(Geometry const& mask)		{ mask_ = mask; }

		// 设置几何蒙层变换
		inline void SetMaskTransform(Matrix3x2 const& matrix)	{ mask_transform_ = matrix; }

	public:
		inline ComPtr<ID2D1Layer> GetLayer() const				{ return layer_; }

		inline void SetLayer(ComPtr<ID2D1Layer> layer)			{ layer_ = layer; }

	protected:
		Rect area_;
		Float32 opacity_;
		Geometry mask_;
		Matrix3x2 mask_transform_;
		ComPtr<ID2D1Layer> layer_;
	};
}
