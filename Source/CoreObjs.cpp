#include "PreCompiled.h"

CTexture *CCore::CreateTexture(const char *FileName, bool CreateAlpha, const CVector3 AlphaColor) {
	//	Check up if texture was already loaded
	for (unsigned int i=0; i<(unsigned int)Textures.size(); i++) 
		if (std::string(Textures[i]->Name) == std::string(FileName)) 
			return Textures[i];
	//	Create new texture object
	CTexture *Texture = new CTexture; 
	if (!Texture) {
		Logger->WriteError("Unable to create texture object");
		return 0;
	}	//	if (!Texture)
	//	Determine texture filename extension
	std::string FN = FileName;
	size_t t = FN.rfind('.', FN.size());
	std::string Ext = FN.substr(t+1, FN.size()-t-1);
	_strupr((char *)Ext.c_str());

	if (Ext == std::string("JPG")) {
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	}	//	if (Ext == std::string("JPG"))
	if (Ext == std::string("BMP")) {
		ilEnable(IL_FORMAT_SET);
		ilFormatFunc(IL_RGB);
	}	//	if (Ext == std::string("BMP"))

	unsigned int ilImage = 0;
	ilGenImages(1, &ilImage); 
	if (!ilImage) {
		Logger->WriteError("Unable to create texture object");
		delete Texture; Texture = 0; return 0;
	}	//	if (!ilImage)
	ilBindImage(ilImage);

	if (!ilLoadImage((ILstring)(std::string("Data\\Textures\\")+FN).c_str())) {
		Logger->WriteError("Unable to load texture from file %s", FileName);
		delete Texture; Texture = 0; return 0;
	}	//	if (!ilLoadImage(...))
	if (!ilGetData()) {
		Logger->WriteError("Unable to get data from loaded texture from file %s", FileName);
		delete Texture; Texture = 0; return 0;
	}	//	if (!ilGetData())

	Texture->Name			= _strdup(FileName);
	Texture->Width			= ilGetInteger(IL_IMAGE_WIDTH);
	Texture->Height			= ilGetInteger(IL_IMAGE_HEIGHT);
	Texture->BytesPerPixel	= ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	Texture->Format			= (CTexture::EFormat)ilGetInteger(IL_IMAGE_FORMAT);
	Texture->Type			= (CTexture::EType)ilGetInteger(IL_IMAGE_TYPE);

	//	Create gl texture and set properties
	glGenTextures(1, &Texture->ID);
	if (!Texture->ID) {
		Logger->WriteError("Unable to create OpenGL texture object");
		delete Texture; Texture = 0; return 0;
	}	//	if (!Texture->ID)
	glBindTexture(GL_TEXTURE_2D, Texture->ID);

	//	Our texture data array
	int Size = Texture->BytesPerPixel * Texture->Width * Texture->Height;
	unsigned char *Data = new unsigned char[Size];
	unsigned char *AlphaData = 0;
	memcpy(Data, ilGetData(), Size);

	//	Add alpha component to texture if needed
	if (CreateAlpha) {
		if (Texture->BytesPerPixel == 3) {
			AlphaData = new unsigned char[4*Texture->Width*Texture->Height];

			for (int i=0;  i<(unsigned int)(Texture->Width*Texture->Height);  i++) {
				AlphaData[i*4+0] = Data[i*3+0];
				AlphaData[i*4+1] = Data[i*3+1];
				AlphaData[i*4+2] = Data[i*3+2];
				if (Data[i*3+0] == AlphaColor.x &&
					Data[i*3+1] == AlphaColor.y &&
					Data[i*3+2] == AlphaColor.z) AlphaData[i*4+3] = 0;
				else AlphaData[i*4+3] = 255;
			}	//	for (i=0;  i<(unsigned int)Size2;  i++)

			Texture->Format = CTexture::TF_RGBA;
			Texture->BytesPerPixel = 4;
		} else CreateAlpha = false;
	}	//	if (CreateAlpha)

	//	Create gl texture
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (CreateAlpha) {
	gluBuild2DMipmaps(GL_TEXTURE_2D, Texture->BytesPerPixel, Texture->Width,
		Texture->Height, Texture->Format, Texture->Type, AlphaData);
	} else {	//	if (CreateAlpha)
	gluBuild2DMipmaps(GL_TEXTURE_2D, Texture->BytesPerPixel, Texture->Width,
		Texture->Height, Texture->Format, Texture->Type, Data);
	}	//	if (CreateAlpha)

	if (!Texture->ID) {
		Logger->WriteError("Unable to create OpenGL texture object");
		delete Texture; Texture = 0; return 0;
	}	//	if (!Texture->ID)

	if (Ext == std::string("JPG")) ilEnable(IL_ORIGIN_SET);
	if (Ext == std::string("BMP")) ilEnable(IL_FORMAT_SET);
	ilDeleteImages(1, &ilImage);

	if (!CreateAlpha) { 
		if (Data) { delete [] Data; Data = 0; } 
	} else {
		if (AlphaData) { delete [] AlphaData; AlphaData = 0; }
	}

	//	Add pointer to all textures vector
	Textures.push_back(Texture);
	Logger->WriteMessage("Texture loaded from file %s", FileName);
	return Texture;
}	//	CTexture *CCore::CreateTexture(const char *FileName)

CFont *CCore::CreateFont(const char *FileName, const float Size, const float Space) {
	CFont *Font = new CFont;
	if (!Font) {
		Logger->WriteError("Unable to create font object");
		return 0;
	}	//	if (!Font)

	if (!(Font->Texture = CreateTexture(FileName, true))) {
		Logger->WriteError("Unable to load font from file %s", FileName);
		delete Font; Font = 0; return 0;
	}	//	if (!(Font->Texture = CreateTexture(Name)))
	Font->Name = _strdup(FileName);
	Font->Size	= Size;
	Font->Space	= Space;

	Font->DisplayListID = glGenLists(256);
	if (!Font->DisplayListID) {
		Logger->WriteError("Unable to create display list for font from file %s", FileName);
		delete Font; Font = 0; return 0;
	}	//	if (!Font->DisplayListID)
	glBindTexture(GL_TEXTURE_2D, Font->Texture->ID);
	glPushMatrix();
	for (int i=0;  i<256;  i++) {
		float cx = float(i % 16) / 16.0f, cy = float(i / 16) / 16.0f;

		glNewList(Font->DisplayListID+i, GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(cx, 1.0f-cy-0.0625f);			glVertex2f(0.0f, 0.0f);
				glTexCoord2f(cx+0.0625f, 1.0f-cy-0.0625f);	glVertex2f(Size, 0.0f);
				glTexCoord2f(cx+0.0625f, 1.0f-cy);			glVertex2f(Size, Size);
				glTexCoord2f(cx, 1.0f-cy);					glVertex2f(0.0f, Size);
			glEnd();	//	glBegin(GL_QUADS);
			glTranslatef(Space, 0.0f, 0.0f);
		glEndList();	//	glNewList(Font->DisplayListID + i, GL_COMPILE);
	}	//	for (int i=0;  i<256;  i++)
	glPopMatrix();	//	glPushMatrix();

	Fonts.push_back(Font);
	Logger->WriteMessage("Font loaded from file %s", FileName);
	return Font;
}	//	CFont *CCore::CreateFont(const char *FileName, const float Size, const float Space)

CMesh *CCore::CreateMesh(const char *FileName, const char *MeshName) {
	CMesh *Mesh = new CMesh;
	if (!Mesh) {
		Logger->WriteError("Unable to create mesh object");
		return 0;
	}	//	if (!Mesh)

	//	Open file stream
	std::ifstream File((char *)(std::string("Data\\Meshes\\")+std::string(FileName)).c_str(),
		std::ios::in | std::ios::binary);
	if (!File) {
		Logger->WriteError("Unable to open file stream to read mesh data from file %s", FileName);
		delete Mesh; Mesh = 0; return 0;
	}	//	if (!File)

	char Ch = 0;
	int I = 0;
	std::string S = "";

	while (File.good()) {
		//	Read mesh name
		S = ""; do { File >> Ch; S += Ch; } while (Ch != 0); S.resize(S.size()-1);
		if (!S.size()) {
			Logger->WriteError("Unable to read mesh data from file %s", FileName);
			delete Mesh; Mesh = 0; File.close(); return 0;
		}	//	if (!S.size())
		Mesh->Name = _strdup(S.c_str());

		//	Read mesh subsets
		File.read((char *)&I, sizeof(unsigned int)); Mesh->Subsets.resize(I);
		for (int i=0;  i<(int)Mesh->Subsets.size();  i++) {
			//	Read diffuse texture filename
			S = ""; do { File >> Ch; S += Ch; } while (Ch != 0); S.resize(S.size()-1);
			if (!S.size()) {
				Logger->WriteError("Unable to read mesh data from file %s", FileName);
				delete Mesh; Mesh = 0; File.close(); return 0;
			}	//	if (!S.size())
			//	Create texture and material
			CTexture *Diffuse = CreateTexture((char *)S.c_str());
			if (!Diffuse) { delete Mesh; Mesh = 0; File.close(); return 0; }

			Mesh->Subsets[i].Material = new CMaterial;
			if (!Mesh->Subsets[i].Material) {
				Logger->WriteError("Unable to create material object");
				delete Mesh; Mesh = 0; File.close(); return 0;
			}	//	if (!Mesh->Subsets[i].Material)
			Mesh->Subsets[i].Material->Diffuse = Diffuse;
		}	//	for (int i=0; i<Mesh->Subsets.size(); i++)

		File.read((char *)&I, sizeof(int)); Mesh->Vertices.resize(I);
		File.read((char *)&I, sizeof(int)); Mesh->Faces.resize(I); Mesh->Triangles.resize(I);

		for (int i=0;  i<(int)Mesh->Vertices.size();  i++) {
			File.read((char *)&Mesh->Vertices[i].Position.x, sizeof(float)*3);
			File.read((char *)&Mesh->Vertices[i].Normal.x, sizeof(float)*3);
			File.read((char *)&Mesh->Vertices[i].Texture.x, sizeof(float)*2);
		}	//	for (i=0;  i<Mesh->GetVerticesCount();  i++)

		for (int i=0;  i<(int)Mesh->Faces.size();  i++) {
			File.read((char *)&Mesh->Faces[i].Indices[0], sizeof(int)*3);
			memcpy(&Mesh->Triangles[i].Indices[0], &Mesh->Faces[i].Indices[0], sizeof(int)*3);
			File.read((char *)&Mesh->Faces[i].MaterialID, sizeof(int));
		}	//	for (i=0;  i<Mesh->GetFacesCount();  i++)

		for (int i=0;  i<(int)Mesh->Faces.size();  i++) {
			for (int j=i;  j<(int)Mesh->Faces.size();  j++) {
				if (Mesh->Faces[i].MaterialID > Mesh->Faces[j].MaterialID) {
					CMesh::CFace xFace = Mesh->Faces[i];
					Mesh->Faces[i] = Mesh->Faces[j];
					Mesh->Faces[j] = xFace;

					CMesh::CTriangle xTriangle = Mesh->Triangles[i];
					Mesh->Triangles[i] = Mesh->Triangles[j];
					Mesh->Triangles[j] = xTriangle;
				}
			}
		}

		for (int i=0;  i<(int)Mesh->Faces.size();  i++) {
			Mesh->Subsets[Mesh->Faces[i].MaterialID].Faces.push_back(Mesh->Faces[i]);
			Mesh->Subsets[Mesh->Faces[i].MaterialID].Triangles.push_back(Mesh->Triangles[i]);
		}

		//	Add vertex buffer offset value
		Mesh->Subsets[0].StartIndex = 0;
		for (int i=1;  i<(int)Mesh->Subsets.size();  i++)
			Mesh->Subsets[i].StartIndex =
			(int)Mesh->Subsets[i-1].Faces.size()+Mesh->Subsets[i-1].StartIndex;


		if (std::string(Mesh->Name) == std::string(MeshName)) {
			File.close();
			Mesh->DisplayListID = 0;
			Meshes.push_back(Mesh);
			Logger->WriteMessage("Mesh with name %s loaded from file %s", MeshName, FileName);
			if (!Render->PrecacheMesh(Mesh)) {
				Logger->WriteError("Unable to precache mesh from file %s", FileName);
				delete Mesh; Mesh = 0; File.close(); return 0;
			}	//	if (!Render->PrecacheMesh(Mesh))
			Logger->WriteMessage("Mesh with name %s precached", MeshName);
			return Mesh;
		}	//	if (std::string(Mesh->Name) == std::string(_MeshName))
	}	//	while (File.good())

	File.close();
	return 0;
}	//	CMesh *CCore::CreateMesh(const char *FileName, const char *MeshName)

CParticleSystem *CCore::CreateParticleSystem(const bool Looped, const int ParticlesCount,
	const int SpawnRate, const float SpawnTime, const float Size,
	const float LifeFadeMin, const float LifeFadeMax, const CVector3 &Position,
	const CVector3 &PositionMin, const CVector3 &PositionMax,
	const CVector3 &VelocityMin, const CVector3 &VelocityMax,
	const CVector3 &GravityMin, const CVector3 &GravityMax, const char *Texture) {

	if (!Texture) return 0;

	CParticleSystem *PS = new CParticleSystem;
	if (!PS) {
		Logger->WriteError("Unable to create particle system object");
		return 0;
	}	//	if (!PS)

	PS->Looped				= Looped;
	PS->ParticlesCount		= ParticlesCount;
	PS->SpawnRate			= SpawnRate;
	PS->SpawnTime			= SpawnTime;
	PS->SpawnTimeCounter	= 0.0f;
	PS->Size				= Size;
	PS->LifeFadeMin			= LifeFadeMin;
	PS->LifeFadeMax			= LifeFadeMax;
	PS->Position			= Position;
	PS->PositionMin			= PositionMin;
	PS->PositionMax			= PositionMax;
	PS->VelocityMin			= VelocityMin;
	PS->VelocityMax			= VelocityMax;
	PS->GravityMin			= GravityMin;
	PS->GravityMax			= GravityMax;
	PS->Texture				= CreateTexture(Texture);
	if (!PS->Texture) {
		Logger->WriteError("Unable to init particle system");
		delete PS; PS = 0; return 0;
	}	//	if (!PS->Texture)

	ParticleSystems.push_back(PS);
	Logger->WriteMessage("Particle system with sprite %s created", Texture);
	return PS;
}	//	CParticleSystem *CCore::CreateParticleSystem(...)

void CCore::UpdateParticleSystem(const int i) {
	float _DeltaTime = Timer->GetDeltaTime();
	CParticleSystem *PS = ParticleSystems[i];

	//	Spawn new particles according delta time
	PS->SpawnTimeCounter += _DeltaTime;
	if (PS->SpawnTimeCounter >= PS->SpawnTime) {
		int SpawnCount = PS->SpawnRate;
		if (SpawnCount + (int)PS->Particles.size() > PS->ParticlesCount)
			SpawnCount = PS->ParticlesCount - (int)PS->Particles.size();

		if (!PS->Active) SpawnCount = 0;

		for (int i=0;  i<SpawnCount;  ++i) {
			CParticleSystem::CParticle *Particle = new CParticleSystem::CParticle;

			//	Init data of created particle
			Particle->Active = true;
			Particle->Life = 1.0f;
			Particle->LifeFade = ((float)rand()/(float)RAND_MAX)*(PS->LifeFadeMax-PS->LifeFadeMin)+PS->LifeFadeMin;
			Particle->Position = PS->Position + Random(PS->PositionMin, PS->PositionMax);
			Particle->Velocity = Random(PS->VelocityMin, PS->VelocityMax);
			Particle->Gravity = Random(PS->GravityMin, PS->GravityMax);

			PS->Particles.push_back(Particle);
		}	//	for (unsigned int i=0;  i<SpawnRate;  ++i)

		PS->SpawnTimeCounter = 0.0f;
	}	//	if (Time >= SpawnTime)

	//	Update all spawned particles
	for (int i=0;  i<(int)PS->Particles.size();  ++i) {
		if (!PS->Particles[i]->Active) continue;
		PS->Particles[i]->Position += PS->Particles[i]->Velocity * _DeltaTime;
		PS->Particles[i]->Velocity += PS->Particles[i]->Gravity * _DeltaTime;
		PS->Particles[i]->Life -= PS->Particles[i]->LifeFade * _DeltaTime;
		//	Destroy particle - if it was died
		if (PS->Particles[i]->Life <= 0.0f) {
			PS->Particles[i]->Active = false;
			delete PS->Particles[i];
			PS->Particles.erase(PS->Particles.begin()+i);
		}	//	if (Particles[i]->Life <= 0.0f)
	}	//	for (i=0;  i<Particles.size();  ++i)
}	//	void CCore::UpdateParticleSystem(const unsigned int i)

CFlareEffect *CCore::CreateFlareEffect(const bool Fadeable, const float Distance,
	const float Size, const CVector3 &Position, const char *Texture) {

	if (!Texture) return 0;

	CFlareEffect *FE = new CFlareEffect;
	if (!FE) {
		Logger->WriteError("Unable to create flare effect object");
		return 0;
	}	//	if (!FE)

	FE->Fadeable	= Fadeable;
	FE->Distance	= Distance;
	FE->Size		= Size;
	FE->Position	= Position;
	FE->Texture		= CreateTexture(Texture);
	if (!FE->Texture) {
		Logger->WriteError("Unable to init flare effect");
		delete FE; FE = 0; return 0;
	}	//	if (!FE->Texture)

	FlareEffects.push_back(FE);
	Logger->WriteMessage("Flare effect with sprite %s created", Texture);
	return FE;
}	//	CFlareEffect *CCore::CreateFlareEffect(...)