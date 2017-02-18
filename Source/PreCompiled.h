#pragma once

#define WIN32_LEAN_AND_MEAN 1
#define VC_LEANMEAN 1
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <string>
#include <vector>

//#define GLEW_STATIC 1
#include "glew\glew.h"
#include "glew\wglew.h"
#include <gl\gl.h>
#include <gl\glu.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "IL\il.h"
#include "fmod\fmod.h"

#include "Math.h"
#include "Material.h"
#include "Font.h"
#include "LightEffects.h"
#include "Particles.h"
#include "Mesh.h"
#include "Timer.h"
#include "Logger.h"
#include "Input.h"
#include "Render.h"
#include "Core.h"
#include "Game.h"

#include "..\Resource\Resource.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "..\\Asteroid Fields 2\\Source\\glew\\glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "..\\Asteroid Fields 2\\Source\\fmod\\fmodvc.lib")
#pragma comment(lib, "..\\Asteroid Fields 2\\Source\\IL\\DevIL.lib")