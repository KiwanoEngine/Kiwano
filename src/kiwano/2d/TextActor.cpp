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

#include <kiwano/2d/TextActor.h>
#include <kiwano/core/Logger.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	namespace
	{
		TextStyle text_default_style;

		void InitDefaultTextStyle()
		{
			static bool inited = false;
			if (!inited)
			{
				inited = true;

				// 默认使用白色画刷填充文字
				text_default_style.fill_brush = new Brush;
				text_default_style.fill_brush->SetColor(Color::White);
			}
		}
	}

	void TextActor::SetDefaultStyle(TextStyle const & style)
	{
		text_default_style = style;
	}

	const TextStyle& TextActor::GetDefaultStyle()
	{
		InitDefaultTextStyle();
		return text_default_style;
	}

	TextActor::TextActor()
		: TextActor(String())
	{
	}

	TextActor::TextActor(String const& text)
		: TextActor(text, GetDefaultStyle())
	{
	}

	TextActor::TextActor(String const& text, const TextStyle& style)
		: show_underline_(false)
		, show_strikethrough_(false)
	{
		SetText(text);
		SetStyle(style);
	}

	TextActor::~TextActor()
	{
	}

	void TextActor::OnRender(RenderContext& ctx)
	{
		ctx.DrawTextLayout(text_layout_);
	}

	void TextActor::OnUpdate(Duration dt)
	{
		UpdateLayout();
	}

	void TextActor::UpdateLayout()
	{
		if (text_layout_.IsDirty())
		{
			text_layout_.Update();
		}

		if (text_layout_.GetDirtyFlag() & TextLayout::DirtyFlag::Updated)
		{
			text_layout_.SetDirtyFlag(TextLayout::DirtyFlag::Clean);

			if (show_underline_)
				text_layout_.SetUnderline(true, 0, text_layout_.GetText().length());

			if (show_strikethrough_)
				text_layout_.SetStrikethrough(true, 0, text_layout_.GetText().length());

			SetSize(text_layout_.GetLayoutSize());
		}
	}

	bool TextActor::CheckVisibilty(RenderContext& ctx) const
	{
		return text_layout_.IsValid() && Actor::CheckVisibilty(ctx);
	}

	void TextActor::SetFillColor(Color const& color)
	{
		if (!text_layout_.GetFillBrush() || text_layout_.GetFillBrush() == GetDefaultStyle().fill_brush)
		{
			BrushPtr brush = new Brush;
			text_layout_.SetFillBrush(brush);
		}
		text_layout_.GetFillBrush()->SetColor(color);
	}

	void TextActor::SetOutlineColor(Color const& outline_color)
	{
		if (!text_layout_.GetOutlineBrush() || text_layout_.GetOutlineBrush() == GetDefaultStyle().outline_brush)
		{
			BrushPtr brush = new Brush;
			text_layout_.SetOutlineBrush(brush);
		}
		text_layout_.GetOutlineBrush()->SetColor(outline_color);
	}

}
