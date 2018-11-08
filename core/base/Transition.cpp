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
#include "render.h"
#include "window.h"

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
		, out_layer_param_()
		, in_layer_param_()
	{
		duration_ = std::max(duration, 0.f);
	}

	Transition::~Transition()
	{
		SafeRelease(out_layer_);
		SafeRelease(in_layer_);
		SafeRelease(out_scene_);
		SafeRelease(in_scene_);
	}

	bool Transition::IsDone()
	{
		return done_;
	}

	void Transition::Initialize(Scene * prev, Scene * next, Game * game)
	{
		started_ = time::Now();
		out_scene_ = prev;
		in_scene_ = next;

		if (out_scene_)
			out_scene_->Retain();

		if (in_scene_)
			in_scene_->Retain();

		if (in_scene_)
		{
			ThrowIfFailed(
				render::D2D.HwndRenderTarget->CreateLayer(&in_layer_)
			);
		}

		if (out_scene_)
		{
			ThrowIfFailed(
				render::D2D.HwndRenderTarget->CreateLayer(&out_layer_)
			);
		}

		window_size_ = window::instance.GetSize();
		out_layer_param_ = in_layer_param_ = D2D1::LayerParameters(
			D2D1::RectF(
				0.f,
				0.f,
				window_size_.width,
				window_size_.height
			),
			nullptr,
			D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
			D2D1::Matrix3x2F::Identity(),
			1.f,
			render::D2D.SolidColorBrush,
			D2D1_LAYER_OPTIONS_NONE
		);
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
		auto render_target = render::D2D.HwndRenderTarget;

		if (out_scene_)
		{
			render_target->SetTransform(out_scene_->GetTransform());
			render_target->PushAxisAlignedClip(
				D2D1::RectF(
					0.f,
					0.f,
					window_size_.width,
					window_size_.height
				),
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
			);
			render_target->PushLayer(out_layer_param_, out_layer_);

			out_scene_->Draw();

			render_target->PopLayer();
			render_target->PopAxisAlignedClip();
		}

		if (in_scene_)
		{
			render_target->SetTransform(in_scene_->GetTransform());
			render_target->PushAxisAlignedClip(
				D2D1::RectF(
					0.f,
					0.f,
					window_size_.width,
					window_size_.height
				),
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
			);
			render_target->PushLayer(in_layer_param_, in_layer_);

			in_scene_->Draw();

			render_target->PopLayer();
			render_target->PopAxisAlignedClip();
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

	void BoxTransition::Initialize(Scene * prev, Scene * next, Game * game)
	{
		Transition::Initialize(prev, next, game);

		in_layer_param_.opacity = 0;
	}

	void BoxTransition::Update()
	{
		Transition::Update();

		if (process_ < .5f)
		{
			out_layer_param_.contentBounds = D2D1::RectF(
				window_size_.width * process_,
				window_size_.height * process_,
				window_size_.width * (1 - process_),
				window_size_.height * (1 - process_)
			);
		}
		else
		{
			out_layer_param_.opacity = 0;
			in_layer_param_.opacity = 1;
			in_layer_param_.contentBounds = D2D1::RectF(
				window_size_.width * (1 - process_),
				window_size_.height * (1 - process_),
				window_size_.width * process_,
				window_size_.height * process_
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

	void EmergeTransition::Initialize(Scene * prev, Scene * next, Game * game)
	{
		Transition::Initialize(prev, next, game);

		out_layer_param_.opacity = 1;
		in_layer_param_.opacity = 0;
	}

	void EmergeTransition::Update()
	{
		Transition::Update();

		out_layer_param_.opacity = 1 - process_;
		in_layer_param_.opacity = process_;
	}

	//-------------------------------------------------------
	// FadeTransition
	//-------------------------------------------------------

	FadeTransition::FadeTransition(float duration)
		: Transition(duration)
	{
	}

	void FadeTransition::Initialize(Scene * prev, Scene * next, Game * game)
	{
		Transition::Initialize(prev, next, game);

		out_layer_param_.opacity = 1;
		in_layer_param_.opacity = 0;
	}

	void FadeTransition::Update()
	{
		Transition::Update();

		if (process_ < 0.5)
		{
			out_layer_param_.opacity = 1 - process_ * 2;
			in_layer_param_.opacity = 0;
		}
		else
		{
			out_layer_param_.opacity = 0;
			in_layer_param_.opacity = (process_ - 0.5f) * 2;
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

	void MoveTransition::Initialize(Scene * prev, Scene * next, Game * game)
	{
		Transition::Initialize(prev, next, game);

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

	//-------------------------------------------------------
	// RotationTransition
	//-------------------------------------------------------

	RotationTransition::RotationTransition(float duration, float rotation)
		: Transition(duration)
		, rotation_(rotation)
	{
	}

	void RotationTransition::Initialize(Scene * prev, Scene * next, Game * game)
	{
		Transition::Initialize(prev, next, game);

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
