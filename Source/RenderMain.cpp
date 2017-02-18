#include "PreCompiled.h"

bool CRender::Init(const HWND Window, const int Width, const int Height, const int ColorBits,
	const int DepthBits, const int AlphaBits, const int StencilBits, const int AccumBits) {

	this->Window = Window;
	this->Width = Width;
	this->Height = Height;
	this->ColorBits = ColorBits;
	this->DepthBits = DepthBits;
	this->AlphaBits = AlphaBits;
	this->StencilBits = StencilBits;
	this->AccumBits = AccumBits;

	CCore::GetLogger()->WriteMessage("Initializing OpenGL render");

	//	Describe needed pixel format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion		= 1;
	pfd.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType		= PFD_TYPE_RGBA;
	pfd.cColorBits		= ColorBits;
	pfd.cDepthBits		= DepthBits;
	pfd.cAlphaBits		= AlphaBits;
	pfd.cStencilBits	= StencilBits;
	pfd.cAccumBits		= AccumBits;
	pfd.iLayerType		= PFD_MAIN_PLANE;
	//	Init OpenGL
	int PixelFormat;
	if (!(DeviceContext = GetDC(Window))) return false;
	if (!(PixelFormat = ChoosePixelFormat(DeviceContext, &pfd))) return false;
	if(!SetPixelFormat(DeviceContext, PixelFormat, &pfd)) return false;
	if (!(RenderContext = wglCreateContext(DeviceContext))) return false;
	if(!wglMakeCurrent(DeviceContext, RenderContext)) return false;

	//	Rendering system's info
	Info.Vendor = (char *)glGetString(GL_VENDOR);
	Info.Renderer = (char *)glGetString(GL_RENDERER);
	Info.Version = (char *)glGetString(GL_VERSION); 
	Info.Extensions = (char *)glGetString(GL_EXTENSIONS);
	CCore::GetLogger()->WriteMessage("GL_VENDOR = %s", Info.Vendor);
	CCore::GetLogger()->WriteMessage("GL_RENDERER = %s", Info.Renderer);
	CCore::GetLogger()->WriteMessage("GL_VERSION = %s", Info.Version);
	CCore::GetLogger()->WriteMessage("GL_EXTENSIONS = %s", Info.Extensions);

	//	Working with OpenGL extensions and rendering system's caps
	glewInit();
	CCore::GetLogger()->WriteMessage("All available OpenGL extensions initialized");

	Info.ARB_multitexture = glewIsSupported("GL_ARB_multitexture") ? true : false;
	Info.EXT_swap_control = wglewIsSupported("WGL_EXT_swap_control") ? true : false;
	Info.ARB_vertex_program = glewIsSupported("GL_ARB_vertex_program") ? true : false;
	Info.ARB_fragment_program = glewIsSupported("GL_ARB_fragment_program") ? true : false;
	Info.ARB_vertex_buffer_object = glewIsSupported("GL_ARB_vertex_buffer_object") ? true : false;

	if (Info.ARB_multitexture)
		CCore::GetLogger()->WriteMessage("Querying GL_ARB_multitexture... Supported");	
	else CCore::GetLogger()->WriteWarning("Querying GL_ARB_multitexture... Not supported");
	if (Info.EXT_swap_control)
		CCore::GetLogger()->WriteMessage("Querying WGL_EXT_swap_control... Supported");	
	else CCore::GetLogger()->WriteWarning("Querying WGL_EXT_swap_control... Not supported");
	if (Info.ARB_vertex_program)
		CCore::GetLogger()->WriteMessage("Querying GL_ARB_vertex_program... Supported");	
	else CCore::GetLogger()->WriteWarning("Querying GL_ARB_vertex_program... Not supported");
	if (Info.ARB_fragment_program)
		CCore::GetLogger()->WriteMessage("Querying GL_ARB_fragment_program... Supported");	
	else CCore::GetLogger()->WriteWarning("Querying GL_ARB_fragment_program... Not supported");
	if (Info.ARB_vertex_buffer_object)
		CCore::GetLogger()->WriteMessage("Querying GL_ARB_vertex_buffer_object... Supported");	
	else CCore::GetLogger()->WriteWarning("Querying GL_ARB_vertex_buffer_object... Not supported");

	//	Turn off vertical synchronization - if available
	if (Info.EXT_swap_control) {
		wglSwapIntervalEXT(FALSE);
		CCore::GetLogger()->WriteMessage("VSync turned off - through WGL_EXT_swap_control");
	} else CCore::GetLogger()->WriteWarning("Unable to turn off VSync - WGL_EXT_swap_control not supported");

	//	Set some render states
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	//	Create main viewport
	glViewport(0, 0, Width,  Height);

	return true;
}	//	bool CRender::Init(...)

void CRender::DeInit(void) {
	if (RenderContext) { wglMakeCurrent(0, 0); wglDeleteContext(RenderContext); }
	if (DeviceContext) { ReleaseDC(Window, DeviceContext); }
}	//	void CRender::DeInit(void)

void CRender::BeginScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}	//	void CRender::BeginScene(void)

void CRender::EndScene(void) {
	glFlush();
	SwapBuffers(DeviceContext);
}	//	void CRender::EndScene(void)

void CRender::SetMode(const EMode Mode) {
	this->Mode = Mode;

	if (Mode == RM_2D) {
		glMatrixMode(GL_PROJECTION); glLoadMatrixf(Projection2D.m);
		glMatrixMode(GL_MODELVIEW); glLoadMatrixf(ModelView2D.m);
	}	//	if (Mode == RM_2D)
	if (Mode == RM_3D) {
		glMatrixMode(GL_PROJECTION); glLoadMatrixf(Projection3D.m);
		glMatrixMode(GL_MODELVIEW); glLoadMatrixf(ModelView3D.m);
	}	//	if (Mode == RM_3D)
}	//	void CRender::SetMode(const EMode Mode)