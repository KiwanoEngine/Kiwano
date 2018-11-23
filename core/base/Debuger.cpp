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

#include "Debuger.h"
#include "../utils/string.h"
#include <sstream>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

namespace easy2d
{

	DebugerNode::DebugerNode()
	{
		debug_text_ = new Text();
		debug_text_->SetPosition(10, 10);
		this->AddChild(debug_text_);

		Font font;
		font.size = 16.f;
		font.weight = FontWeight::Normal;
		debug_text_->SetFont(font);

		TextStyle style;
		style.wrap = false;
		style.line_spacing = 18.f;
		debug_text_->SetStyle(style);

		Object::__RemoveObjectFromTracingList(this);
		Object::__RemoveObjectFromTracingList(debug_text_.Get());
	}

	DebugerNode::~DebugerNode()
	{
	}

	void DebugerNode::AddDebugText(std::wstring const & text)
	{
		try
		{
			texts_.push_back(text);
		}
		catch (...)
		{
		}
	}

	void DebugerNode::ClearDebugText()
	{
		texts_.clear();
	}

	void DebugerNode::OnUpdate(Duration const & dt)
	{
		try
		{
			frame_time_.push_back(time::Now());
			while (frame_time_.back() - frame_time_.front() >= time::Second)
			{
				frame_time_.erase(frame_time_.begin());
			}
		}
		catch (std::exception& e)
		{
			debug_text_->SetText(StringMultiByteToWideChar(e.what()));
			return;
		}
		
		std::wstringstream ss;
		ss << "fps=" << frame_time_.size() << std::endl;

#ifdef E2D_DEBUG

		ss << "objects=" << Object::__GetTracingObjects().size() << std::endl;

#endif

		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		if (pmc.PrivateUsage > 1024 * 1024)
			ss << "memory=" <<  pmc.PrivateUsage / 1024 / 1024 << "Mb " << (pmc.PrivateUsage / 1024) % 1024 << "Kb";
		else
			ss << "memory=" << pmc.PrivateUsage / 1024 << "Kb";

		for (const auto& text : texts_)
			ss << std::endl << text;

		debug_text_->SetText(ss.str());
	}

}
