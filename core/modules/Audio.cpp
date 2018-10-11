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


e2d::Audio::Audio()
	: enum_(nullptr)
	, devices_(nullptr)
	, device_(nullptr)
	, attributes_(nullptr)
	, sink_(nullptr)
	, device_id(nullptr)
	, audio_volume(nullptr)
{
	ThrowIfFailed(
		MFStartup(MF_VERSION)
	);

	// Create the device enumerator.
	ThrowIfFailed(
		CoCreateInstance(
			__uuidof(MMDeviceEnumerator),
			NULL,
			CLSCTX_ALL,
			__uuidof(IMMDeviceEnumerator),
			(void**)&enum_
		)
	);

	// Enumerate the rendering devices.
	ThrowIfFailed(
		enum_->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &devices_)
	);

	// Get ID of the first device in the list.
	ThrowIfFailed(
		devices_->Item(0, &device_)
	);

	ThrowIfFailed(
		device_->GetId(&device_id)
	);

	// Create an attribute store and set the device ID attribute.
	ThrowIfFailed(
		MFCreateAttributes(&attributes_, 2)
	);

	ThrowIfFailed(
		attributes_->SetString(
			MF_AUDIO_RENDERER_ATTRIBUTE_ENDPOINT_ID,
			device_id
		)
	);

	// Create the audio renderer.
	ThrowIfFailed(
		MFCreateAudioRenderer(attributes_, &sink_)
	);

	IMFGetService* service = NULL;

	ThrowIfFailed(
		sink_->QueryInterface(IID_IMFGetService, (void **)&service)
	);
	
	ThrowIfFailed(
		service->GetService(MR_POLICY_VOLUME_SERVICE, IID_PPV_ARGS(&audio_volume))
	);

	SafeRelease(service);
}

e2d::Audio::~Audio()
{
	SafeRelease(enum_);
	SafeRelease(devices_);
	SafeRelease(device_);
	SafeRelease(attributes_);
	SafeRelease(audio_volume);
	SafeRelease(sink_);
	CoTaskMemFree(device_id);

	MFShutdown();
}

float e2d::Audio::GetVolume()
{
	float volume = 0.f;
	if (audio_volume)
	{
		HRESULT hr = audio_volume->GetMasterVolume(&volume);
		if (SUCCEEDED(hr))
		{
			return volume;
		}
	}
	return 0.f;
}

void e2d::Audio::SetVolume(float volume)
{
	if (audio_volume)
	{
		volume = std::min(std::max(volume, 0.f), 1.f);
		HRESULT hr = audio_volume->SetMasterVolume(volume);
		printf("บวบว%#X\n", hr);
	}
}

bool e2d::Audio::GetMute()
{
	BOOL mute = FALSE;
	if (audio_volume)
	{
		HRESULT hr = audio_volume->GetMute(&mute);
		if (SUCCEEDED(hr))
		{
			return mute ? true : false;
		}
	}
	return FALSE;
}

void e2d::Audio::SetMute(bool mute)
{
	if (audio_volume)
	{
		audio_volume->SetMute(mute ? TRUE : FALSE);
	}
}
