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
#include <kiwano/core/Resource.h>
#include <kiwano/renderer/RenderTarget.h>
#include <kiwano/renderer/GifImage.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(GifSprite);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief GIF ����
	*/
	class KGE_API GifSprite
		: public Actor
	{
	public:
		/// \~chinese
		/// @brief GIF����ѭ�������ص�
		using LoopDoneCallback	= Function<void(int /* times */)>;

		/// \~chinese
		/// @brief GIF���Ž����ص�
		using DoneCallback		= Function<void()>;

		GifSprite();

		/// \~chinese
		/// @brief ����GIF����
		/// @param file_path GIFͼƬ·��
		GifSprite(String const& file_path);

		/// \~chinese
		/// @brief ����GIF����
		/// @param res GIFͼƬ��Դ
		GifSprite(Resource const& res);

		/// \~chinese
		/// @brief ����GIF����
		/// @param gif GIFͼƬ
		GifSprite(GifImagePtr gif);

		/// \~chinese
		/// @brief ����GIFͼƬ
		/// @param file_path GIFͼƬ·��
		bool Load(String const& file_path);

		/// \~chinese
		/// @brief ����GIFͼƬ
		/// @param res GIFͼƬ��Դ
		bool Load(Resource const& res);

		/// \~chinese
		/// @brief ����GIFͼƬ
		/// @param gif GIFͼƬ
		bool Load(GifImagePtr gif);

		/// \~chinese
		/// @brief ���� GIF ����ѭ������
		void SetLoopCount(int loops);

		/// \~chinese
		/// @brief ���� GIF ����ÿ��ѭ�������ص�����
		void SetLoopDoneCallback(LoopDoneCallback const& cb);

		/// \~chinese
		/// @brief ���� GIF ���������ص�����
		void SetDoneCallback(DoneCallback const& cb);

		/// \~chinese
		/// @brief ���� GIF ͼ��
		void SetGifImage(GifImagePtr gif);

		/// \~chinese
		/// @brief ���²��� GIF ����
		void RestartAnimation();

		/// \~chinese
		/// @brief ��ȡ GIF ����ѭ�������ص�
		LoopDoneCallback GetLoopDoneCallback() const;

		/// \~chinese
		/// @brief ��ȡ GIF �������Ž����ص�
		DoneCallback GetDoneCallback() const;

		/// \~chinese
		/// @brief ��ȡ GIF ͼƬ
		GifImagePtr GetGifImage() const;

		void OnRender(RenderTarget* rt) override;

	private:
		void Update(Duration dt) override;

		/// \~chinese
		/// @brief �Ƿ������һ֡
		bool IsLastFrame() const;

		/// \~chinese
		/// @brief �����Ƿ��ѽ���
		bool EndOfAnimation() const;

		/// \~chinese
		/// @brief �ϳ���һ֡
		void ComposeNextFrame();

		/// \~chinese
		/// @brief ������ǰͼ��֡
		void DisposeCurrentFrame();

		/// \~chinese
		/// @brief ������һ֡
		void OverlayNextFrame();

		/// \~chinese
		/// @brief ����ϳɺ��ͼ��֡
		void SaveComposedFrame();

		/// \~chinese
		/// @brief �ָ��ѱ����ͼ��֡
		void RestoreSavedFrame();

		/// \~chinese
		/// @brief ��յ�ǰͼ������
		void ClearCurrentFrameArea();

	private:
		bool				animating_;
		int					total_loop_count_;
		int					loop_count_;
		size_t				next_index_;
		Duration			frame_elapsed_;
		LoopDoneCallback	loop_cb_;
		DoneCallback		done_cb_;
		GifImagePtr			gif_;
		GifImage::Frame		frame_;
		TexturePtr			saved_frame_;
		TexturePtr			frame_to_render_;
		TextureRenderTarget	frame_rt_;
	};

	/** @} */

	inline void							GifSprite::SetLoopCount(int loops)							{ total_loop_count_ = loops; }

	inline void							GifSprite::SetLoopDoneCallback(LoopDoneCallback const& cb)	{ loop_cb_ = cb; }

	inline void							GifSprite::SetDoneCallback(DoneCallback const& cb)			{ done_cb_ = cb; }

	inline GifSprite::LoopDoneCallback	GifSprite::GetLoopDoneCallback() const						{ return loop_cb_; }

	inline GifSprite::DoneCallback		GifSprite::GetDoneCallback() const							{ return done_cb_; }

	inline GifImagePtr					GifSprite::GetGifImage() const								{ return gif_; }

	inline bool							GifSprite::IsLastFrame() const								{ return (next_index_ == 0); }

	inline bool							GifSprite::EndOfAnimation() const							{ return IsLastFrame() && loop_count_ == total_loop_count_ + 1; }
}
