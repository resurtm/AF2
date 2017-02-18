#pragma once

class CFont {
public:
	inline CFont(void) {}
	inline ~CFont(void) {}

	char *Name;
	float Size, Space;
	unsigned int DisplayListID;
	CTexture *Texture;

protected:
private:
};	//	class CFont;