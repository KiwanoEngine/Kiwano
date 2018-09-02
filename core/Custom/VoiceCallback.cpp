#include "..\e2dcustom.h"
#include "..\e2dcommon.h"

e2d::VoiceCallback::VoiceCallback()
{
}

e2d::VoiceCallback::~VoiceCallback()
{
}

void e2d::VoiceCallback::OnLoopEnd(void * pBufferContext)
{
	if (_loopEndFunc)
	{
		_loopEndFunc();
	}
}

void e2d::VoiceCallback::OnStreamEnd()
{
	if (_streamEndFunc)
	{
		_streamEndFunc();
	}
}

void e2d::VoiceCallback::OnBufferEnd(void * pBufferContext)
{
	if (_loopEndFunc)
	{
		_loopEndFunc();
	}
}

void e2d::VoiceCallback::SetFuncOnStreamEnd(const Function & func)
{
	_streamEndFunc = func;
}

void e2d::VoiceCallback::SetFuncOnLoopEnd(const Function & func)
{
	_loopEndFunc = func;
}
