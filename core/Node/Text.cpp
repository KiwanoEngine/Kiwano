#include "..\e2dnode.h"

//-------------------------------------------------------
// Style
//-------------------------------------------------------

e2d::Text::Style::Style()
	: color(Color::White)
	, alignment(Align::Left)
	, wrapping(false)
	, wrappingWidth(0.0)
	, lineSpacing(0.0)
	, hasUnderline(false)
	, hasStrikethrough(false)
	, hasOutline(true)
	, outlineColor(Color(Color::Black, 0.5))
	, outlineWidth(1.0)
	, outlineJoin(LineJoin::Round)
{}

e2d::Text::Style::Style(
	Color color,
	Align alignment,
	bool wrapping,
	double wrappingWidth,
	double lineSpacing,
	bool hasUnderline,
	bool hasStrikethrough,
	bool hasOutline,
	Color outlineColor,
	double outlineWidth,
	LineJoin outlineJoin
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
	, outlineJoin(outlineJoin)
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

e2d::String e2d::Text::getText() const
{
	return _text;
}

e2d::Font e2d::Text::getFont() const
{
	return _font;
}

e2d::Text::Style e2d::Text::getStyle() const
{
	return _style;
}

e2d::String e2d::Text::getFontFamily() const
{
	return _font.family;
}

double e2d::Text::getFontSize() const
{
	return _font.size;
}

UINT e2d::Text::getFontWeight() const
{
	return _font.weight;
}

e2d::Color e2d::Text::getColor() const
{
	return _style.color;
}

e2d::Color e2d::Text::getOutlineColor() const
{
	return _style.outlineColor;
}

double e2d::Text::getOutlineWidth() const
{
	return _style.outlineWidth;
}

e2d::LineJoin e2d::Text::getOutlineJoin() const
{
	return _style.outlineJoin;
}

int e2d::Text::getLineCount() const
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

bool e2d::Text::isItalic() const
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

void e2d::Text::setText(const String& text)
{
	_text = text;
	_reset();
}

void e2d::Text::setStyle(const Style& style)
{
	_style = style;
	_reset();
}

void e2d::Text::setFont(const Font & font)
{
	_font = font;
	_reset();
}

void e2d::Text::setFontFamily(const String& family)
{
	_font.family = family;
	_reset();
}

void e2d::Text::setFontSize(double size)
{
	_font.size = size;
	_reset();
}

void e2d::Text::setFontWeight(UINT weight)
{
	_font.weight = weight;
	_reset();
}

void e2d::Text::setColor(Color color)
{
	_style.color = color;
}

void e2d::Text::setItalic(bool value)
{
	_font.italic = value;
	_reset();
}

void e2d::Text::setWrapping(bool wrapping)
{
	if (_style.wrapping != wrapping)
	{
		_style.wrapping = wrapping;
		_reset();
	}
}

void e2d::Text::setWrappingWidth(double wrappingWidth)
{
	if (_style.wrappingWidth != wrappingWidth)
	{
		_style.wrappingWidth = max(wrappingWidth, 0);

		if (_style.wrapping)
		{
			_reset();
		}
	}
}

void e2d::Text::setLineSpacing(double lineSpacing)
{
	if (_style.lineSpacing != lineSpacing)
	{
		_style.lineSpacing = lineSpacing;
		_reset();
	}
}

void e2d::Text::setAlignment(Align align)
{
	if (_style.alignment != align)
	{
		_style.alignment = align;
		_reset();
	}
}

void e2d::Text::setUnderline(bool hasUnderline)
{
	if (_style.hasUnderline != hasUnderline)
	{
		_style.hasUnderline = hasUnderline;
		if (!_textFormat)
			_createFormat();
		_createLayout();
	}
}

void e2d::Text::setStrikethrough(bool hasStrikethrough)
{
	if (_style.hasStrikethrough != hasStrikethrough)
	{
		_style.hasStrikethrough = hasStrikethrough;
		if (!_textFormat)
			_createFormat();
		_createLayout();
	}
}

void e2d::Text::setOutline(bool hasOutline)
{
	_style.hasOutline = hasOutline;
}

void e2d::Text::setOutlineColor(Color outlineColor)
{
	_style.outlineColor = outlineColor;
}

void e2d::Text::setOutlineWidth(double outlineWidth)
{
	_style.outlineWidth = outlineWidth;
}

void e2d::Text::setOutlineJoin(LineJoin outlineJoin)
{
	_style.outlineJoin = outlineJoin;
}

void e2d::Text::onRender()
{
	if (_textLayout)
	{
		// 创建文本区域
		D2D1_RECT_F textLayoutRect = D2D1::RectF(0, 0, _width, _height);
		// 设置画刷颜色和透明度
		Renderer::getSolidColorBrush()->SetOpacity(_displayOpacity);
		// 获取文本渲染器
		auto pTextRenderer = Renderer::getTextRenderer();
		pTextRenderer->SetTextStyle(
			_style.color.toD2DColorF(),
			_style.hasOutline,
			_style.outlineColor.toD2DColorF(),
			float(_style.outlineWidth),
			D2D1_LINE_JOIN(_style.outlineJoin)
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

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextFormat(
		(const WCHAR *)_font.family,
		nullptr,
		DWRITE_FONT_WEIGHT(_font.weight),
		_font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		float(_font.size),
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
		if (_style.lineSpacing == 0.0)
		{
			_textFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
		}
		else
		{
			_textFormat->SetLineSpacing(
				DWRITE_LINE_SPACING_METHOD_UNIFORM,
				float(_style.lineSpacing),
				float(_style.lineSpacing) * 0.8f
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
	if (_text.isEmpty())
	{
		this->setSize(0, 0);
		return;
	}

	if (_textFormat == nullptr)
	{
		WARN("Text::_createLayout failed! _textFormat NULL pointer exception.");
		return;
	}
	
	UINT32 length = (UINT32)_text.getLength();

	// 创建 TextLayout
	HRESULT hr;
	// 对文本自动换行情况下进行处理
	if (_style.wrapping)
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(
			(const WCHAR *)_text,
			length,
			_textFormat,
			float(_style.wrappingWidth),
			0,
			&_textLayout
		);
		if (_textLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_textLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->setSize(metrics.layoutWidth, metrics.height);
		}
	}
	else
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout((const WCHAR *)_text, length, _textFormat, 0, 0, &_textLayout);
		// 为防止文本对齐问题，根据刚才创建的 layout 宽度重新创建它
		if (_textLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_textLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->setSize(metrics.width, metrics.height);
			// 重新创建 layout
			SafeRelease(_textLayout);
			hr = Renderer::getIDWriteFactory()->CreateTextLayout((const WCHAR *)_text, length, _textFormat, _width, 0, &_textLayout);
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
