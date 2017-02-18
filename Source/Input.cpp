#include "PreCompiled.h"

bool CInput::Init(const HINSTANCE _Instance, const HWND _Window) {
	if (DirectInput8Create(_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, 
		(VOID**)&DirectInput, 0) != DI_OK) 
		return false;

	if (DirectInput->CreateDevice(GUID_SysKeyboard, &KeyboardDevice, 0) != DI_OK) 
		return false;
	if (!KeyboardDevice) return false;
	if (KeyboardDevice->SetDataFormat(&c_dfDIKeyboard) != DI_OK) {
		KeyboardDevice->Release(); KeyboardDevice = 0;
		return false;
	}	//	if (KeyboardDevice->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
	if (KeyboardDevice->SetCooperativeLevel(_Window, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) 
		!= DI_OK) {
		KeyboardDevice->Release(); KeyboardDevice = 0;
		return false;
	}	//	if (KeyboardDevice->SetCooperativeLevel(...) != DI_OK)
	KeyboardDevice->Acquire();

	if (DirectInput->CreateDevice(GUID_SysMouse, &MouseDevice, 0) != DI_OK) return false;
	if (!MouseDevice) return false;
	if (MouseDevice->SetDataFormat(&c_dfDIMouse) != DI_OK) {
		MouseDevice->Release();	MouseDevice = 0;
		return false;
	}	//	if (MouseDevice->SetDataFormat(&c_dfDIMouse) != DI_OK)
	if (MouseDevice->SetCooperativeLevel(_Window, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) 
		!= DI_OK) {
		MouseDevice->Release();	MouseDevice = 0;
		return false;
	}	//	if (MouseDevice->SetCooperativeLevel(...)
	MouseDevice->Acquire();

	return true;
}	//	bool CInput::Init(const HINSTANCE _Instance, const HWND _Window)

void CInput::DeInit(void) {
	if (KeyboardDevice) {
		KeyboardDevice->Unacquire();
		KeyboardDevice->Release(); KeyboardDevice = 0;
	}	//	if (KeyboardDevice)

	if (MouseDevice) {
		MouseDevice->Unacquire();
		MouseDevice->Release(); MouseDevice = 0;
	}	//	if (MouseDevice)

	if (DirectInput) { DirectInput->Release(); DirectInput = 0; }
}	//	void CInput::DeInit(void)

void CInput::Update(void) {
	if (!KeyboardDevice || !MouseDevice) return;
	if (KeyboardDevice->GetDeviceState(256, Keys) != DI_OK) return;
	DIMOUSESTATE ms;
	if (MouseDevice->GetDeviceState(sizeof(ms), &ms) != DI_OK) return;
	memcpy(MousePosition, &ms, sizeof(int)*3);
	for (int i=0; i<4; i++) MouseButtons[i] = ms.rgbButtons[i] != 0;
}	//	void CInput::Update(void)