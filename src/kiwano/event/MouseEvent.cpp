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

MouseWheelEvent::MouseWheelEvent()
    : MouseEvent(KGE_EVENT(MouseWheelEvent))
    , wheel(0.f)
{
}

}  // namespace kiwano
