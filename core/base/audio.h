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

#pragma once
#include "macros.h"
#include "Singleton.hpp"
#include <xaudio2.h>

namespace easy2d
{
	class Voice
	{
		E2D_DISABLE_COPY(Voice);

	public:
		Voice();

		Voice(
			IXAudio2SourceVoice* source_voice
		);

		~Voice();

		HRESULT Play(
			const BYTE* wave_data,
			UINT32 data_size,
			UINT32 loop_count
		);

		HRESULT Pause();

		HRESULT Resume();

		HRESULT Stop();

		HRESULT GetVolume(
			float* volume
		) const;

		HRESULT SetVolume(
			float volume
		);

		HRESULT GetBuffersQueued(
			UINT32* queued
		) const;

		void Destroy();

		void SetSourceVoice(
			IXAudio2SourceVoice* source_voice
		);

	protected:
		IXAudio2SourceVoice* source_voice_;
	};

	namespace devices
	{
		class AudioDevice
		{
			E2D_DECLARE_SINGLETON(AudioDevice);

			E2D_DISABLE_COPY(AudioDevice);

		public:
			void Initialize();

			// 开启设备
			void Open();

			// 关闭设备
			void Close();

			HRESULT CreateVoice(
				Voice* voice,
				WAVEFORMATEX * wfx
			);

			void DeleteVoice(
				Voice* voice
			);

			void ClearVoiceCache();

		protected:
			AudioDevice();

			~AudioDevice();

		protected:
			IXAudio2 * x_audio2_;
			IXAudio2MasteringVoice*	mastering_voice_;
			std::list<Voice*> voice_cache_;
		};

		E2D_DECLARE_SINGLETON_TYPE(AudioDevice, Audio);
	}
}
