#include "..\e2daction.h"

e2d::Animation::Animation()
	: interval_(1)
{
}

e2d::Animation::Animation(const Images& frames)
	: interval_(1)
{
	this->Add(frames);
}

e2d::Animation::Animation(float interval)
	: interval_(interval)
{
}

e2d::Animation::Animation(float interval, const Images& frames)
	: interval_(interval)
{
	this->Add(frames);
}

e2d::Animation::~Animation()
{
	for (auto frame : frames_)
	{
		SafeRelease(frame);
	}
}

void e2d::Animation::SetInterval(float interval)
{
	interval_ = std::max(interval, 0.f);
}

void e2d::Animation::Add(Image * frame)
{
	WARN_IF(frame == nullptr, "Animation::Add failed, frame Is nullptr.");
	if (frame)
	{
		frames_.push_back(frame);
		frame->Retain();
	}
}

void e2d::Animation::Add(const Images& frames)
{
	for (const auto &image : frames)
	{
		this->Add(image);
	}
}

float e2d::Animation::GetInterval() const
{
	return interval_;
}

const e2d::Animation::Images& e2d::Animation::GetFrames() const
{
	return frames_;
}

e2d::Animation * e2d::Animation::Clone() const
{
	auto animation = new Animation(interval_);
	if (animation)
	{
		for (const auto& frame : frames_)
		{
			animation->Add(frame);
		}
	}
	return animation;
}

e2d::Animation * e2d::Animation::Reverse() const
{
	auto& oldFrames = this->GetFrames();
	Images frames(oldFrames.size());

	if (!oldFrames.empty())
	{
		for (auto iter = oldFrames.crbegin(),
			iterCrend = oldFrames.crend();
			iter != iterCrend;
			++iter)
		{
			Image* frame = *iter;
			if (frame)
			{
				frames.push_back(frame);
			}
		}
	}

	return new Animation(this->GetInterval(), frames);
}
