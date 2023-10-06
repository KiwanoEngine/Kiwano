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

#include <kiwano/utils/Logger.h>
#include <kiwano-audio/Ogg/OggTranscoder.h>
#include <3rd-party/vorbis/vorbisfile.h>

namespace kiwano
{
namespace audio
{

class OggAudioData : public AudioData
{
public:
    OggAudioData(std::vector<char> raw, uint32_t size, const AudioMeta& meta)
        : raw_(std::move(raw))
    {
        data_ = BinaryData{ raw_.data(), size };
        meta_ = meta;
    }

    std::vector<char> raw_;
};

AudioDataPtr OggTranscoder::Decode(const String& file_path)
{
    OggVorbis_File vf;

    int err = ov_fopen(file_path.c_str(), &vf);
    if (err != 0)
    {
        KGE_ERROR(strings::Format("%s failed (%d): %s", __FUNCTION__, err, "Load audio failed"));
        return nullptr;
    }

    // read metadata
    vorbis_info* vi = ov_info(&vf, -1);

    AudioMeta meta;
    meta.samples_per_sec = uint32_t(vi->rate);
    meta.channels        = uint16_t(vi->channels);

    // read ogg audio
    size_t buffer_size = 4096;
    size_t grow_size   = buffer_size;
    int    bitstream;

    std::vector<char> data;
    size_t pos = data.size();
    while (true)
    {
        if (data.size() < pos + buffer_size)
        {
            // allocate buffer
            data.resize(pos + grow_size);
            if (grow_size < 1024000)
            {
                grow_size = size_t(double(grow_size) * 1.25);
            }
        }
        int bytes_read = ov_read(&vf, data.data() + pos, int(buffer_size), 0, 2, 1, &bitstream);
        if (bytes_read <= 0)
            break;
        pos += bytes_read;
    }
    ov_clear(&vf);

    AudioDataPtr output = new OggAudioData(std::move(data), uint32_t(pos), meta);
    return output;
}

AudioDataPtr OggTranscoder::Decode(const Resource& res)
{
    KGE_ERROR("Loading audio from memory resource is not supported for OggTranscoder");
    return nullptr;
}

}  // namespace audio
}  // namespace kiwano
