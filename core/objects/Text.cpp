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

#include "..\e2dobject.h"
#include "..\e2dmodule.h"

//-------------------------------------------------------
// Style
//-------------------------------------------------------

e2d::Text::Style::Style()
	: color(Color::White)
	, alignment(Align::Left)
	, wrap(false)
	, wrap_width(0.f)
	, line_spacing(0.f)
	, underline(false)
	, strikethrough(false)
	, outline(true)
	, outline_color(Color(Color::Black, 0.5))
	, outline_width(1.f)
	, outline_stroke(Stroke::Round)
{}

e2d::Text::Style::Style(
	Color color,
	Align alignment,
	bool wrap,
	float wrap_width,
	float line_spacing,
	bool underline,
	bool strikethrough,
	bool outline,
	Color outline_color,
	float outline_width,
	Stroke outline_stroke
)
	: color(color)
	, alignment(alignment)
	, wrap(wrap)
	, wrap_width(wrap_width)
	, line_spacing(line_spacing)
	, underline(underline)
	, strikethrough(strikethrough)
	, outline(outline)
	, outline_color(outline_color)
	, outline_width(outline_width)
	, outline_stroke(outline_stroke)
{}



//-------------------------------------------------------
// Text
//-------------------------------------------------------

e2d::Text::Text()
	: font_()
	, style_()
	, text_layout_(nullptr)
	, text_format_(nullptr)
{
}

e2d::Text::Text(const String & text, const Font & font, const Style & style)
	: font_(font)
	, style_(style)
	, text_layout_(nullptr)
	, text_format_(nullptr)
	, text_(text)
{
	Reset();
}

e2d::Text::~Text()
{
	SafeRelease(text_format_);
	SafeRelease(text_layout_);
}

const e2d::String& e2d::Text::GetText() const
{
	return text_;
}

const e2d::Font& e2d::Text::GetFont() const
{
	return font_;
}

const e2d::Text::Style& e2d::Text::GetStyle() const
{
	return style_;
}

const e2d::String& e2d::Text::GetFontFamily() const
{
	return font_.family;
}

float e2d::Text::GetFontSize() const
{
	return font_.size;
}

UINT e2d::Text::GetFontWeight() const
{
	return font_.weight;
}

const e2d::Color& e2d::Text::GetColor() const
{
	return style_.color;
}

const e2d::Color& e2d::Text::GetOutlineColor() const
{
	return style_.outline_color;
}

float e2d::Text::GetOutlineWidth() const
{
	return style_.outline_width;
}

e2d::Stroke e2d::Text::GetOutlineStroke() const
{
	return style_.outline_stroke;
}

int e2d::Text::GetLineCount() const
{
	if (text_layout_)
	{
		DWRITE_TEXT_METRICS metrics;
		text_layout_->GetMetrics(&metrics);
		return static_cast<int>(metrics.lineCount);
	}
	else
	{
		return 0;
	}
}

bool e2d::Text::IsItalic() const
{
	return font_.italic;
}

bool e2d::Text::strikethrough() const
{
	return style_.strikethrough;
}

bool e2d::Text::underline() const
{
	return style_.underline;
}

bool e2d::Text::outline() const
{
	return style_.outline;
}

void e2d::Text::SetText(const String& text)
{
	text_ = text;
	Reset();
}

void e2d::Text::SetStyle(const Style& style)
{
	style_ = style;
	Reset();
}

void e2d::Text::SetFont(const Font & font)
{
	font_ = font;
	Reset();
}

void e2d::Text::SetFontFamily(const String& family)
{
	font_.family = family;
	Reset();
}

void e2d::Text::SetFontSize(float size)
{
	font_.size = size;
	Reset();
}

void e2d::Text::SetFontWeight(UINT weight)
{
	font_.weight = weight;
	Reset();
}

void e2d::Text::SetColor(Color color)
{
	style_.color = color;
}

void e2d::Text::SetItalic(bool value)
{
	font_.italic = value;
	Reset();
}

void e2d::Text::SetWrapEnabled(bool wrap)
{
	if (style_.wrap != wrap)
	{
		style_.wrap = wrap;
		Reset();
	}
}

void e2d::Text::SetWrapWidth(float wrap_width)
{
	if (style_.wrap_width != wrap_width)
	{
		style_.wrap_width = std::max(wrap_width, 0.f);

		if (style_.wrap)
		{
			Reset();
		}
	}
}

void e2d::Text::SetLineSpacing(float line_spacing)
{
	if (style_.line_spacing != line_spacing)
	{
		style_.line_spacing = line_spacing;
		Reset();
	}
}

void e2d::Text::SetAlignment(Align align)
{
	if (style_.alignment != align)
	{
		style_.alignment = align;
		Reset();
	}
}

void e2d::Text::SetUnderline(bool underline)
{
	if (style_.underline != underline)
	{
		style_.underline = underline;
		if (!text_format_)
			CreateFormat();
		CreateLayout();
	}
}

void e2d::Text::SetStrikethrough(bool strikethrough)
{
	if (style_.strikethrough != strikethrough)
	{
		style_.strikethrough = strikethrough;
		if (!text_format_)
			CreateFormat();
		CreateLayout();
	}
}

void e2d::Text::SetOutline(bool outline)
{
	style_.outline = outline;
}

void e2d::Text::SetOutlineColor(Color outline_color)
{
	style_.outline_color = outline_color;
}

void e2d::Text::SetOutlineWidth(float outline_width)
{
	style_.outline_width = outline_width;
}

void e2d::Text::SetOutlineStroke(Stroke outline_stroke)
{
	style_.outline_stroke = outline_stroke;
}

void e2d::Text::Draw() const
{
	if (text_layout_)
	{
		auto graphics = Device::GetGraphics();
		// 创建文本区域
		D2D1_RECT_F textLayoutRect = D2D1::RectF(0, 0, transform_.size.width, transform_.size.height);
		// 设置画刷颜色和透明度
		graphics->GetSolidBrush()->SetOpacity(display_opacity_);
		// 获取文本渲染器
		auto text_renderer = graphics->GetTextRender();
		text_renderer->SetTextStyle(
			(D2D1_COLOR_F)style_.color,
			style_.outline,
			(D2D1_COLOR_F)style_.outline_color,
			style_.outline_width,
			D2D1_LINE_JOIN(style_.outline_stroke)
		);
		text_layout_->Draw(nullptr, text_renderer, 0, 0);
	}
}

void e2d::Text::Reset()
{
	// 创建文字格式化
	CreateFormat();
	// 创建文字布局
	CreateLayout();
}

void e2d::Text::CreateFormat()
{
	SafeRelease(text_format_);

	ThrowIfFailed(
		Device::GetGraphics()->GetWriteFactory()->CreateTextFormat(
			(const wchar_t *)font_.family,
			nullptr,
			DWRITE_FONT_WEIGHT(font_.weight),
			font_.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			font_.size,
			L"",
			&text_format_
		)
	);

	// 设置文字对齐方式
	text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(style_.alignment));

	// 设置行间距
	if (style_.line_spacing == 0.f)
	{
		text_format_->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
	}
	else
	{
		text_format_->SetLineSpacing(
			DWRITE_LINE_SPACING_METHOD_UNIFORM,
			style_.line_spacing,
			style_.line_spacing * 0.8f
		);
	}

	// 打开文本自动换行时，设置换行属性
	if (style_.wrap)
	{
		text_format_->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
	}
	else
	{
		text_format_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}
}

void e2d::Text::CreateLayout()
{
	SafeRelease(text_layout_);

	// 文本为空字符串时，重置属性
	if (text_.IsEmpty())
	{
		this->SetSize(0, 0);
		return;
	}

	if (text_format_ == nullptr)
	{
		WARN("Text::CreateLayout failed! text_format_ NULL pointer exception.");
		return;
	}
	
	UINT32 length = (UINT32)text_.GetLength();
	auto writeFactory = Device::GetGraphics()->GetWriteFactory();

	// 对文本自动换行情况下进行处理
	if (style_.wrap)
	{
		ThrowIfFailed(
			writeFactory->CreateTextLayout(
				(const wchar_t *)text_,
				length,
				text_format_,
				style_.wrap_width,
				0,
				&text_layout_
			)
		);
		// 获取文本布局的宽度和高度
		DWRITE_TEXT_METRICS metrics;
		text_layout_->GetMetrics(&metrics);
		// 重设文本宽高
		this->SetSize(metrics.layoutWidth, metrics.height);
	}
	else
	{
		// 为防止文本对齐问题，根据先创建 layout 以获取宽度
		ThrowIfFailed(
			writeFactory->CreateTextLayout(
				(const wchar_t *)text_,
				length,
				text_format_,
				0,
				0,
				&text_layout_
			)
		);

		// 获取文本布局的宽度和高度
		DWRITE_TEXT_METRICS metrics;
		text_layout_->GetMetrics(&metrics);
		// 重设文本宽高
		this->SetSize(metrics.width, metrics.height);

		// 重新创建 layout
		SafeRelease(text_layout_);
		ThrowIfFailed(
			writeFactory->CreateTextLayout(
				(const wchar_t *)text_,
				length,
				text_format_,
				transform_.size.width,
				0,
				&text_layout_
			)
		);
	}

	// 添加下划线和删除线
	DWRITE_TEXT_RANGE Range = { 0, length };
	if (style_.underline)
	{
		text_layout_->SetUnderline(true, Range);
	}
	if (style_.strikethrough)
	{
		text_layout_->SetStrikethrough(true, Range);
	}
}
