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
#include <kiwano/core/Common.h>
#include <kiwano/math/Math.h>

namespace kiwano
{
/**
 * \~chinese
 * \defgroup Serialization 序列化
 */

/**
 * \addtogroup Serialization
 * @{
 */

/// \~chinese
/// @brief 序列化器
struct Serializer
{
    /// \~chinese
    /// @brief 写入字节序列
    virtual void WriteBytes(const uint8_t* bytes, size_t size) = 0;

    /// \~chinese
    /// @brief 写入值
    template <typename _Ty>
    void WriteValue(const _Ty& value)
    {
        static_assert(std::is_trivial<_Ty>::value, "_Ty must be trivial type.");

        size_t size = sizeof(_Ty) / sizeof(uint8_t);
        this->WriteBytes(reinterpret_cast<const uint8_t*>(&value), size);
    }

    template <typename _Ty>
    Serializer& operator<<(const _Ty& value)
    {
        this->WriteValue(value);
        return (*this);
    }
};

/// \~chinese
/// @brief 字节串序列化器
struct ByteSerializer : public Serializer
{
    ByteSerializer(Vector<uint8_t>& bytes)
        : bytes_(bytes)
    {
    }

    void WriteBytes(const uint8_t* bytes, size_t size) override
    {
        size_t old_size = bytes_.size();
        this->GenerateSize(size);
        std::memcpy(&bytes_[old_size], bytes, size);
    }

private:
    void GenerateSize(size_t size)
    {
        size_t old_size = bytes_.size();
        size_t total_size = old_size + size;
        if (total_size > bytes_.capacity())
        {
            if (total_size < 1024)
            {
                size_t expected_size = size_t(std::floor(double(old_size) * 0.5));
                bytes_.reserve(old_size + std::max(size, expected_size));
            }
            else if (size < old_size * 2)
            {
                size_t expected_size = size_t(std::floor(double(old_size) * 0.25));
                bytes_.reserve(old_size + std::max(size, expected_size));
            }
            else
            {
                bytes_.reserve(old_size + size);
            }
        }
        bytes_.resize(total_size);
    }

    Vector<uint8_t>& bytes_;
};

/// \~chinese
/// @brief 流序列化器
struct StreamSerializer : public Serializer
{
    StreamSerializer(std::basic_ostream<char>& stream)
        : stream_(stream)
    {
    }

    void WriteBytes(const uint8_t* bytes, size_t size) override
    {
        stream_.write(reinterpret_cast<const char*>(bytes), size);
    }

private:
    std::basic_ostream<char>& stream_;
};

/// \~chinese
/// @brief 文件序列化器
struct FileSerializer : public Serializer
{
    FileSerializer(FILE* file)
        : file_(file)
    {
    }

    void WriteBytes(const uint8_t* bytes, size_t size) override
    {
        std::fwrite(bytes, sizeof(uint8_t), size, file_);
    }

private:
    FILE* file_;
};

/// \~chinese
/// @brief 反序列化器
struct Deserializer
{
    /// \~chinese
    /// @brief 读取字节序列
    virtual void ReadBytes(uint8_t* bytes, size_t size) = 0;

    /// \~chinese
    /// @brief 读取值
    template <typename _Ty>
    void ReadValue(_Ty* value)
    {
        static_assert(std::is_trivial<_Ty>::value, "_Ty must be trivial type.");

        size_t size = sizeof(_Ty) / sizeof(uint8_t);
        this->ReadBytes(reinterpret_cast<uint8_t*>(value), size);
    }

    template <typename _Ty>
    Deserializer& operator>>(_Ty& value)
    {
        this->ReadValue(&value);
        return (*this);
    }
};

/// \~chinese
/// @brief 字节串反序列化器
struct ByteDeserializer : public Deserializer
{
    ByteDeserializer(const Vector<uint8_t>& bytes)
        : bytes_(bytes)
        , index_(0)
    {
    }

    void ReadBytes(uint8_t* bytes, size_t size) override
    {
        if (index_ + size > bytes_.size())
            throw std::ios_base::failure("ByteDeserializer::ReadBytes");

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&bytes_[index_]);
        std::memcpy(bytes, ptr, size);
        index_ += size;
    }

private:
    size_t index_;
    const Vector<uint8_t>& bytes_;
};

/// \~chinese
/// @brief 流反序列化器
struct StreamDeserializer : public Deserializer
{
    StreamDeserializer(std::basic_istream<char>& stream)
        : stream_(stream)
    {
    }

    void ReadBytes(uint8_t* bytes, size_t size) override
    {
        stream_.read(reinterpret_cast<char*>(bytes), size);
    }

private:
    std::basic_istream<char>& stream_;
};

/// \~chinese
/// @brief 文件反序列化器
struct FileDeserializer : public Deserializer
{
    FileDeserializer(FILE* file)
        : file_(file)
    {
    }

    void ReadBytes(uint8_t* bytes, size_t size) override
    {
        size_t count = std::fread(bytes, sizeof(uint8_t), size, file_);
        if (count < size)
        {
            throw std::ios_base::failure("FileDeserializer::ReadBytes");
        }
    }

private:
    FILE* file_;
};

/// \~chinese
/// @brief 可序列化对象
class Serializable
{
public:
    /// \~chinese
    /// @brief 序列化为字节串
    inline Vector<uint8_t> Serialize() const
    {
        Vector<uint8_t> data;
        ByteSerializer  serializer(data);
        this->DoSerialize(&serializer);
        return data;
    }

    /// \~chinese
    /// @brief 从字节串反序列化
    inline void Deserialize(const Vector<uint8_t>& data)
    {
        ByteDeserializer deserializer(data);
        this->DoDeserialize(&deserializer);
    }

    /// \~chinese
    /// @brief 执行序列化
    virtual void DoSerialize(Serializer* serializer) const = 0;

    /// \~chinese
    /// @brief 执行反序列化
    virtual void DoDeserialize(Deserializer* deserializer) = 0;
};


//
// operator<< for Serializer
//
inline Serializer& operator<<(Serializer& serializer, const char* str)
{
    size_t len = std::char_traits<char>::length(str);
    serializer.WriteValue(len);
    if (len)
    {
        serializer.WriteBytes(reinterpret_cast<const uint8_t*>(str), len);
    }
    return serializer;
}

inline Serializer& operator<<(Serializer& serializer, StringView str)
{
    return serializer << str.data();
}

template <typename _Ty>
inline Serializer& operator<<(Serializer& serializer, const Vector<_Ty>& arr)
{
    size_t size = arr.size();
    serializer.WriteValue(size);
    for (const auto& v : arr)
    {
        serializer << v;
    }
    return serializer;
}

template <typename _Ty>
inline Serializer& operator<<(Serializer& serializer, const List<_Ty>& list)
{
    size_t size = list.size();
    serializer.WriteValue(size);
    for (const auto& v : list)
    {
        serializer << v;
    }
    return serializer;
}

template <typename _Ty>
inline Serializer& operator<<(Serializer& serializer, const Set<_Ty>& set)
{
    size_t size = set.size();
    serializer.WriteValue(size);
    for (const auto& v : set)
    {
        serializer << v;
    }
    return serializer;
}

template <typename _KTy, typename _Ty>
inline Serializer& operator<<(Serializer& serializer, const Map<_KTy, _Ty>& map)
{
    size_t size = map.size();
    serializer.WriteValue(size);
    for (const auto& p : map)
    {
        serializer << p.first << p.second;
    }
    return serializer;
}

inline Serializer& operator<<(Serializer& serializer, const math::Vec2T<float>& vec)
{
    return serializer << vec.x << vec.y;
}

inline Serializer& operator<<(Serializer& serializer, const math::RectT<float>& rect)
{
    return serializer << rect.left_top << rect.right_bottom;
}

inline Serializer& operator<<(Serializer& serializer, const math::TransformT<float>& transform)
{
    return serializer << transform.position << transform.rotation << transform.scale << transform.skew;
}


//
// operator>> for Deserializer
//
inline Deserializer& operator>>(Deserializer& deserializer, char* str)
{
    size_t len = 0;
    deserializer.ReadValue(&len);
    if (len)
    {
        deserializer.ReadBytes(reinterpret_cast<uint8_t*>(str), len);
    }
    return deserializer;
}

inline Deserializer& operator>>(Deserializer& deserializer, String& str)
{
    size_t len = 0;
    deserializer.ReadValue(&len);
    if (len)
    {
        str.resize(len);
        deserializer.ReadBytes(reinterpret_cast<uint8_t*>(&str[0]), len);
    }
    return deserializer;
}

template <typename _Ty>
inline Deserializer& operator>>(Deserializer& deserializer, Vector<_Ty>& arr)
{
    size_t len = 0;
    deserializer.ReadValue(&len);
    for (size_t i = 0; i < len; ++i)
    {
        _Ty value;
        deserializer >> value;
        arr.push_back(value);
    }
    return deserializer;
}

template <typename _Ty>
inline Deserializer& operator>>(Deserializer& deserializer, List<_Ty>& list)
{
    size_t len = 0;
    deserializer.ReadValue(&len);
    for (size_t i = 0; i < len; ++i)
    {
        _Ty value;
        deserializer >> value;
        list.push_back(value);
    }
    return deserializer;
}

template <typename _Ty>
inline Deserializer& operator>>(Deserializer& deserializer, Set<_Ty>& set)
{
    size_t len = 0;
    deserializer.ReadValue(&len);
    for (size_t i = 0; i < len; ++i)
    {
        _Ty value;
        deserializer >> value;
        set.insert(value);
    }
    return deserializer;
}

template <typename _KTy, typename _Ty>
inline Deserializer& operator>>(Deserializer& deserializer, Map<_KTy, _Ty>& map)
{
    size_t len = 0;
    deserializer.ReadValue(&len);
    for (size_t i = 0; i < len; ++i)
    {
        _KTy key;
        _Ty  value;
        deserializer >> key >> value;
        map[key] = value;
    }
    return deserializer;
}

inline Deserializer& operator>>(Deserializer& deserializer, math::Vec2T<float>& vec)
{
    return deserializer >> vec.x >> vec.y;
}

inline Deserializer& operator>>(Deserializer& deserializer, math::RectT<float>& rect)
{
    return deserializer >> rect.left_top >> rect.right_bottom;
}

inline Deserializer& operator>>(Deserializer& deserializer, math::TransformT<float>& transform)
{
    return deserializer >> transform.position >> transform.rotation >> transform.scale >> transform.skew;
}

/** @} */


}  // namespace kiwano
