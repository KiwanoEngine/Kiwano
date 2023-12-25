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
    va_list args = nullptr;
    va_start(args, format);

    String result = FormatArgs(format, args);

    va_end(args);
    return result;
}

WideString Format(const wchar_t* format, ...)
{
    va_list args = nullptr;
    va_start(args, format);

    WideString result = FormatArgs(format, args);

    va_end(args);
    return result;
}

String FormatArgs(const char* format, va_list args)
{
    String result;
    if (format)
    {
        const auto len = static_cast<size_t>(::_vscprintf(format, args) + 1);
        if (len)
        {
            result.resize(len - 1);
            ::_vsnprintf_s(&result[0], len, len, format, args);
        }
    }
    return result;
}

WideString FormatArgs(const wchar_t* format, va_list args)
{
    WideString result;
    if (format)
    {
        const auto len = static_cast<size_t>(::_vscwprintf(format, args) + 1);
        if (len)
        {
            result.resize(len - 1);
            ::_vsnwprintf_s(&result[0], len, len, format, args);
        }
    }
    return result;
}

String WideToNarrowWithCodePage(WideStringView str, UINT code_page)
{
    if (str.empty())
        return String();

    int len = ::WideCharToMultiByte(code_page, 0, str.data(), -1, NULL, 0, NULL, NULL);
    if (len > 0)
    {
        String result;
        result.resize(len - 1);

        ::WideCharToMultiByte(code_page, 0, str.data(), -1, &result[0], len, NULL, NULL);
        return result;
    }
    return String();
}

WideString NarrowToWideWithCodePage(StringView str, UINT code_page)
{
    if (str.empty())
        return WideString();

    int len = ::MultiByteToWideChar(code_page, 0, str.data(), -1, NULL, 0);
    if (len > 0)
    {
        WideString result;
        result.resize(len - 1);

        ::MultiByteToWideChar(code_page, 0, str.data(), -1, &result[0], len);
        return result;
    }
    return WideString();
}

String WideToNarrow(WideStringView str)
{
    return WideToNarrowWithCodePage(str, CP_ACP);
}

WideString NarrowToWide(StringView str)
{
    return NarrowToWideWithCodePage(str, CP_ACP);
}

String WideToUTF8(WideStringView str)
{
    return WideToNarrowWithCodePage(str, CP_UTF8);
}

WideString UTF8ToWide(StringView str)
{
    return NarrowToWideWithCodePage(str, CP_UTF8);
}

#endif  // KGE_PLATFORM_WINDOWS

}  // namespace string
}  // namespace kiwano
