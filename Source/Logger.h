#pragma once

class CLogger {
public:
	inline CLogger(void) {}
	inline ~CLogger(void) {}

	inline bool IsOpened(void) { return Opened; }
	inline bool IsEnabled(void) { return Enabled; }
	inline char *GetName(void) { return Name; }

	bool Init(const char *LogFileName, const bool Enabled);
	void DeInit(void);
	void WriteMessage(const char *Text, ...);
	void WriteWarning(const char *Text, ...);
	void WriteError(const char *Text, ...);

protected:
	bool Opened, Enabled;
	char *Name;

private:
};	//	class CLogger;