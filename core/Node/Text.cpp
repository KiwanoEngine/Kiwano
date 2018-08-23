#include "..\e2dnode.h"

//-------------------------------------------------------
// Style
//-------------------------------------------------------

e2d::Text::Style::Style()
	: color(Color::White)
	, alignment(Align::Left)
	, wrapping(false)
	, wrappingWidth(0.f)
	, lineSpacing(0.f)
	, hasUnderline(false)
	, hasStrikethrough(false)
	, hasOutline(true)
	, outlineColor(Color(Color::Black, 0.5))
	, outlineWidth(1.f)
	, outlineStroke(Stroke::Round)
{}

e2d::Text::Style::Style(
	Color color,
	Align alignment,
	bool wrapping,
	float wrappingWidth,
	float lineSpacing,
	bool hasUnderline,
	bool hasStrikethrough,
	bool hasOutline,
	Color outlineColor,
	float outlineWidth,
	Stroke outlineStroke
)
	: color(color)
	, alignment(alignment)
	, wrapping(wrapping)
	, wrappingWidth(wrappingWidth)
	, lineSpacing(lineSpacing)
	, hasUnderline(hasUnderline)
	, hasStrikethrough(hasStrikethrough)
	, hasOutline(hasOutline)
	, outlineColor(outlineColor)
	, outlineWidth(outlineWidth)
	, outlineStroke(outlineStroke)
{}



//-------------------------------------------------------
// Text
//-------------------------------------------------------

e2d::Text::Text()
	: _font()
	, _style()
	, _textLayout(nullptr)
	, _textFormat(nullptr)
{
}

e2d::Text::Text(const String & text, const Font & font, const Style & style)
	: _font(font)
	, _style(style)
	, _textLayout(nullptr)
	, _textFormat(nullptr)
	, _text(text)
{
	_reset();
}

e2d::Text::~Text()
{
	SafeRelease(_textFormat);
	SafeRelease(_textLayout);
}

const e2d::String& e2d::Text::text() const
{
	return _text;
}

const e2d::Font& e2d::Text::font() const
{
	return _font;
}

const e2d::Text::Style& e2d::Text::style() const
{
	return _style;
}

const e2d::String& e2d::Text::fontFamily() const
{
	return _font.family;
}

float e2d::Text::fontSize() const
{
	return _font.size;
}

UINT e2d::Text::fontWeight() const
{
	return _font.weight;
}

const e2d::Color& e2d::Text::color() const
{
	return _style.color;
}

const e2d::Color& e2d::Text::outlineColor() const
{
	return _style.outlineColor;
}

float e2d::Text::outlineWidth() const
{
	return _style.outlineWidth;
}

e2d::Stroke e2d::Text::outlineStroke() const
{
	return _style.outlineStroke;
}

int e2d::Text::lineCount() const
{
	if (_textLayout)
	{
		DWRITE_TEXT_METRICS metrics;
		_textLayout->GetMetrics(&metrics);
		return static_cast<int>(metrics.lineCount);
	}
	else
	{
		return 0;
	}
}

bool e2d::Text::italic() const
{
	return _font.italic;
}

bool e2d::Text::hasStrikethrough() const
{
	return _style.hasStrikethrough;
}

bool e2d::Text::hasUnderline() const
{
	return _style.hasUnderline;
}

bool e2d::Text::hasOutline() const
{
	return _style.hasOutline;
}

e2d::Text& e2d::Text::text(const String& text)
{
	_text = text;
	_reset();
	return *this;
}

e2d::Text& e2d::Text::style(const Style& style)
{
	_style = style;
	_reset();
	return *this;
}

e2d::Text& e2d::Text::font(const Font & font)
{
	_font = font;
	_reset();
	return *this;
}

e2d::Text& e2d::Text::fontFamily(const String& family)
{
	_font.family = family;
	_reset();
	return *this;
}

e2d::Text& e2d::Text::fontSize(float size)
{
	_font.size = size;
	_reset();
	return *this;
}

e2d::Text& e2d::Text::fontWeight(UINT weight)
{
	_font.weight = weight;
	_reset();
	return *this;
}

e2d::Text& e2d::Text::color(Color color)
{
	_style.color = color;
	return *this;
}

e2d::Text& e2d::Text::italic(bool value)
{
	_font.italic = value;
	_reset();
	return *this;
}

e2d::Text& e2d::Text::wrapping(bool wrapping)
{
	if (_style.wrapping != wrapping)
	{
		_style.wrapping = wrapping;
		_reset();
	}
	return *this;
}

e2d::Text& e2d::Text::wrappingWidth(float wrappingWidth)
{
	if (_style.wrappingWidth != wrappingWidth)
	{
		_style.wrappingWidth = std::max(wrappingWidth, 0.f);

		if (_style.wrapping)
		{
			_reset();
		}
	}
	return *this;
}

e2d::Text& e2d::Text::lineSpacing(float lineSpacing)
{
	if (_style.lineSpacing != lineSpacing)
	{
		_style.lineSpacing = lineSpacing;
		_reset();
	}
	return *this;
}

e2d::Text& e2d::Text::alignment(Align align)
{
	if (_style.alignment != align)
	{
		_style.alignment = align;
		_reset();
	}
	return *this;
}

e2d::Text& e2d::Text::underline(bool hasUnderline)
{
	if (_style.hasUnderline != hasUnderline)
	{
		_style.hasUnderline = hasUnderline;
		if (!_textFormat)
			_createFormat();
		_createLayout();
	}
	return *this;
}

e2d::Text& e2d::Text::strikethrough(bool hasStrikethrough)
{
	if (_style.hasStrikethrough != hasStrikethrough)
	{
		_style.hasStrikethrough = hasStrikethrough;
		if (!_textFormat)
			_createFormat();
		_createLayout();
	}
	return *this;
}

e2d::Text& e2d::Text::outline(bool hasOutline)
{
	_style.hasOutline = hasOutline;
	return *this;
}

e2d::Text& e2d::Text::outlineColor(Color outlineColor)
{
	_style.outlineColor = outlineColor;
	return *this;
}

e2d::Text& e2d::Text::outlineWidth(float outlineWidth)
{
	_style.outlineWidth = outlineWidth;
	return *this;
}

e2d::Text& e2d::Text::outlineStroke(Stroke outlineStroke)
{
	_style.outlineStroke = outlineStroke;
	return *this;
}

void e2d::Text::draw(Renderer * renderer) const
{
	if (_textLayout)
	{
		// 创建文本区域
		D2D1_RECT_F textLayoutRect = D2D1::RectF(0, 0, _size.width, _size.height);
		// 设置画刷颜色和透明度
		renderer->solidBrush()->SetOpacity(_displayOpacity);
		// 获取文本渲染器
		auto pTextRenderer = renderer->textRenderer();
		pTextRenderer->SetTextStyle(
			(D2D1_COLOR_F)_style.color,
			_style.hasOutline,
			(D2D1_COLOR_F)_style.outlineColor,
			_style.outlineWidth,
			D2D1_LINE_JOIN(_style.outlineStroke)
		);
		_textLayout->Draw(nullptr, pTextRenderer, 0, 0);
	}
}

void e2d::Text::_reset()
{
	// 创建文字格式化
	_createFormat();
	// 创建文字布局
	_createLayout();
}

void e2d::Text::_createFormat()
{
	SafeRelease(_textFormat);

	HRESULT hr = Game::instance()->renderer()->writeFactory()->CreateTextFormat(
		(const WCHAR *)_font.family,
		nullptr,
		DWRITE_FONT_WEIGHT(_font.weight),
		_font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		_font.size,
		L"",
		&_textFormat
	);

	if (FAILED(hr))
	{
		WARN("Text::_createFormat error : Create IDWriteTextFormat failed!");
		_textFormat = nullptr;
		return;
	}
	else
	{
		// 设置文字对齐方式
		_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(_style.alignment));
		// 设置行间距
		if (_style.lineSpacing == 0.f)
		{
			_textFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
		}
		else
		{
			_textFormat->SetLineSpacing(
				DWRITE_LINE_SPACING_METHOD_UNIFORM,
				_style.lineSpacing,
				_style.lineSpacing * 0.8f
			);
		}
		// 打开文本自动换行时，设置换行属性
		if (_style.wrapping)
		{
			_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		}
		else
		{
			_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}
}

void e2d::Text::_createLayout()
{
	SafeRelease(_textLayout);

	// 文本为空字符串时，重置属性
	if (_text.empty())
	{
		this->size(0, 0);
		return;
	}

	if (_textFormat == nullptr)
	{
		WARN("Text::_createLayout failed! _textFormat NULL pointer exception.");
		return;
	}
	
	HRESULT hr;
	UINT32 length = (UINT32)_text.length();
	auto writeFactory = Game::instance()->renderer()->writeFactory();

	// 对文本自动换行情况下进行处理
	if (_style.wrapping)
	{
		hr = writeFactory->CreateTextLayout(
			(const WCHAR *)_text,
			length,
			_textFormat,
			_style.wrappingWidth,
			0,
			&_textLayout
		);
		if (_textLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_textLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->size(metrics.layoutWidth, metrics.height);
		}
	}
	else
	{
		hr = writeFactory->CreateTextLayout((const WCHAR *)_text, length, _textFormat, 0, 0, &_textLayout);
		// 为防止文本对齐问题，根据刚才创建的 layout 宽度重新创建它
		if (_textLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_textLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->size(metrics.width, metrics.height);
			// 重新创建 layout
			SafeRelease(_textLayout);
			hr = writeFactory->CreateTextLayout((const WCHAR *)_text, length, _textFormat, _size.width, 0, &_textLayout);
		}
	}

	if (FAILED(hr))
	{
		WARN("Text::_createLayout error : Create IDWriteTextLayout failed!");
		_textLayout = nullptr;
		return;
	}

	// 添加下划线和删除线
	DWRITE_TEXT_RANGE range = { 0, length };
	if (_style.hasUnderline)
	{
		_textLayout->SetUnderline(true, range);
	}
	if (_style.hasStrikethrough)
	{
		_textLayout->SetStrikethrough(true, range);
	}
}
