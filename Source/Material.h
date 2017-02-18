#pragma once

class CTexture {
public:
	enum EFormat {
		TF_COLOR_INDEX = 0x1900, TF_RGB = 0x1907, TF_RGBA = 0x1908, TF_BGR = 0x80E0,
		TF_BGRA = 0x80E1, TF_LUMINANCE = 0x1909, TF_LUMINANCE_ALPHA = 0x190A,
	};	//	enum EFormat;
	enum EType {
		TT_BYTE = 0x1400, TT_UNSIGNED_BYTE = 0x1401, TT_SHORT = 0x1402,
		TT_UNSIGNED_SHORT = 0x1403, TT_INT = 0x1404, TT_UNSIGNED_INT = 0x1405,
		TT_FLOAT = 0x1406, TT_DOUBLE = 0x140A,
	};	//	enum EType;

	inline CTexture(void) {}
	inline ~CTexture(void) {}

	char *Name;
	unsigned int Width, Height, BytesPerPixel, ID;
	EFormat Format;
	EType Type;

protected:
private:
};	//	class CTexture;

class CMaterial {
public:
	inline CMaterial(void) {}
	inline ~CMaterial(void) {}

	char *Name;
	CTexture *Diffuse;

protected:
private:
};	//	class CMaterial;