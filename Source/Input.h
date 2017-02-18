#pragma once

class CInput {
public:
	inline CInput(void) {}
	inline ~CInput(void) {}

	inline int GetMousePositionX(void) { return MousePosition[0]; }
	inline int GetMousePositionY(void) { return MousePosition[1]; }
	inline int GetMousePositionZ(void) { return MousePosition[2]; }
	inline bool GetMouseButton(const unsigned int i) { return MouseButtons[i]; }
	inline bool GetKey(const unsigned int i) { return Keys[i]; }
	inline void SetKey(const unsigned int i, const bool b) { Keys[i] = b; }

	bool Init(const HINSTANCE _Instance, const HWND _Window);
	void DeInit(void);
	void Update(void);

protected:
	LPDIRECTINPUT8 DirectInput;
	LPDIRECTINPUTDEVICE8 KeyboardDevice, MouseDevice;
	int MousePosition[3];
	bool MouseButtons[4], Keys[256];

private:
};	//	class CInput;