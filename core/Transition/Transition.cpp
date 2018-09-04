#include "..\e2dbase.h"
#include "..\e2dtransition.h"
#include "..\e2dnode.h"

e2d::Transition::Transition(Scene* scene, float duration)
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
	if (in_scene_)
		in_scene_->Retain();
}

e2d::Transition::~Transition()
{
	SafeRelease(out_layer_);
	SafeRelease(in_layer_);
	GC::GetInstance()->SafeRelease(out_scene_);
	GC::GetInstance()->SafeRelease(in_scene_);
}

bool e2d::Transition::IsDone()
{
	return done_;
}

bool e2d::Transition::Init(Game * game, Scene * prev)
{
	started_ = Time::Now();
	out_scene_ = prev;

	if (out_scene_)
		out_scene_->Retain();
	
	HRESULT hr = S_OK;
	auto renderer = Renderer::GetInstance();
	if (in_scene_)
	{
		hr = renderer->GetRenderTarget()->CreateLayer(&in_layer_);
	}

	if (SUCCEEDED(hr) && out_scene_)
	{
		hr = renderer->GetRenderTarget()->CreateLayer(&out_layer_);
	}

	if (FAILED(hr))
	{
		return false;
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
	auto renderTarget = Renderer::GetInstance()->GetRenderTarget();
	auto size = Window::GetInstance()->GetSize();

	if (out_scene_)
	{
		auto rootPos = out_scene_->GetPos();
		auto clipRect = D2D1::RectF(
			std::max(rootPos.x, 0.f),
			std::max(rootPos.y, 0.f),
			std::min(rootPos.x + size.width, size.width),
			std::min(rootPos.y + size.height, size.height)
		);
		renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		renderTarget->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		renderTarget->PushLayer(out_layer_param_, out_layer_);

		out_scene_->Visit();

		renderTarget->PopLayer();
		renderTarget->PopAxisAlignedClip();
	}

	if (in_scene_)
	{
		Point rootPos = in_scene_->GetPos();
		auto clipRect = D2D1::RectF(
			std::max(rootPos.x, 0.f),
			std::max(rootPos.y, 0.f),
			std::min(rootPos.x + size.width, size.width),
			std::min(rootPos.y + size.height, size.height)
		);
		renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		renderTarget->PushAxisAlignedClip(clipRect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		renderTarget->PushLayer(in_layer_param_, in_layer_);

		in_scene_->Visit();

		renderTarget->PopLayer();
		renderTarget->PopAxisAlignedClip();
	}
}

void e2d::Transition::Stop()
{
	done_ = true;
	Reset();
}
