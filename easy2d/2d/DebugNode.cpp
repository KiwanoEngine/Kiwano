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

#include "DebugNode.h"
#include "Text.h"
#include "../renderer/render.h"
#include <sstream>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

namespace easy2d
{
	DebugNode::DebugNode()
	{
		debug_text_ = new Text();
		debug_text_->SetPosition(20, 20);
		this->AddChild(debug_text_);

		Font font;
		font.size = 16.f;
		font.weight = FontWeight::Normal;
		debug_text_->SetFont(font);

		TextStyle style;
		style.wrap = false;
		style.line_spacing = 20.f;
		debug_text_->SetStyle(style);
	}

	DebugNode::~DebugNode()
	{
	}

	void DebugNode::OnRender()
	{
		Renderer::Instance().GetSolidColorBrush()->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));

		Renderer::Instance().GetDeviceResources()->GetD2DDeviceContext()->FillRectangle(
			D2D1_RECT_F{ 10, 10, 30 + debug_text_->GetWidth(), 30 + debug_text_->GetHeight() },
			Renderer::Instance().GetSolidColorBrush()
		);
	}

	void DebugNode::OnUpdate(Duration dt)
	{
		E2D_NOT_USED(dt);

		frame_time_.push_back(time::Now());
		while (frame_time_.back() - frame_time_.front() >= time::Second)
		{
			frame_time_.erase(frame_time_.begin());
		}
		
		std::wstringstream ss;
		ss << "Fps: " << frame_time_.size() << std::endl;

#ifdef E2D_DEBUG
		ss << "Objects: " << Object::__GetTracingObjects().size() << std::endl;
#endif

		ss << "Render: " << Renderer::Instance().GetStatus().duration.Milliseconds() << "ms" << std::endl;

		ss << "Primitives / sec: " << Renderer::Instance().GetStatus().primitives * frame_time_.size() << std::endl;

		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		ss << "Memory: " << pmc.PrivateUsage / 1024 << "kb";

		debug_text_->SetText(ss.str());

		debug_text_->SetSize(debug_text_->GetLayoutSize());
	}

}
