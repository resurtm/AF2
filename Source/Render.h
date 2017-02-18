#pragma once

class CRender {
public:
	enum EMode {
		RM_2D,
		RM_3D,
	};	//	enum EMode;
	enum ERenderMethod {
		RRM_VERTEX_ARRAY,
		RRM_DISPLAY_LIST,
		RRM_VERTEX_BUFFER,
	};	//	enum ERenderMethod;
	class CInfo {
	public:
		char *Vendor, *Renderer, *Version, *Extensions;
		bool ARB_multitexture, EXT_swap_control;
		bool ARB_vertex_program, ARB_fragment_program;
		bool ARB_vertex_buffer_object;

	protected:
	private:
	};	//	class CInfo;

	inline CRender(void) {}
	inline ~CRender(void) {}

	inline CInfo &GetInfo(void) { return Info; }

	bool Init(const HWND Window, const int Width, const int Height, const int ColorBits,
		const int DepthBits, const int AlphaBits, const int StencilBits, const int AccumBits);
	void DeInit(void);
	//	Begin/EndScene
	void BeginScene(void);
	void EndScene(void);
	//	Render modes
	void SetMode(const EMode Mode);
	inline EMode GetMode(void) { return Mode; }
	//	Render modes matrices
	inline void SetModelView2D(const CMatrix &m) { ModelView2D = m; }
	inline void SetProjection2D(const CMatrix &m) { Projection2D = m; }
	inline CMatrix &GetModelView2D(void) { return ModelView2D; }
	inline CMatrix &GetProjection2D(void) { return Projection2D; }
	inline void SetModelView3D(const CMatrix &m) { ModelView3D = m; }
	inline void SetProjection3D(const CMatrix &m) { Projection3D = m; }
	inline CMatrix &GetModelView3D(void) { return ModelView3D; }
	inline CMatrix &GetProjection3D(void) { return Projection3D; }
	//	Rendering
	bool PrecacheMesh(CMesh *Mesh);
	void RenderMesh(CMesh *M, CMatrix &m = CMatrix(), ERenderMethod RM = RRM_VERTEX_ARRAY);
	//	Sprites rendering
	void BeginRenderSprites(void);
	void EndRenderSprites(void);
	//	Text rendering
	void WriteText(const CFont *Font, const CVector2 &Position, const CVector3 &Color, const char *Text, ...);
	//	Particle systems rendering
	void RenderParticleSystem(const CParticleSystem *PS);
	//	Light effects rendering
	void RenderFlareEffect(const CFlareEffect *FlareEffect);

protected:
	HWND Window;
	HDC DeviceContext;
	HGLRC RenderContext;

	CInfo Info;
	int Width, Height, ColorBits, DepthBits, AlphaBits, StencilBits, AccumBits;

	EMode Mode;
	CMatrix Projection2D, ModelView2D, Projection3D, ModelView3D;

private:
};	//	class CRender;