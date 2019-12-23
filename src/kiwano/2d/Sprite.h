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
#include <kiwano/2d/Actor.h>
#include <kiwano/2d/Frame.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Sprite);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief ����
	*/
	class KGE_API Sprite
		: public Actor
	{
	public:
		Sprite();

		/// \~chinese
		/// @brief �ӱ���ͼƬ���쾫��
		/// @param file_path ����ͼƬ·��
		explicit Sprite(String const& file_path);

		/// \~chinese
		/// @brief ��ͼƬ��Դ���쾫��
		/// @param res ͼƬ��Դ
		explicit Sprite(Resource const& res);

		/// \~chinese
		/// @brief ��ͼ��֡���쾫��
		/// @param[in] frame ͼ��֡
		explicit Sprite(FramePtr frame);

		/// \~chinese
		/// @brief �ӱ���ͼƬ���쾫�鲢�ü�
		/// @param file_path ����ͼƬ·��
		/// @param crop_rect �ü�����
		Sprite(String const& file_path, Rect const& crop_rect);

		/// \~chinese
		/// @brief ��ͼƬ��Դ���쾫�鲢�ü�
		/// @param res ͼƬ��Դ
		/// @param crop_rect �ü�����
		Sprite(Resource const& res, Rect const& crop_rect);

		virtual ~Sprite();

		/// \~chinese
		/// @brief ���ر���ͼƬ
		/// @param file_path ����ͼƬ·��
		bool Load(String const& file_path);

		/// \~chinese
		/// @brief ����ͼ����Դ
		/// @param res ͼƬ��Դ
		bool Load(Resource const& res);

		/// \~chinese
		/// @brief ʹ�þ�������ü�����
		/// @param crop_rect �ü�����
		void SetCropRect(const Rect& crop_rect);

		/// \~chinese
		/// @brief ��ȡ֡ͼ��
		FramePtr GetFrame() const;

		/// \~chinese
		/// @brief ����ͼ��֡
		/// @param[in] frame ͼ��֡
		void SetFrame(FramePtr frame);

		void OnRender(RenderTarget* rt) override;

	private:
		FramePtr frame_;
	};

	/** @} */

	inline FramePtr Sprite::GetFrame() const { return frame_; }

}
