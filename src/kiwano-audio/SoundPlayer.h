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

#pragma once
#include <kiwano-audio/Sound.h>

namespace kiwano
{
namespace audio
{

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief ��Ƶ������
 */
class KGE_API SoundPlayer : public BaseObject
{
public:
    using SoundList = List<RefPtr<Sound>>;

    SoundPlayer();

    ~SoundPlayer();

    /// \~chinese
    /// @brief Ԥ������Ƶ
    /// @details
    RefPtr<AudioData> Preload(StringView file_path);

    /// \~chinese
    /// @brief Ԥ������Ƶ��Դ
    RefPtr<AudioData> Preload(const Resource& res, StringView ext = "");

    /// \~chinese
    /// @brief ������Ƶ
    /// @param sound ��Ƶ
    /// @param loop_count ����ѭ������������ -1 Ϊѭ������
    void Play(RefPtr<Sound> sound, int loop_count = 0);

    /// \~chinese
    /// @brief ������Ƶ
    /// @param file_path ������Ƶ�ļ�·��
    /// @param loop_count ����ѭ������������ -1 Ϊѭ������
    RefPtr<Sound> Play(StringView file_path, int loop_count = 0);

    /// \~chinese
    /// @brief ������Ƶ
    /// @param res ��Ƶ��Դ
    /// @param loop_count ����ѭ������������ -1 Ϊѭ������
    RefPtr<Sound> Play(const Resource& res, int loop_count = 0);

    /// \~chinese
    /// @brief ��ͣ������Ƶ
    void PauseAll();

    /// \~chinese
    /// @brief ��������������Ƶ
    void ResumeAll();

    /// \~chinese
    /// @brief ֹͣ������Ƶ
    void StopAll();

    /// \~chinese
    /// @brief ��ȡ���ڲ��ŵ���Ƶ�б�
    const SoundList& GetPlayingList() const;

    /// \~chinese
    /// @brief ��ȡ����
    float GetVolume() const;

    /// \~chinese
    /// @brief ��������
    /// @param volume ������С��1.0 Ϊԭʼ����, ���� 1 Ϊ�Ŵ�����, 0 Ϊ��С����
    void SetVolume(float volume);

    /// \~chinese
    /// @brief ��ջ���
    void ClearCache();

protected:
    void OnEnd(Sound* sound);

    float OnVolumeChanged(Sound* sound, float volume);

    void SetCallback(Sound* sound);

    void RemoveCallback(Sound* sound);

    void ClearTrash();

protected:
    float                 volume_;
    SoundList             sound_list_;
    SoundList             trash_;
    RefPtr<SoundCallback> callback_;

    UnorderedMap<size_t, RefPtr<AudioData>> cache_;
};

/** @} */

inline const SoundPlayer::SoundList& SoundPlayer::GetPlayingList() const
{
    return sound_list_;
}

}  // namespace audio
}  // namespace kiwano
