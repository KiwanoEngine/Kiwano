#include "..\e2dtransition.h"
#include "..\e2dobject.h"
#include "..\e2dmodule.h"

e2d::Transition::Transition(float duration)
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

e2d::Transition::~Transition()
{
	SafeRelease(out_layer_);
	SafeRelease(in_layer_);
	SafeRelease(out_scene_);
	SafeRelease(in_scene_);
}

bool e2d::Transition::IsDone()
{
	return done_;
}

void e2d::Transition::Init(Scene * prev, Scene * next)
{
	started_ = Time::Now();
	out_scene_ = prev;
	in_scene_ = next;

	if (out_scene_)
		out_scene_->Retain();

	if (in_scene_)
		in_scene_->Retain();
	
	auto renderer = Renderer::GetInstance();
	if (in_scene_)
	{
		ThrowIfFailed(
			renderer->GetRenderTarget()->CreateLayer(&in_layer_)
		);
	}

	if (out_scene_)
	{
		ThrowIfFailed(
			renderer->GetRenderTarget()->CreateLayer(&out_layer_)
		);
	}

	window_size_ = Window::GetInstance()->GetSize();
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
		renderer->GetSolidBrush(),
		D2D1_LAYER_OPTIONS_NONE
	);
}

void e2d::Transition::Update()
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

void e2d::Transition::Draw()
{
	auto render_target = Renderer::GetInstance()->GetRenderTarget();

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

void e2d::Transition::Stop()
{
	done_ = true;
	Reset();
}
