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
#include <kiwano/renderer/win32/D2DDeviceResources.h>
#include <kiwano/core/ObjectBase.h>

namespace kiwano
{
	class RenderTarget;
	class TextureRenderTarget;
	class Renderer;

	/**
	* \~chinese
	* @brief ��ֵģʽ
	* @details ��ֵģʽָ����λͼ�����ź���תʱ������ɫ�ļ��㷽ʽ
	*/
	enum class InterpolationMode
	{
		Linear,		///< ˫���Բ�ֵ������Χ�ĸ����ؽ����������Բ�ֵ����, ��ͼ��Ŵ�ʱ���ܻ�ģ��
		Nearest,	///< ���ڽ���ֵ��ȡ���ڽ������ص����ɫֵ
	};

	KGE_DECLARE_SMART_PTR(Texture);

	/**
	* \~chinese
	* @brief ����
	*/
	class KGE_API Texture
		: public ObjectBase
	{
		friend class RenderTarget;
		friend class TextureRenderTarget;
		friend class Renderer;

	public:
		Texture();

		virtual ~Texture();

		/// \~chinese
		/// @brief ���ر����ļ�
		bool Load(String const& file_path);

		/// \~chinese
		/// @brief ������Դ
		bool Load(Resource const& res);

		/// \~chinese
		/// @brief �Ƿ���Ч
		bool IsValid() const;

		/// \~chinese
		/// @brief ��ȡ������
		float GetWidth() const;

		/// \~chinese
		/// @brief ��ȡ����߶�
		float GetHeight() const;

		/// \~chinese
		/// @brief ��ȡ�����С
		Size GetSize() const;

		/// \~chinese
		/// @brief ��ȡ���ؿ��
		uint32_t GetWidthInPixels() const;

		/// \~chinese
		/// @brief ��ȡ���ظ߶�
		uint32_t GetHeightInPixels() const;

		/// \~chinese
		/// @brief ��ȡ���ش�С
		math::Vec2T<uint32_t> GetSizeInPixels() const;

		/// \~chinese
		/// @brief ��ȡ���ز�ֵ��ʽ
		InterpolationMode GetBitmapInterpolationMode() const;

		/// \~chinese
		/// @brief ��ȡ���ظ�ʽ
		D2D1_PIXEL_FORMAT GetPixelFormat() const;

		/// \~chinese
		/// @brief ��������
		/// @param copy_from Դ����
		void CopyFrom(TexturePtr copy_from);

		/// \~chinese
		/// @brief ��������
		/// @param copy_from Դ����
		/// @param src_rect Դ����ü�����
		/// @param dest_point ������Ŀ��λ��
		void CopyFrom(TexturePtr copy_from, Rect const& src_rect, Point const& dest_point);

		/// \~chinese
		/// @brief �������ز�ֵ��ʽ
		void SetInterpolationMode(InterpolationMode mode);

		/// \~chinese
		/// @brief ����Ĭ�ϵ����ز�ֵ��ʽ
		static void SetDefaultInterpolationMode(InterpolationMode mode);

		/// \~chinese
		/// @brief ��ȡĬ�ϵ����ز�ֵ��ʽ
		static InterpolationMode GetDefaultInterpolationMode();

	private:
		/// \~chinese
		/// @brief ��ȡԴλͼ
		ComPtr<ID2D1Bitmap> GetBitmap() const;

		/// \~chinese
		/// @brief ����Դλͼ
		void SetBitmap(ComPtr<ID2D1Bitmap> bitmap);

	private:
		ComPtr<ID2D1Bitmap>	bitmap_;
		InterpolationMode	interpolation_mode_;

		static InterpolationMode default_interpolation_mode_;
	};
}
