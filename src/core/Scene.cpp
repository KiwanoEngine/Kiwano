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

#include "Scene.h"
#include "logs.h"
#include "render.h"

namespace easy2d
{
	Scene::Scene()
		: mouse_cursor_(MouseCursor::Arrow)
		, last_mouse_cursor(MouseCursor(-1))
	{
		scene_ = this;

		SetSize(Renderer::Instance().GetOutputSize());
	}

	Scene::~Scene()
	{
	}

	void Scene::OnEnter()
	{
		E2D_LOG(L"Scene entered");
	}

	void Scene::OnExit()
	{
		E2D_LOG(L"Scene exited");
	}

	void Scene::Update(Duration dt)
	{
		Node::Update(dt);

		if (last_mouse_cursor != mouse_cursor_)
		{
			last_mouse_cursor = mouse_cursor_;

			LPTSTR win32_cursor = IDC_ARROW;
			switch (mouse_cursor_)
			{
			case MouseCursor::Arrow:		win32_cursor = IDC_ARROW; break;
			case MouseCursor::TextInput:	win32_cursor = IDC_IBEAM; break;
			case MouseCursor::SizeAll:		win32_cursor = IDC_SIZEALL; break;
			case MouseCursor::SizeWE:		win32_cursor = IDC_SIZEWE; break;
			case MouseCursor::SizeNS:		win32_cursor = IDC_SIZENS; break;
			case MouseCursor::SizeNESW:		win32_cursor = IDC_SIZENESW; break;
			case MouseCursor::SizeNWSE:		win32_cursor = IDC_SIZENWSE; break;
			case MouseCursor::Hand:			win32_cursor = IDC_HAND; break;
			}
			::SetCursor(::LoadCursorW(nullptr, win32_cursor));
		}
	}

	void Scene::SetMouseCursor(MouseCursor cursor)
	{
		mouse_cursor_ = cursor;
	}

}
