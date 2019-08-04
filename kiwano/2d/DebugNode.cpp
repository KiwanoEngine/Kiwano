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

#include "DebugNode.h"
#include "Text.h"
#include "../renderer/render.h"
#include <sstream>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

namespace kiwano
{
	DebugNode::DebugNode()
		: background_color_(0.0f, 0.0f, 0.0f, 0.7f)
	{
		SetName(L"kiwano-debug-node");
		SetPosition(10, 10);
		SetResponsible(true);
		SetCascadeOpacityEnabled(true);

		debug_text_ = new Text;
		debug_text_->SetPosition(10, 10);
		this->AddChild(debug_text_);

		Font font;
		font.size = 16.f;
		font.weight = FontWeight::Normal;
		debug_text_->SetFont(font);

		TextStyle style;
		style.wrap = false;
		style.line_spacing = 20.f;
		debug_text_->SetStyle(style);

		AddListener(Event::MouseHover, [=](const Event&) { SetOpacity(0.4f); });
		AddListener(Event::MouseOut, [=](const Event&) { SetOpacity(1.f); });
	}

	DebugNode::~DebugNode()
	{
	}

	void DebugNode::OnRender()
	{
		Renderer::Instance()->GetSolidColorBrush()->SetColor(DX::ConvertToColorF(background_color_));

		Renderer::Instance()->GetD2DDeviceResources()->GetDeviceContext()->FillRoundedRectangle(
			D2D1::RoundedRect(DX::ConvertToRectF(GetBounds()), 5.f, 5.f),
			Renderer::Instance()->GetSolidColorBrush()
		);
	}

	void DebugNode::OnUpdate(Duration dt)
	{
		KGE_NOT_USED(dt);

		frame_time_.push_back(Time::Now());
		while (frame_time_.back() - frame_time_.front() >= time::Sec)
		{
			frame_time_.erase(frame_time_.begin());
		}
		
		std::wstringstream ss;
		ss << "Fps: " << frame_time_.size() << std::endl;

#if defined(KGE_DEBUG)
		if (Object::IsTracingLeaks())
		{
			ss << "Objects: " << Object::__GetTracingObjects().size() << std::endl;
		}
#endif

		ss << "Render: " << Renderer::Instance()->GetStatus().duration.Milliseconds() << "ms" << std::endl;

		ss << "Primitives / sec: " << Renderer::Instance()->GetStatus().primitives * frame_time_.size() << std::endl;

		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		ss << "Memory: " << pmc.PrivateUsage / 1024 << "kb";

		debug_text_->SetText(ss.str());
		SetSize(Size{ 20 + debug_text_->GetLayoutSize().x, 20 + debug_text_->GetLayoutSize().y });
	}

}
