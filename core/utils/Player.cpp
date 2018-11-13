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

#include "Player.h"
#include "../base/Music.h"

namespace easy2d
{
	Player::Player()
		: volume_(1.f)
	{
	}

	Player::~Player()
	{
		ClearCache();
	}

	bool Player::Load(const String & file_path)
	{
		if (file_path.empty())
			return false;

		spMusic music = new (std::nothrow) Music();

		if (music)
		{
			if (music->Load(file_path))
			{
				music->SetVolume(volume_);

				size_t hash_code = std::hash<String>{}(file_path);
				musics_cache_.insert(std::make_pair(hash_code, music));
				return true;
			}
		}
		return false;
	}

	bool Player::Play(const String & file_path, int loop_count)
	{
		if (file_path.empty())
			return false;

		if (Load(file_path))
		{
			auto music = musics_cache_[std::hash<String>{}(file_path)];
			if (music->Play(loop_count))
			{
				return true;
			}
		}
		return false;
	}

	void Player::Pause(const String & file_path)
	{
		if (file_path.empty())
			return;

		size_t hash_code = std::hash<String>{}(file_path);
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			musics_cache_[hash_code]->Pause();
	}

	void Player::Resume(const String & file_path)
	{
		if (file_path.empty())
			return;

		size_t hash_code = std::hash<String>{}(file_path);
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			musics_cache_[hash_code]->Resume();
	}

	void Player::Stop(const String & file_path)
	{
		if (file_path.empty())
			return;

		size_t hash_code = std::hash<String>{}(file_path);
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			musics_cache_[hash_code]->Stop();
	}

	bool Player::IsPlaying(const String & file_path)
	{
		if (file_path.empty())
			return false;

		size_t hash_code = std::hash<String>{}(file_path);
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			return musics_cache_[hash_code]->IsPlaying();
		return false;
	}

	bool Player::Load(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			return true;

		spMusic music = new (std::nothrow) Music();

		if (music)
		{
			if (music->Load(res))
			{
				music->SetVolume(volume_);
				musics_cache_.insert(std::make_pair(hash_code, music));
				return true;
			}
		}
		return false;
	}

	bool Player::Play(Resource& res, int loop_count)
	{
		if (Load(res))
		{
			size_t hash_code = res.GetHashCode();
			auto music = musics_cache_[hash_code];
			if (music->Play(loop_count))
			{
				return true;
			}
		}
		return false;
	}

	void Player::Pause(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			musics_cache_[hash_code]->Pause();
	}

	void Player::Resume(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			musics_cache_[hash_code]->Resume();
	}

	void Player::Stop(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			musics_cache_[hash_code]->Stop();
	}

	bool Player::IsPlaying(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_cache_.end() != musics_cache_.find(hash_code))
			return musics_cache_[hash_code]->IsPlaying();
		return false;
	}

	float Player::GetVolume() const
	{
		return volume_;
	}

	void Player::SetVolume(float volume)
	{
		volume_ = std::min(std::max(volume, -224.f), 224.f);
		for (const auto& pair : musics_cache_)
		{
			pair.second->SetVolume(volume_);
		}
	}

	void Player::PauseAll()
	{
		for (const auto& pair : musics_cache_)
		{
			pair.second->Pause();
		}
	}

	void Player::ResumeAll()
	{
		for (const auto& pair : musics_cache_)
		{
			pair.second->Resume();
		}
	}

	void Player::StopAll()
	{
		for (const auto& pair : musics_cache_)
		{
			pair.second->Stop();
		}
	}

	void Player::ClearCache()
	{
		musics_cache_.clear();
	}
}