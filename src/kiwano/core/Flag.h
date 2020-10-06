// Copyright (c) 2019-2020 Kiwano - Nomango
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
#include <kiwano/core/BitOperator.h>
#include <cstdint>  // uint8_t

namespace kiwano
{

template <typename _Ty>
class Flag
{
public:
    static_assert(std::is_arithmetic<_Ty>::value, "_Ty must be an arithmetic type");

    typedef _Ty value_type;

    _Ty value;

    inline Flag()
        : value()
    {
    }

    inline Flag(_Ty value)
        : value(value)
    {
    }

    inline void Set(_Ty value)
    {
        bits::Set(this->value, value);
    }

    inline void Unset(_Ty value)
    {
        bits::Unset(this->value, value);
    }

    inline bool Has(_Ty value) const
    {
        return bits::Has(this->value, value);
    }
};

template <typename _Ty>
struct IsFlag : public std::false_type
{
};

template <typename _Ty>
struct IsFlag<Flag<_Ty>> : public std::true_type
{
};

typedef Flag<uint8_t>  FlagUint8;
typedef Flag<uint16_t> FlagUint16;
typedef Flag<uint32_t> FlagUint32;
typedef Flag<uint64_t> FlagUint64;
typedef Flag<int8_t>   FlagInt8;
typedef Flag<int16_t>  FlagInt16;
typedef Flag<int32_t>  FlagInt32;
typedef Flag<int64_t>  FlagInt64;

namespace bits
{

template <>
inline void Set<FlagUint8>(FlagUint8& old, FlagUint8 flag)
{
    old.Set(flag.value);
}

template <>
inline void Set<FlagUint16>(FlagUint16& old, FlagUint16 flag)
{
    old.Set(flag.value);
}

template <>
inline void Set<FlagUint32>(FlagUint32& old, FlagUint32 flag)
{
    old.Set(flag.value);
}

template <>
inline void Set<FlagUint64>(FlagUint64& old, FlagUint64 flag)
{
    old.Set(flag.value);
}

template <>
inline void Set<FlagInt8>(FlagInt8& old, FlagInt8 flag)
{
    old.Set(flag.value);
}

template <>
inline void Set<FlagInt16>(FlagInt16& old, FlagInt16 flag)
{
    old.Set(flag.value);
}

template <>
inline void Set<FlagInt32>(FlagInt32& old, FlagInt32 flag)
{
    old.Set(flag.value);
}

template <>
inline void Set<FlagInt64>(FlagInt64& old, FlagInt64 flag)
{
    old.Set(flag.value);
}


template <>
inline void Unset<FlagUint8>(FlagUint8& old, FlagUint8 flag)
{
    old.Unset(flag.value);
}

template <>
inline void Unset<FlagUint16>(FlagUint16& old, FlagUint16 flag)
{
    old.Unset(flag.value);
}

template <>
inline void Unset<FlagUint32>(FlagUint32& old, FlagUint32 flag)
{
    old.Unset(flag.value);
}

template <>
inline void Unset<FlagUint64>(FlagUint64& old, FlagUint64 flag)
{
    old.Unset(flag.value);
}

template <>
inline void Unset<FlagInt8>(FlagInt8& old, FlagInt8 flag)
{
    old.Unset(flag.value);
}

template <>
inline void Unset<FlagInt16>(FlagInt16& old, FlagInt16 flag)
{
    old.Unset(flag.value);
}

template <>
inline void Unset<FlagInt32>(FlagInt32& old, FlagInt32 flag)
{
    old.Unset(flag.value);
}

template <>
inline void Unset<FlagInt64>(FlagInt64& old, FlagInt64 flag)
{
    old.Unset(flag.value);
}


template <>
inline bool Has<FlagUint8>(FlagUint8 old, FlagUint8 flag)
{
    return old.Has(flag.value);
}

template <>
inline bool Has<FlagUint16>(FlagUint16 old, FlagUint16 flag)
{
    return old.Has(flag.value);
}

template <>
inline bool Has<FlagUint32>(FlagUint32 old, FlagUint32 flag)
{
    return old.Has(flag.value);
}

template <>
inline bool Has<FlagUint64>(FlagUint64 old, FlagUint64 flag)
{
    return old.Has(flag.value);
}

template <>
inline bool Has<FlagInt8>(FlagInt8 old, FlagInt8 flag)
{
    return old.Has(flag.value);
}

template <>
inline bool Has<FlagInt16>(FlagInt16 old, FlagInt16 flag)
{
    return old.Has(flag.value);
}

template <>
inline bool Has<FlagInt32>(FlagInt32 old, FlagInt32 flag)
{
    return old.Has(flag.value);
}

template <>
inline bool Has<FlagInt64>(FlagInt64 old, FlagInt64 flag)
{
    return old.Has(flag.value);
}


}  // namespace bits
}  // namespace kiwano
