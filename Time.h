#pragma once



class Time
{
public:
	static void Init();
	static void Tick();

	static double GetDeltaTime();
	/*static 
	static 
	*/
private:
	static long long int GetTimeNow();
	static double GetSecondsPerCount();

	static long long int previousTime ;
	static long long int currentTime;
	static double secondsPerCount;
	static double deltaTime;
};

