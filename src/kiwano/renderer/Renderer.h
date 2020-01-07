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
	* \defgroup Render ��Ⱦ����
	*/

	/**
	* \addtogroup Render
	* @{
	*/

	/**
	* \~chinese
	* @brief ��Ⱦ����
	*/
	struct RenderConfig
	{
		Color	clear_color;	///< ������ɫ
		bool	vsync;			///< ��ֱͬ��

		RenderConfig(
			Color clear_color	= Color::Black,
			bool vsync			= true
		);
	};


	/**
	* \~chinese
	* @brief ��Ⱦ��
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
		/// @brief ��ȡ������ɫ
		Color const& GetClearColor() const;

		/// \~chinese
		/// @brief ����������ɫ
		void SetClearColor(Color const& clear_color);

		/// \~chinese
		/// @brief ������رմ�ֱͬ��
		void SetVSyncEnabled(bool enabled);

		/// \~chinese
		/// @brief ����DPI
		void SetDpi(float dpi);

		/// \~chinese
		/// @brief ��������
		/// @param[out] texture ����
		/// @param[in] file_path ͼƬ·��
		void CreateTexture(
			Texture& texture,
			String const& file_path
		);

		/// \~chinese
		/// @brief ��������
		/// @param[out] texture ����
		/// @param[in] resource ͼƬ��Դ
		void CreateTexture(
			Texture& texture,
			Resource const& resource
		);

		/// \~chinese
		/// @brief ����GIFͼ��
		/// @param[out] gif GIFͼ��
		/// @param[in] file_path ͼƬ·��
		void CreateGifImage(
			GifImage& gif,
			String const& file_path
		);

		/// \~chinese
		/// @brief ����GIFͼ��
		/// @param[out] gif GIFͼ��
		/// @param[in] resource ͼƬ��Դ
		void CreateGifImage(
			GifImage& gif,
			Resource const& resource
		);

		/// \~chinese
		/// @brief ����GIFͼ��֡
		/// @param[out] frame GIFͼ��֡
		/// @param[in] gif GIFͼ��
		/// @param[in] frame_index ֡�±�
		void CreateGifImageFrame(
			GifImage::Frame& frame,
			GifImage const& gif,
			size_t frame_index
		);

		/// \~chinese
		/// @brief �������弯
		/// @param[out] font ����
		/// @param[in] file_paths �����ļ�·��
		void CreateFontCollection(
			Font& font,
			Vector<String> const& file_paths
		);

		/// \~chinese
		/// @brief �������弯
		/// @param[out] font ����
		/// @param[in] res_arr ������Դ
		void CreateFontCollection(
			Font& font,
			Vector<Resource> const& res_arr
		);

		/// \~chinese
		/// @brief �������ָ�ʽ
		/// @param[out] layout ���岼��
		void CreateTextFormat(
			TextLayout& layout
		);

		/// \~chinese
		/// @brief �������ֲ���
		/// @param[out] layout ���岼��
		void CreateTextLayout(
			TextLayout& layout
		);

		/// \~chinese
		/// @brief �����߶���״
		/// @param[out] geo ��״
		/// @param[in] begin_pos �߶����
		/// @param[in] end_pos �߶��յ�
		void CreateLineGeometry(
			Geometry& geo,
			Point const& begin_pos,
			Point const& end_pos
		);

		/// \~chinese
		/// @brief ����������״
		/// @param[out] geo ��״
		/// @param[in] rect ���δ�С
		void CreateRectGeometry(
			Geometry& geo,
			Rect const& rect
		);

		/// \~chinese
		/// @brief ����Բ�Ǿ�����״
		/// @param[out] geo ��״
		/// @param[in] rect ���δ�С
		/// @param[in] radius Բ�ǰ뾶
		void CreateRoundedRectGeometry(
			Geometry& geo,
			Rect const& rect,
			Vec2 const& radius
		);

		/// \~chinese
		/// @brief ������Բ��״
		/// @param[out] geo ��״
		/// @param[in] center ��ԲԲ��
		/// @param[in] radius ��Բ�뾶
		void CreateEllipseGeometry(
			Geometry& geo,
			Point const& center,
			Vec2 const& radius
		);

		/// \~chinese
		/// @brief ��������ͼ��������
		/// @param[out] sink ��״������
		void CreateGeometrySink(
			GeometrySink& sink
		);

		/// \~chinese
		/// @brief ����������ȾĿ��
		/// @param[out] render_target ��ȾĿ��
		void CreateTextureRenderTarget(
			TextureRenderTargetPtr& render_target
		);

		/// \~chinese
		/// @brief ������ɫ��ˢ
		/// @param[out] brush ��ˢ
		/// @param[in] color ��ɫ
		void CreateSolidBrush(
			Brush& brush,
			Color const& color
		);

		/// \~chinese
		/// @brief �������Խ��仭ˢ
		/// @param[out] brush ��ˢ
		/// @param[in] begin ������ʼ��
		/// @param[in] end ������ֹ��
		/// @param[in] stops ����ת���㼯��
		/// @param[in] extend_mode ��������ģʽ
		void CreateLinearGradientBrush(
			Brush& brush,
			Point const& begin,
			Point const& end,
			Vector<GradientStop> const& stops,
			GradientExtendMode extend_mode
		);

		/// \~chinese
		/// @brief �������򽥱仭ˢ
		/// @param[out] brush ��ˢ
		/// @param[in] center ���򽥱�Բ��
		/// @param[in] offset ���򽥱�ƫ��
		/// @param[in] radius ���򽥱�뾶
		/// @param[in] stops ����ת���㼯��
		/// @param[in] extend_mode ��������ģʽ
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
		/// @brief ��ȡĿ�괰��
		HWND GetTargetWindow() const;

		/// \~chinese
		/// @brief ��ȡ��Ⱦ�����С
		Size const& GetOutputSize() const;

		/// \~chinese
		/// @brief ��ȡDirect2D�豸��Դ
		ID2DDeviceResources* GetD2DDeviceResources();

		/// \~chinese
		/// @brief ��ȡDirect3D�豸��Դ
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
