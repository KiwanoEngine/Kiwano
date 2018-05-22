#include "..\e2dcustom.h"
#include "..\e2dbase.h"
#include "..\e2dtool.h"

e2d::VoiceCallback::VoiceCallback(Music * music)
	: _music(music)
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
	e2d::GC::release(_music);
}

void e2d::VoiceCallback::OnBufferStart(void * pBufferContext)
{
	e2d::GC::retain(_music);
}

void e2d::VoiceCallback::OnVoiceProcessingPassEnd()
{
}

void e2d::VoiceCallback::OnVoiceProcessingPassStart(UINT32 SamplesRequired)
{
}

void e2d::VoiceCallback::OnVoiceError(void * pBufferContext, HRESULT Error)
{
}

void e2d::VoiceCallback::SetFuncOnStreamEnd(const Function & func)
{
	_streamEndFunc = func;
}

void e2d::VoiceCallback::SetFuncOnLoopEnd(const Function & func)
{
	_loopEndFunc = func;
}