#include "..\e2dutil.h"


e2d::Player * e2d::Player::instance_ = nullptr;

e2d::Player * e2d::Player::GetInstance()
{
	if (!instance_)
	{
		instance_ = new (std::nothrow) Player;
	}
	return instance_;
}

void e2d::Player::DestroyInstance()
{
	if (instance_)
	{
		delete instance_;
		instance_ = nullptr;
	}
}

e2d::Player::Player()
	: volume_(1.f)
{
}

e2d::Player::~Player()
{
	if (!musics_.empty())
	{
		for (const auto& pair : musics_)
		{
			delete pair.second;
		}
	}
}

bool e2d::Player::Preload(const String & file_path)
{
	if (file_path.IsEmpty())
		return false;

	Music * music = new (std::nothrow) Music();

	if (music && music->Open(file_path))
	{
		music->SetVolume(volume_);
		musics_.insert(std::make_pair(file_path.GetHash(), music));
		return true;
	}
	return false;
}

bool e2d::Player::Play(const String & file_path, int loop_count)
{
	if (file_path.IsEmpty())
		return false;

	if (Player::Preload(file_path))
	{
		auto music = musics_[file_path.GetHash()];
		if (music->Play(loop_count))
		{
			return true;
		}
	}
	return false;
}

void e2d::Player::Pause(const String & file_path)
{
	if (file_path.IsEmpty())
		return;

	size_t hash = file_path.GetHash();
	if (musics_.end() != musics_.find(hash))
		musics_[hash]->Pause();
}

void e2d::Player::Resume(const String & file_path)
{
	if (file_path.IsEmpty())
		return;

	size_t hash = file_path.GetHash();
	if (musics_.end() != musics_.find(hash))
		musics_[hash]->Resume();
}

void e2d::Player::Stop(const String & file_path)
{
	if (file_path.IsEmpty())
		return;

	size_t hash = file_path.GetHash();
	if (musics_.end() != musics_.find(hash))
		musics_[hash]->Stop();
}

bool e2d::Player::IsPlaying(const String & file_path)
{
	if (file_path.IsEmpty())
		return false;

	size_t hash = file_path.GetHash();
	if (musics_.end() != musics_.find(hash))
		return musics_[hash]->IsPlaying();
	return false;
}

bool e2d::Player::Preload(const Resource& res)
{
	if (musics_.end() != musics_.find(res.name))
		return true;

	Music * music = new (std::nothrow) Music();

	if (music && music->Open(res))
	{
		music->SetVolume(volume_);
		musics_.insert(std::make_pair(res.name, music));
		return true;
	}
	return false;
}

bool e2d::Player::Play(const Resource& res, int loop_count)
{
	if (Player::Preload(res))
	{
		auto music = musics_[res.name];
		if (music->Play(loop_count))
		{
			return true;
		}
	}
	return false;
}

void e2d::Player::Pause(const Resource& res)
{
	if (musics_.end() != musics_.find(res.name))
		musics_[res.name]->Pause();
}

void e2d::Player::Resume(const Resource& res)
{
	if (musics_.end() != musics_.find(res.name))
		musics_[res.name]->Resume();
}

void e2d::Player::Stop(const Resource& res)
{
	if (musics_.end() != musics_.find(res.name))
		musics_[res.name]->Stop();
}

bool e2d::Player::IsPlaying(const Resource& res)
{
	if (musics_.end() != musics_.find(res.name))
		return musics_[res.name]->IsPlaying();
	return false;
}

float e2d::Player::GetVolume()
{
	return volume_;
}

void e2d::Player::SetVolume(float volume)
{
	volume_ = std::min(std::max(volume, -224.f), 224.f);
	for (const auto& pair : musics_)
	{
		pair.second->SetVolume(volume_);
	}
}

void e2d::Player::PauseAll()
{
	for (const auto& pair : musics_)
	{
		pair.second->Pause();
	}
}

void e2d::Player::ResumeAll()
{
	for (const auto& pair : musics_)
	{
		pair.second->Resume();
	}
}

void e2d::Player::StopAll()
{
	for (const auto& pair : musics_)
	{
		pair.second->Stop();
	}
}

void e2d::Player::ClearCache()
{
	for (const auto& pair : musics_)
	{
		delete pair.second;
	}
	musics_.clear();
}
