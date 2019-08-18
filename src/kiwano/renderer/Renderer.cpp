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
#include "../utils/FileUtil.h"

namespace kiwano
{
	Renderer::Renderer()
		: hwnd_(nullptr)
		, vsync_(true)
		, clear_color_(Color::Black)
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::SetupComponent()
	{
		KGE_LOG(L"Creating device resources");

		hwnd_ = Window::GetInstance()->GetHandle();
		output_size_ = Window::GetInstance()->GetSize();

		d2d_res_ = nullptr;
		d3d_res_ = nullptr;
		drawing_state_block_ = nullptr;

		HRESULT hr = hwnd_ ? S_OK : E_FAIL;

		// Direct2D device resources
		if (SUCCEEDED(hr))
		{
			hr = ID2DDeviceResources::Create(&d2d_res_);
		}

		// Direct3D device resources
		if (SUCCEEDED(hr))
		{
#if defined(KGE_USE_DIRECTX10)
			hr = ID3D10DeviceResources::Create(
				&d3d_res_,
				d2d_res_.get(),
				hwnd_
			);
#else
			hr = ID3D11DeviceResources::Create(
				&d3d_res_,
				d2d_res_.get(),
				hwnd_
			);
#endif
		}

		// DrawingStateBlock
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetFactory()->CreateDrawingStateBlock(
				&drawing_state_block_
			);
		}

		// Other device resources
		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResources();
		}

		// FontFileLoader and FontCollectionLoader
		if (SUCCEEDED(hr))
		{
			hr = IFontCollectionLoader::Create(&font_collection_loader_);
		}

		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetDWriteFactory()->RegisterFontCollectionLoader(font_collection_loader_.get());
		}

		// ResourceFontFileLoader and ResourceFontCollectionLoader
		if (SUCCEEDED(hr))
		{
			hr = IResourceFontFileLoader::Create(&res_font_file_loader_);
		}

		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetDWriteFactory()->RegisterFontFileLoader(res_font_file_loader_.get());
		}

		if (SUCCEEDED(hr))
		{
			hr = IResourceFontCollectionLoader::Create(&res_font_collection_loader_, res_font_file_loader_.get());
		}

		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetDWriteFactory()->RegisterFontCollectionLoader(res_font_collection_loader_.get());
		}

		ThrowIfFailed(hr);
	}

	void Renderer::DestroyComponent()
	{
		KGE_LOG(L"Destroying device resources");

		d2d_res_->GetDWriteFactory()->UnregisterFontFileLoader(res_font_file_loader_.get());
		res_font_file_loader_.reset();

		d2d_res_->GetDWriteFactory()->UnregisterFontCollectionLoader(res_font_collection_loader_.get());
		res_font_collection_loader_.reset();

		drawing_state_block_.reset();
		solid_color_brush_.reset();
		d2d_res_.reset();
		d3d_res_.reset();
	}

	void Renderer::BeforeRender()
	{
		HRESULT hr = S_OK;

		if (!IsValid())
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			render_target_->SaveDrawingState(drawing_state_block_.get());
			BeginDraw();
		}

		if (SUCCEEDED(hr))
		{
			hr = d3d_res_->ClearRenderTarget(clear_color_);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::AfterRender()
	{
		HRESULT hr = S_OK;
		
		if (!IsValid())
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			EndDraw();

			render_target_->RestoreDrawingState(drawing_state_block_.get());
		}

		if (SUCCEEDED(hr))
		{
			hr = d3d_res_->Present(vsync_);
		}

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
			hr = HandleDeviceLost();
		}

		ThrowIfFailed(hr);
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
		HRESULT hr = InitDeviceResources(
			d2d_res_->GetDeviceContext(),
			d2d_res_
		);

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

	void Renderer::CreateImage(Image& image, String const& file_path)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<ID2D1Bitmap> bitmap;
			hr = d2d_res_->CreateBitmapFromFile(bitmap, file_path);

			if (SUCCEEDED(hr))
			{
				image.SetBitmap(bitmap);
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load image failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateImage(Image& image, Resource const& res)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<ID2D1Bitmap> bitmap;
			hr = d2d_res_->CreateBitmapFromResource(bitmap, res);

			if (SUCCEEDED(hr))
			{
				image.SetBitmap(bitmap);
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load image failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateGifImage(GifImage& image, String const& file_path)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (!FileUtil::ExistsFile(file_path))
		{
			KGE_WARNING_LOG(L"Gif image file '%s' not found!", file_path.c_str());
			hr = E_FAIL;
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<IWICBitmapDecoder> decoder;
			hr = d2d_res_->GetWICImagingFactory()->CreateDecoderFromFilename(
				file_path.c_str(),
				nullptr,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&decoder
			);

			if (SUCCEEDED(hr))
			{
				image.SetDecoder(decoder);
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load GIF image failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateGifImage(GifImage& image, Resource const& res)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		Resource::Data res_data = res.GetData();

		hr = res_data ? S_OK : E_FAIL;

		if (SUCCEEDED(hr))
		{
			ComPtr<IWICStream> stream;
			hr = d2d_res_->GetWICImagingFactory()->CreateStream(&stream);

			if (SUCCEEDED(hr))
			{
				hr = stream->InitializeFromMemory(
					static_cast<WICInProcPointer>(res_data.buffer),
					res_data.size
				);
			}

			if (SUCCEEDED(hr))
			{
				ComPtr<IWICBitmapDecoder> decoder;
				hr = d2d_res_->GetWICImagingFactory()->CreateDecoderFromStream(
					stream.get(),
					nullptr,
					WICDecodeMetadataCacheOnLoad,
					&decoder
				);

				if (SUCCEEDED(hr))
				{
					image.SetDecoder(decoder);
				}
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load GIF image failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateFontCollection(FontCollection& collection, Vector<String> const& file_paths)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			for (const auto& file_path : file_paths)
			{
				if (!FileUtil::ExistsFile(file_path))
				{
					KGE_WARNING_LOG(L"Font file '%s' not found!", file_path.c_str());
					hr = E_FAIL;
				}
			}
		}

		if (SUCCEEDED(hr))
		{
			LPVOID collection_key = nullptr;
			UINT32 collection_key_size = 0;

			hr = font_collection_loader_->AddFilePaths(file_paths, &collection_key, &collection_key_size);

			if (SUCCEEDED(hr))
			{
				ComPtr<IDWriteFontCollection> font_collection;
				hr = d2d_res_->GetDWriteFactory()->CreateCustomFontCollection(
					font_collection_loader_.get(),
					collection_key,
					collection_key_size,
					&font_collection
				);

				if (SUCCEEDED(hr))
				{
					collection.SetFontCollection(font_collection);
				}
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load font failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateFontCollection(FontCollection& collection, Vector<Resource> const& res_arr)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			LPVOID collection_key = nullptr;
			UINT32 collection_key_size = 0;

			hr = res_font_collection_loader_->AddResources(res_arr, &collection_key, &collection_key_size);

			if (SUCCEEDED(hr))
			{
				ComPtr<IDWriteFontCollection> font_collection;
				hr = d2d_res_->GetDWriteFactory()->CreateCustomFontCollection(
					res_font_collection_loader_.get(),
					collection_key,
					collection_key_size,
					&font_collection
				);

				if (SUCCEEDED(hr))
				{
					collection.SetFontCollection(font_collection);
				}
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load font failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateTextFormat(TextFormat& format, Font const& font)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<IDWriteTextFormat> output;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->CreateTextFormat(output, font);
		}

		if (SUCCEEDED(hr))
		{
			format.SetTextFormat(output);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateTextLayout(TextLayout& layout, String const& text, TextStyle const& style, TextFormat const& format)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<IDWriteTextLayout> output;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->CreateTextLayout(
				output,
				text,
				style,
				format.GetTextFormat()
			);
		}

		if (SUCCEEDED(hr))
		{
			layout.SetTextLayout(output);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateLineGeometry(Geometry& geo, Point const& begin_pos, Point const& end_pos)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
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
		if (!d2d_res_)
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
		if (!d2d_res_)
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
		if (!d2d_res_)
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
		if (!d2d_res_)
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

	void Renderer::CreateImageRenderTarget(ImageRenderTarget& render_target)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<ID2D1BitmapRenderTarget> output;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetDeviceContext()->CreateCompatibleRenderTarget(&output);
		}

		if (SUCCEEDED(hr))
		{
			hr = render_target.InitDeviceResources(output, d2d_res_);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::SetVSyncEnabled(bool enabled)
	{
		vsync_ = enabled;
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

	void Renderer::SetClearColor(const Color& color)
	{
		clear_color_ = color;
	}

	bool Renderer::CheckVisibility(Size const& content_size, Matrix3x2 const& transform)
	{
		return Rect{ Point{}, output_size_ }.Intersects(
			transform.Transform(Rect{ Point{}, content_size })
		);
	}

}
