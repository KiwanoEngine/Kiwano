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
#include "..\e2dmodule.h"

easy2d::Transition::Transition(float duration)
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

easy2d::Transition::~Transition()
{
	SafeRelease(out_layer_);
	SafeRelease(in_layer_);
	SafeRelease(out_scene_);
	SafeRelease(in_scene_);
}

bool easy2d::Transition::IsDone()
{
	return done_;
}

void easy2d::Transition::Init(Scene * prev, Scene * next, Game * game)
{
	started_ = Time::Now();
	out_scene_ = prev;
	in_scene_ = next;

	if (out_scene_)
		out_scene_->Retain();

	if (in_scene_)
		in_scene_->Retain();
	
	auto graphics = Device::GetGraphics();
	if (in_scene_)
	{
		ThrowIfFailed(
			graphics->GetRenderTarget()->CreateLayer(&in_layer_)
		);
	}

	if (out_scene_)
	{
		ThrowIfFailed(
			graphics->GetRenderTarget()->CreateLayer(&out_layer_)
		);
	}

	window_size_ = game->GetSize();
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
		graphics->GetSolidBrush(),
		D2D1_LAYER_OPTIONS_NONE
	);
}

void easy2d::Transition::Update()
{
	if (duration_ == 0)
	{
		process_ = 1;
	}
	else
	{
		process_ = (Time::Now() - started_).Seconds() / duration_;
		process_ = std::min(process_, 1.f);
	}

	if (process_ >= 1)
	{
		this->Stop();
	}
}

void easy2d::Transition::Draw()
{
	auto render_target = Device::GetGraphics()->GetRenderTarget();

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

void easy2d::Transition::Stop()
{
	done_ = true;
	Reset();
}
