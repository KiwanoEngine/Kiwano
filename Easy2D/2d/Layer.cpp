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
#include "Layer.h"
#include "../renderer/render.h"

namespace easy2d
{
	Layer::Layer()
	{
		SetSize(Renderer::Instance().GetOutputSize());

		AddListener(Event::MouseBtnDown, Closure(this, &Layer::HandleMessages));
		AddListener(Event::MouseBtnUp, Closure(this, &Layer::HandleMessages));
		AddListener(Event::MouseMove, Closure(this, &Layer::HandleMessages));
		AddListener(Event::MouseWheel, Closure(this, &Layer::HandleMessages));

		AddListener(Event::KeyDown, Closure(this, &Layer::HandleMessages));
		AddListener(Event::KeyUp, Closure(this, &Layer::HandleMessages));
		AddListener(Event::Char, Closure(this, &Layer::HandleMessages));
	}

	Layer::~Layer()
	{
	}

	void Layer::Dispatch(Event& evt)
	{
		if (!IsVisible())
			return;

		NodePtr prev;
		for (auto child = children_.Last(); child; child = prev)
		{
			prev = child->PrevItem();
			child->Dispatch(evt);
		}

		EventDispatcher::Dispatch(evt);
	}

	void Layer::HandleMessages(Event const & evt)
	{
		switch (evt.type)
		{
		case Event::MouseBtnDown:
			OnMouseButtonDown(evt.mouse.button, Point{ evt.mouse.x, evt.mouse.y });
			break;
		case Event::MouseBtnUp:
			OnMouseButtonUp(evt.mouse.button, Point{ evt.mouse.x, evt.mouse.y });
			break;
		case Event::MouseMove:
			OnMouseMoved(Point{ evt.mouse.x, evt.mouse.y });
			break;
		case Event::MouseWheel:
			OnMouseWheel(evt.mouse.wheel);
			break;
		case Event::KeyDown:
			OnKeyDown(evt.key.code);
			break;
		case Event::KeyUp:
			OnKeyUp(evt.key.code);
			break;
		case Event::Char:
			OnChar(evt.key.c);
			break;
		}
	}

}
