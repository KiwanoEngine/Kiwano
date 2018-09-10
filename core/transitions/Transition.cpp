#include "..\e2dmodule.h"
#include "..\e2dtransition.h"
#include "..\e2dobject.h"

e2d::Transition::Transition(float duration)
	: done_(false)
	, started_()
	, delta_(0)
	, out_scene_(nullptr)
	, in_scene_(scene)
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

	out_layer_param_ = in_layer_param_ = D2D1::LayerParameters(
		D2D1::InfiniteRect(),
		nullptr,
		D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
		D2D1::Matrix3x2F::Identity(),
		1.f,
		renderer->GetSolidBrush(),
		D2D1_LAYER_OPTIONS_NONE
	);

	return true;
}

void e2d::Transition::Update()
{
	if (duration_ == 0)
	{
		delta_ = 1;
	}
	else
	{
		delta_ = (Time::Now() - started_).Seconds() / duration_;
		delta_ = std::min(delta_, 1.f);
	}
}

void e2d::Transition::Draw()
{
	auto render_target = Renderer::GetInstance()->GetRenderTarget();
	auto size = Window::GetInstance()->GetSize();

	if (out_scene_ && out_scene_->GetRoot())
	{
		auto root_pos = out_scene_->GetRoot()->GetPos();
		auto clip_rect = D2D1::RectF(
			std::max(root_pos.x, 0.f),
			std::max(root_pos.y, 0.f),
			std::min(root_pos.x + size.width, size.width),
			std::min(root_pos.y + size.height, size.height)
		);
		render_target->SetTransform(D2D1::Matrix3x2F::Identity());
		render_target->PushAxisAlignedClip(clip_rect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		render_target->PushLayer(out_layer_param_, out_layer_);

		out_scene_->Draw();

		render_target->PopLayer();
		render_target->PopAxisAlignedClip();
	}

	if (in_scene_ && in_scene_->GetRoot())
	{
		Point root_pos = in_scene_->GetRoot()->GetPos();
		auto clip_rect = D2D1::RectF(
			std::max(root_pos.x, 0.f),
			std::max(root_pos.y, 0.f),
			std::min(root_pos.x + size.width, size.width),
			std::min(root_pos.y + size.height, size.height)
		);
		render_target->SetTransform(D2D1::Matrix3x2F::Identity());
		render_target->PushAxisAlignedClip(clip_rect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
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
