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

#include "Transition.h"
#include "Node.h"
#include "Scene.h"
#include "../base/window.h"
#include "../base/logs.h"
#include "../renderer/render.h"

namespace kiwano
{
	//-------------------------------------------------------
	// Transition
	//-------------------------------------------------------

	Transition::Transition(Duration duration)
		: done_(false)
		, duration_(duration)
		, delta_()
		, process_(0)
		, window_size_()
		, out_scene_(nullptr)
		, in_scene_(nullptr)
		, out_layer_(nullptr)
		, in_layer_(nullptr)
		, out_layer_prop_()
		, in_layer_prop_()
	{
	}

	Transition::~Transition()
	{
	}

	bool Transition::IsDone()
	{
		return done_;
	}

	void Transition::Init(ScenePtr const& prev, ScenePtr const& next)
	{
		process_ = 0;
		delta_ = Duration{};

		out_scene_ = prev;
		in_scene_ = next;

		if (in_scene_)
		{
			ThrowIfFailed(
				Renderer::Instance().CreateLayer(in_layer_)
			);
		}

		if (out_scene_)
		{
			ThrowIfFailed(
				Renderer::Instance().CreateLayer(out_layer_)
			);
		}

		window_size_ = Renderer::Instance().GetOutputSize();
		out_layer_prop_ = in_layer_prop_ = LayerProperties{ Rect(Point(), window_size_),1.f };
	}

	void Transition::Update(Duration dt)
	{
		if (duration_.IsZero())
		{
			process_ = 1;
		}
		else
		{
			delta_ += dt;
			process_ = std::min(delta_ / duration_, 1.f);
		}

		if (process_ >= 1)
		{
			this->Stop();
		}
	}

	void Transition::Render()
	{
		auto& renderer = Renderer::Instance();

		if (out_scene_)
		{
			renderer.PushClip(
				out_scene_->GetTransformMatrix(),
				window_size_
			);
			renderer.PushLayer(out_layer_, out_layer_prop_);

			out_scene_->Render();

			renderer.PopLayer();
			renderer.PopClip();
		}

		if (in_scene_)
		{
			renderer.PushClip(
				in_scene_->GetTransformMatrix(),
				window_size_
			);
			renderer.PushLayer(in_layer_, in_layer_prop_);

			in_scene_->Render();

			renderer.PopLayer();
			renderer.PopClip();
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

	BoxTransition::BoxTransition(Duration duration)
		: Transition(duration)
	{
	}

	void BoxTransition::Init(ScenePtr const& prev, ScenePtr const& next)
	{
		Transition::Init(prev, next);

		in_layer_prop_.opacity = 0;
	}

	void BoxTransition::Update(Duration dt)
	{
		Transition::Update(dt);

		if (process_ < .5f)
		{
			out_layer_prop_.area = Rect(
				window_size_.x * process_,
				window_size_.y * process_,
				window_size_.x * (1 - process_ * 2),
				window_size_.y * (1 - process_ * 2)
			);
		}
		else
		{
			out_layer_prop_.opacity = 0;
			in_layer_prop_.opacity = 1;
			in_layer_prop_.area = Rect(
				window_size_.x * (1 - process_),
				window_size_.y * (1 - process_),
				window_size_.x * (2 * process_ - 1),
				window_size_.y * (2 * process_ - 1)
			);
		}
	}

	//-------------------------------------------------------
	// EmergeTransition
	//-------------------------------------------------------

	EmergeTransition::EmergeTransition(Duration duration)
		: Transition(duration)
	{
	}

	void EmergeTransition::Init(ScenePtr const& prev, ScenePtr const& next)
	{
		Transition::Init(prev, next);

		out_layer_prop_.opacity = 1;
		in_layer_prop_.opacity = 0;
	}

	void EmergeTransition::Update(Duration dt)
	{
		Transition::Update(dt);

		out_layer_prop_.opacity = 1 - process_;
		in_layer_prop_.opacity = process_;
	}

	//-------------------------------------------------------
	// FadeTransition
	//-------------------------------------------------------

	FadeTransition::FadeTransition(Duration duration)
		: Transition(duration)
	{
	}

	void FadeTransition::Init(ScenePtr const& prev, ScenePtr const& next)
	{
		Transition::Init(prev, next);

		out_layer_prop_.opacity = 1;
		in_layer_prop_.opacity = 0;
	}

	void FadeTransition::Update(Duration dt)
	{
		Transition::Update(dt);

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

	MoveTransition::MoveTransition(Duration duration, Direction direction)
		: Transition(duration)
		, direction_(direction)
	{
	}

	void MoveTransition::Init(ScenePtr const& prev, ScenePtr const& next)
	{
		Transition::Init(prev, next);

		switch (direction_)
		{
		case Direction::Up:
			pos_delta_ = Point(0, -window_size_.y);
			start_pos_ = Point(0, window_size_.y);
			break;
		case Direction::Down:
			pos_delta_ = Point(0, window_size_.y);
			start_pos_ = Point(0, -window_size_.y);
			break;
		case Direction::Left:
			pos_delta_ = Point(-window_size_.x, 0);
			start_pos_ = Point(window_size_.x, 0);
			break;
		case Direction::Right:
			pos_delta_ = Point(window_size_.x, 0);
			start_pos_ = Point(-window_size_.x, 0);
			break;
		}

		if (out_scene_)
		{
			out_scene_->SetTransform(Transform{});
		}

		if (in_scene_)
		{
			auto transform = Transform{};
			transform.position = start_pos_;
			in_scene_->SetTransform(transform);
		}
	}

	void MoveTransition::Update(Duration dt)
	{
		Transition::Update(dt);

		if (out_scene_)
		{
			auto transform = Transform{};
			transform.position = pos_delta_ * process_;
			out_scene_->SetTransform(transform);
		}

		if (in_scene_)
		{
			auto transform = Transform{};
			transform.position = start_pos_ + pos_delta_ * process_;
			in_scene_->SetTransform(transform);
		}
	}

	void MoveTransition::Reset()
	{
		if (out_scene_)
		{
			out_scene_->SetTransform(Transform{});
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(Transform{});
		}
	}

	//-------------------------------------------------------
	// RotationTransition
	//-------------------------------------------------------

	RotationTransition::RotationTransition(Duration duration, float rotation)
		: Transition(duration)
		, rotation_(rotation)
	{
	}

	void RotationTransition::Init(ScenePtr const& prev, ScenePtr const& next)
	{
		Transition::Init(prev, next);

		auto transform = Transform{};
		transform.position = Point{ window_size_.x / 2, window_size_.y / 2 };

		if (out_scene_)
		{
			out_scene_->SetTransform(transform);
			out_scene_->SetAnchor(0.5f, 0.5f);
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(transform);
			in_scene_->SetAnchor(0.5f, 0.5f);
		}

		in_layer_prop_.opacity = 0;
	}

	void RotationTransition::Update(Duration dt)
	{
		Transition::Update(dt);

		if (process_ < .5f)
		{
			if (out_scene_)
			{
				auto transform = out_scene_->GetTransform();
				transform.scale = Point{ (.5f - process_) * 2, (.5f - process_) * 2 };
				transform.rotation = rotation_ * (.5f - process_) * 2;
				out_scene_->SetTransform(transform);
			}
		}
		else
		{
			if (in_scene_)
			{
				out_layer_prop_.opacity = 0;
				in_layer_prop_.opacity = 1;

				auto transform = in_scene_->GetTransform();
				transform.scale = Point{ (process_ - .5f) * 2, (process_ - .5f) * 2 };
				transform.rotation = rotation_ * (process_ - .5f) * 2;

				in_scene_->SetTransform(transform);
			}
		}
	}

	void RotationTransition::Reset()
	{
		if (out_scene_)
		{
			out_scene_->SetTransform(Transform{});
			out_scene_->SetAnchor(0.f, 0.f);
		}

		if (in_scene_)
		{
			in_scene_->SetTransform(Transform{});
			in_scene_->SetAnchor(0.f, 0.f);
		}
	}
}
