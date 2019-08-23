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
#include "../base/Component.h"
#include "win32/FontCollectionLoader.h"
#include "RenderTarget.h"
#include "GifImage.h"

#if defined(KGE_USE_DIRECTX10)
#	include "D3D10DeviceResources.h"
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

	// 分辨率模式
	// 分辨率模式决定了将画面渲染到视区上的方式
	// Fixed (固定): 分辨率不随视区改变, 且画面始终与视区边界对齐（默认）
	// Center (居中): 分辨率不随视区改变, 且画面始终在视区上居中
	// Stretch (拉伸): 分辨率始终随视区等比例拉伸
	// Adaptive (宽高自适应): 分辨率始终保持宽高比, 且尽可能的填充视区, 可能会出现黑色边界
	enum class ResolutionMode
	{
		Fixed,		/* 固定 */
		Center,		/* 居中 */
		Stretch,	/* 拉伸 */
		Adaptive,	/* 宽高自适应 */
	};

	class KGE_API Renderer
		: public Singleton<Renderer>
		, public Component
		, public RenderTarget
	{
		KGE_DECLARE_SINGLETON(Renderer);

	public:
		// 设置清屏颜色
		void SetClearColor(
			Color const& clear_color
		);

		// 开启或关闭垂直同步
		void SetVSyncEnabled(
			bool enabled
		);

		// 设置画面分辨率
		void SetResolution(
			Size const& resolution
		);

		// 设置分辨率模式
		void SetResolutionMode(
			ResolutionMode mode
		);

	public:
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
			UInt32 frame_index
		);

		void CreateFontCollection(
			FontCollection& collection,
			Vector<String> const& file_paths
		);

		void CreateFontCollection(
			FontCollection& collection,
			Vector<Resource> const& res_arr
		);

		void CreateTextFormat(
			TextFormat& format,
			Font const& font
		);

		void CreateTextLayout(
			TextLayout& layout,
			String const& text,
			TextStyle const& style,
			TextFormat const& format
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

		void CreatePathGeometrySink(
			GeometrySink& sink
		);

		void CreateTextureRenderTarget(
			TextureRenderTarget& render_target
		);

	public:
		void Init(RenderConfig const& config);

		void SetupComponent() override;

		void DestroyComponent() override;

		void BeforeRender() override;

		void AfterRender() override;

		void HandleMessage(HWND hwnd, UInt32 msg, WPARAM wparam, LPARAM lparam) override;

	public:
		inline HWND						GetTargetWindow() const			{ return hwnd_; }

		inline Size const&				GetOutputSize() const			{ return output_size_; }

		inline Size const&				GetResolution() const			{ return resolution_; }

		inline Color const&				GetClearColor() const			{ return clear_color_; }

		inline ID2DDeviceResources*		GetD2DDeviceResources() const	{ KGE_ASSERT(d2d_res_); return d2d_res_.get(); }

		inline ID3DDeviceResources*		GetD3DDeviceResources() const	{ KGE_ASSERT(d3d_res_); return d3d_res_.get(); }

	private:
		Renderer();

		~Renderer();

		HRESULT CreateDeviceResources();

		HRESULT HandleDeviceLost();

		void ResizeTarget(UInt32 width, UInt32 height);

		void UpdateResolution();

	private:
		bool			vsync_;
		HWND			hwnd_;
		Color			clear_color_;
		Size			output_size_;
		Size			resolution_;
		ResolutionMode	resolution_mode_;

		ComPtr<ID2DDeviceResources>		d2d_res_;
		ComPtr<ID3DDeviceResources>		d3d_res_;
		ComPtr<ID2D1DrawingStateBlock>	drawing_state_block_;

		ComPtr<IFontCollectionLoader>			font_collection_loader_;
		ComPtr<IResourceFontFileLoader>			res_font_file_loader_;
		ComPtr<IResourceFontCollectionLoader>	res_font_collection_loader_;
	};
}
