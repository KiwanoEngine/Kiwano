#include <kiwano/base/Event.h>

namespace kiwano
{
	EventType event::MouseMove			= EventType(L"MouseMove");
	EventType event::MouseDown			= EventType(L"MouseBtnDown");
	EventType event::MouseUp			= EventType(L"MouseBtnUp");
	EventType event::MouseWheel			= EventType(L"MouseWheel");
	EventType event::MouseHover			= EventType(L"MouseHover");
	EventType event::MouseOut			= EventType(L"MouseOut");
	EventType event::MouseClick			= EventType(L"MouseClick");

	EventType event::KeyDown			= EventType(L"KeyDown");
	EventType event::KeyUp				= EventType(L"KeyUp");
	EventType event::KeyChar			= EventType(L"KeyChar");

	EventType event::WindowMoved		= EventType(L"WindowMoved");
	EventType event::WindowResized		= EventType(L"WindowResized");
	EventType event::WindowFocusChanged	= EventType(L"WindowFocusChanged");
	EventType event::WindowTitleChanged	= EventType(L"WindowTitleChanged");
	EventType event::WindowClosed		= EventType(L"WindowClosed");


	Event::Event(EventType const& type)
		: type(type)
	{
	}

	Event::~Event()
	{
	}

	MouseEvent::MouseEvent(EventType const& type)
		: Event(type)
		, pos()
		, left_btn_down(false)
		, right_btn_down(false)
		, target(nullptr)
	{
	}

	MouseMoveEvent::MouseMoveEvent()
		: MouseEvent(event::MouseMove)
		, button(0)
	{
	}

	MouseDownEvent::MouseDownEvent()
		: MouseEvent(event::MouseDown)
		, button(0)
	{
	}

	MouseUpEvent::MouseUpEvent()
		: MouseEvent(event::MouseUp)
		, button(0)
	{
	}

	MouseClickEvent::MouseClickEvent()
		: MouseEvent(event::MouseClick)
		, button(0)
	{
	}

	MouseHoverEvent::MouseHoverEvent()
		: MouseEvent(event::MouseHover)
	{
	}

	MouseOutEvent::MouseOutEvent()
		: MouseEvent(event::MouseOut)
	{
	}

	MouseWheelEvent::MouseWheelEvent()
		: MouseEvent(event::MouseWheel)
		, wheel(0.f)
	{
	}

	KeyDownEvent::KeyDownEvent()
		: Event(event::KeyDown)
		, code(0)
		, count(0)
	{
	}

	KeyUpEvent::KeyUpEvent()
		: Event(event::KeyUp)
		, code(0)
		, count(0)
	{
	}

	KeyCharEvent::KeyCharEvent()
		: Event(event::KeyChar)
		, value()
		, count(0)
	{
	}

	WindowMovedEvent::WindowMovedEvent()
		: Event(event::WindowMoved)
		, x(0)
		, y(0)
	{
	}

	WindowResizedEvent::WindowResizedEvent()
		: Event(event::WindowResized)
		, width(0)
		, height(0)
	{
	}

	WindowFocusChangedEvent::WindowFocusChangedEvent()
		: Event(event::WindowFocusChanged)
		, focus(false)
	{
	}

	WindowTitleChangedEvent::WindowTitleChangedEvent()
		: Event(event::WindowTitleChanged)
		, title()
	{
	}

	WindowClosedEvent::WindowClosedEvent()
		: Event(event::WindowClosed)
	{
	}

}
