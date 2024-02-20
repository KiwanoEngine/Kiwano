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

#include <kiwano/2d/animation/EaseFunc.h>
#include <kiwano/math/Math.h>

namespace kiwano
{

inline EaseFunc MakeEaseIn(float rate)
{
    return std::bind(math::EaseIn, std::placeholders::_1, rate);
}
inline EaseFunc MakeEaseOut(float rate)
{
    return std::bind(math::EaseOut, std::placeholders::_1, rate);
}
inline EaseFunc MakeEaseInOut(float rate)
{
    return std::bind(math::EaseInOut, std::placeholders::_1, rate);
}
inline EaseFunc MakeEaseElasticIn(float period)
{
    return std::bind(math::EaseElasticIn, std::placeholders::_1, period);
}
inline EaseFunc MakeEaseElasticOut(float period)
{
    return std::bind(math::EaseElasticOut, std::placeholders::_1, period);
}
inline EaseFunc MakeEaseElasticInOut(float period)
{
    return std::bind(math::EaseElasticInOut, std::placeholders::_1, period);
}

KGE_API EaseFunc Ease::Linear       = math::Linear;
KGE_API EaseFunc Ease::EaseIn       = MakeEaseIn(2.f);
KGE_API EaseFunc Ease::EaseOut      = MakeEaseOut(2.f);
KGE_API EaseFunc Ease::EaseInOut    = MakeEaseInOut(2.f);
KGE_API EaseFunc Ease::ExpoIn       = math::EaseExponentialIn;
KGE_API EaseFunc Ease::ExpoOut      = math::EaseExponentialOut;
KGE_API EaseFunc Ease::ExpoInOut    = math::EaseExponentialInOut;
KGE_API EaseFunc Ease::BounceIn     = math::EaseBounceIn;
KGE_API EaseFunc Ease::BounceOut    = math::EaseBounceOut;
KGE_API EaseFunc Ease::BounceInOut  = math::EaseBounceInOut;
KGE_API EaseFunc Ease::ElasticIn    = MakeEaseElasticIn(0.3f);
KGE_API EaseFunc Ease::ElasticOut   = MakeEaseElasticOut(0.3f);
KGE_API EaseFunc Ease::ElasticInOut = MakeEaseElasticInOut(0.3f);
KGE_API EaseFunc Ease::SineIn       = math::EaseSineIn;
KGE_API EaseFunc Ease::SineOut      = math::EaseSineOut;
KGE_API EaseFunc Ease::SineInOut    = math::EaseSineInOut;
KGE_API EaseFunc Ease::BackIn       = math::EaseBackIn;
KGE_API EaseFunc Ease::BackOut      = math::EaseBackOut;
KGE_API EaseFunc Ease::BackInOut    = math::EaseBackInOut;
KGE_API EaseFunc Ease::QuadIn       = math::EaseQuadIn;
KGE_API EaseFunc Ease::QuadOut      = math::EaseQuadOut;
KGE_API EaseFunc Ease::QuadInOut    = math::EaseQuadInOut;
KGE_API EaseFunc Ease::CubicIn      = math::EaseCubicIn;
KGE_API EaseFunc Ease::CubicOut     = math::EaseCubicOut;
KGE_API EaseFunc Ease::CubicInOut   = math::EaseCubicInOut;
KGE_API EaseFunc Ease::QuartIn      = math::EaseQuartIn;
KGE_API EaseFunc Ease::QuartOut     = math::EaseQuartOut;
KGE_API EaseFunc Ease::QuartInOut   = math::EaseQuartInOut;
KGE_API EaseFunc Ease::QuintIn      = math::EaseQuintIn;
KGE_API EaseFunc Ease::QuintOut     = math::EaseQuintOut;
KGE_API EaseFunc Ease::QuintInOut   = math::EaseQuintInOut;

}  // namespace kiwano
