#pragma once

class CTimer {
public:
	inline CTimer(void) {}
	inline ~CTimer(void) {}

	inline float GetDeltaTime(void) { if (Pause) return 0.0f; return DeltaTime; }
	inline unsigned int GetUpdatesPerSecond(void) { return UpdatesPerSecond; }
	inline bool IsPause(void) { return Pause; }
	inline void SetPause(const bool b) { Pause = b; }

	bool Init(void);
	void Update(void);

protected:
	bool Pause;
	unsigned int UpdatesPerSecond, UpdatesPerSecondCounter;
	__int64 LastTickCount, Frequency;
	float Resolution, DeltaTime, DeltaTimeCounter;	

private:
};	//	class CTimer;