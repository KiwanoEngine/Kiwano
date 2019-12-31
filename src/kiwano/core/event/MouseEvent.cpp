#include <kiwano/core/event/MouseEvent.h>

namespace kiwano
{
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

}
