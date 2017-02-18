#include "PreCompiled.h"

CLogger *CCore::Logger = 0;

bool CCore::Init(const HINSTANCE Instance, const char *ConfigFileName, FInit UserInit,
	FDeInit UserDeInit, FUpdate UserUpdate, FDraw UserDraw) {

	this->Instance = Instance;
	this->UserInit = UserInit;
	this->UserDeInit = UserDeInit;
	this->UserUpdate = UserUpdate;
	this->UserDraw = UserDraw;

	//	Add to file name path - where cfg is situated
	char _CFG[512];
	sprintf(_CFG, "Data\\Engine\\%s", ConfigFileName);

	//	Create and init logger
	char *LogFileName = _strdup(ReadConfigValue(_CFG, "LogFile.html", "LOGGER", "FileName"));
	bool LogEnabled = atoi(ReadConfigValue(_CFG, "0", "LOGGER", "Enabled")) ? true : false;
	if (!(Logger = new CLogger)) return false;
	if (!Logger->Init(LogFileName, LogEnabled)) return false;
	Logger->WriteMessage("Logger initialized");

	Logger->WriteMessage("Core initialization");

	//	Read all settings from cfg file
	Logger->WriteMessage("Reading values from config file %s", ConfigFileName);
	//	Window settings
	WindowTitle = _strdup(ReadConfigValue(_CFG, "Mangal Development Asteroid Fields 2 v1.1", "WINDOW", "Title"));
	WindowWidth = atoi(ReadConfigValue(_CFG, "800", "WINDOW", "Width"));
	WindowHeight = atoi(ReadConfigValue(_CFG, "600", "WINDOW", "Height"));
	FullScreen = atoi(ReadConfigValue(_CFG, "1", "WINDOW", "FullScreen")) ? true : false;
	//	Render settings
	int ColorBits = atoi(ReadConfigValue(_CFG, "32", "RENDER", "ColorBits"));
	int DepthBits = atoi(ReadConfigValue(_CFG, "16", "RENDER", "DepthBits"));
	int AlphaBits = atoi(ReadConfigValue(_CFG, "0", "RENDER", "AlphaBits"));
	int StencilBits = atoi(ReadConfigValue(_CFG, "0", "RENDER", "StencilBits"));
	int AccumBits = atoi(ReadConfigValue(_CFG, "0", "RENDER", "AccumBits"));

	//	Create window class
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.hIconSm			= LoadIcon(0, IDI_APPLICATION);
	wc.hInstance		= Instance;
	wc.lpfnWndProc		= WindowProc;
	wc.lpszClassName	= "MangalDevelopment_WindowClass";
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	if (!RegisterClassEx(&wc)) {
		Logger->WriteError("Window class registering error");
		DeInit();
		return false;
	}	//	if (!RegisterClassEx(&wc))
	Logger->WriteMessage("Window class registered");
	//	Change to fullscreen if need
	if (FullScreen) {
		DEVMODE dmScreenSettings;					
		memset(&dmScreenSettings, 0, sizeof(DEVMODE));	
		dmScreenSettings.dmSize				= sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth		= WindowWidth;			
		dmScreenSettings.dmPelsHeight		= WindowHeight;		
		dmScreenSettings.dmBitsPerPel		= ColorBits;				
		dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			FullScreen = false;
			Logger->WriteWarning("Unable to set full screen mode - will be used windowed mode");
		} else Logger->WriteMessage("Display mode changed to full screen mode");
	}	//	if (FullScreen)
	//	Set window styles according fullscreen flag
	DWORD dwStyle, dwExStyle;
	if (FullScreen)	{
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		dwExStyle = WS_EX_APPWINDOW;
	} else {	//	if (FullScreen)
		dwStyle = WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}	//	if (FullScreen)
	//	Create window
	if (!(Window = CreateWindowEx(dwExStyle, "MangalDevelopment_WindowClass", WindowTitle,
		dwStyle, 0, 0, WindowWidth, WindowHeight, 0, 0, Instance, 0))) {
			Logger->WriteError("Window creating error");
			DeInit();
			return false;
		}	//	if (!(Window = CreateWindowEx(...)))
	Logger->WriteMessage("Window created");
	//	Set some window states
	ShowWindow(Window, SW_SHOW);
    UpdateWindow(Window);
	SetForegroundWindow(Window);						
	SetFocus(Window);

	//	Create all parts of core
	//	Timer
	if (!(Timer = new CTimer)) {
		Logger->WriteError("Timer creating error");
		DeInit();
		return false;
	}	//	if (!(Timer = new CTimer))
	Logger->WriteMessage("Timer created");
	//	Input
	if (!(Input = new CInput)) {
		Logger->WriteError("Input creating error");
		DeInit();
		return false;
	}	//	if (!(Timer = new CTimer))
	Logger->WriteMessage("Input created");
	//	Render
	if (!(Render = new CRender)) {
		Logger->WriteError("Render creating error");
		DeInit();
		return false;
	}	//	if (!(Render = new CRender))
	Logger->WriteMessage("Render created");

	//	Init all parts of core
	//	Timer
	if (!Timer->Init()) {
		Logger->WriteError("Timer initializing error");
		DeInit();
		return false;
	}	//	if (!Timer->Init())
	Timer->SetPause(false);
	Logger->WriteMessage("Timer initialized");
	//	Input
	if (!Input->Init(Instance, Window)) {
		Logger->WriteError("Input initializing error");
		DeInit();
		return false;
	}	//	if (!Input->Init())
	Logger->WriteMessage("Input initialized");
	//	Render
	if (!Render->Init(Window, WindowWidth, WindowHeight, ColorBits, DepthBits, AlphaBits,
		StencilBits, AccumBits)) {
		Logger->WriteError("Render initializing error");
		DeInit();
		return false;
	}	//	if (!Render->Init())
	Logger->WriteMessage("Render initialized");

	//	Init OpenIL (DevIL)
	ilInit();

	//	User side init function
	Logger->WriteMessage("Calling user side initialization");
	if (!UserInit()) {
		Logger->WriteError("User side initializing error");
		DeInit();
		return false;
	}	//	if (!UserInit())
	Logger->WriteMessage("User side initialized");

	Logger->WriteMessage("Core initialized");
	return true;
}	//	bool CCore::Init(const HINSTANCE Instance, const char *ConfigFileName)

void CCore::DeInit(void) {
	Logger->WriteMessage("Core deinitialization");

	//	User side deinit function
	Logger->WriteMessage("Calling user side deinitialization");
	UserDeInit();
	Logger->WriteMessage("User side deinitialized");

	if (Timer) { delete Timer; Timer = 0; }
	Logger->WriteMessage("Timer deinitialized");
	if (Input) { Input->DeInit(); delete Input; Input = 0; }
	Logger->WriteMessage("Input deinitialized");
	if (Render) { Render->DeInit(); delete Render; Render = 0; }
	Logger->WriteMessage("Render deinitialized");

	Logger->WriteMessage("Core deinitialized");

	if (Window) {
		DestroyWindow(Window); Window = 0;
		Logger->WriteMessage("Window destroyed");
	}	//	if (Window)

	Logger->WriteMessage("Logger deinitialization");
	if (Logger) { Logger->DeInit(); delete Logger; Logger = 0; }
}	//	void CCore::DeInit(void)

void CCore::MainLoop(void) {
	MSG Msg;
	unsigned long StartTicks = 0;
	MainLoopActive = true;
	Timer->Update();
	Logger->WriteMessage("Main loop started");

	while (MainLoopActive) {
		while (PeekMessage(&Msg, 0, 0, 0, PM_NOREMOVE))
			if (GetMessage(&Msg, 0, 0, 0)) {
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			} else MainLoopActive = false;
		StartTicks = GetTickCount();

		Timer->Update();
		Input->Update();
		UserUpdate();

		for (int i=0;  i<(int)ParticleSystems.size();  i++) UpdateParticleSystem(i);

		Render->BeginScene();
		UserDraw();
		Render->EndScene();

		while ((GetTickCount() - StartTicks) < 15);
	}	//	while (MainLoopActive)

	Logger->WriteMessage("Main loop ended");
}	//	void CCore::MainLoop(void)

void CCore::StopMainLoop(void) { 
	MainLoopActive = false; 
	Logger->WriteMessage("Main loop stopped");
}	//	void CCore::StopMainLoop(void)

char *CCore::ReadConfigValue(const char *ConfigFileName, const char *DefValue,
	const char *Group, const char *Name) {
	static char Buf[512];
	GetPrivateProfileString(Group, Name, DefValue, Buf, 512, ConfigFileName);
	return Buf;
}	//	char *CCore::ReadConfigValue(...)

LRESULT CCore::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}	//	switch (uMsg)

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}	//	LRESULT CCore::WindowProc(...)