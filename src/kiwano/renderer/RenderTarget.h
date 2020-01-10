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

#pragma once
#include <kiwano/core/time.h>
#include <kiwano/core/ObjectBase.h>
#include <kiwano/renderer/Brush.h>
#include <kiwano/renderer/Texture.h>
#include <kiwano/renderer/Geometry.h>
#include <kiwano/renderer/TextLayout.h>
#include <kiwano/renderer/LayerArea.h>
#include <kiwano/renderer/win32/TextRenderer.h>

namespace kiwano
{
	class Renderer;

	KGE_DECLARE_SMART_PTR(RenderTarget);
	KGE_DECLARE_SMART_PTR(TextureRenderTarget);

	/**
	* \addtogroup Render
	* @{
	*/

	/// \~chinese
	/// @brief ���ֿ����ģʽ
	enum class TextAntialiasMode
	{
		Default,	///< ϵͳĬ��
		ClearType,	///< ClearType �����
		GrayScale,	///< �Ҷȿ����
		None		///< �����ÿ����
	};


	/// \~chinese
	/// @brief ��ȾĿ��
	/// @details ��ȾĿ�꽫��ɻ���ͼԪ�Ļ��ƣ��������ƽ��������ض���Ŀ���У��細�ڻ�����
	class KGE_API RenderTarget
		: public virtual ObjectBase
	{
		friend class Renderer;

	public:
		/// \~chinese
		/// @brief �Ƿ���Ч
		bool IsValid() const;

		/// \~chinese
		/// @brief �Ƿ���Ч
		void BeginDraw();

		/// \~chinese
		/// @brief �Ƿ���Ч
		void EndDraw();

		/// \~chinese
		/// @brief �Ƿ���Ч
		void DrawGeometry(
			Geometry const& geometry,
			float stroke_width,
			const StrokeStyle& stroke = StrokeStyle()
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void FillGeometry(
			Geometry const& geometry
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void DrawLine(
			Point const& point1,
			Point const& point2,
			float stroke_width,
			const StrokeStyle& stroke = StrokeStyle()
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void DrawRectangle(
			Rect const& rect,
			float stroke_width,
			const StrokeStyle& stroke = StrokeStyle()
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void FillRectangle(
			Rect const& rect
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void DrawRoundedRectangle(
			Rect const& rect,
			Vec2 const& radius,
			float stroke_width,
			const StrokeStyle& stroke = StrokeStyle()
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void FillRoundedRectangle(
			Rect const& rect,
			Vec2 const& radius
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void DrawEllipse(
			Point const& center,
			Vec2 const& radius,
			float stroke_width,
			const StrokeStyle& stroke = StrokeStyle()
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void FillEllipse(
			Point const& center,
			Vec2 const& radius
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void DrawTexture(
			Texture const& texture,
			Rect const& src_rect,
			Rect const& dest_rect
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void DrawTexture(
			Texture const& texture,
			const Rect* src_rect = nullptr,
			const Rect* dest_rect = nullptr
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void DrawTextLayout(
			TextLayout const& layout,
			Point const& offset = Point{}
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void CreateTexture(
			Texture& texture,
			math::Vec2T<uint32_t> size,
			D2D1_PIXEL_FORMAT format
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void PushClipRect(Rect const& clip_rect);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void PopClipRect();

		/// \~chinese
		/// @brief �Ƿ���Ч
		void PushLayer(LayerArea& layer);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void PopLayer();

		/// \~chinese
		/// @brief �Ƿ���Ч
		void Clear();

		/// \~chinese
		/// @brief �Ƿ���Ч
		void Clear(Color const& clear_color);

		/// \~chinese
		/// @brief �Ƿ���Ч
		float GetBrushOpacity() const;

		/// \~chinese
		/// @brief �Ƿ���Ч
		BrushPtr GetCurrentBrush() const;

		/// \~chinese
		/// @brief �Ƿ���Ч
		Matrix3x2 GetGlobalTransform() const;

		/// \~chinese
		/// @brief �Ƿ���Ч
		void SetBrushOpacity(
			float opacity
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void SetCurrentBrush(
			BrushPtr brush
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void SetTransform(
			const Matrix3x2& matrix
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void SetGlobalTransform(
			const Matrix3x2& matrix
		);

		/// \~chinese
		/// @brief �Ƿ���Ч
		void SetGlobalTransform(
			const Matrix3x2* matrix
		);

		/// \~chinese
		/// @brief ���ÿ����ģʽ
		void SetAntialiasMode(
			bool enabled
		);

		/// \~chinese
		/// @brief �������ֿ����ģʽ
		void SetTextAntialiasMode(
			TextAntialiasMode mode
		);

		/// \~chinese
		/// @brief ���߽��Ƿ���������
		bool CheckVisibility(
			Rect const& bounds,
			Matrix3x2 const& transform
		);

		/// \~chinese
		/// @brief ������ȾĿ���С
		void Resize(Size const& size);

	public:
		/// \~chinese
		/// @brief ��ȾĿ��״̬
		struct Status
		{
			uint32_t primitives;	///< ��ȾͼԪ����
			Time start;				///< ��Ⱦ��ʼʱ��
			Duration duration;		///< ��Ⱦʱ��

			Status();
		};

		/// \~chinese
		/// @brief ���û����״̬�ռ�����
		void SetCollectingStatus(bool enable);

		/// \~chinese
		/// @brief ��ȡ��ȾĿ��״̬
		Status const& GetStatus() const;

	protected:
		RenderTarget();

		ComPtr<ID2D1RenderTarget> GetRenderTarget() const;

		ComPtr<ITextRenderer> GetTextRenderer() const;

	private:
		/// \~chinese
		/// @brief �����豸������Դ
		HRESULT CreateDeviceResources(ComPtr<ID2D1Factory> factory, ComPtr<ID2D1RenderTarget> rt);

		/// \~chinese
		/// @brief �����豸������Դ
		void DiscardDeviceResources();

		/// \~chinese
		/// @brief ������ȾͼԪ����
		void IncreasePrimitivesCount(uint32_t increase = 1) const;

	private:
		bool							antialias_;
		bool							fast_global_transform_;
		float							brush_opacity_;
		TextAntialiasMode				text_antialias_;
		ComPtr<ITextRenderer>			text_renderer_;
		ComPtr<ID2D1RenderTarget>		render_target_;
		BrushPtr						current_brush_;
		Rect							visible_size_;
		Matrix3x2						global_transform_;

		mutable bool					collecting_status_;
		mutable Status					status_;
	};


	/// \~chinese
	/// @brief ������ȾĿ��
	/// @details ������ȾĿ�꽫��Ⱦ�����һ�����������
	class KGE_API TextureRenderTarget
		: public RenderTarget
	{
		friend class Renderer;

	public:
		/// \~chinese
		/// @brief �Ƿ���Ч
		bool IsValid() const;

		/// \~chinese
		/// @brief ��ȡ��Ⱦ���
		/// @param[out] texture �������
		/// @return �����Ƿ�ɹ�
		bool GetOutput(Texture& texture);

	private:
		TextureRenderTarget();

		ComPtr<ID2D1BitmapRenderTarget> GetBitmapRenderTarget() const;

		void SetBitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> rt);

	private:
		ComPtr<ID2D1BitmapRenderTarget> bitmap_rt_;
	};

	/** @} */


	inline RenderTarget::Status::Status()
		: primitives(0)
	{
	}

	inline RenderTarget::Status const& RenderTarget::GetStatus() const
	{
		return status_;
	}

	inline ComPtr<ID2D1RenderTarget> RenderTarget::GetRenderTarget() const
	{
		KGE_ASSERT(render_target_);
		return render_target_;
	}

	inline ComPtr<ITextRenderer> RenderTarget::GetTextRenderer() const
	{
		KGE_ASSERT(text_renderer_);
		return text_renderer_;
	}

	inline float RenderTarget::GetBrushOpacity() const
	{
		return brush_opacity_;
	}

	inline BrushPtr RenderTarget::GetCurrentBrush() const
	{
		return current_brush_;
	}

	inline Matrix3x2 RenderTarget::GetGlobalTransform() const
	{
		return global_transform_;
	}

	inline void RenderTarget::SetBrushOpacity(float opacity)
	{
		brush_opacity_ = opacity;
	}

	inline void RenderTarget::SetGlobalTransform(const Matrix3x2& matrix)
	{
		SetGlobalTransform(&matrix);
	}

	inline void RenderTarget::SetCurrentBrush(BrushPtr brush)
	{
		current_brush_ = brush;
		if (current_brush_)
		{
			current_brush_->SetOpacity(brush_opacity_);
		}
	}

	inline bool TextureRenderTarget::IsValid() const
	{
		return bitmap_rt_ != nullptr;
	}

	inline ComPtr<ID2D1BitmapRenderTarget> TextureRenderTarget::GetBitmapRenderTarget() const
	{
		return bitmap_rt_;
	}

	inline void TextureRenderTarget::SetBitmapRenderTarget(ComPtr<ID2D1BitmapRenderTarget> rt)
	{
		bitmap_rt_ = rt;
	}
}
