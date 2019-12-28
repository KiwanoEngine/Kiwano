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
#include <kiwano/core/Component.h>
#include <kiwano/renderer/win32/FontCollectionLoader.h>
#include <kiwano/renderer/RenderTarget.h>
#include <kiwano/renderer/GifImage.h>
#include <kiwano/renderer/Font.h>
#include <kiwano/renderer/TextStyle.hpp>

#if defined(KGE_USE_DIRECTX10)
#	include "win32/D3D10DeviceResources.h"
#else
#	include "win32/D3D11DeviceResources.h"
#endif

namespace kiwano
{

#if defined(KGE_USE_DIRECTX10)
	typedef ID3D10DeviceResources ID3DDeviceResources;
#else
	typedef ID3D11DeviceResources ID3DDeviceResources;
#endif

	// 渲染设置
	struct RenderConfig
	{
		Color	clear_color;	// 清屏颜色
		bool	vsync;			// 垂直同步

		RenderConfig(
			Color clear_color	= Color::Black,
			bool vsync			= true
		);
	};


	// 渲染器
	class KGE_API Renderer
		: public Singleton<Renderer>
		, public RenderComponent
		, public EventComponent
		, public RenderTarget
	{
		friend Singleton<Renderer>;

	public:
		// 设置清屏颜色
		void SetClearColor(
			Color const& clear_color
		);

		// 开启或关闭垂直同步
		void SetVSyncEnabled(
			bool enabled
		);

		void CreateTexture(
			Texture& texture,
			String const& file_path
		);

		void CreateTexture(
			Texture& texture,
			Resource const& resource
		);

		void CreateGifImage(
			GifImage& gif,
			String const& file_path
		);

		void CreateGifImage(
			GifImage& gif,
			Resource const& resource
		);

		void CreateGifImageFrame(
			GifImage::Frame& frame,
			GifImage const& gif,
			size_t frame_index
		);

		void CreateFontCollection(
			Font& font,
			Vector<String> const& file_paths
		);

		void CreateFontCollection(
			Font& font,
			Vector<Resource> const& res_arr
		);

		void CreateTextFormat(
			TextLayout& layout
		);

		void CreateTextLayout(
			TextLayout& layout
		);

		void CreateLineGeometry(
			Geometry& geo,
			Point const& begin_pos,
			Point const& end_pos
		);

		void CreateRectGeometry(
			Geometry& geo,
			Rect const& rect
		);

		void CreateRoundedRectGeometry(
			Geometry& geo,
			Rect const& rect,
			Vec2 const& radius
		);

		void CreateEllipseGeometry(
			Geometry& geo,
			Point const& center,
			Vec2 const& radius
		);

		void CreateGeometrySink(
			GeometrySink& sink
		);

		void CreateTextureRenderTarget(
			TextureRenderTargetPtr& render_target
		);

		void CreateSolidBrush(
			Brush& brush,
			Color const& color
		);

		void CreateLinearGradientBrush(
			Brush& brush,
			Point const& begin,
			Point const& end,
			Vector<GradientStop> const& stops,
			GradientExtendMode extend_mode
		);

		void CreateRadialGradientBrush(
			Brush& brush,
			Point const& center,
			Vec2 const& offset,
			Vec2 const& radius,
			Vector<GradientStop> const& stops,
			GradientExtendMode extend_mode
		);

		void SetDpi(
			float dpi
		);

	public:
		void Init(RenderConfig const& config);

		void SetupComponent() override;

		void DestroyComponent() override;

		void BeforeRender() override;

		void AfterRender() override;

		void HandleMessage(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam) override;

	public:
		inline HWND						GetTargetWindow() const			{ return hwnd_; }

		inline Size const&				GetOutputSize() const			{ return output_size_; }

		inline Color const&				GetClearColor() const			{ return clear_color_; }

		inline ID2DDeviceResources*		GetD2DDeviceResources()			{ KGE_ASSERT(d2d_res_); return d2d_res_.get(); }

		inline ID3DDeviceResources*		GetD3DDeviceResources()			{ KGE_ASSERT(d3d_res_); return d3d_res_.get(); }

	private:
		Renderer();

		~Renderer();

		HRESULT HandleDeviceLost();

		void ResizeTarget(uint32_t width, uint32_t height);

	private:
		bool	vsync_;
		HWND	hwnd_;
		Color	clear_color_;
		Size	output_size_;

		ComPtr<ID2DDeviceResources>				d2d_res_;
		ComPtr<ID3DDeviceResources>				d3d_res_;
		ComPtr<ID2D1DrawingStateBlock>			drawing_state_block_;
		ComPtr<IFontCollectionLoader>			font_collection_loader_;
		ComPtr<IResourceFontFileLoader>			res_font_file_loader_;
		ComPtr<IResourceFontCollectionLoader>	res_font_collection_loader_;
	};
}
