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

#include "Sprite.h"
#include "../renderer/render.h"

namespace kiwano
{
	Sprite::Sprite()
		: frame_(nullptr)
	{
	}

	Sprite::Sprite(Resource const& res)
		: frame_(nullptr)
	{
		Load(res);
	}

	Sprite::Sprite(Resource const& res, const Rect& crop_rect)
		: frame_(nullptr)
	{
		Load(res);
		Crop(crop_rect);
	}

	Sprite::Sprite(FramePtr frame)
		: frame_(nullptr)
	{
		SetFrame(frame);
	}

	Sprite::~Sprite()
	{
	}

	bool Sprite::Load(Resource const& res)
	{
		FramePtr frame = new (std::nothrow) Frame;
		if (frame->Load(res))
		{
			SetFrame(frame);
			return true;
		}
		return false;
	}

	void Sprite::Crop(const Rect& crop_rect)
	{
		if (frame_)
		{
			frame_->Crop(crop_rect);
			SetSize(frame_->GetWidth(), frame_->GetHeight());
		}
	}

	void Sprite::SetFrame(FramePtr frame)
	{
		if (frame_ != frame)
		{
			frame_ = frame;
			if (frame_)
			{
				SetSize(frame_->GetWidth(), frame_->GetHeight());
			}
		}
	}

	void Sprite::OnRender(Renderer* renderer)
	{
		if (frame_ && renderer->CheckVisibility(size_, transform_matrix_))
		{
			PrepareRender(renderer);
			renderer->DrawBitmap(frame_->GetImage()->GetBitmap(), frame_->GetCropRect(), GetBounds());
		}
	}
}
