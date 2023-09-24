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

#include <kiwano-audio/AudioModule.h>
#include <kiwano-audio/Sound.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{
namespace audio
{

SoundPtr Sound::Preload(const String& file_path)
{
    auto ptr = MakePtr<Sound>();

    size_t hash_code = std::hash<String>{}(file_path);
    if (TranscoderPtr transcoder = TranscoderCache::GetInstance().Get(hash_code))
    {
        if (ptr->Load(transcoder))
            return ptr;
        return nullptr;
    }

    if (ptr && ptr->Load(file_path))
    {
        TranscoderCache::GetInstance().Add(hash_code, ptr->coder_);
    }
    return ptr;
}

SoundPtr Sound::Preload(const Resource& res)
{
    auto ptr = MakePtr<Sound>();

    size_t hash_code = res.GetId();
    if (TranscoderPtr transcoder = TranscoderCache::GetInstance().Get(hash_code))
    {
        if (ptr->Load(transcoder))
            return ptr;
        return nullptr;
    }

    if (ptr && ptr->Load(res))
    {
        TranscoderCache::GetInstance().Add(hash_code, ptr->coder_);
    }
    return ptr;
}

Sound::Sound(const String& file_path)
    : Sound()
{
    Load(file_path);
}

Sound::Sound(const Resource& res)
    : Sound()
{
    Load(res);
}

Sound::Sound()
    : opened_(false)
    , playing_(false)
{
}

Sound::~Sound()
{
    Close();
}

bool Sound::Load(const String& file_path)
{
    if (opened_)
    {
        Close();
    }

    TranscoderPtr transcoder = AudioModule::GetInstance().CreateTranscoder(file_path);
    if (!transcoder)
    {
        return false;
    }
    return Load(transcoder);
}

bool Sound::Load(const Resource& res)
{
    if (opened_)
    {
        Close();
    }

    TranscoderPtr transcoder = AudioModule::GetInstance().CreateTranscoder(res);
    if (!transcoder)
    {
        return false;
    }
    return Load(transcoder);
}

bool Sound::Load(TranscoderPtr transcoder)
{
    if (opened_)
    {
        Close();
    }
    if (!AudioModule::GetInstance().CreateSound(*this, transcoder))
    {
        return false;
    }

    coder_  = transcoder;
    opened_ = true;
    return true;
}

void Sound::Play(int loop_count)
{
    if (!opened_)
    {
        KGE_ERRORF("Sound must be opened first!");
        return;
    }

    auto voice = GetNativePtr<IXAudio2SourceVoice>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    // if sound stream is not empty, stop() will clear it
    XAUDIO2_VOICE_STATE state;
    voice->GetState(&state);
    if (state.BuffersQueued)
        Stop();

    // clamp loop count
    loop_count = (loop_count < 0) ? XAUDIO2_LOOP_INFINITE : std::min(loop_count, XAUDIO2_LOOP_INFINITE - 1);

    auto buffer = coder_->GetBuffer();

    XAUDIO2_BUFFER xaudio2_buffer = { 0 };
    xaudio2_buffer.pAudioData     = buffer.data;
    xaudio2_buffer.Flags          = XAUDIO2_END_OF_STREAM;
    xaudio2_buffer.AudioBytes     = buffer.size;
    xaudio2_buffer.LoopCount      = static_cast<uint32_t>(loop_count);

    HRESULT hr = voice->SubmitSourceBuffer(&xaudio2_buffer);
    if (SUCCEEDED(hr))
    {
        hr = voice->Start();
    }

    if (FAILED(hr))
    {
        KGE_ERRORF("Submitting source buffer failed with HRESULT of %08X", hr);
    }

    playing_ = SUCCEEDED(hr);
}

void Sound::Pause()
{
    auto voice = GetNativePtr<IXAudio2SourceVoice>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    if (SUCCEEDED(voice->Stop()))
        playing_ = false;
}

void Sound::Resume()
{
    auto voice = GetNativePtr<IXAudio2SourceVoice>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    if (SUCCEEDED(voice->Start()))
        playing_ = true;
}

void Sound::Stop()
{
    auto voice = GetNativePtr<IXAudio2SourceVoice>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    HRESULT hr = voice->Stop();

    if (SUCCEEDED(hr))
        hr = voice->ExitLoop();

    if (SUCCEEDED(hr))
        hr = voice->FlushSourceBuffers();

    if (SUCCEEDED(hr))
        playing_ = false;
}

void Sound::Close()
{
    auto voice = GetNativePtr<IXAudio2SourceVoice>();
    if (voice)
    {
        voice->Stop();
        voice->FlushSourceBuffers();
        voice->DestroyVoice();
    }

    coder_   = nullptr;
    opened_  = false;
    playing_ = false;
}

bool Sound::IsPlaying() const
{
    if (opened_)
    {
        if (!playing_)
            return false;

        auto voice = GetNativePtr<IXAudio2SourceVoice>();
        if (!voice)
            return false;

        XAUDIO2_VOICE_STATE state;
        voice->GetState(&state);
        return !!state.BuffersQueued;
    }
    return false;
}

float Sound::GetVolume() const
{
    auto voice = GetNativePtr<IXAudio2SourceVoice>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    float volume = 0.0f;
    voice->GetVolume(&volume);
    return volume;
}

void Sound::SetVolume(float volume)
{
    auto voice = GetNativePtr<IXAudio2SourceVoice>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    volume = std::min(std::max(volume, -XAUDIO2_MAX_VOLUME_LEVEL), XAUDIO2_MAX_VOLUME_LEVEL);
    voice->SetVolume(volume);
}
}  // namespace audio
}  // namespace kiwano
