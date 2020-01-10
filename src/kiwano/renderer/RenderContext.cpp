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

#include <kiwano/renderer/RenderContext.h>
#include <kiwano/core/Logger.h>

namespace kiwano
{
	//
	// RenderContext
	//

	RenderContext::RenderContext()
		: collecting_status_(false)
		, fast_global_transform_(true)
		, brush_opacity_(1.0f)
		, antialias_(true)
		, text_antialias_(TextAntialiasMode::GrayScale)
	{
		status_.primitives = 0;
	}

	HRESULT RenderContext::CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1RenderTarget> ctx)
	{
		if (!factory || !ctx)
			return E_INVALIDARG;

		render_ctx_ = ctx;
		text_renderer_.reset();
		current_brush_.reset();

		HRESULT hr = ITextRenderer::Create(
			&text_renderer_,
			render_ctx_.get()
		);

		if (SUCCEEDED(hr))
		{
			SetAntialiasMode(antialias_);
			SetTextAntialiasMode(text_antialias_);

			Resize(reinterpret_cast<const Size&>(GetRenderTarget()->GetSize()));
		}

		return hr;
	}

	void RenderContext::DiscardDeviceResources()
	{
		text_renderer_.reset();
		render_ctx_.reset();
		current_brush_.reset();
	}

	bool RenderContext::IsValid() const
	{
		return render_ctx_ != nullptr;
	}

	void RenderContext::BeginDraw()
	{
		if (collecting_status_)
		{
			status_.start = Time::Now();
			status_.primitives = 0;
		}

		if (render_ctx_)
		{
			render_ctx_->BeginDraw();
		}
	}

	void RenderContext::EndDraw()
	{
		win32::ThrowIfFailed( render_ctx_->EndDraw() );

		if (collecting_status_)
		{
			status_.duration = Time::Now() - status_.start;
		}
	}

	void RenderContext::DrawGeometry(Geometry const& geometry, float stroke_width, const StrokeStyle& stroke)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		if (geometry.IsValid())
		{
			render_ctx_->DrawGeometry(
				geometry.GetGeometry().get(),
				current_brush_->GetBrush().get(),
				stroke_width,
				stroke.GetStrokeStyle().get()
			);

			IncreasePrimitivesCount();
		}
	}

	void RenderContext::FillGeometry(Geometry const& geometry)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		if (geometry.IsValid())
		{
			render_ctx_->FillGeometry(
				geometry.GetGeometry().get(),
				current_brush_->GetBrush().get()
			);

			IncreasePrimitivesCount();
		}
	}

	void RenderContext::DrawLine(Point const& point1, Point const& point2, float stroke_width, const StrokeStyle& stroke)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		render_ctx_->DrawLine(
			DX::ConvertToPoint2F(point1),
			DX::ConvertToPoint2F(point2),
			current_brush_->GetBrush().get(),
			stroke_width,
			stroke.GetStrokeStyle().get()
		);

		IncreasePrimitivesCount();
	}

	void RenderContext::DrawRectangle(Rect const& rect, float stroke_width, const StrokeStyle& stroke)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		render_ctx_->DrawRectangle(
			DX::ConvertToRectF(rect),
			current_brush_->GetBrush().get(),
			stroke_width,
			stroke.GetStrokeStyle().get()
		);

		IncreasePrimitivesCount();
	}

	void RenderContext::FillRectangle(Rect const& rect)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		render_ctx_->FillRectangle(
			DX::ConvertToRectF(rect),
			current_brush_->GetBrush().get()
		);

		IncreasePrimitivesCount();
	}

	void RenderContext::DrawRoundedRectangle(Rect const& rect, Vec2 const& radius, float stroke_width, const StrokeStyle& stroke)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		render_ctx_->DrawRoundedRectangle(
			D2D1::RoundedRect(
				DX::ConvertToRectF(rect),
				radius.x,
				radius.y
			),
			current_brush_->GetBrush().get(),
			stroke_width,
			stroke.GetStrokeStyle().get()
		);

		IncreasePrimitivesCount();
	}

	void RenderContext::FillRoundedRectangle(Rect const& rect, Vec2 const& radius)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		render_ctx_->FillRoundedRectangle(
			D2D1::RoundedRect(
				DX::ConvertToRectF(rect),
				radius.x,
				radius.y
			),
			current_brush_->GetBrush().get()
		);

		IncreasePrimitivesCount();
	}

	void RenderContext::DrawEllipse(Point const& center, Vec2 const& radius, float stroke_width, const StrokeStyle& stroke)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		render_ctx_->DrawEllipse(
			D2D1::Ellipse(
				DX::ConvertToPoint2F(center),
				radius.x,
				radius.y
			),
			current_brush_->GetBrush().get(),
			stroke_width,
			stroke.GetStrokeStyle().get()
		);

		IncreasePrimitivesCount();
	}

	void RenderContext::FillEllipse(Point const& center, Vec2 const& radius)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		KGE_ASSERT(current_brush_ && "The brush used for rendering has not been set!");

		render_ctx_->FillEllipse(
			D2D1::Ellipse(
				DX::ConvertToPoint2F(center),
				radius.x,
				radius.y
			),
			current_brush_->GetBrush().get()
		);

		IncreasePrimitivesCount();
	}

	void RenderContext::DrawTexture(Texture const& texture, Rect const& src_rect, Rect const& dest_rect)
	{
		DrawTexture(texture, &src_rect, &dest_rect);
	}

	void RenderContext::DrawTexture(Texture const& texture, const Rect* src_rect, const Rect* dest_rect)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

		if (texture.IsValid())
		{
			auto mode = (texture.GetBitmapInterpolationMode() == InterpolationMode::Linear)
				? D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
				: D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

			render_ctx_->DrawBitmap(
				texture.GetBitmap().get(),
				dest_rect ? &DX::ConvertToRectF(*dest_rect) : nullptr,
				brush_opacity_,
				mode,
				src_rect ? &DX::ConvertToRectF(*src_rect) : nullptr
			);

			IncreasePrimitivesCount();
		}
	}

	void RenderContext::DrawTextLayout(TextLayout const& layout, Point const& offset)
	{
		KGE_ASSERT(text_renderer_ && "Text renderer has not been initialized!");

		if (layout.IsValid())
		{
			ComPtr<ID2D1Brush> fill_brush;
			ComPtr<ID2D1Brush> outline_brush;
			const TextStyle& style = layout.GetStyle();

			if (style.fill_brush)
			{
				fill_brush = style.fill_brush->GetBrush();
				fill_brush->SetOpacity(brush_opacity_);
			}

			if (style.outline_brush)
			{
				outline_brush = style.outline_brush->GetBrush();
				outline_brush->SetOpacity(brush_opacity_);
			}

			HRESULT hr = text_renderer_->DrawTextLayout(
				layout.GetTextLayout().get(),
				offset.x,
				offset.y,
				fill_brush.get(),
				outline_brush.get(),
				style.outline_width,
				style.outline_stroke.GetStrokeStyle().get()
			);

			if (SUCCEEDED(hr))
			{
				IncreasePrimitivesCount(text_renderer_->GetLastPrimitivesCount());
			}
			else
			{
				KGE_ERROR(L"Failed to draw text layout with HRESULT of %08X", hr);
			}
		}
	}

	void RenderContext::CreateTexture(Texture& texture, math::Vec2T<uint32_t> size, D2D1_PIXEL_FORMAT format)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

		ComPtr<ID2D1Bitmap> saved_bitmap;
		HRESULT hr = render_ctx_->CreateBitmap(
			D2D1::SizeU(size.x, size.y),
			D2D1::BitmapProperties(format),
			&saved_bitmap
		);

		if (SUCCEEDED(hr))
		{
			texture.SetBitmap(saved_bitmap);
		}
		else
		{
			win32::ThrowIfFailed(hr);
		}
	}

	void RenderContext::PushClipRect(Rect const& clip_rect)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		render_ctx_->PushAxisAlignedClip(
			DX::ConvertToRectF(clip_rect),
			antialias_ ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED
		);
	}

	void RenderContext::PopClipRect()
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		render_ctx_->PopAxisAlignedClip();
	}

	void RenderContext::PushLayer(LayerArea& layer)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		if (!layer.IsValid())
		{
			ComPtr<ID2D1Layer> output;
			HRESULT hr = render_ctx_->CreateLayer(&output);

			if (SUCCEEDED(hr))
			{
				layer.SetLayer(output);
			}
			else
			{
				win32::ThrowIfFailed(hr);
			}
		}

		if (layer.IsValid())
		{
			render_ctx_->PushLayer(
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
	}

	void RenderContext::PopLayer()
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		render_ctx_->PopLayer();
	}

	void RenderContext::Clear()
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		render_ctx_->Clear();
	}

	void RenderContext::Clear(Color const& clear_color)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");
		render_ctx_->Clear(DX::ConvertToColorF(clear_color));
	}

	void RenderContext::SetTransform(const Matrix3x2& matrix)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

		if (fast_global_transform_)
		{
			render_ctx_->SetTransform(DX::ConvertToMatrix3x2F(&matrix));
		}
		else
		{
			Matrix3x2 result = matrix * global_transform_;
			render_ctx_->SetTransform(DX::ConvertToMatrix3x2F(&result));
		}
	}

	void RenderContext::SetGlobalTransform(const Matrix3x2* matrix)
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

	void RenderContext::SetAntialiasMode(bool enabled)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

		render_ctx_->SetAntialiasMode(enabled ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED);
		antialias_ = enabled;
	}

	void RenderContext::SetTextAntialiasMode(TextAntialiasMode mode)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

		D2D1_TEXT_ANTIALIAS_MODE antialias_mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
		switch (mode)
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

		text_antialias_ = mode;
		render_ctx_->SetTextAntialiasMode(antialias_mode);
	}

	bool RenderContext::CheckVisibility(Rect const& bounds, Matrix3x2 const& transform)
	{
		KGE_ASSERT(render_ctx_ && "Render target has not been initialized!");

		if (fast_global_transform_)
		{
			return visible_size_.Intersects(transform.Transform(bounds));
		}
		return visible_size_.Intersects(Matrix3x2(transform * global_transform_).Transform(bounds));
	}

	void RenderContext::Resize(Size const& size)
	{
		visible_size_ = Rect(Point(), size);
	}

	void RenderContext::SetCollectingStatus(bool enable)
	{
		collecting_status_ = enable;
	}

	void RenderContext::IncreasePrimitivesCount(uint32_t increase) const
	{
		if (collecting_status_)
		{
			status_.primitives += increase;
		}
	}


	//
	// TextureRenderContext
	//

	TextureRenderContext::TextureRenderContext()
	{
	}

	bool TextureRenderContext::GetOutput(Texture& texture)
	{
		HRESULT hr = E_FAIL;

		if (bitmap_rt_)
		{
			ComPtr<ID2D1Bitmap> bitmap;

			hr = bitmap_rt_->GetBitmap(&bitmap);

			if (SUCCEEDED(hr))
			{
				texture.SetBitmap(bitmap);
			}
		}
		return SUCCEEDED(hr);
	}

}
