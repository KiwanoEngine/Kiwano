#include "..\e2dtool.h"


std::map<size_t, easy2d::Music*> easy2d::Player::musics_;

easy2d::Player::Player()
	: volume_(1.f)
{
}

easy2d::Player::~Player()
{
}

bool easy2d::Player::Load(const std::wstring & file_path)
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

bool easy2d::Player::Play(const std::wstring & file_path, int loop_count)
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

void easy2d::Player::Pause(const std::wstring & file_path)
{
	if (file_path.empty())
		return;

	size_t hash_code = std::hash<std::wstring>{}(file_path);
	if (musics_.end() != musics_.find(hash_code))
		musics_[hash_code]->Pause();
}

void easy2d::Player::Resume(const std::wstring & file_path)
{
	if (file_path.empty())
		return;

	size_t hash_code = std::hash<std::wstring>{}(file_path);
	if (musics_.end() != musics_.find(hash_code))
		musics_[hash_code]->Resume();
}

void easy2d::Player::Stop(const std::wstring & file_path)
{
	if (file_path.empty())
		return;

	size_t hash_code = std::hash<std::wstring>{}(file_path);
	if (musics_.end() != musics_.find(hash_code))
		musics_[hash_code]->Stop();
}

bool easy2d::Player::IsPlaying(const std::wstring & file_path)
{
	if (file_path.empty())
		return false;

	size_t hash_code = std::hash<std::wstring>{}(file_path);
	if (musics_.end() != musics_.find(hash_code))
		return musics_[hash_code]->IsPlaying();
	return false;
}

bool easy2d::Player::Load(Resource& res)
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

bool easy2d::Player::Play(Resource& res, int loop_count)
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

void easy2d::Player::Pause(Resource& res)
{
	size_t hash_code = res.GetHashCode();
	if (musics_.end() != musics_.find(hash_code))
		musics_[hash_code]->Pause();
}

void easy2d::Player::Resume(Resource& res)
{
	size_t hash_code = res.GetHashCode();
	if (musics_.end() != musics_.find(hash_code))
		musics_[hash_code]->Resume();
}

void easy2d::Player::Stop(Resource& res)
{
	size_t hash_code = res.GetHashCode();
	if (musics_.end() != musics_.find(hash_code))
		musics_[hash_code]->Stop();
}

bool easy2d::Player::IsPlaying(Resource& res)
{
	size_t hash_code = res.GetHashCode();
	if (musics_.end() != musics_.find(hash_code))
		return musics_[hash_code]->IsPlaying();
	return false;
}

float easy2d::Player::GetVolume() const
{
	return volume_;
}

void easy2d::Player::SetVolume(float volume)
{
	volume_ = std::min(std::max(volume, -224.f), 224.f);
	for (const auto& pair : musics_)
	{
		pair.second->SetVolume(volume_);
	}
}

void easy2d::Player::PauseAll()
{
	for (const auto& pair : musics_)
	{
		pair.second->Pause();
	}
}

void easy2d::Player::ResumeAll()
{
	for (const auto& pair : musics_)
	{
		pair.second->Resume();
	}
}

void easy2d::Player::StopAll()
{
	for (const auto& pair : musics_)
	{
		pair.second->Stop();
	}
}

void easy2d::Player::ClearCache()
{
	if (musics_.empty())
		return;

	for (const auto& pair : musics_)
	{
		pair.second->Release();
	}
	musics_.clear();
}
