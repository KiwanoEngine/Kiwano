#include "..\e2daction.h"

e2d::Callback::Callback(const Function& func) :
	callback_(func)
{
}

e2d::Callback * e2d::Callback::Clone() const
{
	return new Callback(callback_);
}

e2d::Callback * e2d::Callback::Reverse() const
{
	return new Callback(callback_);
}

void e2d::Callback::Init()
{
}

void e2d::Callback::Update()
{
	callback_();
	this->Stop();
}
