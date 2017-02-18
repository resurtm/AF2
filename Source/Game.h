#pragma once

class CPlayerShip {
public:
	inline CPlayerShip(void) {}
	inline ~CPlayerShip(void) {}

	inline CVector3 &GetPosition(void) { return Position; }
	inline CVector3 &GetRotation(void) { return Rotation; }
	inline CMatrix &GetTransform(void) { return Transform; }
	inline CVector3 &GetRocketPosition(void) { return RocketPosition; }
	inline CMesh *GetMesh(void) { return Mesh; }
	inline bool IsRocketLaunched(void) { return RocketLaunched; }
	inline void SetRocketLaunched(const bool b) {
		RocketLaunched = b;
		if (!RocketLaunched) {
			RocketSmoke->Active = RocketFlare->Active = false;
			FSOUND_StopSound(RocketSoundChannel);
			RocketSoundChannel = 0;
			RocketPosition = CVector3(100000.0f);
		}	//	if (!RocketLaunched)
	}	//	inline void SetRocketLaunched(const bool b)

	void Init(const CCore *Core);
	void DeInit(void);
	void Update(void);	
	void Render(void);

	CVector3 Position, Speed, Rotation, Torque, RocketPosition, RocketSpeed;
int EngineSoundChannel, RocketSoundChannel;
FSOUND_SAMPLE *EngineSound, *RocketSound;
protected:
	bool RocketLaunched;
	//CVector3 Position, Rotation, RocketPosition, RocketSpeed;
	CMatrix Transform;
	CMesh *Mesh, *RocketMesh;
	CParticleSystem *EngineSmoke, *RocketSmoke;
	CFlareEffect *EngineFlare, *RocketFlare;
	
	

private:
	CCore *Core;

};	//	class CPlayerShip;

class CAsteroid {
public:
	inline CAsteroid(void) {}
	inline ~CAsteroid(void) {}

	inline CVector3 &GetPosition(void) { return Position; }
	inline CVector3 &GetRotation(void) { return Rotation; }
	inline CMatrix &GetTransform(void) { return Transform; }
	inline CMesh *GetMesh(void) { return Mesh; }
	inline bool IsDestroyed(void) { return Destroyed; }
	void SetDestroyed(const bool b);

	void Init(const CCore *Core, const CMesh *Mesh);
	void DeInit(void);
	void Update(void);
	void Render(void);

protected:
	bool Destroyed;
	float TimeCounter;
	CVector3 Position, Rotation, Torque;
	CMatrix Transform;
	CMesh *Mesh;

	CVector3 DPosition[8], DSpeed[8], DRotation[8], DTorque[8];
	CMatrix DTransform[8];

private:
	CCore *Core;

};	//	class CAsteroid;

inline bool SpheresCollision(CVector3 s1p, const float s1r, CVector3 s2p,
	const float s2r) {
	float dist = (float)sqrt((s1p.x - s2p.x) * (s1p.x - s2p.x) + 
							 (s1p.y - s2p.y) * (s1p.y - s2p.y) + 
							 (s1p.z - s2p.z) * (s1p.z - s2p.z));

	if (dist <= s1r + s2r) return true; 
	return false;
}	//	inline bool SpheresCollision(...)