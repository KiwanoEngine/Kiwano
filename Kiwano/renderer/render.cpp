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

#include "render.h"
#include "../2d/Image.h"
#include "../base/logs.h"
#include "../platform/Application.h"

namespace kiwano
{
	Renderer::Renderer()
		: hwnd_(nullptr)
		, antialias_(true)
		, vsync_(true)
		, text_antialias_(TextAntialias::ClearType)
		, clear_color_(Color::Black)
		, opacity_(1.f)
		, collecting_data_(false)
	{
		status_.primitives = 0;
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::SetupComponent(Application* app)
	{
		KGE_LOG(L"Creating device resources");

		hwnd_ = app->GetWindow()->GetHandle();

		ThrowIfFailed(hwnd_ ? S_OK : E_FAIL);
		
		device_resources_ = nullptr;
		drawing_state_block_ = nullptr;

		ThrowIfFailed(
			DeviceResources::Create(
				&device_resources_,
				hwnd_
			)
		);

		factory_ = device_resources_->GetD2DFactory();
		device_context_ = device_resources_->GetD2DDeviceContext();

		ThrowIfFailed(
			factory_->CreateDrawingStateBlock(
				&drawing_state_block_
			)
		);

		ThrowIfFailed(
			CreateDeviceResources()
		);

		output_size_ = app->GetWindow()->GetSize();
	}

	void Renderer::DestroyComponent()
	{
		KGE_LOG(L"Destroying device resources");

		drawing_state_block_.Reset();
		text_renderer_.Reset();
		solid_color_brush_.Reset();
		device_context_.Reset();
		factory_.Reset();
		device_resources_.Reset();
	}

	HRESULT Renderer::CreateDeviceResources()
	{
		HRESULT hr = S_OK;

		hr = device_context_->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&solid_color_brush_
		);

		if (SUCCEEDED(hr))
		{
			hr = ITextRenderer::Create(
				&text_renderer_,
				device_context_.Get()
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
		HRESULT hr = device_resources_->HandleDeviceLost();

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

		if (collecting_data_)
		{
			status_.start = Time::Now();
			status_.primitives = 0;
		}

		device_context_->SaveDrawingState(drawing_state_block_.Get());

		device_context_->BeginDraw();
		return S_OK;
	}

	HRESULT Renderer::EndDraw()
	{
		if (!device_context_)
			return E_UNEXPECTED;

		HRESULT hr = device_context_->EndDraw();

		device_context_->RestoreDrawingState(drawing_state_block_.Get());

		if (SUCCEEDED(hr))
		{
			// The first argument instructs DXGI to block until VSync.
			hr = device_resources_->GetDXGISwapChain()->Present(vsync_ ? 1 : 0, 0);

			auto main_rt_view = device_resources_->GetD3DRenderTargetView();
			device_resources_->GetD3DDeviceContext()->OMSetRenderTargets(
				1, 
				&main_rt_view,
				device_resources_->GetD3DDepthStencilView()
			);

			device_resources_->GetD3DDeviceContext()->ClearRenderTargetView(
				main_rt_view,
				reinterpret_cast<float*>(&clear_color_)
			);
		}

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
			hr = HandleDeviceLost();
		}

		if (collecting_data_)
		{
			status_.duration = Time::Now() - status_.start;
		}
		return hr;
	}

	HRESULT Renderer::CreateLayer(ComPtr<ID2D1Layer>& layer)
	{
		if (!device_context_)
			return E_UNEXPECTED;

		layer = nullptr;
		return device_context_->CreateLayer(&layer);
	}

	HRESULT Renderer::DrawGeometry(
		ComPtr<ID2D1Geometry> const& geometry,
		Color const& stroke_color,
		float stroke_width,
		StrokeStyle stroke
	)
	{
		if (!solid_color_brush_ || !device_context_)
			return E_UNEXPECTED;

		solid_color_brush_->SetColor(DX::ConvertToColorF(stroke_color));

		device_context_->DrawGeometry(
			geometry.Get(),
			solid_color_brush_.Get(),
			stroke_width,
			device_resources_->GetStrokeStyle(stroke)
		);

		if (collecting_data_)
			++status_.primitives;
		return S_OK;
	}

	HRESULT Renderer::FillGeometry(ComPtr<ID2D1Geometry> const & geometry, Color const& fill_color)
	{
		if (!solid_color_brush_ || !device_context_)
			return E_UNEXPECTED;

		solid_color_brush_->SetColor(DX::ConvertToColorF(fill_color));
		device_context_->FillGeometry(
			geometry.Get(),
			solid_color_brush_.Get()
		);

		return S_OK;
	}

	HRESULT Renderer::DrawImage(ImagePtr const & image, Rect const& dest_rect)
	{
		if (!device_context_)
			return E_UNEXPECTED;

		if (!image->GetBitmap())
			return S_OK;

		device_context_->DrawBitmap(
			image->GetBitmap().Get(),
			DX::ConvertToRectF(dest_rect),
			opacity_,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			DX::ConvertToRectF(image->GetCropRect())
		);

		if (collecting_data_)
			++status_.primitives;
		return S_OK;
	}

	HRESULT Renderer::DrawBitmap(ComPtr<ID2D1Bitmap> const & bitmap, Rect const& src_rect, Rect const& dest_rect)
	{
		if (!device_context_)
			return E_UNEXPECTED;

		if (!bitmap)
			return S_OK;

		// Do not crop bitmap 
		device_context_->DrawBitmap(
			bitmap.Get(),
			DX::ConvertToRectF(dest_rect),
			opacity_,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			DX::ConvertToRectF(src_rect)
		);

		if (collecting_data_)
		++status_.primitives;
		return S_OK;
	}

	HRESULT Renderer::DrawTextLayout(ComPtr<IDWriteTextLayout> const& text_layout)
	{
		if (!text_renderer_)
			return E_UNEXPECTED;

		if (collecting_data_)
			++status_.primitives;
		return text_layout->Draw(nullptr, text_renderer_.Get(), 0, 0);
	}

	void Renderer::SetVSyncEnabled(bool enabled)
	{
		vsync_ = enabled;
	}

	HRESULT Renderer::PushClip(const Matrix & clip_matrix, const Size & clip_size)
	{
		if (!device_context_)
			return E_UNEXPECTED;

		device_context_->SetTransform(DX::ConvertToMatrix3x2F(clip_matrix));
		device_context_->PushAxisAlignedClip(
			D2D1::RectF(0, 0, clip_size.x, clip_size.y),
			D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
		);
		return S_OK;
	}

	HRESULT Renderer::PopClip()
	{
		if (!device_context_)
			return E_UNEXPECTED;

		device_context_->PopAxisAlignedClip();
		return S_OK;
	}

	HRESULT Renderer::PushLayer(ComPtr<ID2D1Layer> const& layer, LayerProperties const& properties)
	{
		if (!device_context_ || !solid_color_brush_)
			return E_UNEXPECTED;

		device_context_->PushLayer(
			D2D1::LayerParameters(
				DX::ConvertToRectF(properties.area),
				nullptr,
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
				D2D1::Matrix3x2F::Identity(),
				properties.opacity,
				solid_color_brush_.Get(),
				D2D1_LAYER_OPTIONS_NONE
			),
			layer.Get()
		);
		return S_OK;
	}

	HRESULT Renderer::PopLayer()
	{
		if (!device_context_)
			return E_UNEXPECTED;

		device_context_->PopLayer();
		return S_OK;
	}

	HRESULT Renderer::Resize(UINT width, UINT height)
	{
		output_size_.x = static_cast<float>(width);
		output_size_.y = static_cast<float>(height);
		if (device_resources_)
		{
			return device_resources_->SetLogicalSize(output_size_);
		}
		return S_OK;
	}

	void Renderer::StartCollectData()
	{
		collecting_data_ = true;
	}

	void Renderer::StopCollectData()
	{
		collecting_data_ = false;
	}

	void Renderer::SetClearColor(const Color & color)
	{
		clear_color_ = color;
	}

	HRESULT Renderer::SetTransform(const Matrix & matrix)
	{
		if (!device_context_)
			return E_UNEXPECTED;

		device_context_->SetTransform(DX::ConvertToMatrix3x2F(matrix));
		return S_OK;
	}

	void Renderer::SetOpacity(float opacity)
	{
		if (opacity_ != opacity)
		{
			opacity_ = opacity;
			solid_color_brush_->SetOpacity(opacity);
		}
	}

	HRESULT Renderer::SetTextStyle(
		Color const& color,
		bool has_outline,
		Color const& outline_color,
		float outline_width,
		StrokeStyle outline_stroke
	)
	{
		if (!text_renderer_ || !device_resources_)
			return E_UNEXPECTED;

		text_renderer_->SetTextStyle(
			DX::ConvertToColorF(color),
			has_outline,
			DX::ConvertToColorF(outline_color),
			outline_width,
			device_resources_->GetStrokeStyle(outline_stroke)
		);
		return S_OK;
	}

	HRESULT Renderer::SetAntialiasMode(bool enabled)
	{
		if (!device_context_)
			return E_UNEXPECTED;

		device_context_->SetAntialiasMode(
			enabled ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED
		);
		antialias_ = enabled;
		return S_OK;
	}

	HRESULT Renderer::SetTextAntialiasMode(TextAntialias mode)
	{
		if (!device_context_)
			return E_UNEXPECTED;

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
		return S_OK;
	}

}
