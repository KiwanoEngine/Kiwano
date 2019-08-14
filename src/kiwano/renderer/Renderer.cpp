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

#include "Renderer.h"
#include "../base/Logger.h"
#include "../base/Window.h"

namespace kiwano
{
	Renderer::Renderer()
		: hwnd_(nullptr)
		, antialias_(true)
		, vsync_(true)
		, text_antialias_(TextAntialias::GrayScale)
		, clear_color_(Color::Black)
		, opacity_(1.f)
		, collecting_status_(false)
	{
		status_.primitives = 0;
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::SetupComponent()
	{
		KGE_LOG(L"Creating device resources");

		hwnd_ = Window::GetInstance()->GetHandle();

		ThrowIfFailed(hwnd_ ? S_OK : E_FAIL);
		
		d2d_res_ = nullptr;
		d3d_res_ = nullptr;
		drawing_state_block_ = nullptr;

		ThrowIfFailed(
			ID2DDeviceResources::Create(
				&d2d_res_
			)
		);

		ThrowIfFailed(
#if defined(KGE_USE_DIRECTX10)
			ID3D10DeviceResources::Create(
				&d3d_res_,
				d2d_res_.get(),
				hwnd_
			)
#else
			ID3D11DeviceResources::Create(
				&d3d_res_,
				d2d_res_.get(),
				hwnd_
			)
#endif
		);

		device_context_ = d2d_res_->GetDeviceContext();

		ThrowIfFailed(
			d2d_res_->GetFactory()->CreateDrawingStateBlock(
				&drawing_state_block_
			)
		);

		ThrowIfFailed(
			CreateDeviceResources()
		);

		output_size_ = Window::GetInstance()->GetSize();
	}

	void Renderer::DestroyComponent()
	{
		KGE_LOG(L"Destroying device resources");

		drawing_state_block_.reset();
		solid_color_brush_.reset();
		d2d_res_.reset();
		d3d_res_.reset();
	}

	void Renderer::BeforeRender()
	{
		ThrowIfFailed(
			BeginDraw()
		);
	}

	void Renderer::AfterRender()
	{
		ThrowIfFailed(
			EndDraw()
		);
	}

	void Renderer::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_SIZE:
		{
			UINT width = LOWORD(lparam);
			UINT height = HIWORD(lparam);

			Resize(width, height);
			break;
		}
		}
	}

	HRESULT Renderer::CreateDeviceResources()
	{
		HRESULT hr = S_OK;

		solid_color_brush_.reset();
		hr = device_context_->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			D2D1::BrushProperties(),
			&solid_color_brush_
		);

		if (SUCCEEDED(hr))
		{
			text_renderer_.reset();
			hr = ITextRenderer::Create(
				&text_renderer_,
				device_context_.get()
			);
		}

		if (SUCCEEDED(hr))
		{
			SetAntialiasMode(antialias_);
			SetTextAntialiasMode(text_antialias_);
		}
		return hr;
	}

	HRESULT Renderer::HandleDeviceLost()
	{
		HRESULT hr = d3d_res_->HandleDeviceLost();

		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources();
		}
		return hr;
	}

	HRESULT Renderer::BeginDraw()
	{
		if (!device_context_)
			return E_UNEXPECTED;

		if (collecting_status_)
		{
			status_.start = Time::Now();
			status_.primitives = 0;
		}

		device_context_->SaveDrawingState(drawing_state_block_.get());

		device_context_->BeginDraw();

		HRESULT hr = d3d_res_->ClearRenderTarget(clear_color_);

		return hr;
	}

	HRESULT Renderer::EndDraw()
	{
		if (!device_context_)
			return E_UNEXPECTED;

		HRESULT hr = device_context_->EndDraw();

		device_context_->RestoreDrawingState(drawing_state_block_.get());

		if (SUCCEEDED(hr))
		{
			hr = d3d_res_->Present(vsync_);
		}

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
			hr = HandleDeviceLost();
		}

		if (collecting_status_)
		{
			status_.duration = Time::Now() - status_.start;
		}
		return hr;
	}

	void Renderer::IncreasePrimitivesCount()
	{
		if (collecting_status_)
		{
			++status_.primitives;
		}
	}

	void Renderer::CreateLayer(ComPtr<ID2D1Layer>& layer)
	{
		HRESULT hr = S_OK;
		ComPtr<ID2D1Layer> new_layer;

		if (!device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			hr = device_context_->CreateLayer(&new_layer);
		}

		if (SUCCEEDED(hr))
		{
			layer = new_layer;
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateLineGeometry(Geometry& geo, Point const& begin_pos, Point const& end_pos)
	{
		HRESULT hr = S_OK;
		if (!device_context_ || !d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<ID2D1PathGeometry> path_geo;
		ComPtr<ID2D1GeometrySink> path_sink;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetFactory()->CreatePathGeometry(&path_geo);
		}

		if (SUCCEEDED(hr))
		{
			hr = path_geo->Open(&path_sink);
		}

		if (SUCCEEDED(hr))
		{
			path_sink->BeginFigure(DX::ConvertToPoint2F(begin_pos), D2D1_FIGURE_BEGIN_FILLED);
			path_sink->AddLine(DX::ConvertToPoint2F(end_pos));
			path_sink->EndFigure(D2D1_FIGURE_END_OPEN);
			hr = path_sink->Close();
		}

		if (SUCCEEDED(hr))
		{
			geo.SetGeometry(path_geo);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateRectGeometry(Geometry& geo, Rect const& rect)
	{
		HRESULT hr = S_OK;
		if (!device_context_ || !d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<ID2D1RectangleGeometry> output;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetFactory()->CreateRectangleGeometry(DX::ConvertToRectF(rect), &output);
		}

		if (SUCCEEDED(hr))
		{
			geo.SetGeometry(output);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateRoundedRectGeometry(Geometry& geo, Rect const& rect, Vec2 const& radius)
	{
		HRESULT hr = S_OK;
		if (!device_context_ || !d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<ID2D1RoundedRectangleGeometry> output;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetFactory()->CreateRoundedRectangleGeometry(
				D2D1::RoundedRect(
					DX::ConvertToRectF(rect),
					radius.x,
					radius.y
				),
				&output);
		}

		if (SUCCEEDED(hr))
		{
			geo.SetGeometry(output);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateEllipseGeometry(Geometry& geo, Point const& center, Vec2 const& radius)
	{
		HRESULT hr = S_OK;
		if (!device_context_ || !d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<ID2D1EllipseGeometry> output;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetFactory()->CreateEllipseGeometry(
				D2D1::Ellipse(
					DX::ConvertToPoint2F(center),
					radius.x,
					radius.y
				),
				&output);
		}

		if (SUCCEEDED(hr))
		{
			geo.SetGeometry(output);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreatePathGeometrySink(GeometrySink& sink)
	{
		HRESULT hr = S_OK;
		if (!device_context_ || !d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<ID2D1PathGeometry> output;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetFactory()->CreatePathGeometry(&output);
		}

		if (SUCCEEDED(hr))
		{
			sink.SetPathGeometry(output);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::DrawGeometry(
		Geometry const& geometry,
		Color const& stroke_color,
		float stroke_width,
		StrokeStyle stroke
	)
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && geometry.GetGeometry())
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(stroke_color));

			device_context_->DrawGeometry(
				geometry.GetGeometry().get(),
				solid_color_brush_.get(),
				stroke_width,
				d2d_res_->GetStrokeStyle(stroke)
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void Renderer::FillGeometry(Geometry const & geometry, Color const& fill_color)
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && geometry.GetGeometry())
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(fill_color));
			device_context_->FillGeometry(
				geometry.GetGeometry().get(),
				solid_color_brush_.get()
			);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::DrawRectangle(Rect const& rect, Color const& stroke_color, float stroke_width, StrokeStyle stroke)
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(stroke_color));

			device_context_->DrawRectangle(
				DX::ConvertToRectF(rect),
				solid_color_brush_.get(),
				stroke_width,
				d2d_res_->GetStrokeStyle(stroke)
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void Renderer::FillRectangle(Rect const& rect, Color const& fill_color)
	{
		HRESULT hr = S_OK;
		if (!solid_color_brush_ || !device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			solid_color_brush_->SetColor(DX::ConvertToColorF(fill_color));
			device_context_->FillRectangle(
				DX::ConvertToRectF(rect),
				solid_color_brush_.get()
			);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::DrawBitmap(ComPtr<ID2D1Bitmap> const & bitmap, Rect const& src_rect, Rect const& dest_rect)
	{
		HRESULT hr = S_OK;
		if (!device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr) && bitmap)
		{
			device_context_->DrawBitmap(
				bitmap.get(),
				DX::ConvertToRectF(dest_rect),
				opacity_,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				DX::ConvertToRectF(src_rect)
			);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void Renderer::DrawTextLayout(ComPtr<IDWriteTextLayout> const& text_layout)
	{
		HRESULT hr = S_OK;
		if (!text_renderer_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			hr = text_layout->Draw(nullptr, text_renderer_.get(), 0, 0);

			IncreasePrimitivesCount();
		}

		ThrowIfFailed(hr);
	}

	void Renderer::SetVSyncEnabled(bool enabled)
	{
		vsync_ = enabled;
	}

	void Renderer::PushClip(const Matrix & clip_matrix, const Size & clip_size)
	{
		HRESULT hr = S_OK;
		if (!device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			device_context_->SetTransform(DX::ConvertToMatrix3x2F(clip_matrix));
			device_context_->PushAxisAlignedClip(
				D2D1::RectF(0, 0, clip_size.x, clip_size.y),
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
			);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::PopClip()
	{
		HRESULT hr = S_OK;
		if (!device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			device_context_->PopAxisAlignedClip();
		}

		ThrowIfFailed(hr);
	}

	void Renderer::PushLayer(ComPtr<ID2D1Layer> const& layer, LayerProperties const& properties)
	{
		HRESULT hr = S_OK;
		if (!device_context_ || !solid_color_brush_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			device_context_->PushLayer(
				D2D1::LayerParameters(
					DX::ConvertToRectF(properties.area),
					nullptr,
					D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
					D2D1::Matrix3x2F::Identity(),
					properties.opacity,
					nullptr,
					D2D1_LAYER_OPTIONS_NONE
				),
				layer.get()
			);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::PopLayer()
	{
		HRESULT hr = S_OK;
		if (!device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			device_context_->PopLayer();
		}

		ThrowIfFailed(hr);
	}

	void Renderer::Resize(UINT width, UINT height)
	{
		HRESULT hr = S_OK;
		if (!d3d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			output_size_.x = static_cast<float>(width);
			output_size_.y = static_cast<float>(height);
			hr = d3d_res_->SetLogicalSize(output_size_);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::SetCollectingStatus(bool collecting)
	{
		collecting_status_ = collecting;
	}

	void Renderer::SetClearColor(const Color & color)
	{
		clear_color_ = color;
	}

	void Renderer::SetTransform(const Matrix & matrix)
	{
		HRESULT hr = S_OK;
		if (!device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			device_context_->SetTransform(DX::ConvertToMatrix3x2F(&matrix));
		}

		ThrowIfFailed(hr);
	}

	void Renderer::SetOpacity(float opacity)
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

	void Renderer::SetTextStyle(
		float opacity,
		Color const& color,
		bool has_outline,
		Color const& outline_color,
		float outline_width,
		StrokeStyle outline_stroke
	)
	{
		HRESULT hr = S_OK;
		if (!text_renderer_ || !d3d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			text_renderer_->SetTextStyle(
				opacity,
				DX::ConvertToColorF(color),
				has_outline,
				DX::ConvertToColorF(outline_color),
				outline_width,
				d2d_res_->GetStrokeStyle(outline_stroke)
			);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::SetAntialiasMode(bool enabled)
	{
		HRESULT hr = S_OK;
		if (!device_context_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			device_context_->SetAntialiasMode(
				enabled ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED
			);
			antialias_ = enabled;
		}

		ThrowIfFailed(hr);
	}

	void Renderer::SetTextAntialiasMode(TextAntialias mode)
	{
		HRESULT hr = S_OK;
		if (!device_context_)
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
			device_context_->SetTextAntialiasMode(antialias_mode);
		}

		ThrowIfFailed(hr);
	}

	bool Renderer::CheckVisibility(Size const& content_size, Matrix const& transform)
	{
		return Rect{ Point{}, output_size_ }.Intersects(
			transform.Transform(Rect{ Point{}, content_size })
		);
	}

}
