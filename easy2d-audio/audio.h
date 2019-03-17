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
#include "Voice.h"

namespace easy2d
{
	class E2D_API Audio
		: public Singleton<Audio>
		, public Component
	{
		E2D_DECLARE_SINGLETON(Audio);

		using VoiceMap = UnorderedSet<Voice*>;

	public:
		void Setup(Application*) override;

		void Destroy() override;

		// �����豸
		void Open();

		// �ر��豸
		void Close();

		HRESULT CreateVoice(
			Voice& voice,
			const WAVEFORMATEX* wfx
		);

		void DeleteVoice(
			Voice* voice
		);

		void ClearVoiceCache();

	protected:
		Audio();

		~Audio();

	protected:
		VoiceMap voice_cache_;
		IXAudio2* x_audio2_;
		IXAudio2MasteringVoice*	mastering_voice_;
	};
}
