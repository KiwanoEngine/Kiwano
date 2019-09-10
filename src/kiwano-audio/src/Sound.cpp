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

#include <kiwano/base/Logger.h>
#include <kiwano/utils/FileUtil.h>
#include "Sound.h"
#include "audio.h"

namespace kiwano
{
	namespace audio
	{

		Sound::Sound()
			: opened_(false)
			, playing_(false)
			, voice_(nullptr)
		{
		}

		Sound::Sound(String const& file_path)
			: Sound()
		{
			Load(file_path);
		}

		Sound::Sound(Resource const& res)
			: Sound()
		{
			Load(res);
		}

		Sound::~Sound()
		{
			Close();
		}

		bool Sound::Load(String const& file_path)
		{
			if (!FileUtil::ExistsFile(file_path))
			{
				KGE_WARNING_LOG(L"Media file '%s' not found", file_path.c_str());
				return false;
			}

			if (opened_)
			{
				Close();
			}

			HRESULT hr = transcoder_.LoadMediaFile(file_path);

			if (FAILED(hr))
			{
				KGE_ERROR_LOG(L"Load media file failed with HRESULT of %08X", hr);
				return false;
			}

			hr = Audio::GetInstance()->CreateVoice(&voice_, transcoder_.GetBuffer());
			if (FAILED(hr))
			{
				Close();

				KGE_ERROR_LOG(L"Create source voice failed with HRESULT of %08X", hr);
				return false;
			}

			opened_ = true;
			return true;
		}

		bool Sound::Load(Resource const& res)
		{
			if (opened_)
			{
				Close();
			}

			HRESULT hr = transcoder_.LoadMediaResource(res);

			if (FAILED(hr))
			{
				KGE_ERROR_LOG(L"Load media resource failed with HRESULT of %08X", hr);
				return false;
			}

			hr = Audio::GetInstance()->CreateVoice(&voice_, transcoder_.GetBuffer());
			if (FAILED(hr))
			{
				Close();

				KGE_ERROR_LOG(L"Create source voice failed with HRESULT of %08X", hr);
				return false;
			}

			opened_ = true;
			return true;
		}

		void Sound::Play(Int32 loop_count)
		{
			if (!opened_)
			{
				KGE_ERROR_LOG(L"Sound must be opened first!");
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
			buffer.pAudioData = wave_buffer.data;
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.AudioBytes = wave_buffer.size;
			buffer.LoopCount = static_cast<UInt32>(loop_count);

			HRESULT hr = voice_->SubmitSourceBuffer(&buffer);
			if (SUCCEEDED(hr))
			{
				hr = voice_->Start();
			}

			if (FAILED(hr))
			{
				KGE_ERROR_LOG(L"Submitting source buffer failed with HRESULT of %08X", hr);
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

			opened_ = false;
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
				UInt32 buffers_queued = state.BuffersQueued;

				if (buffers_queued && playing_)
					return true;
			}
			return false;
		}

		Float32 Sound::GetVolume() const
		{
			KGE_ASSERT(voice_ != nullptr && "IXAudio2SourceVoice* is NULL");

			Float32 volume = 0.0f;
			voice_->GetVolume(&volume);
			return volume;
		}

		void Sound::SetVolume(Float32 volume)
		{
			KGE_ASSERT(voice_ != nullptr && "IXAudio2SourceVoice* is NULL");

			volume = std::min(std::max(volume, -224.f), 224.f);
			voice_->SetVolume(volume);
		}
	}
}
