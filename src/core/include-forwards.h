// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include "macros.h"
#include "Color.h"
#include "Object.h"
#include "helper.hpp"
#include "../DX/helper.hpp"

namespace easy2d
{
	// ª≠± —˘ Ω
	enum class StrokeStyle : int
	{
		Miter = 0,	/* –±«– */
		Bevel = 1,	/* –±Ω« */
		Round = 2	/* ‘≤Ω« */
	};

	// ∑ΩœÚ
	enum class Direction : int
	{
		Up,		/* …œ */
		Down,	/* œ¬ */
		Left,	/* ◊Û */
		Right	/* ”“ */
	};

	// Œƒ◊÷øπæ‚≥› Ù–‘
	enum class TextAntialias
	{
		Default,	// œµÕ≥ƒ¨»œ
		ClearType,	// ClearType øπæ‚≥›
		GrayScale,	// ª“∂»øπæ‚≥›
		None		// ≤ª∆Ù”√øπæ‚≥›
	};

	// Õº≤„ Ù–‘
	struct LayerProperties
	{
		Rect area;
		float opacity;
	};
}
