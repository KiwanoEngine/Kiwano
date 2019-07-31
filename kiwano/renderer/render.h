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

#pragma once
#include "../base/time.h"
#include "../base/Component.h"
#include "../base/Resource.h"
#include "../2d/include-forwards.h"
#include "../2d/Font.hpp"
#include "../2d/TextStyle.hpp"
#include "helper.hpp"
#include "TextRenderer.h"
#include "D2DDeviceResources.h"

#if defined(KGE_USE_DIRECTX10)
#	include "D3D10DeviceResources.h"
#else
#	include "D3D11DeviceResources.h"
#endif

namespace kiwano
{

#if defined(KGE_USE_DIRECTX10)
	typedef ID3D10DeviceResources ID3DDeviceResources;
#else
	typedef ID3D11DeviceResources ID3DDeviceResources;
#endif

	class KGE_API Renderer
		: public Singleton<Renderer>
		, public Component
	{
		KGE_DECLARE_SINGLETON(Renderer);

	public:
		HRESULT CreateLayer(
			ComPtr<ID2D1Layer>& layer
		);

		HRESULT DrawGeometry(
			ComPtr<ID2D1Geometry> const& geometry,
			const Color& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		HRESULT FillGeometry(
			ComPtr<ID2D1Geometry> const& geometry,
			Color const& fill_color
		);

		HRESULT DrawRectangle(
			Rect const& rect,
			const Color& stroke_color,
			float stroke_width,
			StrokeStyle stroke = StrokeStyle::Miter
		);

		HRESULT FillRectangle(
			Rect const& rect,
			Color const& fill_color
		);

		HRESULT DrawImage(
			ImagePtr image,
			Rect const& dest_rect
		);

		HRESULT DrawBitmap(
			ComPtr<ID2D1Bitmap> const& bitmap,
			Rect const& src_rect,
			Rect const& dest_rect
		);

		HRESULT DrawTextLayout(
			ComPtr<IDWriteTextLayout> const& text_layout
		);

		// ����������ɫ
		void SetClearColor(
			Color const& clear_color
		);

		// ���ÿ����ģʽ
		HRESULT SetAntialiasMode(
			bool enabled
		);

		// �������ֿ����ģʽ
		HRESULT SetTextAntialiasMode(
			TextAntialias mode
		);

		// ������رմ�ֱͬ��
		void SetVSyncEnabled(
			bool enabled
		);

		// ���û���͸����
		HRESULT SetOpacity(
			float opacity
		);

		HRESULT SetTransform(
			const Matrix& matrix
		);

		HRESULT SetTextStyle(
			const Color& color,
			bool has_outline,
			const Color& outline_color,
			float outline_width,
			StrokeStyle outline_stroke
		);

		HRESULT PushClip(
			const Matrix& clip_matrix,
			const Size& clip_size
		);

		HRESULT PopClip();

		HRESULT PushLayer(
			ComPtr<ID2D1Layer> const& layer,
			LayerProperties const& properties
		);

		HRESULT PopLayer();

		HRESULT Resize(
			UINT width,
			UINT height
		);

	public:
		void SetupComponent(Application*) override;

		void DestroyComponent() override;

		void BeforeRender() override;

		void AfterRender() override;

		void HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;

		void SetCollectingStatus(bool collecting);

	public:
		struct Status
		{
			Time start;
			Duration duration;
			int primitives;
		};

		inline HWND						GetTargetWindow() const			{ return hwnd_; }

		inline Status const&			GetStatus() const				{ return status_; }

		inline Size const&				GetOutputSize() const			{ return output_size_; }

		inline ID2DDeviceResources*		GetD2DDeviceResources() const	{ KGE_ASSERT(d2d_res_); return d2d_res_.Get(); }

		inline ID3DDeviceResources*		GetD3DDeviceResources() const	{ KGE_ASSERT(d3d_res_); return d3d_res_.Get(); }

		inline ITextRenderer*			GetTextRenderer() const			{ KGE_ASSERT(text_renderer_); return text_renderer_.Get(); }

		inline ID2D1SolidColorBrush*	GetSolidColorBrush() const		{ KGE_ASSERT(solid_color_brush_); return solid_color_brush_.Get(); }

	private:
		Renderer();

		~Renderer();

		HRESULT CreateDeviceResources();

		HRESULT HandleDeviceLost();

		HRESULT BeginDraw();

		HRESULT EndDraw();

	private:
		HWND hwnd_;
		float opacity_;
		bool antialias_;
		bool vsync_;
		bool collecting_status_;

		Size			output_size_;
		Color			clear_color_;
		TextAntialias	text_antialias_;
		Status			status_;

		ComPtr<ID2DDeviceResources>		d2d_res_;
		ComPtr<ID3DDeviceResources>		d3d_res_;
		ComPtr<ID2D1DeviceContext>		device_context_;
		ComPtr<ID2D1DrawingStateBlock>	drawing_state_block_;
		ComPtr<ITextRenderer>			text_renderer_;
		ComPtr<ID2D1SolidColorBrush>	solid_color_brush_;
	};
}