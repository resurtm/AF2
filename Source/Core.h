#pragma once

class CCore {
public:
	typedef bool (* FInit)(void);
	typedef void (* FDeInit)(void);
	typedef void (* FUpdate)(void);
	typedef void (* FDraw)(void);

	inline CCore(void) {}
	inline ~CCore(void) {}

	inline char *GetWindowTitle(void) { return WindowTitle; }
	inline int GetWindowWidth(void) { return WindowWidth; }
	inline int GetWindowHeight(void) { return WindowHeight; }
	inline bool IsFullScreen(void) { return FullScreen; }
	inline bool IsMainLoopActive(void) { return MainLoopActive; }
	inline CTimer *GetTimer(void) { return Timer; }
	inline static CLogger *GetLogger(void) { return Logger; }
	inline CInput *GetInput(void) { return Input; }
	inline CRender *GetRender(void) { return Render; }

	bool Init(const HINSTANCE Instance, const char *ConfigFileName, FInit UserInit,
		FDeInit UserDeInit, FUpdate UserUpdate, FDraw UserDraw);
	void DeInit(void);

	void MainLoop(void);
	void StopMainLoop(void);

	CTexture *CreateTexture(const char *FileName, bool CreateAlpha = false,
		const CVector3 AlphaColor = CVector3());
	CFont *CreateFont(const char *FileName, const float Size, const float Space);
	CMesh *CreateMesh(const char *FileName, const char *MeshName);
	CParticleSystem *CreateParticleSystem(const bool Looped, const int ParticlesCount,
		const int SpawnRate, const float SpawnTime, const float Size,
		const float LifeFadeMin, const float LifeFadeMax, const CVector3 &Position,
		const CVector3 &PositionMin, const CVector3 &PositionMax,
		const CVector3 &VelocityMin, const CVector3 &VelocityMax,
		const CVector3 &GravityMin, const CVector3 &GravityMax, const char *Texture);
	CFlareEffect *CreateFlareEffect(const bool Fadeable, const float Distance,
		const float Size, const CVector3 &Position, const char *Texture);

protected:
	char *ReadConfigValue(const char *ConfigFileName, const char *DefValue,
		const char *Group, const char *Name);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void UpdateParticleSystem(const int i);

	HINSTANCE Instance;
	HWND Window;

	char *WindowTitle;
	int WindowWidth, WindowHeight;
	bool FullScreen;

	bool MainLoopActive;
	FInit UserInit;
	FDeInit UserDeInit;
	FUpdate UserUpdate;
	FDraw UserDraw;

	std::vector<CTexture *> Textures;
	std::vector<CFont *> Fonts;
	std::vector<CMesh *> Meshes;
	std::vector<CParticleSystem *> ParticleSystems;
	std::vector<CFlareEffect *> FlareEffects;

	CTimer *Timer;
	static CLogger *Logger;
	CInput *Input;
	CRender *Render;

private:
};	//	class CCore;