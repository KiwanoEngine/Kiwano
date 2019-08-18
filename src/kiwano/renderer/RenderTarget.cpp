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

#include "RenderTarget.h"
#include "../base/Logger.h"

namespace kiwano
{
	//
	// RenderTarget
	//

	RenderTarget::RenderTarget()
		: opacity_(1.f)
		, collecting_status_(false)
		, antialias_(true)
		, text_antialias_(TextAntialias::GrayScale)
	{
		status_.primitives = 0;
	}

	HRESULT RenderTarget::InitDeviceResources(ComPtr<ID2D1RenderTarget> rt, ComPtr<ID2DDeviceResources> dev_res)
	{
		HRESULT hr = E_FAIL;

		if (rt && dev_res)
		{
			render_target_ = rt;
			d2d_res_ = dev_res;
			hr = S_OK;
		}

		if (SUCCEEDED(hr))
		{
			text_renderer_.reset();
			hr = ITextRenderer::Create(
				&text_renderer_,
				render_target_.get()
			);
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_.reset();
			hr = render_target_->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				D2D1::BrushProperties(),
				&solid_color_brush_
			);
		}

		return hr;
	}

	bool RenderTarget::IsValid() const
	{
		return render_target_ && d2d_res_;
	}

	void RenderTarget::BeginDraw()
	{
		HRESULT hr = E_FAIL;

		if (collecting_status_)
		{
			status_.start = Time::Now();
			status_.primitives = 0;
		}

		if (render_target_)
		{
			render_target_->BeginDraw();
			hr = S_OK;
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::EndDraw()
	{
		ThrowIfFailed(render_target_->EndDraw());

		if (collecting_status_)
		{
			status_.duration = Time::Now() - status_.start;
		}
	}

	void RenderTarget::DrawGeometry(
		Geometry const& geometry,
		Color const& stroke_color,
		Float32 stroke_width,
		StrokeStyle stroke
	) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && geometry.GetGeometry())
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(stroke_color));

			render_target_->DrawGeometry(
				geometry.GetGeometry().get(),
				solid_color_brush_.get(),
				stroke_width,
				d2d_res_->GetStrokeStyle(stroke)
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::FillGeometry(Geometry const& geometry, Color const& fill_color) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && geometry.GetGeometry())
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(fill_color));
			render_target_->FillGeometry(
				geometry.GetGeometry().get(),
				solid_color_brush_.get()
			);
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawLine(Point const& point1, Point const& point2, Color const& stroke_color, Float32 stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(stroke_color));

			render_target_->DrawLine(
				DX::ConvertToPoint2F(point1),
				DX::ConvertToPoint2F(point2),
				solid_color_brush_.get(),
				stroke_width,
				d2d_res_->GetStrokeStyle(stroke)
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawRectangle(Rect const& rect, Color const& stroke_color, Float32 stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(stroke_color));

			render_target_->DrawRectangle(
				DX::ConvertToRectF(rect),
				solid_color_brush_.get(),
				stroke_width,
				d2d_res_->GetStrokeStyle(stroke)
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::FillRectangle(Rect const& rect, Color const& fill_color) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(fill_color));
			render_target_->FillRectangle(
				DX::ConvertToRectF(rect),
				solid_color_brush_.get()
			);
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawRoundedRectangle(Rect const& rect, Vec2 const& radius, Color const& stroke_color, Float32 stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(stroke_color));

			render_target_->DrawRoundedRectangle(
				D2D1::RoundedRect(
					DX::ConvertToRectF(rect),
					radius.x,
					radius.y
				),
				solid_color_brush_.get(),
				stroke_width,
				d2d_res_->GetStrokeStyle(stroke)
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::FillRoundedRectangle(Rect const& rect, Vec2 const& radius, Color const& fill_color) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(fill_color));
			render_target_->FillRoundedRectangle(
				D2D1::RoundedRect(
					DX::ConvertToRectF(rect),
					radius.x,
					radius.y
				),
				solid_color_brush_.get()
			);
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawEllipse(Point const& center, Vec2 const& radius, Color const& stroke_color, Float32 stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(stroke_color));

			render_target_->DrawEllipse(
				D2D1::Ellipse(
					DX::ConvertToPoint2F(center),
					radius.x,
					radius.y
				),
				solid_color_brush_.get(),
				stroke_width,
				d2d_res_->GetStrokeStyle(stroke)
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::FillEllipse(Point const& center, Vec2 const& radius, Color const& fill_color) const
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(fill_color));
			render_target_->FillEllipse(
				D2D1::Ellipse(
					DX::ConvertToPoint2F(center),
					radius.x,
					radius.y
				),
				solid_color_brush_.get()
			);
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawImage(Image const& image, Rect const& src_rect, Rect const& dest_rect) const
	{
		DrawImage(image, &src_rect, &dest_rect);
	}

	void RenderTarget::DrawImage(Image const& image, const Rect* src_rect, const Rect* dest_rect) const
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && image.IsValid())
		{
			render_target_->DrawBitmap(
				image.GetBitmap().get(),
				dest_rect ? &DX::ConvertToRectF(*dest_rect) : nullptr,
				opacity_,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				src_rect ? &DX::ConvertToRectF(*src_rect) : nullptr
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawTextLayout(TextLayout const& layout, Point const& offset) const
	{
		HRESULT hr = S_OK;
		if (!text_renderer_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			text_renderer_->SetTextStyle(
				opacity_,
				DX::ConvertToColorF(layout.GetTextStyle().color),
				layout.GetTextStyle().outline,
				DX::ConvertToColorF(layout.GetTextStyle().outline_color),
				layout.GetTextStyle().outline_width,
				d2d_res_->GetStrokeStyle(layout.GetTextStyle().outline_stroke)
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = layout.GetTextLayout()->Draw(nullptr, text_renderer_.get(), offset.x, offset.y);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::CreateLayer(LayerArea& layer) const
	{
		HRESULT hr = S_OK;

		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<ID2D1Layer> output;
			hr = render_target_->CreateLayer(&output);

			if (SUCCEEDED(hr))
			{
				layer.SetLayer(output);
			}
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::PushClipRect(Rect const& clip_rect)
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->PushAxisAlignedClip(
				DX::ConvertToRectF(clip_rect),
				antialias_ ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED
			);
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::PopClipRect()
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->PopAxisAlignedClip();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::PushLayer(LayerArea const& layer)
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !solid_color_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && layer.IsValid())
		{
			render_target_->PushLayer(
				D2D1::LayerParameters(
					DX::ConvertToRectF(layer.GetAreaRect()),
					layer.GetMaskGeometry().GetGeometry().get(),
					antialias_ ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED,
					D2D1::Matrix3x2F::Identity(),
					layer.GetOpacity(),
					nullptr,
					D2D1_LAYER_OPTIONS_NONE
				),
				layer.GetLayer().get()
			);
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::PopLayer()
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->PopLayer();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::Clear()
	{
		HRESULT hr = E_FAIL;

		if (render_target_)
		{
			render_target_->Clear();
			hr = S_OK;
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::Clear(Color const& clear_color)
	{
		HRESULT hr = E_FAIL;

		if (render_target_)
		{
			render_target_->Clear(DX::ConvertToColorF(clear_color));
			hr = S_OK;
		}

		ThrowIfFailed(hr);
	}

	Float32 RenderTarget::GetOpacity() const
	{
		return opacity_;
	}

	void RenderTarget::SetTransform(const Matrix3x2& matrix)
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->SetTransform(DX::ConvertToMatrix3x2F(&matrix));
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::SetOpacity(Float32 opacity)
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			if (opacity_ != opacity)
			{
				opacity_ = opacity;
				solid_color_brush_->SetOpacity(opacity);
			}
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::SetAntialiasMode(bool enabled)
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->SetAntialiasMode(
				enabled ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED
			);
			antialias_ = enabled;
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::SetTextAntialiasMode(TextAntialias mode)
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			text_antialias_ = mode;
			D2D1_TEXT_ANTIALIAS_MODE antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
			switch (text_antialias_)
			{
			case TextAntialias::Default:
				antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
				break;
			case TextAntialias::ClearType:
				antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
				break;
			case TextAntialias::GrayScale:
				antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE;
				break;
			case TextAntialias::None:
				antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;
				break;
			default:
				break;
			}
			render_target_->SetTextAntialiasMode(antialias_mode);
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::SetCollectingStatus(bool collecting)
	{
		collecting_status_ = collecting;
	}

	void RenderTarget::IncreasePrimitivesCount() const
	{
		if (collecting_status_)
		{
			++status_.primitives;
		}
	}


	//
	// ImageRenderTarget
	//

	ImageRenderTarget::ImageRenderTarget()
	{
	}

	void ImageRenderTarget::GetOutput(Image& output) const
	{
		HRESULT hr = E_FAIL;

		ComPtr<ID2D1BitmapRenderTarget> bitmap_rt;
		if (render_target_)
		{
			hr = render_target_->QueryInterface<ID2D1BitmapRenderTarget>(&bitmap_rt);
		}

		ComPtr<ID2D1Bitmap> bitmap;
		if (SUCCEEDED(hr))
		{
			hr = bitmap_rt->GetBitmap(&bitmap);
		}

		if (SUCCEEDED(hr))
		{
			output.SetBitmap(bitmap);
		}

		ThrowIfFailed(hr);
	}

}
