#include "PreCompiled.h"

bool CRender::PrecacheMesh(CMesh *Mesh) {
	//	Create vertex and index buffers of mesh
	if (Info.ARB_vertex_buffer_object) {
	glGenBuffersARB(1, &Mesh->VertexBufferID);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, Mesh->VertexBufferID);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(CMesh::CVertex)*(int)Mesh->Vertices.size(), 
		&Mesh->Vertices[0].Position[0], GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &Mesh->IndexBufferID);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, Mesh->IndexBufferID);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(CMesh::CTriangle)*Mesh->Triangles.size(), 
		&Mesh->Triangles[0].Indices[0], GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}

	//	Create display list of this mesh
	unsigned int DisplayList = glGenLists(1);
	if (!DisplayList) return false;
	glNewList(DisplayList, GL_COMPILE);
		RenderMesh(Mesh, CMatrix(), RRM_VERTEX_ARRAY);
	glEndList();
	if (!DisplayList) return false;

	Mesh->DisplayListID = DisplayList;

	return true;
}	//	bool CRender::PrecacheMesh(CMesh *Mesh)

void CRender::RenderMesh(CMesh *M, CMatrix &m, ERenderMethod RM) {
	if (!M) return;

	glPushMatrix();
	glMultMatrixf(m.m);

	if (RM == RRM_VERTEX_ARRAY) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(CMesh::CVertex), &M->Vertices[0].Position.x);
		glTexCoordPointer(2, GL_FLOAT, sizeof(CMesh::CVertex), &M->Vertices[0].Texture.x);
		glNormalPointer(GL_FLOAT, sizeof(CMesh::CVertex), &M->Vertices[0].Normal.x);

		for (unsigned int i=0; i<(unsigned int)M->Subsets.size(); i++) {
			if (M->Subsets[i].Material->Diffuse->ID) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, M->Subsets[i].Material->Diffuse->ID);
			} else glDisable(GL_TEXTURE_2D);

			glDrawElements(GL_TRIANGLES, (unsigned int)M->Subsets[i].Triangles.size()*3, 
				GL_UNSIGNED_INT, &M->Subsets[i].Triangles[0].Indices[0]);
		}	//	for (unsigned int i=0; i<(unsigned int)M->Subsets.size(); i++)
			
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}	//	if (RenderMethod == RRM_VERTEX_ARRAY)

	if (RM == RRM_DISPLAY_LIST) {
		if (!M->DisplayListID) return;
		glCallList(M->DisplayListID);
	}	//	if (RenderMethod == RRM_DISPLAY_LIST)

	if (RM == RRM_VERTEX_BUFFER) {
	if (!Info.ARB_vertex_buffer_object) RenderMesh(M, CMatrix(), RRM_VERTEX_ARRAY);

	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, M->VertexBufferID);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, M->IndexBufferID);
		
	glVertexPointer(3, GL_FLOAT, sizeof(CMesh::CVertex), 0);
	glNormalPointer(GL_FLOAT, sizeof(CMesh::CVertex), (char *)0 + sizeof(CVector3));
	glTexCoordPointer(2, GL_FLOAT, sizeof(CMesh::CVertex), (char *)0+sizeof(CVector3)*2);
	
	for (int i=0;  i<(int)M->Subsets.size();  i++) {
		glBindTexture(GL_TEXTURE_2D, M->Subsets[i].Material->Diffuse->ID);
		glDrawElements(GL_TRIANGLES, (int)M->Subsets[i].Faces.size()*3,
			GL_UNSIGNED_INT, (int *)0+M->Subsets[i].StartIndex*3);
	}	//	for (int i=0;  i<(int)Mesh->Subsets.size();  i++)

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}	//	if (RM == RRM_VERTEX_BUFFER)

	glPopMatrix();
}	//	void CRender::RenderMesh(CMesh *M, CMatrix &m, RenderMethod RM)

void CRender::BeginRenderSprites(void) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}	//	void CRender::BeginRenderSprites(void)

void CRender::EndRenderSprites(void) {
	glPopAttrib();
}	//	void CRender::EndRenderSprites(void)

void CRender::WriteText(const CFont *Font, const CVector2 &Position, const CVector3 &Color, const char *Text, ...) {
	if (!Font || !Text) return;

	va_list Args;
	unsigned int Length;
	//char Buffer[512];
	char *Buffer;
	va_start(Args, Text);
	Length = _vscprintf(Text, Args) + 1;
	Buffer = new char[Length * sizeof(char)];
	vsprintf(Buffer, Text, Args);

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(Color.x, Color.y, Color.z);
	glBindTexture(GL_TEXTURE_2D, Font->Texture->ID);
	glPushMatrix();
	glTranslatef(Position.x, Position.y, 0.0f);
	glListBase(Font->DisplayListID);
	glCallLists((int)strlen(Buffer), GL_UNSIGNED_BYTE, Buffer);
	glPopMatrix();

	if (Buffer) { delete [] Buffer; Buffer = 0; }
}	//	void CRender::WriteText(...)

void CRender::RenderParticleSystem(const CParticleSystem *PS) {
	if (!PS) return;

	glDepthMask(0);
	glBindTexture(GL_TEXTURE_2D, PS->Texture->ID);

	CVector3 Right(ModelView3D.m[0], ModelView3D.m[4], ModelView3D.m[8]);
	CVector3 Up(ModelView3D.m[1], ModelView3D.m[5], ModelView3D.m[9]);
	
	CVector3 Coo[4];
	Coo[0] = (!Right-Up)*PS->Size; Coo[1] = (Right-Up)*PS->Size;
	Coo[2] = (Right+Up)*PS->Size; Coo[3] = (!Right+Up)*PS->Size;

	CParticleSystem::CParticle *P = 0;
	glBegin(GL_QUADS);
	for (unsigned int i=0; i<(unsigned int )PS->Particles.size(); i++) {
		P = PS->Particles[i];
		glColor4f(1.0f, 1.0f, 1.0f, P->Life);
		glTexCoord2f(1.0f,1.0f); glVertex3f((P->Position+Coo[0]).x, (P->Position+Coo[0]).y, (P->Position+Coo[0]).z);
		glTexCoord2f(0.0f,1.0f); glVertex3f((P->Position+Coo[1]).x, (P->Position+Coo[1]).y, (P->Position+Coo[1]).z);
		glTexCoord2f(0.0f,0.0f); glVertex3f((P->Position+Coo[2]).x, (P->Position+Coo[2]).y, (P->Position+Coo[2]).z);
		glTexCoord2f(1.0f,0.0f); glVertex3f((P->Position+Coo[3]).x, (P->Position+Coo[3]).y, (P->Position+Coo[3]).z);
	}	//	for (unsigned int i=0; i<_PS->GetParticlesCount(); i++)
	glEnd();	//	glBegin(GL_QUADS);
}	//	void CRender::RenderParticleSystem(const SParticleSystem *PS)

void CRender::RenderFlareEffect(const CFlareEffect *FE) {
	if (!FE) return;
	if (!FE->Active) return;

	glDepthMask(0);
	glBindTexture(GL_TEXTURE_2D, FE->Texture->ID);
/*
	int ViewPort[4];
	glGetIntegerv(GL_VIEWPORT, ViewPort);

	double sx, sy, sz;
	float depth = 0.0f;
	gluProject(FE->Position.x, FE->Position.y, FE->Position.z, (double *)ModelView3D.m,
		(double *)Projection3D.m, ViewPort, &sx, &sy, &sz);
	glReadPixels((int)sx, (int)sy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
*/
//	if (depth > sz || !FE->Fadeable) {
		CVector3 Right(ModelView3D.m[0], ModelView3D.m[4], ModelView3D.m[8]);
		CVector3 Up(ModelView3D.m[1], ModelView3D.m[5], ModelView3D.m[9]);

		CVector3 Coo[4];
		Coo[0] = (-Right-Up)*FE->Size; Coo[1] = (Right-Up)*FE->Size;
		Coo[2] = (Right+Up)*FE->Size; Coo[3] = (-Right+Up)*FE->Size;

		glBegin(GL_QUADS);
		glTexCoord2f(1.0f,1.0f); glVertex3f((FE->Position+Coo[0]).x, (FE->Position+Coo[0]).y, (FE->Position+Coo[0]).z);
		glTexCoord2f(0.0f,1.0f); glVertex3f((FE->Position+Coo[1]).x, (FE->Position+Coo[1]).y, (FE->Position+Coo[1]).z);
		glTexCoord2f(0.0f,0.0f); glVertex3f((FE->Position+Coo[2]).x, (FE->Position+Coo[2]).y, (FE->Position+Coo[2]).z);
		glTexCoord2f(1.0f,0.0f); glVertex3f((FE->Position+Coo[3]).x, (FE->Position+Coo[3]).y, (FE->Position+Coo[3]).z);
		glEnd();
//	}  //	if (depth > sz || !FE->Fadeable)
}  //  void CRender::RenderFlareEffect(const CFlareEffect *FE)