#include "PreCompiled.h"

bool CLogger::Init(const char *LogFileName, const bool Enabled) {
	Name = (char *)LogFileName;
	this->Enabled = Enabled;

	if (!Enabled) return true;

	//	Add to file name path - where log is situated
	char _LogFileName[512];
	sprintf(_LogFileName, "Data\\Engine\\%s", LogFileName);

	//	Create and open stream for writing
	std::fstream Stream(_LogFileName, std::ios::out);
	Stream << "<html>" << std::endl;
	Stream << "<body bgcolor=cornsilk>" << std::endl;
	Stream << "<b><font face=tahoma size=6 color=blue>" << LogFileName << "</font></b><br>" << std::endl;
	Stream.close();

	return true;
}	//	bool CLogger::Init(const char *LogFileName, const bool Enabled)

void CLogger::DeInit(void) {
	if (!Enabled) return;

	//	Add to file name path - where log is situated
	char _Name[512];
	sprintf(_Name, "Data\\Engine\\%s", Name);

	//	Open stream for writing
	std::fstream Stream(_Name, std::ios::out | std::ios::app);
	Stream << "</body>" << std::endl;
	Stream << "</html>" << std::endl;
	Stream.close();
}	//	void CLogger::DeInit(void)

void CLogger::WriteMessage(const char *Text, ...) {
	if (!Enabled || !Text || !Opened) return;

	//	Variables
	va_list Args;
	unsigned int Length;
	char *Buffer;

	va_start(Args, Text);
	Length = _vscprintf(Text, Args) + 1;
	Buffer = (char *)malloc(Length * sizeof(char));
	vsprintf(Buffer, Text, Args);

	//	Get time
	time_t Time;
	time(&Time);
	char *TimeString = ctime(&Time);

	//	Add to file name path - where log is situated
	char _Name[512];
	sprintf(_Name, "Data\\Engine\\%s", Name);

	//	Write this message
	std::fstream Stream(_Name, std::ios::out | std::ios::app);
	Stream << "<font face=tahoma size=1 color=blue>" << TimeString << "</font>" << std::endl;
	Stream << "<font face=tahoma size=2 color=black>" << Buffer << "</font><br>" << std::endl;
	Stream.close();

	free(Buffer);
}	//	void CLogger::WriteMessage(const char *Text, ...)

void CLogger::WriteWarning(const char *Text, ...) {
	if (!Enabled || !Text || !Opened) return;

	//	Variables
	va_list Args;
	unsigned int Length;
	char *Buffer;

	va_start(Args, Text);
	Length = _vscprintf(Text, Args) + 1;
	Buffer = (char *)malloc(Length * sizeof(char));
	vsprintf(Buffer, Text, Args);

	//	Get time
	time_t Time;
	time(&Time);
	char *TimeString = ctime(&Time);

	//	Add to file name path - where log is situated
	char _Name[512];
	sprintf(_Name, "Data\\Engine\\%s", Name);

	//	Write this message
	std::fstream Stream(_Name, std::ios::out | std::ios::app);
	Stream << "<b><font face=tahoma size=1 color=red>" << TimeString << "</font>" << std::endl;
	Stream << "<font face=tahoma size=2 color=red>" << Buffer << "</font></b><br>" << std::endl;
	Stream.close();

	free(Buffer);
}	//	void CLogger::WriteWarning(const char *Text, ...)

void CLogger::WriteError(const char *Text, ...) {
	if (!Enabled || !Text || !Opened) return;

	//	Variables
	va_list Args;
	unsigned int Length;
	char *Buffer;

	va_start(Args, Text);
	Length = _vscprintf(Text, Args) + 1;
	Buffer = (char *)malloc(Length * sizeof(char));
	vsprintf(Buffer, Text, Args);

	//	Get time
	time_t Time;
	time(&Time);
	char *TimeString = ctime(&Time);

	//	Add to file name path - where log is situated
	char _Name[512];
	sprintf(_Name, "Data\\Engine\\%s", Name);

	//	Write this message
	std::fstream Stream(_Name, std::ios::out | std::ios::app);
	Stream << "<b><font face=tahoma size=1 color=darkred>" << TimeString << "</font>" << std::endl;
	Stream << "<font face=tahoma size=2 color=darkred>" << Buffer << "</font></b><br>" << std::endl;
	Stream.close();

	free(Buffer);
}	//	void CLogger::WriteError(const char *Text, ...)