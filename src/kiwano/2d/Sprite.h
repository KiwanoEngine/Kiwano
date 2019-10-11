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
	// ¾«Áé
	class KGE_API Sprite
		: public Actor
	{
	public:
		Sprite();

		explicit Sprite(
			String const& file_path
		);

		Sprite(
			String const& file_path,
			Rect const& crop_rect	/* ²Ã¼ô¾ØÐÎ */
		);

		explicit Sprite(
			Resource const& res
		);

		Sprite(
			Resource const& res,
			Rect const& crop_rect	/* ²Ã¼ô¾ØÐÎ */
		);

		explicit Sprite(
			FramePtr frame
		);

		virtual ~Sprite();

		// ¼ÓÔØÍ¼Ïñ
		bool Load(
			String const& file_path
		);

		// ¼ÓÔØÍ¼Ïñ×ÊÔ´
		bool Load(
			Resource const& res
		);

		// ²Ã¼ô¾ØÐÎ
		void SetCropRect(
			const Rect& crop_rect
		);

		// »ñÈ¡Ö¡Í¼Ïñ
		inline FramePtr GetFrame() const { return frame_; }

		// ÉèÖÃÖ¡Í¼Ïñ
		void SetFrame(FramePtr frame);

		// äÖÈ¾¾«Áé
		void OnRender(RenderTarget* rt) override;

	protected:
		FramePtr frame_;
	};
}
