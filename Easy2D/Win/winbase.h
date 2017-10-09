#pragma once
#include <chrono>
using namespace std::chrono;

steady_clock::time_point GetNow();

void FlushSteadyClock();