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

#include "Transition.h"
#include "Node.h"
#include "Scene.h"
#include "window.h"
#include "render.h"
#include "../math/Matrix.hpp"

namespace easy2d
{
	//-------------------------------------------------------
	// Transition
	//-------------------------------------------------------

	Transition::Transition(float duration)
		: done_(false)
		, started_()
		, process_(0)
		, window_size_()
		, out_scene_(nullptr)
		, in_scene_(nullptr)
		, out_layer_(nullptr)
		, in_layer_(nullptr)
		, out_layer_prop_()
		, in_layer_prop_()
	{
		duration_ = std::max(duration, 0.f);
	}

	Transition::~Transition()
	{
		SafeRelease(out_layer_);
		SafeRelease(in_layer_);
	}

	bool Transition::IsDone()
	{
		return done_;
	}

	void Transition::Init(spScene const& prev, spScene const& next)
	{
		started_ = time::Now();
		out_scene_ = prev;
		in_scene_ = next;

		if (in_scene_)
		{
			ThrowIfFailed(
				devices::Graphics::Instance().CreateLayer(&in_layer_)
			);
		}

		if (out_scene_)
		{
			ThrowIfFailed(
				devices::Graphics::Instance().CreateLayer(&out_layer_)
			);
		}

		window_size_ = Window::Instance().GetSize();
		out_layer_prop_ = in_layer_prop_ = LayerProperties{ Rect(Point(), window_size_),1.f };
	}

	void Transition::Update()
	{
		if (duration_ == 0)
		{
			process_ = 1;
		}
		else
		{
			process_ = (time::Now() - started_).Seconds() / duration_;
			process_ = std::min(process_, 1.f);
		}

		if (process_ >= 1)
		{
			this->Stop();
		}
	}

	void Transition::Draw()
	{
		if (out_scene_)
		{
			devices::Graphics::Instance().PushClip(
				out_scene_->GetTransform(),
				window_size_
			);
			devices::Graphics::Instance().PushLayer(out_layer_, out_layer_prop_);

			out_scene_->Draw();

			devices::Graphics::Instance().PopLayer();
			devices::Graphics::Instance().PopClip();
		}

		if (in_scene_)
		{
			devices::Graphics::Instance().PushClip(
				in_scene_->GetTransform(),
				window_size_
			);
			devices::Graphics::Instance().PushLayer(in_layer_, in_layer_prop_);

			in_scene_->Draw();

			devices::Graphics::Instance().PopLayer();
			devices::Graphics::Instance().PopClip();
		}
	}

	void Transition::Stop()
	{
		done_ = true;
		Reset();
	}

	//-------------------------------------------------------
	// BoxTransition
	//-------------------------------------------------------

	BoxTransition::BoxTransition(float duration)
		: Transition(duration)
	{
	}

	void BoxTransition::Init(spScene const& prev, spScene const& next)
	{
		Transition::Init(prev, next);

		in_layer_prop_.opacity = 0;
	}

	void BoxTransition::Update()
	{
		Transition::Update();

		if (process_ < .5f)
		{
			out_layer_prop_.area = Rect(
				window_size_.width * process_,
				window_size_.height * process_,
				window_size_.width * (1 - process_ * 2),
				window_size_.height * (1 - process_ * 2)
			);
		}
		else
		{
			out_layer_prop_.opacity = 0;
			in_layer_prop_.opacity = 1;
			in_layer_prop_.area = Rect(
				window_size_.width * (1 - process_),
				window_size_.height * (1 - process_),
				window_size_.width * (2 * process_ - 1),
				window_size_.height * (2 * process_ - 1)
			);
		}
	}

	//-------------------------------------------------------
	// EmergeTransition
	//-------------------------------------------------------

	EmergeTransition::EmergeTransition(float duration)
		: Transition(duration)
	{
	}

	void EmergeTransition::Init(spScene const& prev, spScene const& next)
	{
		Transition::Init(prev, next);

		out_layer_prop_.opacity = 1;
		in_layer_prop_.opacity = 0;
	}

	void EmergeTransition::Update()
	{
		Transition::Update();

		out_layer_prop_.opacity = 1 - process_;
		in_layer_prop_.opacity = process_;
	}

	//-------------------------------------------------------
	// FadeTransition
	//-------------------------------------------------------

	FadeTransition::FadeTransition(float duration)
		: Transition(duration)
	{
	}

	void FadeTransition::Init(spScene const& prev, spScene const& next)
	{
		Transition::Init(prev, next);

		out_layer_prop_.opacity = 1;
		in_layer_prop_.opacity = 0;
	}

	void FadeTransition::Update()
	{
		Transition::Update();

		if (process_ < 0.5)
		{
			out_layer_prop_.opacity = 1 - process_ * 2;
			in_layer_prop_.opacity = 0;
		}
		else
		{
			out_layer_prop_.opacity = 0;
			in_layer_prop_.opacity = (process_ - 0.5f) * 2;
		}
	}

	//-------------------------------------------------------
	// MoveTransition
	//-------------------------------------------------------

	MoveTransition::MoveTransition(float duration, Direction direction)
		: Transition(duration)
		, direction_(direction)
	{
	}

	void MoveTransition::Init(spScene const& prev, spScene const& next)
	{
		Transition::Init(prev, next);

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
			out_scene_->SetTransform(math::Matrix());
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(
				math::Matrix::Translation(
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
				math::Matrix::Translation(
					translation.x,
					translation.y
				)
			);
		}

		if (in_scene_)
		{
			auto translation = start_pos_ + pos_delta_ * process_;
			in_scene_->SetTransform(
				math::Matrix::Translation(
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
			out_scene_->SetTransform(math::Matrix());
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(math::Matrix());
		}
	}

	//-------------------------------------------------------
	// RotationTransition
	//-------------------------------------------------------

	RotationTransition::RotationTransition(float duration, float rotation)
		: Transition(duration)
		, rotation_(rotation)
	{
	}

	void RotationTransition::Init(spScene const& prev, spScene const& next)
	{
		Transition::Init(prev, next);

		if (out_scene_)
		{
			out_scene_->SetTransform(math::Matrix());
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(math::Matrix());
		}

		in_layer_prop_.opacity = 0;
	}

	void RotationTransition::Update()
	{
		Transition::Update();

		auto center_pos = math::Vector2(
			window_size_.width / 2,
			window_size_.height / 2
		);

		if (process_ < .5f)
		{
			if (out_scene_)
			{
				out_scene_->SetTransform(
					math::Matrix::Scaling(
						(.5f - process_) * 2,
						(.5f - process_) * 2,
						center_pos
					) * math::Matrix::Rotation(
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
				out_layer_prop_.opacity = 0;
				in_layer_prop_.opacity = 1;

				in_scene_->SetTransform(
					math::Matrix::Scaling(
					(process_ - .5f) * 2,
						(process_ - .5f) * 2,
						center_pos
					) * math::Matrix::Rotation(
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
			out_scene_->SetTransform(math::Matrix());
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(math::Matrix());
		}
	}
}
