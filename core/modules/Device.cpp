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

#include "..\e2dmodule.h"


namespace easy2d
{
	namespace
	{
		Graphics *	graphics_device = nullptr;
		Input *		input_device = nullptr;
		Audio *		audio_device = nullptr;
	}

	Graphics * Device::GetGraphics()
	{
		return graphics_device;
	}

	Input * Device::GetInput()
	{
		return input_device;
	}

	Audio * Device::GetAudio()
	{
		return audio_device;
	}

	void Device::Initialize(HWND hwnd)
	{
		graphics_device = new (std::nothrow) Graphics(hwnd);
		input_device = new (std::nothrow) Input(hwnd);
		audio_device = new (std::nothrow) Audio();
	}

	void Device::Destroy()
	{
		if (audio_device)
		{
			delete audio_device;
			audio_device = nullptr;
		}

		if (input_device)
		{
			delete input_device;
			input_device = nullptr;
		}

		if (graphics_device)
		{
			delete graphics_device;
			graphics_device = nullptr;
		}
	}
}