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
#include "D2DDeviceResources.h"
#include "Geometry.h"

namespace kiwano
{
	// Í¼²ã
	class KGE_API LayerArea
	{
	public:
		LayerArea();

		bool IsValid() const;

		Size GetSize() const;

		inline Rect const& GetAreaRect() const				{ return area_; }

		inline void SetAreaRect(Rect const& area)			{ area_ = area; }

		inline float GetOpacity() const						{ return opacity_; }

		inline void SetOpacity(float opacity)				{ opacity_ = opacity; }

		inline Geometry const& GetMaskGeometry() const		{ return mask_; }

		inline void SetMaskGeometry(Geometry const& mask)	{ mask_ = mask; }

	public:
		inline ComPtr<ID2D1Layer> GetLayer() const			{ return layer_; }

		inline void SetLayer(ComPtr<ID2D1Layer> layer)		{ layer_ = layer; }

	protected:
		Rect area_;
		float opacity_;
		Geometry mask_;
		ComPtr<ID2D1Layer> layer_;
	};
}
