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

#include <kiwano/2d/DebugActor.h>
#include <kiwano/2d/Text.h>
#include <kiwano/renderer/Renderer.h>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

namespace kiwano
{
	namespace
	{
		class comma_numpunct : public std::numpunct<wchar_t>
		{
		protected:
			virtual wchar_t do_thousands_sep() const override
			{
				return L',';
			}

			virtual std::string do_grouping() const override
			{
				return "\03";
			}
		};
	}

	DebugActor::DebugActor()
		: background_color_(0.0f, 0.0f, 0.0f, 0.7f)
	{
		SetName(L"kiwano-debug-actor");
		SetPosition(Point{ 10, 10 });
		SetResponsible(true);
		SetCascadeOpacityEnabled(true);

		debug_text_ = new Text;
		debug_text_->SetPosition(Point{ 10, 10 });
		this->AddChild(debug_text_);

		Font font;
		font.family = L"Arial";
		font.size = 16.f;
		font.weight = FontWeight::Normal;
		debug_text_->SetFont(font);

		TextStyle style;
		style.line_spacing = 20.f;
		debug_text_->SetStyle(style);

		AddListener<MouseHoverEvent>([=](Event&) { SetOpacity(0.4f); });
		AddListener<MouseOutEvent>([=](Event&) { SetOpacity(1.f); });
	}

	DebugActor::~DebugActor()
	{
	}

	void DebugActor::OnRender(RenderTarget* rt)
	{
		PrepareRender(rt);

		rt->SetDefaultBrushColor(background_color_);
		rt->FillRoundedRectangle(GetBounds(), Vec2{ 5.f, 5.f });
	}

	void DebugActor::OnUpdate(Duration dt)
	{
		KGE_NOT_USED(dt);

		frame_time_.push_back(Time::Now());
		while (frame_time_.back() - frame_time_.front() >= Duration::Second)
		{
			frame_time_.erase(frame_time_.begin());
		}

		StringStream ss;

		{
			// For formatting integers with commas
			static std::locale comma_locale(std::locale(), new comma_numpunct);
			(void)ss.imbue(comma_locale);
		}

		ss << "Fps: " << frame_time_.size() << std::endl;

#if defined(KGE_DEBUG)
		if (ObjectBase::IsTracingLeaks())
		{
			ss << "Objects: " << ObjectBase::__GetTracingObjects().size() << std::endl;
		}
#endif

		ss << "Render: " << Renderer::instance().GetStatus().duration.Milliseconds() << "ms" << std::endl;

		ss << "Primitives / sec: " << std::fixed << Renderer::instance().GetStatus().primitives * frame_time_.size() << std::endl;

		ss << "Memory: ";
		{
			PROCESS_MEMORY_COUNTERS_EX pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

			if (pmc.PrivateUsage > 1024 * 1024)
			{
				ss << pmc.PrivateUsage / (1024 * 1024) << "Mb ";
				pmc.PrivateUsage %= (1024 * 1024);
			}

			ss << pmc.PrivateUsage / 1024 << "Kb";
		}

		debug_text_->SetText(ss.str());

		if (debug_text_->GetWidth() > GetWidth() - 20)
		{
			SetWidth(20 + debug_text_->GetWidth());
		}

		if (debug_text_->GetHeight() > GetHeight() - 20)
		{
			SetHeight(20 + debug_text_->GetHeight());
		}
	}

}
