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
	MoveTransition::MoveTransition(float duration, Direction direction)
		: Transition(duration)
		, direction_(direction)
	{
	}

	void MoveTransition::Init(Scene * prev, Scene * next, Game * game)
	{
		Transition::Init(prev, next, game);

		switch (direction_)
		{
		case Direction::Up:
			pos_delta_ = Point(0, -window_size_.height);
			start_pos_ = Point(0, window_size_.height);
			break;
		case Direction::Down:
			pos_delta_ = Point(0, window_size_.height);
			start_pos_ = Point(0, -window_size_.height);
			break;
		case Direction::Left:
			pos_delta_ = Point(-window_size_.width, 0);
			start_pos_ = Point(window_size_.width, 0);
			break;
		case Direction::Right:
			pos_delta_ = Point(window_size_.width, 0);
			start_pos_ = Point(-window_size_.width, 0);
			break;
		}

		if (out_scene_)
		{
			out_scene_->SetTransform(D2D1::Matrix3x2F::Identity());
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(
				D2D1::Matrix3x2F::Translation(
					start_pos_.x,
					start_pos_.y
				)
			);
		}
	}

	void MoveTransition::Update()
	{
		Transition::Update();

		if (out_scene_)
		{
			auto translation = pos_delta_ * process_;
			out_scene_->SetTransform(
				D2D1::Matrix3x2F::Translation(
					translation.x,
					translation.y
				)
			);
		}

		if (in_scene_)
		{
			auto translation = start_pos_ + pos_delta_ * process_;
			in_scene_->SetTransform(
				D2D1::Matrix3x2F::Translation(
					translation.x,
					translation.y
				)
			);
		}
	}

	void MoveTransition::Reset()
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