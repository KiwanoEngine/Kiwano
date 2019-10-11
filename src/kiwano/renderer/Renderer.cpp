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
#include "../base/win32/helper.h"
#include "../base/Window.h"
#include "../utils/FileSystem.h"

namespace kiwano
{
	RenderConfig::RenderConfig(Color clear_color, bool vsync)
		: clear_color(clear_color)
		, vsync(vsync)
	{
	}

	Renderer::Renderer()
		: hwnd_(nullptr)
		, vsync_(true)
		, clear_color_(Color::Black)
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Init(RenderConfig const& config)
	{
		SetClearColor(config.clear_color);
		SetVSyncEnabled(config.vsync);
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
			hr = ID3DDeviceResources::Create(
				&d3d_res_,
				d2d_res_.get(),
				hwnd_
			);
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

		RenderTarget::DiscardDeviceResources();

		d2d_res_->GetDWriteFactory()->UnregisterFontFileLoader(res_font_file_loader_.get());
		res_font_file_loader_.reset();

		d2d_res_->GetDWriteFactory()->UnregisterFontCollectionLoader(res_font_collection_loader_.get());
		res_font_collection_loader_.reset();

		drawing_state_block_.reset();
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
			hr = d3d_res_->ClearRenderTarget(clear_color_);
		}

		if (SUCCEEDED(hr))
		{
			render_target_->SaveDrawingState(drawing_state_block_.get());
			BeginDraw();
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

	void Renderer::HandleMessage(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_SIZE:
		{
			std::uint32_t width = LOWORD(lparam);
			std::uint32_t height = HIWORD(lparam);

			ResizeTarget(width, height);
			break;
		}
		}
	}

	HRESULT Renderer::CreateDeviceResources()
	{
		KGE_ASSERT(d2d_res_);

		HRESULT hr = RenderTarget::CreateDeviceResources(
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

	void Renderer::CreateTexture(Texture& texture, String const& file_path)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (!FileSystem::GetInstance()->IsFileExists(file_path))
		{
			KGE_WARNING_LOG(L"Texture file '%s' not found!", file_path.c_str());
			hr = E_FAIL;
		}

		if (SUCCEEDED(hr))
		{
			String full_path = FileSystem::GetInstance()->GetFullPathForFile(file_path);

			ComPtr<IWICBitmapDecoder> decoder;
			hr = d2d_res_->CreateBitmapDecoderFromFile(decoder, full_path);

			if (SUCCEEDED(hr))
			{
				ComPtr<IWICBitmapFrameDecode> source;
				hr = decoder->GetFrame(0, &source);

				if (SUCCEEDED(hr))
				{
					ComPtr<IWICFormatConverter> converter;
					hr = d2d_res_->CreateBitmapConverter(
						converter,
						source,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						nullptr,
						0.f,
						WICBitmapPaletteTypeMedianCut
					);

					if (SUCCEEDED(hr))
					{
						ComPtr<ID2D1Bitmap> bitmap;
						hr = d2d_res_->CreateBitmapFromConverter(
							bitmap,
							nullptr,
							converter
						);

						if (SUCCEEDED(hr))
						{
							texture.SetBitmap(bitmap);
						}
					}
				}
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load texture failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateTexture(Texture& texture, Resource const& resource)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<IWICBitmapDecoder> decoder;
			hr = d2d_res_->CreateBitmapDecoderFromResource(decoder, resource);

			if (SUCCEEDED(hr))
			{
				ComPtr<IWICBitmapFrameDecode> source;
				hr = decoder->GetFrame(0, &source);

				if (SUCCEEDED(hr))
				{
					ComPtr<IWICFormatConverter> converter;
					hr = d2d_res_->CreateBitmapConverter(
						converter,
						source,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						nullptr,
						0.f,
						WICBitmapPaletteTypeMedianCut
					);

					if (SUCCEEDED(hr))
					{
						ComPtr<ID2D1Bitmap> bitmap;
						hr = d2d_res_->CreateBitmapFromConverter(
							bitmap,
							nullptr,
							converter
						);

						if (SUCCEEDED(hr))
						{
							texture.SetBitmap(bitmap);
						}
					}
				}
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load texture failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateGifImage(GifImage& gif, String const& file_path)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (!FileSystem::GetInstance()->IsFileExists(file_path))
		{
			KGE_WARNING_LOG(L"Gif texture file '%s' not found!", file_path.c_str());
			hr = E_FAIL;
		}

		if (SUCCEEDED(hr))
		{
			String full_path = FileSystem::GetInstance()->GetFullPathForFile(file_path);

			ComPtr<IWICBitmapDecoder> decoder;
			hr = d2d_res_->CreateBitmapDecoderFromFile(decoder, full_path);

			if (SUCCEEDED(hr))
			{
				gif.SetDecoder(decoder);
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load GIF texture failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateGifImage(GifImage& gif, Resource const& resource)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<IWICBitmapDecoder> decoder;
			hr = d2d_res_->CreateBitmapDecoderFromResource(decoder, resource);

			if (SUCCEEDED(hr))
			{
				gif.SetDecoder(decoder);
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load GIF texture failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateGifImageFrame(GifImage::Frame& frame, GifImage const& gif, std::size_t frame_index)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (gif.GetDecoder() == nullptr)
		{
			hr = E_INVALIDARG;
		}

		if (SUCCEEDED(hr))
		{
			ComPtr<IWICBitmapFrameDecode> wic_frame;
			HRESULT hr = gif.GetDecoder()->GetFrame(UINT(frame_index), &wic_frame);

			if (SUCCEEDED(hr))
			{
				ComPtr<IWICFormatConverter> converter;
				d2d_res_->CreateBitmapConverter(
					converter,
					wic_frame,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					nullptr,
					0.f,
					WICBitmapPaletteTypeCustom
				);

				if (SUCCEEDED(hr))
				{
					ComPtr<ID2D1Bitmap> raw_bitmap;
					hr = d2d_res_->CreateBitmapFromConverter(
						raw_bitmap,
						nullptr,
						converter
					);

					if (SUCCEEDED(hr))
					{
						frame.raw.SetBitmap(raw_bitmap);
					}
				}
			}

			if (SUCCEEDED(hr))
			{
				PROPVARIANT prop_val;
				PropVariantInit(&prop_val);

				// Get Metadata Query Reader from the frame
				ComPtr<IWICMetadataQueryReader> metadata_reader;
				hr = wic_frame->GetMetadataQueryReader(&metadata_reader);

				// Get the Metadata for the current frame
				if (SUCCEEDED(hr))
				{
					hr = metadata_reader->GetMetadataByName(L"/imgdesc/Left", &prop_val);
					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
						if (SUCCEEDED(hr))
						{
							frame.rect.left_top.x = static_cast<float>(prop_val.uiVal);
						}
						PropVariantClear(&prop_val);
					}
				}

				if (SUCCEEDED(hr))
				{
					hr = metadata_reader->GetMetadataByName(L"/imgdesc/Top", &prop_val);
					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
						if (SUCCEEDED(hr))
						{
							frame.rect.left_top.y = static_cast<float>(prop_val.uiVal);
						}
						PropVariantClear(&prop_val);
					}
				}

				if (SUCCEEDED(hr))
				{
					hr = metadata_reader->GetMetadataByName(L"/imgdesc/Width", &prop_val);
					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
						if (SUCCEEDED(hr))
						{
							frame.rect.right_bottom.x = frame.rect.left_top.x + static_cast<float>(prop_val.uiVal);
						}
						PropVariantClear(&prop_val);
					}
				}

				if (SUCCEEDED(hr))
				{
					hr = metadata_reader->GetMetadataByName(L"/imgdesc/Height", &prop_val);
					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
						if (SUCCEEDED(hr))
						{
							frame.rect.right_bottom.y = frame.rect.left_top.y + static_cast<float>(prop_val.uiVal);
						}
						PropVariantClear(&prop_val);
					}
				}

				if (SUCCEEDED(hr))
				{
					hr = metadata_reader->GetMetadataByName(L"/grctlext/Delay", &prop_val);

					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);

						if (SUCCEEDED(hr))
						{
							std::uint32_t udelay = 0;
							hr = UIntMult(prop_val.uiVal, 10, &udelay);
							if (SUCCEEDED(hr))
							{
								frame.delay.SetMilliseconds(static_cast<long>(udelay));
							}
						}
						PropVariantClear(&prop_val);
					}
					else
					{
						frame.delay = 0;
					}
				}

				if (SUCCEEDED(hr))
				{
					hr = metadata_reader->GetMetadataByName(L"/grctlext/Disposal", &prop_val);

					if (SUCCEEDED(hr))
					{
						hr = (prop_val.vt == VT_UI1) ? S_OK : E_FAIL;
						if (SUCCEEDED(hr))
						{
							frame.disposal_type = GifImage::DisposalType(prop_val.bVal);
						}
						::PropVariantClear(&prop_val);
					}
					else
					{
						frame.disposal_type = GifImage::DisposalType::Unknown;
					}
				}

				::PropVariantClear(&prop_val);
			}
		}

		if (FAILED(hr))
		{
			KGE_WARNING_LOG(L"Load GIF frame failed with HRESULT of %08X!", hr);
		}
	}

	void Renderer::CreateFontCollection(FontCollection& collection, Vector<String> const& file_paths)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		Vector<String> full_paths(file_paths);

		if (SUCCEEDED(hr))
		{
			for (auto& file_path : full_paths)
			{
				if (!FileSystem::GetInstance()->IsFileExists(file_path))
				{
					KGE_WARNING_LOG(L"Font file '%s' not found!", file_path.c_str());
					hr = E_FAIL;
				}

				file_path = FileSystem::GetInstance()->GetFullPathForFile(file_path);
			}
		}

		if (SUCCEEDED(hr))
		{
			LPVOID collection_key = nullptr;
			std::uint32_t collection_key_size = 0;

			hr = font_collection_loader_->AddFilePaths(full_paths, &collection_key, &collection_key_size);

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
			std::uint32_t collection_key_size = 0;

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

	void Renderer::CreateTextLayout(TextLayout& layout, String const& text, TextFormat const& format)
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

	void Renderer::CreateTextureRenderTarget(TextureRenderTarget& render_target)
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
			hr = render_target.CreateDeviceResources(output, d2d_res_);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateSolidBrush(Brush& brush, Color const& color)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		ComPtr<ID2D1SolidColorBrush> output;
		if (SUCCEEDED(hr))
		{
			hr = d2d_res_->GetDeviceContext()->CreateSolidColorBrush(DX::ConvertToColorF(color), &output);
		}

		if (SUCCEEDED(hr))
		{
			brush.SetBrush(output);
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateLinearGradientBrush(Brush& brush, Point const& begin, Point const& end, Vector<GradientStop> const& stops, GradientExtendMode extend_mode)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			ID2D1GradientStopCollection* collection = nullptr;
			hr = d2d_res_->GetDeviceContext()->CreateGradientStopCollection(
				reinterpret_cast<const D2D1_GRADIENT_STOP*>(&stops[0]),
				UINT32(stops.size()),
				D2D1_GAMMA_2_2,
				D2D1_EXTEND_MODE(extend_mode),
				&collection
			);

			if (SUCCEEDED(hr))
			{
				ComPtr<ID2D1LinearGradientBrush> output;
				hr = d2d_res_->GetDeviceContext()->CreateLinearGradientBrush(
					D2D1::LinearGradientBrushProperties(
						DX::ConvertToPoint2F(begin),
						DX::ConvertToPoint2F(end)
					),
					collection,
					&output
				);

				if (SUCCEEDED(hr))
				{
					brush.SetBrush(output);
				}
			}
		}

		ThrowIfFailed(hr);
	}

	void Renderer::CreateRadialGradientBrush(Brush& brush, Point const& center, Vec2 const& offset, Vec2 const& radius,
		Vector<GradientStop> const& stops, GradientExtendMode extend_mode)
	{
		HRESULT hr = S_OK;
		if (!d2d_res_)
		{
			hr = E_UNEXPECTED;
		}

		if (SUCCEEDED(hr))
		{
			ID2D1GradientStopCollection* collection = nullptr;
			hr = d2d_res_->GetDeviceContext()->CreateGradientStopCollection(
				reinterpret_cast<const D2D1_GRADIENT_STOP*>(&stops[0]),
				UINT32(stops.size()),
				D2D1_GAMMA_2_2,
				D2D1_EXTEND_MODE(extend_mode),
				&collection
			);

			if (SUCCEEDED(hr))
			{
				ComPtr<ID2D1RadialGradientBrush> output;
				hr = d2d_res_->GetDeviceContext()->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(
						DX::ConvertToPoint2F(center),
						DX::ConvertToPoint2F(offset),
						radius.x,
						radius.y
					),
					collection,
					&output
				);

				if (SUCCEEDED(hr))
				{
					brush.SetBrush(output);
				}
			}
		}

		ThrowIfFailed(hr);
	}

	void Renderer::SetVSyncEnabled(bool enabled)
	{
		vsync_ = enabled;
	}

	void Renderer::SetClearColor(const Color& color)
	{
		clear_color_ = color;
	}

	void Renderer::ResizeTarget(std::uint32_t width, std::uint32_t height)
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

}
