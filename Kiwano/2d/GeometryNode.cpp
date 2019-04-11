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

#include "GeometryNode.h"
#include "../renderer/render.h"

namespace kiwano
{
	GeometryNode::GeometryNode()
		: fill_color_(Color::White)
		, stroke_color_(Color(Color::Black, 0))
		, stroke_width_(1.f)
		, outline_join_(StrokeStyle::Miter)
	{
	}

	GeometryNode::GeometryNode(GeometryPtr const& geometry)
		: GeometryNode()
	{
		SetGeometry(geometry);
	}

	GeometryNode::~GeometryNode()
	{
	}

	void GeometryNode::SetGeometry(GeometryPtr const& geometry)
	{
		geometry_ = geometry;
	}

	void GeometryNode::SetFillColor(const Color & color)
	{
		fill_color_ = color;
	}

	void GeometryNode::SetStrokeColor(const Color & color)
	{
		stroke_color_ = color;
	}

	void GeometryNode::SetStrokeWidth(float width)
	{
		stroke_width_ = std::max(width, 0.f);
	}

	void GeometryNode::SetOutlineJoinStyle(StrokeStyle outline_join)
	{
		outline_join_ = outline_join;
	}

	void GeometryNode::OnRender()
	{
		if (geometry_ && geometry_->geo_)
		{
			Renderer::Instance().FillGeometry(
				geometry_->geo_.Get(),
				fill_color_
			);

			Renderer::Instance().DrawGeometry(
				geometry_->geo_,
				stroke_color_,
				stroke_width_,
				outline_join_
			);
		}
	}

}
