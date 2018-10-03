#include "..\e2dmodule.h"

static e2d::Graphics *	graphics_device = nullptr;
static e2d::Input *		input_device = nullptr;
static e2d::Audio *		audio_device = nullptr;

e2d::Graphics * e2d::Device::GetGraphics()
{
	return graphics_device;
}

e2d::Input * e2d::Device::GetInput()
{
	return input_device;
}

e2d::Audio * e2d::Device::GetAudio()
{
	return audio_device;
}

void e2d::Device::Init(HWND hwnd)
{
	graphics_device = new (std::nothrow) Graphics(hwnd);
	input_device = new (std::nothrow) Input(hwnd);
	audio_device = new (std::nothrow) Audio();
}

void e2d::Device::Destroy()
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
