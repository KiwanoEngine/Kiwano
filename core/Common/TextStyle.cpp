#include "..\e2dnode.h"

e2d::TextStyle::TextStyle() 
	: fontFamily("")
	, fontSize(22)
	, color(Color::WHITE)
	, fontWeight(FontWeight::NORMAL)
	, italic(false)
	, alignment(TextAlign::LEFT)
	, wrapping(false)
	, wrappingWidth(0.0)
	, lineSpacing(0.0)
	, hasUnderline(false)
	, hasStrikethrough(false)
	, hasOutline(true)
	, outlineColor(Color(Color::BLACK, 0.5))
	, outlineWidth(1.0)
	, outlineJoin(LineJoin::ROUND)
{}

e2d::TextStyle::TextStyle(
	const String& fontFamily,
	double fontSize, 
	Color color, 
	UINT32 fontWeight, 
	bool italic,
	TextAlign alignment,
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
	: fontFamily(fontFamily)
	, fontSize(fontSize)
	, color(color)
	, fontWeight(fontWeight)
	, italic(italic)
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
