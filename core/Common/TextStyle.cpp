#include "..\e2dnode.h"

e2d::TextStyle::TextStyle() 
	: fontFamily("")
	, fontSize(22)
	, color(Color::WHITE)
	, weight(FontWeight::NORMAL)
	, italic(false)
	, underline(false)
	, strikethrough(false)
	, showOutline(true)
	, outlineColor(Color::BLACK)
	, outlineWidth(1.0)
	, outlineJoin(LineJoin::ROUND)
{}

e2d::TextStyle::TextStyle(
	String fontFamily, 
	double fontSize, 
	UINT32 color, 
	UINT32 textStyleWeight, 
	bool italic, 
	bool hasUnderline, 
	bool hasStrikethrough, 
	bool showOutline,
	UINT32 outlineColor,
	UINT32 outlineWidth,
	int outlineJoin
)
	: fontFamily(fontFamily)
	, fontSize(fontSize)
	, color(color)
	, weight(textStyleWeight)
	, italic(italic)
	, underline(hasUnderline)
	, strikethrough(hasStrikethrough)
	, showOutline(showOutline)
	, outlineColor(outlineColor)
	, outlineWidth(outlineWidth)
	, outlineJoin(outlineJoin)
{}
