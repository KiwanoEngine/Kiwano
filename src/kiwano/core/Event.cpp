#include <kiwano/core/Event.h>

namespace kiwano
{
	Event::Event(EventType const& type)
		: type_(type)
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
		: MouseEvent(KGE_EVENT(MouseMoveEvent))
	{
	}

	MouseDownEvent::MouseDownEvent()
		: MouseEvent(KGE_EVENT(MouseDownEvent))
		, button(0)
	{
	}

	MouseUpEvent::MouseUpEvent()
		: MouseEvent(KGE_EVENT(MouseUpEvent))
		, button(0)
	{
	}

	MouseClickEvent::MouseClickEvent()
		: MouseEvent(KGE_EVENT(MouseClickEvent))
		, button(0)
	{
	}

	MouseHoverEvent::MouseHoverEvent()
		: MouseEvent(KGE_EVENT(MouseHoverEvent))
	{
	}

	MouseOutEvent::MouseOutEvent()
		: MouseEvent(KGE_EVENT(MouseOutEvent))
	{
	}

	MouseWheelEvent::MouseWheelEvent()
		: MouseEvent(KGE_EVENT(MouseWheelEvent))
		, wheel(0.f)
	{
	}

	KeyDownEvent::KeyDownEvent()
		: Event(KGE_EVENT(KeyDownEvent))
		, code(0)
	{
	}

	KeyUpEvent::KeyUpEvent()
		: Event(KGE_EVENT(KeyUpEvent))
		, code(0)
	{
	}

	KeyCharEvent::KeyCharEvent()
		: Event(KGE_EVENT(KeyCharEvent))
		, value()
	{
	}

	WindowMovedEvent::WindowMovedEvent()
		: Event(KGE_EVENT(WindowMovedEvent))
		, x(0)
		, y(0)
	{
	}

	WindowResizedEvent::WindowResizedEvent()
		: Event(KGE_EVENT(WindowResizedEvent))
		, width(0)
		, height(0)
	{
	}

	WindowFocusChangedEvent::WindowFocusChangedEvent()
		: Event(KGE_EVENT(WindowFocusChangedEvent))
		, focus(false)
	{
	}

	WindowTitleChangedEvent::WindowTitleChangedEvent()
		: Event(KGE_EVENT(WindowTitleChangedEvent))
		, title()
	{
	}

	WindowClosedEvent::WindowClosedEvent()
		: Event(KGE_EVENT(WindowClosedEvent))
	{
	}

}
