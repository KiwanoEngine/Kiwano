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

namespace kiwano
{
namespace audio
{

SoundPlayer::SoundPlayer()
    : volume_(1.f)
{
}

SoundPlayer::~SoundPlayer()
{
    ClearCache();
}

size_t SoundPlayer::GetId(const String& file_path) const
{
    return std::hash<String>()(file_path);
}

size_t SoundPlayer::GetId(const Resource& res) const
{
    return static_cast<size_t>(res.GetId());
}

size_t SoundPlayer::Load(const String& file_path)
{
    size_t id = GetId(file_path);
    if (sound_cache_.end() != sound_cache_.find(id))
        return id;

    SoundPtr sound = MakePtr<Sound>();
    if (sound && sound->Load(file_path))
    {
        sound->SetVolume(volume_);
        sound_cache_.insert(std::make_pair(id, sound));
        return id;
    }
    return 0;
}

size_t SoundPlayer::Load(const Resource& res)
{
    size_t id = GetId(res);
    if (sound_cache_.end() != sound_cache_.find(id))
        return id;

    SoundPtr sound = MakePtr<Sound>();

    if (sound && sound->Load(res))
    {
        sound->SetVolume(volume_);
        sound_cache_.insert(std::make_pair(id, sound));
        return id;
    }
    return 0;
}

void SoundPlayer::Play(size_t id, int loop_count)
{
    if (auto sound = GetSound(id))
        sound->Play(loop_count);
}

void SoundPlayer::Pause(size_t id)
{
    if (auto sound = GetSound(id))
        sound->Pause();
}

void SoundPlayer::Resume(size_t id)
{
    if (auto sound = GetSound(id))
        sound->Resume();
}

void SoundPlayer::Stop(size_t id)
{
    if (auto sound = GetSound(id))
        sound->Stop();
}

bool SoundPlayer::IsPlaying(size_t id)
{
    if (auto sound = GetSound(id))
        return sound->IsPlaying();
    return false;
}

float SoundPlayer::GetVolume() const
{
    return volume_;
}

void SoundPlayer::SetVolume(float volume)
{
    volume_ = std::min(std::max(volume, -224.f), 224.f);
    for (auto& pair : sound_cache_)
    {
        pair.second->SetVolume(volume_);
    }
}

SoundPtr SoundPlayer::GetSound(size_t id) const
{
    auto iter = sound_cache_.find(id);
    if (iter != sound_cache_.end())
        return iter->second;
    return SoundPtr();
}

void SoundPlayer::PauseAll()
{
    for (auto& pair : sound_cache_)
    {
        pair.second->Pause();
    }
}

void SoundPlayer::ResumeAll()
{
    for (auto& pair : sound_cache_)
    {
        pair.second->Resume();
    }
}

void SoundPlayer::StopAll()
{
    for (auto& pair : sound_cache_)
    {
        pair.second->Stop();
    }
}

void SoundPlayer::ReleaseSound(size_t id)
{
    sound_cache_.erase(id);
}

void SoundPlayer::ClearCache()
{
    sound_cache_.clear();
}
}  // namespace audio
}  // namespace kiwano
