// Copyright (c) 2016-2018 Easy2D - Nomango
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

#include "..\e2dtransition.h"
#include "..\e2dobject.h"

namespace easy2d
{
	RotationTransition::RotationTransition(float duration, float rotation)
		: Transition(duration)
		, rotation_(rotation)
	{
	}

	void RotationTransition::Init(Scene * prev, Scene * next, Game * game)
	{
		Transition::Init(prev, next, game);

		if (out_scene_)
		{
			out_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
		}

		in_layer_param_.opacity = 0;
	}

	void RotationTransition::Update()
	{
		Transition::Update();

		auto center_pos = D2D1::Point2F(
			window_size_.width / 2,
			window_size_.height / 2
		);

		if (process_ < .5f)
		{
			if (out_scene_)
			{
				out_scene_->SetTransform(
					D2D1::Matrix3x2F::Scale(
					(.5f - process_) * 2,
						(.5f - process_) * 2,
						center_pos
					) * D2D1::Matrix3x2F::Rotation(
						rotation_ * (.5f - process_) * 2,
						center_pos
					)
				);
			}
		}
		else
		{
			if (in_scene_)
			{
				out_layer_param_.opacity = 0;
				in_layer_param_.opacity = 1;

				in_scene_->SetTransform(
					D2D1::Matrix3x2F::Scale(
					(process_ - .5f) * 2,
						(process_ - .5f) * 2,
						center_pos
					) * D2D1::Matrix3x2F::Rotation(
						rotation_ * (process_ - .5f) * 2,
						center_pos
					)
				);
			}
		}
	}

	void RotationTransition::Reset()
	{
		if (out_scene_)
		{
			out_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
		}
	}
}