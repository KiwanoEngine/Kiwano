#include <kiwano/core/event/KeyEvent.h>

namespace kiwano
{

KeyEvent::KeyEvent(const EventType& type)
    : Event(type)
{
}

KeyDownEvent::KeyDownEvent()
    : KeyEvent(KGE_EVENT(KeyDownEvent))
    , code()
{
}

KeyUpEvent::KeyUpEvent()
    : KeyEvent(KGE_EVENT(KeyUpEvent))
    , code()
{
}

KeyCharEvent::KeyCharEvent()
    : KeyEvent(KGE_EVENT(KeyCharEvent))
    , value()
{
}

}  // namespace kiwano
