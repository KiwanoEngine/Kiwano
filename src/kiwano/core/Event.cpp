#include <kiwano/core/Event.h>

namespace kiwano
{
	namespace events
	{
		KGE_IMPLEMENT_EVENT_TYPE(MouseMove, MouseMoveEvent);
		KGE_IMPLEMENT_EVENT_TYPE(MouseDown, MouseDownEvent);
		KGE_IMPLEMENT_EVENT_TYPE(MouseUp, MouseUpEvent);
		KGE_IMPLEMENT_EVENT_TYPE(MouseWheel, MouseWheelEvent);
		KGE_IMPLEMENT_EVENT_TYPE(MouseHover, MouseHoverEvent);
		KGE_IMPLEMENT_EVENT_TYPE(MouseOut, MouseOutEvent);
		KGE_IMPLEMENT_EVENT_TYPE(MouseClick, MouseClickEvent);

		KGE_IMPLEMENT_EVENT_TYPE(KeyDown, KeyDownEvent);
		KGE_IMPLEMENT_EVENT_TYPE(KeyUp, KeyUpEvent);
		KGE_IMPLEMENT_EVENT_TYPE(KeyChar, KeyCharEvent);

		KGE_IMPLEMENT_EVENT_TYPE(WindowMoved, WindowMovedEvent);
		KGE_IMPLEMENT_EVENT_TYPE(WindowResized, WindowResizedEvent);
		KGE_IMPLEMENT_EVENT_TYPE(WindowFocusChanged, WindowFocusChangedEvent);
		KGE_IMPLEMENT_EVENT_TYPE(WindowTitleChanged, WindowTitleChangedEvent);
		KGE_IMPLEMENT_EVENT_TYPE(WindowClosed, WindowClosedEvent);
	}

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
		: MouseEvent(events::MouseMove)
		, button(0)
	{
	}

	MouseDownEvent::MouseDownEvent()
		: MouseEvent(events::MouseDown)
		, button(0)
	{
	}

	MouseUpEvent::MouseUpEvent()
		: MouseEvent(events::MouseUp)
		, button(0)
	{
	}

	MouseClickEvent::MouseClickEvent()
		: MouseEvent(events::MouseClick)
		, button(0)
	{
	}

	MouseHoverEvent::MouseHoverEvent()
		: MouseEvent(events::MouseHover)
	{
	}

	MouseOutEvent::MouseOutEvent()
		: MouseEvent(events::MouseOut)
	{
	}

	MouseWheelEvent::MouseWheelEvent()
		: MouseEvent(events::MouseWheel)
		, wheel(0.f)
	{
	}

	KeyDownEvent::KeyDownEvent()
		: Event(events::KeyDown)
		, code(0)
	{
	}

	KeyUpEvent::KeyUpEvent()
		: Event(events::KeyUp)
		, code(0)
	{
	}

	KeyCharEvent::KeyCharEvent()
		: Event(events::KeyChar)
		, value()
	{
	}

	WindowMovedEvent::WindowMovedEvent()
		: Event(events::WindowMoved)
		, x(0)
		, y(0)
	{
	}

	WindowResizedEvent::WindowResizedEvent()
		: Event(events::WindowResized)
		, width(0)
		, height(0)
	{
	}

	WindowFocusChangedEvent::WindowFocusChangedEvent()
		: Event(events::WindowFocusChanged)
		, focus(false)
	{
	}

	WindowTitleChangedEvent::WindowTitleChangedEvent()
		: Event(events::WindowTitleChanged)
		, title()
	{
	}

	WindowClosedEvent::WindowClosedEvent()
		: Event(events::WindowClosed)
	{
	}

}
