#include "..\e2dimpl.h"
#include "..\e2dutil.h"

e2d::VoiceCallback::VoiceCallback()
{
}

e2d::VoiceCallback::~VoiceCallback()
{
}

void e2d::VoiceCallback::OnLoopEnd(void * pBufferContext)
{
	if (loop_end_callback_)
	{
		loop_end_callback_();
	}
}

void e2d::VoiceCallback::OnStreamEnd()
{
	if (stream_end_callback_)
	{
		stream_end_callback_();
	}
}

void e2d::VoiceCallback::OnBufferEnd(void * pBufferContext)
{
	if (loop_end_callback_)
	{
		loop_end_callback_();
	}
}

void e2d::VoiceCallback::SetCallbackOnStreamEnd(const Function & func)
{
	stream_end_callback_ = func;
}

void e2d::VoiceCallback::SetCallbackOnLoopEnd(const Function & func)
{
	loop_end_callback_ = func;
}
