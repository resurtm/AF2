#include "PreCompiled.h"

bool CTimer::Init(void) {
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency)) return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&LastTickCount)) return false;
	Resolution = 1.0f / float(Frequency);
	DeltaTimeCounter = 0.0f;
	UpdatesPerSecondCounter = 0;
	return true;
}	//	bool CTimer::Init(void)

void CTimer::Update(void) {
	__int64 TickCount;
	QueryPerformanceCounter((LARGE_INTEGER *)&TickCount);
	DeltaTime = (TickCount - LastTickCount) * Resolution;
	LastTickCount = TickCount;

	DeltaTimeCounter += DeltaTime;
	UpdatesPerSecondCounter++;
	if (DeltaTimeCounter >= 1.0f) {
		UpdatesPerSecond = UpdatesPerSecondCounter;
		DeltaTimeCounter = 0.0f;
		UpdatesPerSecondCounter = 0;
	}	//	if (DeltaTimeCounter >= 1.0f)
}	//	void Update(void)