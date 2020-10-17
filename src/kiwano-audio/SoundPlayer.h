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
#include <kiwano/base/ObjectBase.h>

namespace kiwano
{
namespace audio
{
KGE_DECLARE_SMART_PTR(SoundPlayer);

/**
 * \addtogroup Audio
 * @{
 */

/**
 * \~chinese
 * @brief ��Ƶ������
 */
class KGE_API SoundPlayer : public ObjectBase
{
public:
    SoundPlayer();

    ~SoundPlayer();

    /// \~chinese
    /// @brief ���ر�����Ƶ�ļ�
    /// @param file_path ������Ƶ�ļ�·��
    /// @return ��Ƶ��ʶ��
    size_t Load(const String& file_path);

    /// \~chinese
    /// @brief ������Ƶ��Դ
    /// @param res ��Ƶ��Դ
    /// @return ��Ƶ��ʶ��
    size_t Load(const Resource& res);

    /// \~chinese
    /// @brief ������Ƶ
    /// @param id ��Ƶ��ʶ��
    /// @param loop_count ����ѭ������������ -1 Ϊѭ������
    void Play(size_t id, int loop_count = 0);

    /// \~chinese
    /// @brief ��ͣ��Ƶ
    /// @param id ��Ƶ��ʶ��
    void Pause(size_t id);

    /// \~chinese
    /// @brief ����������Ƶ
    /// @param id ��Ƶ��ʶ��
    void Resume(size_t id);

    /// \~chinese
    /// @brief ֹͣ��Ƶ
    /// @param id ��Ƶ��ʶ��
    void Stop(size_t id);

    /// \~chinese
    /// @brief ��ȡ��Ƶ����״̬
    /// @param id ��Ƶ��ʶ��
    bool IsPlaying(size_t id);

    /// \~chinese
    /// @brief ��ȡ����
    float GetVolume() const;

    /// \~chinese
    /// @brief ��������
    /// @param volume ������С��1.0 Ϊԭʼ����, ���� 1 Ϊ�Ŵ�����, 0 Ϊ��С����
    void SetVolume(float volume);

    /// \~chinese
    /// @brief ��ȡ������Ƶ�ļ�id
    /// @param file_path ������Ƶ�ļ�·��
    /// @return ��Ƶ��ʶ��
    size_t GetId(const String& file_path) const;

    /// \~chinese
    /// @brief ��ȡ��Ƶ��Դid
    /// @param res ��Ƶ��Դ
    /// @return ��Ƶ��ʶ��
    size_t GetId(const Resource& res) const;

    /// \~chinese
    /// @brief ��ȡ���ֶ���
    /// @param id ��Ƶ��ʶ��
    SoundPtr GetSound(size_t id) const;

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
    /// @brief �������
    void ClearCache();

private:
    float volume_;

    using SoundMap = Map<size_t, SoundPtr>;
    SoundMap sound_cache_;
};

/** @} */
}  // namespace audio
}  // namespace kiwano
