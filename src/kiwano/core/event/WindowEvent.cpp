#include <kiwano/core/event/WindowEvent.h>

namespace kiwano
{
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
