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
#include "Layer.h"
#include "../renderer/Renderer.h"

namespace kiwano
{
	Layer::Layer()
		: swallow_(false)
	{
		SetSize(Renderer::GetInstance()->GetOutputSize());

		auto handler = Closure(this, &Layer::HandleMessages);

		AddListener(Event::MouseBtnDown, handler);
		AddListener(Event::MouseBtnUp, handler);
		AddListener(Event::MouseMove, handler);
		AddListener(Event::MouseWheel, handler);

		AddListener(Event::KeyDown, handler);
		AddListener(Event::KeyUp, handler);
		AddListener(Event::Char, handler);
	}

	Layer::~Layer()
	{
	}

	void Layer::Dispatch(Event& evt)
	{
		if (!IsVisible())
			return;

		if (!swallow_)
		{
			ActorPtr prev;
			for (auto child = children_.last_item(); child; child = prev)
			{
				prev = child->prev_item();
				child->Dispatch(evt);
			}
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
