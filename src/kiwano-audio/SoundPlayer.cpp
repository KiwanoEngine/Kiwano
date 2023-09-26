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

#include <kiwano-audio/SoundPlayer.h>
#include <kiwano-audio/AudioModule.h>
#include <kiwano/platform/Application.h>

namespace kiwano
{
namespace audio
{

SoundPlayer::SoundPlayer()
    : volume_(1.f)
{
    class SoundCallbackFunc : public SoundCallback
    {
    public:
        Function<void(Sound* sound)>                cb_on_end;
        Function<float(Sound* sound, float volume)> cb_on_volume_changed;

        void OnEnd(Sound* sound) override
        {
            cb_on_end(sound);
        }

        float OnVolumeChanged(Sound* sound, float volume) override
        {
            return cb_on_volume_changed(sound, volume);
        }
    };
    auto cb       = MakePtr<SoundCallbackFunc>();
    cb->cb_on_end = std::bind(&SoundPlayer::OnEnd, this, std::placeholders::_1);
    cb->cb_on_volume_changed =
        std::bind(&SoundPlayer::OnVolumeChanged, this, std::placeholders::_1, std::placeholders::_2);
    callback_ = cb;
}

SoundPlayer::~SoundPlayer()
{}

TranscoderPtr SoundPlayer::Preload(const String& file_path)
{
    size_t hash_code = std::hash<String>{}(file_path);
    if (cache_.count(hash_code))
    {
        return cache_.at(hash_code);
    }
    TranscoderPtr ptr = AudioModule::GetInstance().CreateTranscoder(file_path);
    if (ptr)
    {
        cache_.insert(std::make_pair(hash_code, ptr));
    }
    return ptr;
}

TranscoderPtr SoundPlayer::Preload(const Resource& res)
{
    size_t hash_code = res.GetId();
    if (cache_.count(hash_code))
    {
        return cache_.at(hash_code);
    }
    TranscoderPtr ptr = AudioModule::GetInstance().CreateTranscoder(res);
    if (ptr)
    {
        cache_.insert(std::make_pair(hash_code, ptr));
    }
    return ptr;
}

void SoundPlayer::Play(SoundPtr sound, int loop_count)
{
    if (sound)
    {
        SetCallback(sound.Get());
        sound->Play(loop_count);
        sound_list_.push_back(sound);
    }
}

SoundPtr SoundPlayer::Play(const String& file_path, int loop_count, std::initializer_list<SoundCallbackPtr> callbacks)
{
    TranscoderPtr transcoder = Preload(file_path);

    SoundPtr sound = new Sound(transcoder);
    for (const auto& cb : callbacks)
    {
        sound->AddCallback(cb);
    }
    Play(sound, loop_count);
    return sound;
}

SoundPtr SoundPlayer::Play(const Resource& res, int loop_count, std::initializer_list<SoundCallbackPtr> callbacks)
{
    TranscoderPtr transcoder = Preload(res);

    SoundPtr sound = new Sound(transcoder);
    for (const auto& cb : callbacks)
    {
        sound->AddCallback(cb);
    }
    Play(sound, loop_count);
    return sound;
}

float SoundPlayer::GetVolume() const
{
    return volume_;
}

void SoundPlayer::SetVolume(float volume)
{
    volume_ = volume;
}

void SoundPlayer::PauseAll()
{
    for (auto& sound : sound_list_)
    {
        sound->Pause();
    }
}

void SoundPlayer::ResumeAll()
{
    for (auto& sound : sound_list_)
    {
        sound->Resume();
    }
}

void SoundPlayer::StopAll()
{
    for (auto& sound : sound_list_)
    {
        sound->Stop();
    }
}

void SoundPlayer::ClearCache()
{
    cache_.clear();
}

void SoundPlayer::OnEnd(Sound* sound)
{
    // remove callback
    RemoveCallback(sound);

    // remove sound after stopped
    auto iter = std::find(sound_list_.begin(), sound_list_.end(), sound);
    if (iter != sound_list_.end())
    {
        trash_.push_back(*iter);
        sound_list_.erase(iter);
    }

    // clear trash in main thread
    Application::GetInstance().PerformInMainThread(std::bind(&SoundPlayer::ClearTrash, this));
}

float SoundPlayer::OnVolumeChanged(Sound* sound, float volume)
{
    return volume * volume_;
}

void SoundPlayer::SetCallback(Sound* sound)
{
    // add callback if not exists
    auto& cbs  = sound->GetCallbacks();
    auto  iter = std::find(cbs.begin(), cbs.end(), callback_);
    if (iter == cbs.end())
    {
        sound->AddCallback(callback_);
    }
    sound->ResetVolume();
}

void SoundPlayer::RemoveCallback(Sound* sound)
{
    auto& cbs = sound->GetCallbacks();
    auto  iter = std::find(cbs.begin(), cbs.end(), callback_);
    if (iter != cbs.end())
    {
        *iter = nullptr; // will be removed by sound
    }
}

void SoundPlayer::ClearTrash()
{
    trash_.clear();
}

}  // namespace audio
}  // namespace kiwano
