#include "..\e2dtool.h"


namespace easy2d
{
	std::map<size_t, Music*> Player::musics_;

	Player::Player()
		: volume_(1.f)
	{
	}

	Player::~Player()
	{
	}

	bool Player::Load(const std::wstring & file_path)
	{
		if (file_path.empty())
			return false;

		Music * music = new (std::nothrow) Music();

		if (music)
		{
			if (music->Load(file_path))
			{
				music->SetVolume(volume_);

				size_t hash_code = std::hash<std::wstring>{}(file_path);
				musics_.insert(std::make_pair(hash_code, music));
				return true;
			}
			else
			{
				music->Release();
			}
		}
		return false;
	}

	bool Player::Play(const std::wstring & file_path, int loop_count)
	{
		if (file_path.empty())
			return false;

		if (Load(file_path))
		{
			auto music = musics_[std::hash<std::wstring>{}(file_path)];
			if (music->Play(loop_count))
			{
				return true;
			}
		}
		return false;
	}

	void Player::Pause(const std::wstring & file_path)
	{
		if (file_path.empty())
			return;

		size_t hash_code = std::hash<std::wstring>{}(file_path);
		if (musics_.end() != musics_.find(hash_code))
			musics_[hash_code]->Pause();
	}

	void Player::Resume(const std::wstring & file_path)
	{
		if (file_path.empty())
			return;

		size_t hash_code = std::hash<std::wstring>{}(file_path);
		if (musics_.end() != musics_.find(hash_code))
			musics_[hash_code]->Resume();
	}

	void Player::Stop(const std::wstring & file_path)
	{
		if (file_path.empty())
			return;

		size_t hash_code = std::hash<std::wstring>{}(file_path);
		if (musics_.end() != musics_.find(hash_code))
			musics_[hash_code]->Stop();
	}

	bool Player::IsPlaying(const std::wstring & file_path)
	{
		if (file_path.empty())
			return false;

		size_t hash_code = std::hash<std::wstring>{}(file_path);
		if (musics_.end() != musics_.find(hash_code))
			return musics_[hash_code]->IsPlaying();
		return false;
	}

	bool Player::Load(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_.end() != musics_.find(hash_code))
			return true;

		Music * music = new (std::nothrow) Music();

		if (music)
		{
			if (music->Load(res))
			{
				music->SetVolume(volume_);
				musics_.insert(std::make_pair(hash_code, music));
				return true;
			}
			else
			{
				music->Release();
			}
		}
		return false;
	}

	bool Player::Play(Resource& res, int loop_count)
	{
		if (Load(res))
		{
			size_t hash_code = res.GetHashCode();
			auto music = musics_[hash_code];
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
		if (musics_.end() != musics_.find(hash_code))
			musics_[hash_code]->Pause();
	}

	void Player::Resume(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_.end() != musics_.find(hash_code))
			musics_[hash_code]->Resume();
	}

	void Player::Stop(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_.end() != musics_.find(hash_code))
			musics_[hash_code]->Stop();
	}

	bool Player::IsPlaying(Resource& res)
	{
		size_t hash_code = res.GetHashCode();
		if (musics_.end() != musics_.find(hash_code))
			return musics_[hash_code]->IsPlaying();
		return false;
	}

	float Player::GetVolume() const
	{
		return volume_;
	}

	void Player::SetVolume(float volume)
	{
		volume_ = std::min(std::max(volume, -224.f), 224.f);
		for (const auto& pair : musics_)
		{
			pair.second->SetVolume(volume_);
		}
	}

	void Player::PauseAll()
	{
		for (const auto& pair : musics_)
		{
			pair.second->Pause();
		}
	}

	void Player::ResumeAll()
	{
		for (const auto& pair : musics_)
		{
			pair.second->Resume();
		}
	}

	void Player::StopAll()
	{
		for (const auto& pair : musics_)
		{
			pair.second->Stop();
		}
	}

	void Player::ClearCache()
	{
		if (musics_.empty())
			return;

		for (const auto& pair : musics_)
		{
			pair.second->Release();
		}
		musics_.clear();
	}
}