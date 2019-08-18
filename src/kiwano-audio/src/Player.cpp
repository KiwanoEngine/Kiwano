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

#include "Player.h"

namespace kiwano
{
	namespace audio
	{
		Player::Player()
			: volume_(1.f)
		{
		}

		Player::~Player()
		{
			ClearCache();
		}

		size_t Player::Load(String const& file_path)
		{
			size_t hash_code = file_path.hash();
			if (sound_cache_.end() != sound_cache_.find(hash_code))
				return true;

			SoundPtr sound = new (std::nothrow) Sound;

			if (sound)
			{
				if (sound->Load(file_path))
				{
					sound->SetVolume(volume_);
					sound_cache_.insert(std::make_pair(hash_code, sound));
					return true;
				}
			}
			return false;
		}

		size_t Player::Load(Resource const& res)
		{
			size_t hash_code = res.GetId();
			if (sound_cache_.end() != sound_cache_.find(hash_code))
				return true;

			SoundPtr sound = new (std::nothrow) Sound;

			if (sound)
			{
				if (sound->Load(res))
				{
					sound->SetVolume(volume_);
					sound_cache_.insert(std::make_pair(hash_code, sound));
					return true;
				}
			}
			return false;
		}

		void Player::Play(size_t id, int loop_count)
		{
			auto iter = sound_cache_.find(id);
			if (sound_cache_.end() != iter)
				iter->second->Play(loop_count);
		}

		void Player::Pause(size_t id)
		{
			auto iter = sound_cache_.find(id);
			if (sound_cache_.end() != iter)
				iter->second->Pause();
		}

		void Player::Resume(size_t id)
		{
			auto iter = sound_cache_.find(id);
			if (sound_cache_.end() != iter)
				iter->second->Resume();
		}

		void Player::Stop(size_t id)
		{
			auto iter = sound_cache_.find(id);
			if (sound_cache_.end() != iter)
				iter->second->Stop();
		}

		bool Player::IsPlaying(size_t id)
		{
			auto iter = sound_cache_.find(id);
			if (sound_cache_.end() != iter)
				return iter->second->IsPlaying();
			return false;
		}

		float Player::GetVolume() const
		{
			return volume_;
		}

		void Player::SetVolume(float volume)
		{
			volume_ = std::min(std::max(volume, -224.f), 224.f);
			for (const auto& pair : sound_cache_)
			{
				pair.second->SetVolume(volume_);
			}
		}

		void Player::PauseAll()
		{
			for (const auto& pair : sound_cache_)
			{
				pair.second->Pause();
			}
		}

		void Player::ResumeAll()
		{
			for (const auto& pair : sound_cache_)
			{
				pair.second->Resume();
			}
		}

		void Player::StopAll()
		{
			for (const auto& pair : sound_cache_)
			{
				pair.second->Stop();
			}
		}

		void Player::ClearCache()
		{
			sound_cache_.clear();
		}
	}
}
