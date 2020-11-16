#include "Time.h"

#include <windows.h>

long long int Time::previousTime;
long long int Time::currentTime;
double Time::secondsPerCount;
double Time::deltaTime;

void Time::Init()
{
	previousTime = GetTimeNow();
	secondsPerCount = GetSecondsPerCount();
}

double Time::GetSecondsPerCount() {
	long long int frequency = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	return 1.0 / (double)frequency;
}

long long int Time::GetTimeNow() {
	long long int now = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	return now;
}

void Time::Tick() {
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	deltaTime = (currentTime - previousTime) * secondsPerCount;
	previousTime = currentTime;
}

double Time::GetDeltaTime()
{
	return deltaTime;
}
