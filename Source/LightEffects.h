#pragma once

class CFlareEffect {
public:
	inline CFlareEffect(void) {}
	inline ~CFlareEffect(void) {}

	bool Active, Fadeable;
	float Distance, Size;
	CVector3 Position;
	CTexture *Texture;

protected:
private:
};	//	class CFlareEffect;