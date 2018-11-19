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

#include "GeometryNode.h"
#include "render.h"

namespace easy2d
{
	GeometryNode::GeometryNode()
		: fill_color_(Color::White)
		, stroke_color_(Color(Color::Black, 0))
		, stroke_width_(1.f)
		, outline_join_(StrokeStyle::Miter)
	{
	}

	GeometryNode::GeometryNode(spGeometry const& geometry)
		: GeometryNode()
	{
		SetGeometry(geometry);
	}

	GeometryNode::~GeometryNode()
	{
	}

	void GeometryNode::SetGeometry(spGeometry const& geometry)
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

	void GeometryNode::OnDraw()
	{
		if (geometry_)
		{
			auto graphics = devices::Graphics::Instance();
			graphics->SetTransform(geometry_->GetTransformMatrix() * GetTransformMatrix());

			graphics->FillGeometry(
				geometry_->GetD2DGeometry(),
				fill_color_
			);

			graphics->DrawGeometry(
				geometry_->GetD2DGeometry(),
				stroke_color_,
				stroke_width_,
				outline_join_
			);
		}
	}

	void GeometryNode::DrawBorder()
	{
		if (visible_)
		{
			DrawChildrenBorder();
		}
	}

}
