#include "..\e2dnode.h"

e2d::TextStyle::TextStyle() 
	: fontFamily("")
	, fontSize(22)
	, color(Color::WHITE)
	, fontWeight(FontWeight::NORMAL)
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
	Color color, 
	UINT32 fontWeight, 
	bool italic, 
	bool hasUnderline, 
	bool hasStrikethrough, 
	bool showOutline,
	Color outlineColor,
	double outlineWidth,
	int outlineJoin
)
	: fontFamily(fontFamily)
	, fontSize(fontSize)
	, color(color)
	, fontWeight(fontWeight)
	, italic(italic)
	, underline(hasUnderline)
	, strikethrough(hasStrikethrough)
	, showOutline(showOutline)
	, outlineColor(outlineColor)
	, outlineWidth(outlineWidth)
	, outlineJoin(outlineJoin)
{}
