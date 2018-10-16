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

#include "..\e2dtransition.h"
#include "..\e2dobject.h"

easy2d::BoxTransition::BoxTransition(float duration)
	: Transition(duration)
{
}

void easy2d::BoxTransition::Init(Scene * prev, Scene * next, Game * game)
{
	Transition::Init(prev, next, game);

	in_layer_param_.opacity = 0;
}

void easy2d::BoxTransition::Update()
{
	Transition::Update();

	if (process_ < .5f)
	{
		out_layer_param_.contentBounds = D2D1::RectF(
			window_size_.width * process_,
			window_size_.height * process_,
			window_size_.width * (1 - process_),
			window_size_.height * (1 - process_)
		);
	}
	else
	{
		out_layer_param_.opacity = 0;
		in_layer_param_.opacity = 1;
		in_layer_param_.contentBounds = D2D1::RectF(
			window_size_.width * (1 - process_),
			window_size_.height * (1 - process_),
			window_size_.width * process_,
			window_size_.height * process_
		);
	}
}
