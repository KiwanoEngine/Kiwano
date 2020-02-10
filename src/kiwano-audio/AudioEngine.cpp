// Copyright (c) 2016-2018 Kiwano - Nomango
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

#include <kiwano-audio/AudioEngine.h>
#include <kiwano-audio/libraries.h>
#include <kiwano/core/Logger.h>
#include <kiwano/platform/win32/helper.h>  // win32::ThrowIfFailed

namespace kiwano
{
namespace audio
{
AudioEngine::AudioEngine()
    : x_audio2_(nullptr)
    , mastering_voice_(nullptr)
{
}

AudioEngine::~AudioEngine() {}

void AudioEngine::SetupComponent()
{
    KGE_SYS_LOG("Creating audio resources");

    HRESULT hr = dlls::MediaFoundation::Get().MFStartup(MF_VERSION, MFSTARTUP_FULL);

    if (SUCCEEDED(hr))
    {
        hr = dlls::XAudio2::Get().XAudio2Create(&x_audio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
    }

    if (SUCCEEDED(hr))
    {
        hr = x_audio2_->CreateMasteringVoice(&mastering_voice_);
    }

    win32::ThrowIfFailed(hr);
}

void AudioEngine::DestroyComponent()
{
    KGE_SYS_LOG("Destroying audio resources");

    if (mastering_voice_)
    {
        mastering_voice_->DestroyVoice();
        mastering_voice_ = nullptr;
    }

    if (x_audio2_)
    {
        x_audio2_->Release();
        x_audio2_ = nullptr;
    }

    dlls::MediaFoundation::Get().MFShutdown();
}

bool AudioEngine::CreateSound(Sound& sound, const Transcoder::Buffer& buffer)
{
    KGE_ASSERT(x_audio2_ && "AudioEngine hasn't been initialized!");

    HRESULT hr = S_OK;

    if (buffer.format == nullptr)
        hr = E_INVALIDARG;

    if (SUCCEEDED(hr))
    {
        IXAudio2SourceVoice* voice = nullptr;
        hr                         = x_audio2_->CreateSourceVoice(&voice, buffer.format, 0, XAUDIO2_DEFAULT_FREQ_RATIO);

        if (SUCCEEDED(hr))
        {
            IXAudio2SourceVoice* old = sound.GetXAudio2Voice();
            if (old)
            {
                old->DestroyVoice();
                old = nullptr;
            }

            sound.SetXAudio2Voice(voice);
        }
    }

    win32::WarnIfFailed(hr);
    return SUCCEEDED(hr);
}

void AudioEngine::Open()
{
    KGE_ASSERT(x_audio2_ && "AudioEngine hasn't been initialized!");

    if (x_audio2_)
        x_audio2_->StartEngine();
}

void AudioEngine::Close()
{
    KGE_ASSERT(x_audio2_ && "AudioEngine hasn't been initialized!");

    if (x_audio2_)
        x_audio2_->StopEngine();
}
}  // namespace audio
}  // namespace kiwano
