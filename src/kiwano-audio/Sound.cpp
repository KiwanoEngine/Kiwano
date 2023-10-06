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
#include <xaudio2.h>

namespace kiwano
{
namespace audio
{

Sound::Sound(const String& file_path)
    : Sound()
{
    Load(AudioModule::GetInstance().Decode(file_path));
}

Sound::Sound(const Resource& res, const String& ext)
    : Sound()
{
    Load(AudioModule::GetInstance().Decode(res, ext));
}

Sound::Sound(AudioDataPtr data)
    : Sound()
{
    Load(data);
}

Sound::Sound()
    : opened_(false)
    , playing_(false)
    , volume_(1.f)
{
}

Sound::~Sound()
{
    Close();
}

bool Sound::Load(AudioDataPtr data)
{
    if (!data)
    {
        return false;
    }

    if (opened_)
    {
        Close();
    }
    if (!AudioModule::GetInstance().CreateSound(*this, data))
    {
        return false;
    }

    // reset volume
    ResetVolume();

    data_   = data;
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

    auto voice = GetNative<IXAudio2SourceVoice*>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    // if sound stream is not empty, stop() will clear it
    XAUDIO2_VOICE_STATE state;
    voice->GetState(&state);
    if (state.BuffersQueued)
        Stop();

    // clamp loop count
    loop_count = (loop_count < 0) ? XAUDIO2_LOOP_INFINITE : std::min(loop_count, XAUDIO2_LOOP_INFINITE - 1);

    auto data = data_->GetData();

    XAUDIO2_BUFFER xaudio2_buffer = { 0 };
    xaudio2_buffer.pAudioData     = reinterpret_cast<BYTE*>(data.buffer);
    xaudio2_buffer.Flags          = XAUDIO2_END_OF_STREAM;
    xaudio2_buffer.AudioBytes     = UINT32(data.size);
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
    auto voice = GetNative<IXAudio2SourceVoice*>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    HRESULT hr = voice->Stop();
    if (SUCCEEDED(hr))
        playing_ = false;

    if (FAILED(hr))
    {
        KGE_ERRORF("Pause voice failed with HRESULT of %08X", hr);
    }
}

void Sound::Resume()
{
    auto voice = GetNative<IXAudio2SourceVoice*>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    HRESULT hr = voice->Start();
    if (SUCCEEDED(hr))
        playing_ = true;

    if (FAILED(hr))
    {
        KGE_ERRORF("Start voice failed with HRESULT of %08X", hr);
    }
}

void Sound::Stop()
{
    auto voice = GetNative<IXAudio2SourceVoice*>();
    KGE_ASSERT(voice != nullptr && "IXAudio2SourceVoice* is NULL");

    HRESULT hr = voice->Stop();

    if (SUCCEEDED(hr))
        hr = voice->ExitLoop();

    if (SUCCEEDED(hr))
        hr = voice->FlushSourceBuffers();

    if (SUCCEEDED(hr))
        playing_ = false;

    if (FAILED(hr))
    {
        KGE_ERRORF("Stop voice failed with HRESULT of %08X", hr);
    }
}

void Sound::Close()
{
    auto voice = GetNative<IXAudio2SourceVoice*>();
    if (voice)
    {
        voice->Stop();
        voice->FlushSourceBuffers();
        voice->DestroyVoice();
    }

    data_    = nullptr;
    opened_  = false;
    playing_ = false;
}

bool Sound::IsPlaying() const
{
    if (opened_)
    {
        if (!playing_)
            return false;

        auto voice = GetNative<IXAudio2SourceVoice*>();
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
    return volume_;
}

void Sound::SetVolume(float volume)
{
    if (volume_ == volume)
    {
        return;
    }
    volume_ = volume;

    auto voice = GetNative<IXAudio2SourceVoice*>();
    if (voice)
    {
        float actual_volume = GetCallbackChain()->OnVolumeChanged(this, volume_);
        actual_volume       = std::min(std::max(actual_volume, -XAUDIO2_MAX_VOLUME_LEVEL), XAUDIO2_MAX_VOLUME_LEVEL);
        voice->SetVolume(actual_volume);
    }
}

void Sound::ResetVolume()
{
    const float old_volume = volume_;

    volume_ += 1.f;
    SetVolume(old_volume);
}

SoundCallbackPtr Sound::GetCallbackChain()
{
    class SoundCallbackChain : public SoundCallback
    {
    public:
        Sound* sound = nullptr;

        void OnStart(Sound*) override
        {
            for (auto& cb : sound->GetCallbacks())
            {
                if (cb)
                {
                    cb->OnStart(sound);
                }
            }
            RemoveUsedCallbacks();
        }

        void OnLoopEnd(Sound*) override
        {
            for (auto& cb : sound->GetCallbacks())
            {
                if (cb)
                {
                    cb->OnLoopEnd(sound);
                }
            }
            RemoveUsedCallbacks();
        }

        void OnEnd(Sound*) override
        {
            for (auto& cb : sound->GetCallbacks())
            {
                if (cb)
                {
                    cb->OnEnd(sound);
                }
            }
            RemoveUsedCallbacks();
        }

        float OnVolumeChanged(Sound*, float volume) override
        {
            float actual_volume = volume;
            for (auto& cb : sound->GetCallbacks())
            {
                if (cb)
                {
                    actual_volume = cb->OnVolumeChanged(sound, volume);
                }
            }
            return actual_volume;
        }

        void RemoveUsedCallbacks()
        {
            auto& cbs  = sound->GetCallbacks();
            auto  iter = cbs.begin();
            while (iter != cbs.end())
            {
                if (*iter == nullptr)
                {
                    iter = cbs.erase(iter);
                }
                else
                {
                    iter++;
                }
            }
        }
    };

    if (!callback_chain_)
    {
        auto chain      = MakePtr<SoundCallbackChain>();
        chain->sound    = this;
        callback_chain_ = chain;
    }
    return callback_chain_;
}

SoundCallbackPtr SoundCallback::OnStart(const Function<void(Sound* sound)>& cb)
{
    class SoundCallbackFunc : public SoundCallback
    {
    public:
        Function<void(Sound* sound)> cb;

        void OnStart(Sound* sound) override
        {
            if (cb)
            {
                cb(sound);
            }
        }
    };
    auto ptr = MakePtr<SoundCallbackFunc>();
    ptr->cb  = cb;
    return ptr;
}

SoundCallbackPtr SoundCallback::OnLoopEnd(const Function<void(Sound* sound)>& cb)
{
    class SoundCallbackFunc : public SoundCallback
    {
    public:
        Function<void(Sound* sound)> cb;

        void OnLoopEnd(Sound* sound) override
        {
            if (cb)
            {
                cb(sound);
            }
        }
    };
    auto ptr = MakePtr<SoundCallbackFunc>();
    ptr->cb  = cb;
    return ptr;
}

SoundCallbackPtr SoundCallback::OnEnd(const Function<void(Sound* sound)>& cb)
{
    class SoundCallbackFunc : public SoundCallback
    {
    public:
        Function<void(Sound* sound)> cb;

        void OnEnd(Sound* sound) override
        {
            if (cb)
            {
                cb(sound);
            }
        }
    };
    auto ptr = MakePtr<SoundCallbackFunc>();
    ptr->cb  = cb;
    return ptr;
}

SoundCallbackPtr SoundCallback::OnVolumeChanged(const Function<float(Sound* sound, float volume)>& cb)
{
    class SoundCallbackFunc : public SoundCallback
    {
    public:
        Function<float(Sound* sound, float volume)> cb;

        float OnVolumeChanged(Sound* sound, float volume) override
        {
            if (cb)
            {
                return cb(sound, volume);
            }
            return volume;
        }
    };
    auto ptr = MakePtr<SoundCallbackFunc>();
    ptr->cb  = cb;
    return ptr;
}

}  // namespace audio
}  // namespace kiwano
