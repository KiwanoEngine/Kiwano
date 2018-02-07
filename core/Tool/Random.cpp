#include "..\etools.h"

std::default_random_engine &e2d::Random::getEngine()
{
	static std::random_device device;
	static std::default_random_engine engine(device());
	return engine;
}
