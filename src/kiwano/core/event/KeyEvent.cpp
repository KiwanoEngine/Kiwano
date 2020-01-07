#include <kiwano/core/event/KeyEvent.h>

namespace kiwano
{
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

}
