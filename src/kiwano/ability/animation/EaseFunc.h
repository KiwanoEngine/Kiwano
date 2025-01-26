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

namespace kiwano
{

/// \~chinese
/// @brief ��������
using EaseFunc = Function<float(float)>;

/// \~chinese
/// @brief ��������ö��
/// @details �鿴 https://easings.net ��ȡ������Ϣ
struct Ease
{
    static KGE_API EaseFunc Linear;        ///< ����
    static KGE_API EaseFunc EaseIn;        ///< �������
    static KGE_API EaseFunc EaseOut;       ///< �ɿ����
    static KGE_API EaseFunc EaseInOut;     ///< �������, ���ɿ����
    static KGE_API EaseFunc ExpoIn;        ///< �����伫��
    static KGE_API EaseFunc ExpoOut;       ///< �ɼ������
    static KGE_API EaseFunc ExpoInOut;     ///< ����������, ���ɼ������
    static KGE_API EaseFunc ElasticIn;     ///< ����㸳�赯��
    static KGE_API EaseFunc ElasticOut;    ///< ���յ㸳�赯��
    static KGE_API EaseFunc ElasticInOut;  ///< �������յ㸳�赯��
    static KGE_API EaseFunc BounceIn;      ///< ����㸳�跴����
    static KGE_API EaseFunc BounceOut;     ///< ���յ㸳�跴����
    static KGE_API EaseFunc BounceInOut;   ///< �������յ㸳�跴����
    static KGE_API EaseFunc BackIn;
    static KGE_API EaseFunc BackOut;
    static KGE_API EaseFunc BackInOut;
    static KGE_API EaseFunc QuadIn;
    static KGE_API EaseFunc QuadOut;
    static KGE_API EaseFunc QuadInOut;
    static KGE_API EaseFunc CubicIn;
    static KGE_API EaseFunc CubicOut;
    static KGE_API EaseFunc CubicInOut;
    static KGE_API EaseFunc QuartIn;
    static KGE_API EaseFunc QuartOut;
    static KGE_API EaseFunc QuartInOut;
    static KGE_API EaseFunc QuintIn;
    static KGE_API EaseFunc QuintOut;
    static KGE_API EaseFunc QuintInOut;
    static KGE_API EaseFunc SineIn;
    static KGE_API EaseFunc SineOut;
    static KGE_API EaseFunc SineInOut;
};

}  // namespace kiwano
