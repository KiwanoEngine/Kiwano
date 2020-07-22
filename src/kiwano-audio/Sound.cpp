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
#include <kiwano/platform/FileSystem.h>

namespace kiwano
{
namespace audio
{
SoundPtr Sound::Create(const String& file_path)
{
    SoundPtr ptr = new (autogc) Sound;
    if (ptr)
    {
        if (!ptr->Load(file_path))
            return nullptr;
    }
    return ptr;
}

SoundPtr Sound::Create(const Resource& res)
{
    SoundPtr ptr = new (autogc) Sound;
    if (ptr)
    {
        if (!ptr->Load(res))
            return nullptr;
    }
    return ptr;
}

Sound::Sound()
    : opened_(false)
    , playing_(false)
    , voice_(nullptr)
{
}

Sound::~Sound()
{
    Close();
}

bool Sound::Load(const String& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_WARNF("Media file '%s' not found", file_path.c_str());
        return false;
    }

    if (opened_)
    {
        Close();
    }

    String full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);

    HRESULT hr = transcoder_.LoadMediaFile(full_path);
    if (FAILED(hr))
    {
        KGE_ERRORF("Load media file failed with HRESULT of %08X", hr);
        return false;
    }

    if (!AudioModule::GetInstance().CreateSound(*this, transcoder_.GetBuffer()))
    {
        Close();
        return false;
    }

    opened_ = true;
    return true;
}

bool Sound::Load(const Resource& res)
{
    if (opened_)
    {
        Close();
    }

    HRESULT hr = transcoder_.LoadMediaResource(res);
    if (FAILED(hr))
    {
        KGE_ERRORF("Load media resource failed with HRESULT of %08X", hr);
        return false;
    }

    if (!AudioModule::GetInstance().CreateSound(*this, transcoder_.GetBuffer()))
    {
        Close();
        return false;
    }

    opened_ = true;
    return true;
}

bool Sound::IsValid() const
{
    return voice_ != nullptr;
}

void Sound::Play(int loop_count)
{
    if (!opened_)
    {
        KGE_ERRORF("Sound must be opened first!");
        return;
    }

    KGE_ASSERT(voice_ != nullptr && "IXAudio2SourceVoice* is NULL");

    // if sound stream is not empty, stop() will clear it
    XAUDIO2_VOICE_STATE state;
    voice_->GetState(&state);
    if (state.BuffersQueued)
        Stop();

    // clamp loop count
    loop_count = (loop_count < 0) ? XAUDIO2_LOOP_INFINITE : std::min(loop_count, XAUDIO2_LOOP_INFINITE - 1);

    auto wave_buffer = transcoder_.GetBuffer();

    XAUDIO2_BUFFER buffer = { 0 };
    buffer.pAudioData     = wave_buffer.data;
    buffer.Flags          = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes     = wave_buffer.size;
    buffer.LoopCount      = static_cast<uint32_t>(loop_count);

    HRESULT hr = voice_->SubmitSourceBuffer(&buffer);
    if (SUCCEEDED(hr))
    {
        hr = voice_->Start();
    }

    if (FAILED(hr))
    {
        KGE_ERRORF("Submitting source buffer failed with HRESULT of %08X", hr);
    }

    playing_ = SUCCEEDED(hr);
}

void Sound::Pause()
{
    KGE_ASSERT(voice_ != nullptr && "IXAudio2SourceVoice* is NULL");

    if (SUCCEEDED(voice_->Stop()))
        playing_ = false;
}

void Sound::Resume()
{
    KGE_ASSERT(voice_ != nullptr && "IXAudio2SourceVoice* is NULL");

    if (SUCCEEDED(voice_->Start()))
        playing_ = true;
}

void Sound::Stop()
{
    KGE_ASSERT(voice_ != nullptr && "IXAudio2SourceVoice* is NULL");

    HRESULT hr = voice_->Stop();

    if (SUCCEEDED(hr))
        hr = voice_->ExitLoop();

    if (SUCCEEDED(hr))
        hr = voice_->FlushSourceBuffers();

    if (SUCCEEDED(hr))
        playing_ = false;
}

void Sound::Close()
{
    if (voice_)
    {
        voice_->Stop();
        voice_->FlushSourceBuffers();
        voice_->DestroyVoice();
        voice_ = nullptr;
    }

    transcoder_.ClearBuffer();

    opened_  = false;
    playing_ = false;
}

bool Sound::IsPlaying() const
{
    if (opened_)
    {
        if (!voice_)
            return false;

        XAUDIO2_VOICE_STATE state;
        voice_->GetState(&state);
        uint32_t buffers_queued = state.BuffersQueued;

        if (buffers_queued && playing_)
            return true;
    }
    return false;
}

float Sound::GetVolume() const
{
    KGE_ASSERT(voice_ != nullptr && "IXAudio2SourceVoice* is NULL");

    float volume = 0.0f;
    voice_->GetVolume(&volume);
    return volume;
}

void Sound::SetVolume(float volume)
{
    KGE_ASSERT(voice_ != nullptr && "IXAudio2SourceVoice* is NULL");

    volume = std::min(std::max(volume, -224.f), 224.f);
    voice_->SetVolume(volume);
}
}  // namespace audio
}  // namespace kiwano
