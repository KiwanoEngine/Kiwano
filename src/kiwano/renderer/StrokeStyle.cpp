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

#include <kiwano/renderer/StrokeStyle.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{

	StrokeStyle::StrokeStyle()
	{
	}

	StrokeStyle StrokeStyle::Create(CapStyle cap, LineJoinStyle line_join, DashStyle dash, float dash_offset)
	{
		switch (dash)
		{
		case DashStyle::Solid:
		{
			return StrokeStyle::Create(cap, line_join, nullptr, 0, dash_offset);
		}

		case DashStyle::Dash:
		{
			float dash_array[] = { 2, 2 };
			return StrokeStyle::Create(cap, line_join, dash_array, dash_offset);
		}

		case DashStyle::Dot:
		{
			float dash_array[] = { 0, 2 };
			return StrokeStyle::Create(cap, line_join, dash_array, dash_offset);
		}

		case DashStyle::DashDot:
		{
			float dash_array[] = { 2, 2, 0, 2 };
			return StrokeStyle::Create(cap, line_join, dash_array, dash_offset);
		}

		case DashStyle::DashDotDot:
		{
			float dash_array[] = { 2, 2, 0, 2, 0, 2 };
			return StrokeStyle::Create(cap, line_join, dash_array, dash_offset);
		}

		default:
			break;
		}
		return StrokeStyle();
	}

	StrokeStyle StrokeStyle::Create(CapStyle cap, LineJoinStyle line_join, const float* dash_array, size_t dash_size, float dash_offset)
	{
		StrokeStyle stroke_style;
		Renderer::Instance().CreateStrokeStyle(stroke_style, cap, line_join, dash_array, dash_size, dash_offset);
		return stroke_style;
	}

}
