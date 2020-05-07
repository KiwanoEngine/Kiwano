#include <kiwano/core/event/WindowEvent.h>

namespace kiwano
{

WindowEvent::WindowEvent(const EventType& type)
    : Event(type)
{
}

WindowMovedEvent::WindowMovedEvent()
    : WindowEvent(KGE_EVENT(WindowMovedEvent))
    , x(0)
    , y(0)
{
}

WindowResizedEvent::WindowResizedEvent()
    : WindowEvent(KGE_EVENT(WindowResizedEvent))
    , width(0)
    , height(0)
{
}

WindowFocusChangedEvent::WindowFocusChangedEvent()
    : WindowEvent(KGE_EVENT(WindowFocusChangedEvent))
    , focus(false)
{
}

WindowTitleChangedEvent::WindowTitleChangedEvent()
    : WindowEvent(KGE_EVENT(WindowTitleChangedEvent))
    , title()
{
}

WindowClosedEvent::WindowClosedEvent()
    : WindowEvent(KGE_EVENT(WindowClosedEvent))
{
}

WindowFullscreenEvent::WindowFullscreenEvent() 
    : WindowEvent(KGE_EVENT(WindowFullscreenEvent))
    , fullscreen(false)
{
}

}  // namespace kiwano
