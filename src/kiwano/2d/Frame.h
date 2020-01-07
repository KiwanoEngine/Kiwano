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
#include <kiwano/core/ObjectBase.h>
#include <kiwano/renderer/Texture.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Frame);

	/**
	* \~chinese
	* @brief ͼ��֡
	*/
	class KGE_API Frame
		: public ObjectBase
	{
	public:
		/// \~chinese
		/// @brief ������ͼ��֡
		Frame();

		/// \~chinese
		/// @brief ����ͼ��
		/// @param file_path ͼ��·��
		bool Load(String const& file_path);

		/// \~chinese
		/// @brief ����ͼ��
		/// @param res ͼ����Դ
		bool Load(Resource const& res);

		/// \~chinese
		/// @brief �ü�ͼ��֡Ϊ����
		/// @param crop_rect �ü����ζ���
		void SetCropRect(Rect const& crop_rect);

		/// \~chinese
		/// @brief ��������
		/// @param texture ����
		void SetTexture(TexturePtr texture);

		/// \~chinese
		/// @brief �Ƿ���Ч
		bool IsValid() const;

		/// \~chinese
		/// @brief ��ȡ���
		float GetWidth() const;

		/// \~chinese
		/// @brief ��ȡ�߶�
		float GetHeight() const;

		/// \~chinese
		/// @brief ��ȡ��С
		Size GetSize() const;

		/// \~chinese
		/// @brief ��ȡ�ü�λ��
		Point GetCropPoint() const;

		/// \~chinese
		/// @brief ��ȡ�ü�����
		Rect const& GetCropRect() const;

		/// \~chinese
		/// @brief ��ȡ����
		TexturePtr GetTexture() const;

	private:
		TexturePtr texture_;
		Rect crop_rect_;
	};

	inline bool				Frame::IsValid() const		{ return texture_ && texture_->IsValid(); }
	inline float			Frame::GetWidth() const		{ return crop_rect_.GetWidth(); }
	inline float			Frame::GetHeight() const	{ return crop_rect_.GetHeight(); }
	inline Size				Frame::GetSize() const		{ return crop_rect_.GetSize(); }
	inline Point			Frame::GetCropPoint() const { return crop_rect_.GetLeftTop(); }
	inline Rect const&		Frame::GetCropRect() const	{ return crop_rect_; }
	inline TexturePtr		Frame::GetTexture() const	{ return texture_; }
}
