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
#include "../renderer/Image.h"

namespace kiwano
{
	// ֡ͼ��
	class KGE_API Frame
		: public ObjectBase
	{
	public:
		Frame();

		explicit Frame(
			String const& file_path
		);

		explicit Frame(
			Resource const& res
		);

		explicit Frame(
			Image const& image
		);

		bool Load(
			String const& file_path
		);

		bool Load(
			Resource const& res
		);

		// �ü�����
		void SetCropRect(
			Rect const& crop_rect	/* �ü����� */
		);

		// ����λͼ
		void SetImage(
			Image const& image
		);

		// ��ȡ���
		Float32 GetWidth() const					{ return crop_rect_.size.x; }

		// ��ȡ�߶�
		Float32 GetHeight() const					{ return crop_rect_.size.y; }

		// ��ȡ��С
		Size GetSize() const					{ return crop_rect_.size; }

		// ��ȡ�ü�λ��
		Point GetCropPoint() const				{ return crop_rect_.origin; }

		// ��ȡ�ü�����
		inline Rect const& GetCropRect() const	{ return crop_rect_; }

		// ��ȡλͼ
		inline Image const& GetImage() const	{ return image_; }

	protected:
		Image image_;
		Rect crop_rect_;
	};
}
