#include "..\e2dnode.h"

e2d::Canvas::Canvas(float width, float height)
	: _renderer(nullptr)
	, _renderTarget(nullptr)
	, _fillBrush(nullptr)
	, _lineBrush(nullptr)
	, _strokeStyle(nullptr)
	, _strokeWidth(1.0f)
	, _stroke(Stroke::Miter)
{
	_renderer = Game::getInstance()->getRenderer();
	_renderTarget = _renderer->getRenderTarget();
	_renderTarget->AddRef();

	ThrowIfFailed(
		_renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&_fillBrush
		)
	);

	ThrowIfFailed(
		_renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&_lineBrush
		)
	);
	
	this->setClipEnabled(true);
	this->setWidth(width);
	this->setHeight(height);
	this->setStrokeStyle(_stroke);
}

e2d::Canvas::~Canvas()
{
	SafeRelease(_lineBrush);
	SafeRelease(_fillBrush);
	SafeRelease(_renderTarget);
}

void e2d::Canvas::setLineColor(const Color & color)
{
	_lineBrush->SetColor(D2D_COLOR_F(color));
}

void e2d::Canvas::setFillColor(const Color & color)
{
	_fillBrush->SetColor(D2D_COLOR_F(color));
}

void e2d::Canvas::setStrokeWidth(float width)
{
	_strokeWidth = std::max(width, 0.f);
}

void e2d::Canvas::setStrokeStyle(Stroke strokeStyle)
{
	switch (strokeStyle)
	{
	case e2d::Stroke::Miter:
		_strokeStyle = _renderer->getMiterStrokeStyle();
		break;
	case e2d::Stroke::Bevel:
		_strokeStyle = _renderer->getBevelStrokeStyle();
		break;
	case e2d::Stroke::Round:
		_strokeStyle = _renderer->getRoundStrokeStyle();
		break;
	}
}

const e2d::Color & e2d::Canvas::getLineColor() const
{
	return Color(_lineBrush->GetColor());
}

const e2d::Color & e2d::Canvas::getFillColor() const
{
	return Color(_fillBrush->GetColor());
}

float e2d::Canvas::getStrokeWidth() const
{
	return _strokeWidth;
}

e2d::Stroke e2d::Canvas::getStrokeStyle() const
{
	return _stroke;
}

void e2d::Canvas::drawLine(const Point & begin, const Point & end)
{
	_renderTarget->DrawLine(
		D2D1::Point2F(begin.x, begin.y),
		D2D1::Point2F(end.x, end.y),
		_lineBrush,
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::Canvas::drawCircle(const Point & center, float radius)
{
	_renderTarget->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(
				center.x,
				center.y
			),
			radius,
			radius
		),
		_lineBrush,
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::Canvas::drawEllipse(const Point & center, float radiusX, float radiusY)
{
	_renderTarget->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(
				center.x,
				center.y
			),
			radiusX,
			radiusY
		),
		_lineBrush,
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::Canvas::drawRect(const Rect & rect)
{
	_renderTarget->DrawRectangle(
		D2D1::RectF(
			rect.origin.x,
			rect.origin.y,
			rect.origin.x + rect.size.width,
			rect.origin.y + rect.size.height
		),
		_lineBrush,
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::Canvas::drawRoundedRect(const Rect & rect, float radiusX, float radiusY)
{
	_renderTarget->DrawRoundedRectangle(
		D2D1::RoundedRect(
			D2D1::RectF(
				rect.origin.x,
				rect.origin.y,
				rect.origin.x + rect.size.width,
				rect.origin.y + rect.size.height
			),
			radiusX,
			radiusY
		),
		_lineBrush,
		_strokeWidth,
		_strokeStyle
	);
}

void e2d::Canvas::fillCircle(const Point & center, float radius)
{
	_renderTarget->FillEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(
				center.x,
				center.y
			),
			radius,
			radius
		),
		_fillBrush
	);
}

void e2d::Canvas::fillEllipse(const Point & center, float radiusX, float radiusY)
{
	_renderTarget->FillEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(
				center.x,
				center.y
			),
			radiusX,
			radiusY
		),
		_fillBrush
	);
}

void e2d::Canvas::fillRect(const Rect & rect)
{
	_renderTarget->FillRectangle(
		D2D1::RectF(
			rect.origin.x,
			rect.origin.y,
			rect.origin.x + rect.size.width,
			rect.origin.y + rect.size.height
		),
		_fillBrush
	);
}

void e2d::Canvas::fillRoundedRect(const Rect & rect, float radiusX, float radiusY)
{
	_renderTarget->FillRoundedRectangle(
		D2D1::RoundedRect(
			D2D1::RectF(
				rect.origin.x,
				rect.origin.y,
				rect.origin.x + rect.size.width,
				rect.origin.y + rect.size.height
			),
			radiusX,
			radiusY
		),
		_fillBrush
	);
}
