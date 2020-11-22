#include "Time.h"

#undef max
#undef min
#include <Windows.h>

long long int Time::previousTime;
long long int Time::currentTime;
double Time::secondsPerCount;
float Time::deltaTime;

void Time::Init()
{
	previousTime = GetTimeNow();
	secondsPerCount = GetSecondsPerCount();
}

double Time::GetSecondsPerCount()
{
	long long int frequency = 0;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	return 1.0 / static_cast<double>(frequency);
}

long long int Time::GetTimeNow()
{
	long long int now = 0;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&now));
	return now;
}

void Time::Tick()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
	deltaTime = (currentTime - previousTime) * secondsPerCount;
	previousTime = currentTime;
}