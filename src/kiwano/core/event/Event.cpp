#include <kiwano/core/event/Event.h>

namespace kiwano
{
Event::Event(const EventType& type)
    : type_(type)
{
}

Event::~Event() {}

}  // namespace kiwano
