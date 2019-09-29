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
#include "win32/D2DDeviceResources.h"

namespace kiwano
{
	// ��ֵģʽ
	// ��ֵģʽָ����λͼ�����ź���תʱ������ɫ�ļ��㷽ʽ
	// Linear (˫���Բ�ֵ): ����Χ�ĸ����ؽ����������Բ�ֵ����, ��ͼ��Ŵ�ʱ���ܻ�ģ����Ĭ�ϣ�
	// Nearest (���ڽ���ֵ): ȡ���ڽ������ص����ɫֵ
	enum class InterpolationMode
	{
		Linear,		// ˫���Բ�ֵ
		Nearest,	// ���ڽ���ֵ
	};


	// ����
	class KGE_API Texture
	{
	public:
		Texture();

		explicit Texture(
			String const& file_path
		);

		explicit Texture(
			Resource const& res
		);

		explicit Texture(
			ComPtr<ID2D1Bitmap> const& bitmap
		);

		virtual ~Texture();

		// ���ر����ļ�
		bool Load(
			String const& file_path
		);

		// ������Դ
		bool Load(
			Resource const& res
		);

		// ��Դ�Ƿ���Ч
		bool IsValid() const;

		// ��ȡ���
		float GetWidth() const;

		// ��ȡ�߶�
		float GetHeight() const;

		// ��ȡ��С
		Size GetSize() const;

		// ��ȡ���ؿ��
		std::uint32_t GetWidthInPixels() const;

		// ��ȡ���ظ߶�
		std::uint32_t GetHeightInPixels() const;

		// ��ȡ���ش�С
		math::Vec2T<std::uint32_t> GetSizeInPixels() const;

		// ��ȡ���ز�ֵ��ʽ
		InterpolationMode GetBitmapInterpolationMode() const;

		// ����λͼ�ڴ�
		void CopyFrom(Texture const& copy_from);

		// ����λͼ�ڴ�
		void CopyFrom(Texture const& copy_from, Rect const& src_rect, Point const& dest_point);

		// �������ز�ֵ��ʽ
		void SetInterpolationMode(InterpolationMode mode);

		// ����Ĭ�ϵ����ز�ֵ��ʽ
		static void SetDefaultInterpolationMode(InterpolationMode mode);

	public:
		// ��ȡԴλͼ
		ComPtr<ID2D1Bitmap> GetBitmap() const;

		// ����Դλͼ
		void SetBitmap(ComPtr<ID2D1Bitmap> bitmap);

		// ��ȡ���ظ�ʽ
		D2D1_PIXEL_FORMAT GetPixelFormat() const;

	protected:
		ComPtr<ID2D1Bitmap>	bitmap_;
		InterpolationMode	interpolation_mode_;

		static InterpolationMode default_interpolation_mode_;
	};
}
