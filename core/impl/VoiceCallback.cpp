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
