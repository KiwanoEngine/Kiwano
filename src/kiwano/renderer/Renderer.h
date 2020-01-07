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

	/**
	* \~chinese
	* \defgroup Render 渲染引擎
	*/

	/**
	* \addtogroup Render
	* @{
	*/

	/**
	* \~chinese
	* @brief 渲染设置
	*/
	struct RenderConfig
	{
		Color	clear_color;	///< 清屏颜色
		bool	vsync;			///< 垂直同步

		RenderConfig(
			Color clear_color	= Color::Black,
			bool vsync			= true
		);
	};


	/**
	* \~chinese
	* @brief 渲染器
	*/
	class KGE_API Renderer
		: public Singleton<Renderer>
		, public RenderComponent
		, public EventComponent
		, public RenderTarget
	{
		friend Singleton<Renderer>;

	public:
		/// \~chinese
		/// @brief 获取清屏颜色
		Color const& GetClearColor() const;

		/// \~chinese
		/// @brief 设置清屏颜色
		void SetClearColor(Color const& clear_color);

		/// \~chinese
		/// @brief 开启或关闭垂直同步
		void SetVSyncEnabled(bool enabled);

		/// \~chinese
		/// @brief 设置DPI
		void SetDpi(float dpi);

		/// \~chinese
		/// @brief 创建纹理
		/// @param[out] texture 纹理
		/// @param[in] file_path 图片路径
		void CreateTexture(
			Texture& texture,
			String const& file_path
		);

		/// \~chinese
		/// @brief 创建纹理
		/// @param[out] texture 纹理
		/// @param[in] resource 图片资源
		void CreateTexture(
			Texture& texture,
			Resource const& resource
		);

		/// \~chinese
		/// @brief 创建GIF图像
		/// @param[out] gif GIF图像
		/// @param[in] file_path 图片路径
		void CreateGifImage(
			GifImage& gif,
			String const& file_path
		);

		/// \~chinese
		/// @brief 创建GIF图像
		/// @param[out] gif GIF图像
		/// @param[in] resource 图片资源
		void CreateGifImage(
			GifImage& gif,
			Resource const& resource
		);

		/// \~chinese
		/// @brief 创建GIF图像帧
		/// @param[out] frame GIF图像帧
		/// @param[in] gif GIF图像
		/// @param[in] frame_index 帧下标
		void CreateGifImageFrame(
			GifImage::Frame& frame,
			GifImage const& gif,
			size_t frame_index
		);

		/// \~chinese
		/// @brief 创建字体集
		/// @param[out] font 字体
		/// @param[in] file_paths 字体文件路径
		void CreateFontCollection(
			Font& font,
			Vector<String> const& file_paths
		);

		/// \~chinese
		/// @brief 创建字体集
		/// @param[out] font 字体
		/// @param[in] res_arr 字体资源
		void CreateFontCollection(
			Font& font,
			Vector<Resource> const& res_arr
		);

		/// \~chinese
		/// @brief 创建文字格式
		/// @param[out] layout 字体布局
		void CreateTextFormat(
			TextLayout& layout
		);

		/// \~chinese
		/// @brief 创建文字布局
		/// @param[out] layout 字体布局
		void CreateTextLayout(
			TextLayout& layout
		);

		/// \~chinese
		/// @brief 创建线段形状
		/// @param[out] geo 形状
		/// @param[in] begin_pos 线段起点
		/// @param[in] end_pos 线段终点
		void CreateLineGeometry(
			Geometry& geo,
			Point const& begin_pos,
			Point const& end_pos
		);

		/// \~chinese
		/// @brief 创建矩形形状
		/// @param[out] geo 形状
		/// @param[in] rect 矩形大小
		void CreateRectGeometry(
			Geometry& geo,
			Rect const& rect
		);

		/// \~chinese
		/// @brief 创建圆角矩形形状
		/// @param[out] geo 形状
		/// @param[in] rect 矩形大小
		/// @param[in] radius 圆角半径
		void CreateRoundedRectGeometry(
			Geometry& geo,
			Rect const& rect,
			Vec2 const& radius
		);

		/// \~chinese
		/// @brief 创建椭圆形状
		/// @param[out] geo 形状
		/// @param[in] center 椭圆圆心
		/// @param[in] radius 椭圆半径
		void CreateEllipseGeometry(
			Geometry& geo,
			Point const& center,
			Vec2 const& radius
		);

		/// \~chinese
		/// @brief 创建几何图形生成器
		/// @param[out] sink 形状生成器
		void CreateGeometrySink(
			GeometrySink& sink
		);

		/// \~chinese
		/// @brief 创建纹理渲染目标
		/// @param[out] render_target 渲染目标
		void CreateTextureRenderTarget(
			TextureRenderTargetPtr& render_target
		);

		/// \~chinese
		/// @brief 创建纯色画刷
		/// @param[out] brush 画刷
		/// @param[in] color 颜色
		void CreateSolidBrush(
			Brush& brush,
			Color const& color
		);

		/// \~chinese
		/// @brief 创建线性渐变画刷
		/// @param[out] brush 画刷
		/// @param[in] begin 渐变起始点
		/// @param[in] end 渐变终止点
		/// @param[in] stops 渐变转换点集合
		/// @param[in] extend_mode 渐变扩充模式
		void CreateLinearGradientBrush(
			Brush& brush,
			Point const& begin,
			Point const& end,
			Vector<GradientStop> const& stops,
			GradientExtendMode extend_mode
		);

		/// \~chinese
		/// @brief 创建径向渐变画刷
		/// @param[out] brush 画刷
		/// @param[in] center 径向渐变圆心
		/// @param[in] offset 径向渐变偏移
		/// @param[in] radius 径向渐变半径
		/// @param[in] stops 渐变转换点集合
		/// @param[in] extend_mode 渐变扩充模式
		void CreateRadialGradientBrush(
			Brush& brush,
			Point const& center,
			Vec2 const& offset,
			Vec2 const& radius,
			Vector<GradientStop> const& stops,
			GradientExtendMode extend_mode
		);

	public:
		/// \~chinese
		/// @brief 获取目标窗口
		HWND GetTargetWindow() const;

		/// \~chinese
		/// @brief 获取渲染输出大小
		Size const& GetOutputSize() const;

		/// \~chinese
		/// @brief 获取Direct2D设备资源
		ID2DDeviceResources* GetD2DDeviceResources();

		/// \~chinese
		/// @brief 获取Direct3D设备资源
		ID3DDeviceResources* GetD3DDeviceResources();

	public:
		void Init(RenderConfig const& config);

		void SetupComponent() override;

		void DestroyComponent() override;

		void BeforeRender() override;

		void AfterRender() override;

		void HandleMessage(HWND hwnd, UINT32 msg, WPARAM wparam, LPARAM lparam) override;

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

	/** @} */

	inline HWND					Renderer::GetTargetWindow() const	{ return hwnd_; }

	inline Size const&			Renderer::GetOutputSize() const		{ return output_size_; }

	inline Color const&			Renderer::GetClearColor() const		{ return clear_color_; }

	inline ID2DDeviceResources*	Renderer::GetD2DDeviceResources()	{ KGE_ASSERT(d2d_res_); return d2d_res_.get(); }

	inline ID3DDeviceResources*	Renderer::GetD3DDeviceResources()	{ KGE_ASSERT(d3d_res_); return d3d_res_.get(); }
}
