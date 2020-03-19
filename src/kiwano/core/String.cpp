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
#include <kiwano/macros.h>
#include <kiwano/core/String.h>

namespace kiwano
{
namespace strings
{

#if defined(KGE_PLATFORM_WINDOWS)

String Format(const char* format, ...)
{
    String result;
    if (format)
    {
        va_list args = nullptr;
        va_start(args, format);

        const auto len = static_cast<size_t>(::_vscprintf(format, args) + 1);
        if (len)
        {
            result.resize(len);
            ::_vsnprintf_s(&result[0], len, len, format, args);
        }
        va_end(args);
    }
    return result;
}

WideString Format(const wchar_t* format, ...)
{
    WideString result;
    if (format)
    {
        va_list args = nullptr;
        va_start(args, format);

        const auto len = static_cast<size_t>(::_vscwprintf(format, args) + 1);
        if (len)
        {
            result.resize(len);
            ::_vsnwprintf_s(&result[0], len, len, format, args);
        }
        va_end(args);
    }
    return result;
}

String ToNarrow(const WideString& str)
{
    if (str.empty())
        return String();

    int chars_num = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
    if (chars_num)
    {
        String result;
        result.resize(chars_num);

        ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &result[0], chars_num, NULL, NULL);
        return result;
    }
    return String();
}

WideString ToWide(const String& str)
{
    if (str.empty())
        return WideString();

    int chars_num = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    if (chars_num)
    {
        WideString result;
        result.resize(chars_num);

        ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &result[0], chars_num);
        return result;
    }
    return WideString();
}

#endif  // KGE_PLATFORM_WINDOWS

}  // namespace string
}  // namespace kiwano
