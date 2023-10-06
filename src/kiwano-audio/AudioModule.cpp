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
#include <kiwano/core/Exception.h>
#include <kiwano/utils/Logger.h>
#include <kiwano/platform/FileSystem.h>
#include <kiwano-audio/AudioModule.h>
#include <kiwano-audio/libraries.h>
#include <kiwano-audio/MediaFoundation/MFTranscoder.h>
#include <kiwano-audio/Ogg/OggTranscoder.h>

namespace kiwano
{
namespace audio
{

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
    SoundCallback* cb = nullptr;

    VoiceCallback(SoundCallback* cb)
        : cb(cb)
    {
    }

    ~VoiceCallback() {}

    STDMETHOD_(void, OnBufferStart(void* pBufferContext))
    {
        cb->OnStart(nullptr);
    }

    STDMETHOD_(void, OnLoopEnd(void* pBufferContext))
    {
        cb->OnLoopEnd(nullptr);
    }

    STDMETHOD_(void, OnBufferEnd(void* pBufferContext))
    {
        cb->OnEnd(nullptr);
    }

    STDMETHOD_(void, OnStreamEnd()) {}

    STDMETHOD_(void, OnVoiceProcessingPassEnd()) {}

    STDMETHOD_(void, OnVoiceProcessingPassStart(UINT32 SamplesRequired)) {}

    STDMETHOD_(void, OnVoiceError(void* pBufferContext, HRESULT Error))
    {
        KGE_ERRORF("Voice error with HRESULT of %08X", Error);
    }
};

WORD ConvertWaveFormat(AudioFormat format)
{
    switch (format)
    {
    case kiwano::audio::AudioFormat::PCM:
        return WAVE_FORMAT_PCM;
    }
    return 0;
}

AudioModule::AudioModule()
    : x_audio2_(nullptr)
    , mastering_voice_(nullptr)
{
}

AudioModule::~AudioModule() {}

void AudioModule::SetupModule()
{
    KGE_DEBUG_LOGF("Creating audio resources");

    HRESULT hr = dlls::XAudio2::Get().XAudio2Create(&x_audio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

    if (SUCCEEDED(hr))
    {
        hr = x_audio2_->CreateMasteringVoice(&mastering_voice_);
    }

    if (SUCCEEDED(hr))
    {
        RegisterTranscoder("*", MakePtr<MFTranscoder>());
        RegisterTranscoder("ogg", MakePtr<OggTranscoder>());
    }

    KGE_THROW_IF_FAILED(hr, "Create audio resources failed");
}

void AudioModule::DestroyModule()
{
    KGE_DEBUG_LOGF("Destroying audio resources");

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
}

bool AudioModule::CreateSound(Sound& sound, AudioDataPtr data)
{
    KGE_ASSERT(x_audio2_ && "AudioModule hasn't been initialized!");

    HRESULT hr = S_OK;

    WAVEFORMATEX* wave_fmt = data->GetNative<WAVEFORMATEX*>();
    if (wave_fmt == nullptr)
    {
        const auto   meta   = data->GetMeta();
        WAVEFORMATEX tmp    = { 0 };
        tmp.wFormatTag      = ConvertWaveFormat(meta.format);
        tmp.nChannels       = WORD(meta.channels);
        tmp.nSamplesPerSec  = DWORD(meta.samples_per_sec);
        tmp.wBitsPerSample  = WORD(meta.bits_per_sample);
        tmp.nBlockAlign     = WORD(meta.block_align);
        tmp.nAvgBytesPerSec = DWORD(meta.samples_per_sec * meta.block_align);

        data->SetNative(tmp);
        wave_fmt = const_cast<WAVEFORMATEX*>(data->GetNative().CastPtr<WAVEFORMATEX>());
    }

    if (SUCCEEDED(hr))
    {
        auto chain = sound.GetCallbackChain();
        chain->SetNative(VoiceCallback{ chain.Get() });
        auto callback = const_cast<VoiceCallback*>(chain->GetNative().CastPtr<VoiceCallback>());

        IXAudio2SourceVoice* voice = nullptr;
        hr = x_audio2_->CreateSourceVoice(&voice, wave_fmt, 0, XAUDIO2_DEFAULT_FREQ_RATIO, callback);
        if (SUCCEEDED(hr))
        {
            sound.Close();
            sound.SetNative(voice);
        }
    }

    if (FAILED(hr))
    {
        KGE_ERRORF("Create IXAudio2SourceVoice failed with HRESULT of %08X", hr);
        return false;
    }
    return true;
}

void AudioModule::Open()
{
    KGE_ASSERT(x_audio2_ && "AudioModule hasn't been initialized!");
    if (x_audio2_)
        x_audio2_->StartEngine();
}

void AudioModule::Close()
{
    KGE_ASSERT(x_audio2_ && "AudioModule hasn't been initialized!");

    if (x_audio2_)
        x_audio2_->StopEngine();
}

void AudioModule::RegisterTranscoder(const String& ext, TranscoderPtr transcoder)
{
    registered_transcoders_.insert(std::make_pair(ext, transcoder));
}

TranscoderPtr AudioModule::GetTranscoder(const String& ext)
{
    auto iter = registered_transcoders_.find(ext);
    if (iter != registered_transcoders_.end())
    {
        return iter->second;
    }
    return registered_transcoders_.at("*");
}

AudioDataPtr AudioModule::Decode(const String& file_path)
{
    if (!FileSystem::GetInstance().IsFileExists(file_path))
    {
        KGE_WARNF("Media file '%s' not found", file_path.c_str());
        return nullptr;
    }

    const auto ext = FileSystem::GetInstance().GetFileExt(file_path);

    auto transcoder = GetTranscoder(ext);
    if (!transcoder)
    {
        return nullptr;
    }
    String full_path = FileSystem::GetInstance().GetFullPathForFile(file_path);
    return transcoder->Decode(full_path);
}

AudioDataPtr AudioModule::Decode(const Resource& res, const String& ext)
{
    auto transcoder = GetTranscoder(ext);
    if (!transcoder)
    {
        return nullptr;
    }
    return transcoder->Decode(res);
}

}  // namespace audio
}  // namespace kiwano
