#include "..\easy2d.h"


std::default_random_engine &Math::getEngine()
{
	static std::random_device device;
	static std::default_random_engine engine(device());
	return engine;
}
