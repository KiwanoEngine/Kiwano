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

#include <kiwano/renderer/RenderTarget.h>
#include <kiwano/base/win32/helper.h>

namespace kiwano
{
	//
	// RenderTarget
	//

	RenderTarget::RenderTarget()
		: opacity_(1.f)
		, collecting_status_(false)
		, fast_global_transform_(true)
		, antialias_(true)
		, text_antialias_(TextAntialiasMode::GrayScale)
	{
		status_.primitives = 0;
	}

	HRESULT RenderTarget::CreateDeviceResources(ComPtr<ID2D1RenderTarget> rt, ComPtr<ID2DDeviceResources> dev_res)
	{
		HRESULT hr = E_FAIL;

		if (rt && dev_res)
		{
			render_target_ = rt;
			device_resources_ = dev_res;
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
			default_brush_.reset();
			hr = render_target_->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				D2D1::BrushProperties(),
				&default_brush_
			);
		}

		if (SUCCEEDED(hr))
		{
			current_brush_ = default_brush_;
		}

		return hr;
	}

	void RenderTarget::DiscardDeviceResources()
	{
		text_renderer_.reset();
		render_target_.reset();
		default_brush_.reset();
		current_brush_.reset();
		device_resources_.reset();
	}

	bool RenderTarget::IsValid() const
	{
		return render_target_ && device_resources_;
	}

	void RenderTarget::BeginDraw()
	{
		if (collecting_status_)
		{
			status_.start = Time::Now();
			status_.primitives = 0;
		}

		if (render_target_)
		{
			render_target_->BeginDraw();
		}
	}

	void RenderTarget::EndDraw()
	{
		ThrowIfFailed( render_target_->EndDraw() );

		if (collecting_status_)
		{
			status_.duration = Time::Now() - status_.start;
		}
	}

	void RenderTarget::DrawGeometry(Geometry const& geometry, float stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && geometry.GetGeometry())
		{
			render_target_->DrawGeometry(
				geometry.GetGeometry().get(),
				current_brush_.get(),
				stroke_width,
				GetStrokeStyle(stroke).get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::FillGeometry(Geometry const& geometry) const
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && geometry.GetGeometry())
		{
			render_target_->FillGeometry(
				geometry.GetGeometry().get(),
				current_brush_.get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawLine(Point const& point1, Point const& point2, float stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->DrawLine(
				DX::ConvertToPoint2F(point1),
				DX::ConvertToPoint2F(point2),
				current_brush_.get(),
				stroke_width,
				GetStrokeStyle(stroke).get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawRectangle(Rect const& rect, float stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;

		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->DrawRectangle(
				DX::ConvertToRectF(rect),
				current_brush_.get(),
				stroke_width,
				GetStrokeStyle(stroke).get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::FillRectangle(Rect const& rect) const
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->FillRectangle(
				DX::ConvertToRectF(rect),
				current_brush_.get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawRoundedRectangle(Rect const& rect, Vec2 const& radius, float stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->DrawRoundedRectangle(
				D2D1::RoundedRect(
					DX::ConvertToRectF(rect),
					radius.x,
					radius.y
				),
				current_brush_.get(),
				stroke_width,
				GetStrokeStyle(stroke).get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::FillRoundedRectangle(Rect const& rect, Vec2 const& radius) const
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->FillRoundedRectangle(
				D2D1::RoundedRect(
					DX::ConvertToRectF(rect),
					radius.x,
					radius.y
				),
				current_brush_.get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawEllipse(Point const& center, Vec2 const& radius, float stroke_width, StrokeStyle stroke) const
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->DrawEllipse(
				D2D1::Ellipse(
					DX::ConvertToPoint2F(center),
					radius.x,
					radius.y
				),
				current_brush_.get(),
				stroke_width,
				GetStrokeStyle(stroke).get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::FillEllipse(Point const& center, Vec2 const& radius) const
	{
		HRESULT hr = S_OK;
		if (!render_target_ || !current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->FillEllipse(
				D2D1::Ellipse(
					DX::ConvertToPoint2F(center),
					radius.x,
					radius.y
				),
				current_brush_.get()
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::DrawTexture(Texture const& texture, Rect const& src_rect, Rect const& dest_rect) const
	{
		DrawTexture(texture, &src_rect, &dest_rect);
	}

	void RenderTarget::DrawTexture(Texture const& texture, const Rect* src_rect, const Rect* dest_rect) const
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && texture.IsValid())
		{
			auto mode = (texture.GetBitmapInterpolationMode() == InterpolationMode::Linear)
				? D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
				: D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

			render_target_->DrawBitmap(
				texture.GetBitmap().get(),
				dest_rect ? &DX::ConvertToRectF(*dest_rect) : nullptr,
				opacity_,
				mode,
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
				GetStrokeStyle(layout.GetTextStyle().outline_stroke).get()
			);

			hr = layout.GetTextLayout()->Draw(nullptr, text_renderer_.get(), offset.x, offset.y);
		}

		if (SUCCEEDED(hr))
		{
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

	void RenderTarget::PushLayer(LayerArea& layer)
	{
		HRESULT hr = S_OK;
		if (!render_target_)
		{
			hr = E_UNEXPECTED;
		}

		if (!layer.IsValid())
		{
			CreateLayer(layer);
		}

		if (SUCCEEDED(hr) && layer.IsValid())
		{
			render_target_->PushLayer(
				D2D1::LayerParameters(
					DX::ConvertToRectF(layer.GetAreaRect()),
					layer.GetMaskGeometry().GetGeometry().get(),
					antialias_ ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED,
					DX::ConvertToMatrix3x2F(layer.GetMaskTransform()),
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

	float RenderTarget::GetOpacity() const
	{
		return opacity_;
	}

	Brush RenderTarget::GetCurrentBrush() const
	{
		return Brush( current_brush_ );
	}

	Matrix3x2 RenderTarget::GetGlobalTransform() const
	{
		return global_transform_;
	}

	ComPtr<ID2D1StrokeStyle> RenderTarget::GetStrokeStyle(StrokeStyle style) const
	{
		switch (style)
		{
		case StrokeStyle::Miter: return device_resources_->GetMiterStrokeStyle(); break;
		case StrokeStyle::Bevel: return device_resources_->GetBevelStrokeStyle(); break;
		case StrokeStyle::Round: return device_resources_->GetRoundStrokeStyle(); break;
		}
		return nullptr;
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
			if (fast_global_transform_)
			{
				render_target_->SetTransform(DX::ConvertToMatrix3x2F(&matrix));
			}
			else
			{
				Matrix3x2 result = matrix * global_transform_;
				render_target_->SetTransform(DX::ConvertToMatrix3x2F(&result));
			}
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::SetGlobalTransform(const Matrix3x2& matrix)
	{
		SetGlobalTransform(&matrix);
	}

	void RenderTarget::SetGlobalTransform(const Matrix3x2* matrix)
	{
		if (matrix)
		{
			global_transform_ = *matrix;
			fast_global_transform_ = false;
		}
		else
		{
			fast_global_transform_ = true;
		}
	}

	void RenderTarget::SetOpacity(float opacity)
	{
		HRESULT hr = S_OK;
		if (!current_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			if (opacity_ != opacity)
			{
				opacity_ = opacity;
				current_brush_->SetOpacity(opacity);
			}
		}

		ThrowIfFailed(hr);
	}

	void RenderTarget::SetCurrentBrush(Brush const& brush)
	{
		current_brush_ = brush.GetBrush();
	}

	void RenderTarget::SetDefaultBrushColor(Color const& color)
	{
		KGE_ASSERT(default_brush_);
		default_brush_->SetColor(DX::ConvertToColorF(color));
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

	void RenderTarget::SetTextAntialiasMode(TextAntialiasMode mode)
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
			case TextAntialiasMode::Default:
				antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
				break;
			case TextAntialiasMode::ClearType:
				antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
				break;
			case TextAntialiasMode::GrayScale:
				antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE;
				break;
			case TextAntialiasMode::None:
				antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;
				break;
			default:
				break;
			}
			render_target_->SetTextAntialiasMode(antialias_mode);
		}

		ThrowIfFailed(hr);
	}

	bool RenderTarget::CheckVisibility(Rect const& bounds, Matrix3x2 const& transform)
	{
		Rect visible_size = { Point{}, reinterpret_cast<const Size&>(render_target_->GetSize()) };
		if (fast_global_transform_)
		{
			return visible_size.Intersects(transform.Transform(bounds));
		}
		return visible_size.Intersects(Matrix3x2(transform * global_transform_).Transform(bounds));
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
	// TextureRenderTarget
	//

	TextureRenderTarget::TextureRenderTarget()
	{
	}

	Texture TextureRenderTarget::GetOutput() const
	{
		HRESULT hr = E_FAIL;

		if (render_target_)
		{
			ComPtr<ID2D1BitmapRenderTarget> bitmap_rt;
			hr = render_target_->QueryInterface<ID2D1BitmapRenderTarget>(&bitmap_rt);

			if (SUCCEEDED(hr))
			{
				ComPtr<ID2D1Bitmap> bitmap;
				hr = bitmap_rt->GetBitmap(&bitmap);

				if (SUCCEEDED(hr))
				{
					return Texture(bitmap);
				}
			}
		}

		ThrowIfFailed(hr);
		return Texture();
	}

}
