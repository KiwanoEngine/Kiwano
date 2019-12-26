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

#include <kiwano/2d/Sprite.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	Sprite::Sprite()
	{
	}

	Sprite::Sprite(String const& file_path)
	{
		Load(file_path);
	}

	Sprite::Sprite(String const& file_path, Rect const& crop_rect)
	{
		Load(file_path);
		SetCropRect(crop_rect);
	}

	Sprite::Sprite(Resource const& res)
	{
		Load(res);
	}

	Sprite::Sprite(Resource const& res, const Rect& crop_rect)
	{
		Load(res);
		SetCropRect(crop_rect);
	}

	Sprite::Sprite(FramePtr frame)
		: frame_(nullptr)
	{
		SetFrame(frame);
	}

	Sprite::~Sprite()
	{
	}

	bool Sprite::Load(String const& file_path)
	{
		FramePtr frame = new (std::nothrow) Frame;
		if (frame->Load(file_path))
		{
			SetFrame(frame);
			return true;
		}
		return false;
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

	void Sprite::SetCropRect(const Rect& crop_rect)
	{
		if (frame_)
		{
			frame_->SetCropRect(crop_rect);
			SetSize(Size{ frame_->GetWidth(), frame_->GetHeight() });
		}
	}

	void Sprite::SetFrame(FramePtr frame)
	{
		if (frame_ != frame)
		{
			frame_ = frame;
			if (frame_)
			{
				SetSize(Size{ frame_->GetWidth(), frame_->GetHeight() });
			}
		}
	}

	void Sprite::OnRender(RenderTarget* rt)
	{
		if (frame_ && CheckVisibilty(rt))
		{
			PrepareRender(rt);

			rt->DrawTexture(*frame_->GetTexture(), &frame_->GetCropRect(), &GetBounds());
		}
	}
}
