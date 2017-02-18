#pragma once

class CMesh {
public:
	struct CVertex {
		CVector3 Position, Normal;
		CVector2 Texture;
	};	//	struct CVertex;
	struct CFace { 
		CVector3 Position, Normal; 
		unsigned int Indices[3], MaterialID; 
	};	//	struct CFace;
	struct CTriangle { 
		unsigned int Indices[3]; 
	};	//	struct CTriangle;
	struct CSubset {
		std::vector<CFace> Faces;
		std::vector<CTriangle> Triangles;
		CMaterial *Material;
		unsigned int StartIndex;
	};	//	struct CSubset;

	inline CMesh(void) {}
	inline ~CMesh(void) {}

	char *Name;
	unsigned int DisplayListID, VertexBufferID, IndexBufferID;
	std::vector<CVertex> Vertices;
	std::vector<CFace> Faces;
	std::vector<CTriangle> Triangles;
	std::vector<CSubset> Subsets;

protected:
private:
};	//	class SMesh;