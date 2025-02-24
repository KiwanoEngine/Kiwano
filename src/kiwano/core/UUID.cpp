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
#include "UUID.h"
#include <stdexcept>

namespace kiwano
{

String UUID::ToString() const
{
    char s[39];
    ::sprintf_s(s, sizeof(s), "%08x-%04x-%04x-%04x-%012llx", part1, part2, part3, (part4[0] << 8) | part4[1],
                *((unsigned long long*)&part4[2]));
    return String(s);
}

UUID UUID::FromString(StringView s)
{
    UUID uuid = { 0 };

    unsigned int data4[8] = { 0 };
    if (::sscanf_s(s.data(), "%8lx-%4hx-%4hx-%2x%2x-%2x%2x%2x%2x%2x%2x", &uuid.part1, &uuid.part2, &uuid.part3,
                   &data4[0], &data4[1], &data4[2], &data4[3], &data4[4], &data4[5], &data4[6], &data4[7])
        != 11)
    {
        throw std::invalid_argument("Invalid UUID format");
    }
    for (int i = 0; i < 8; ++i)
    {
        uuid.part4[i] = static_cast<uint8_t>(data4[i]);
    }
    return uuid;
}

}  // namespace kiwano
