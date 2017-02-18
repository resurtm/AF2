#pragma once

class CParticleSystem {
public:
	struct CParticle {
		bool Active;
		float Life, LifeFade;
		CVector3 Position, Velocity, Gravity;
	};	//	struct CParticle;

	inline CParticleSystem(void) {}
	inline ~CParticleSystem(void) {}

	bool Active, Looped;
	int ParticlesCount, SpawnRate;
	float SpawnTime, SpawnTimeCounter, Size, LifeFadeMin, LifeFadeMax;
	CVector3 Position;
	CVector3 PositionMin, PositionMax;
	CVector3 VelocityMin, VelocityMax;
	CVector3 GravityMin, GravityMax;
	CTexture *Texture;
	std::vector<CParticle *> Particles;

protected:
private:
};	//	class CParticleSystem;