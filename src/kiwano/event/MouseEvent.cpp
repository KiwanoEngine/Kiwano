#include <kiwano/event/MouseEvent.h>

namespace kiwano
{
MouseEvent::MouseEvent(const EventType& type)
    : Event(type)
    , pos()
{
}

MouseMoveEvent::MouseMoveEvent()
    : MouseEvent(KGE_EVENT(MouseMoveEvent))
{
}

MouseDownEvent::MouseDownEvent()
    : MouseEvent(KGE_EVENT(MouseDownEvent))
    , button()
{
}

MouseUpEvent::MouseUpEvent()
    : MouseEvent(KGE_EVENT(MouseUpEvent))
    , button()
{
}

MouseClickEvent::MouseClickEvent()
    : MouseEvent(KGE_EVENT(MouseClickEvent))
    , button()
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

}  // namespace kiwano
