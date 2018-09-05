#include "..\e2daction.h"
#include "..\e2dcomponent.h"

e2d::Animate::Animate() 
	: frame_index_(0)
	, animation_(nullptr)
{
}

e2d::Animate::Animate(Animation * animation)
	: frame_index_(0)
	, animation_(nullptr)
{
	this->SetAnimation(animation);
}

e2d::Animate::~Animate()
{
	GC::GetInstance()->SafeRelease(animation_);
}

e2d::Animation * e2d::Animate::GetAnimation() const
{
	return animation_;
}

void e2d::Animate::SetAnimation(Animation * animation)
{
	if (animation && animation != animation_ && !animation->GetFrames().empty())
	{
		if (animation_) animation_->Release();
		animation_ = animation;
		animation_->Retain();
	}
}

void e2d::Animate::Init()
{
	Action::Init();

	auto target = dynamic_cast<Sprite*>(target_);
	if (target && animation_)
	{
		target->Open(animation_->GetFrames()[frame_index_]);
		++frame_index_;
	}
}

void e2d::Animate::Update()
{
	Action::Update();

	if (!animation_)
	{
		this->Stop();
		return;
	}

	while ((Time::Now() - started_).Seconds() >= animation_->GetInterval())
	{
		auto& frames = animation_->GetFrames();
		auto target = dynamic_cast<Sprite*>(target_);

		if (target)
		{
			target->Open(frames[frame_index_]);
		}

		started_ += Duration(animation_->GetInterval());
		++frame_index_;

		if (frame_index_ == frames.size())
		{
			this->Stop();
			break;
		}
	}
}

void e2d::Animate::ResetTime()
{
	Action::ResetTime();
}

void e2d::Animate::Reset()
{
	Action::Reset();
	frame_index_ = 0;
}

e2d::Animate * e2d::Animate::Clone() const
{
	if (animation_)
	{
		return new (e2d::autorelease) Animate(animation_);
	}
	return nullptr;
}

e2d::Animate * e2d::Animate::Reverse() const
{
	if (animation_)
	{
		auto animation = animation_->Reverse();
		if (animation)
		{
			return new (e2d::autorelease) Animate(animation);
		}
	}
	return nullptr;
}
