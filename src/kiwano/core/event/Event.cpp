#include <kiwano/core/event/Event.h>

namespace kiwano
{
Event::Event(EventType const& type)
    : type_(type)
{
}

Event::~Event() {}

}  // namespace kiwano
