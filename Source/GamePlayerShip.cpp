#include "PreCompiled.h"

void CPlayerShip::Init(const CCore *_Core) {
	this->Core = (CCore *)_Core;
	RocketLaunched = false;
/*
	Mesh = Core->CreateMesh("Ship.mangal", "Ship");
	EngineSmoke = Core->CreateParticleSystem(true, 35, 2, 0.035f, 25.0f, 1.85f, 1.9f,
		CVector3(), CVector3(), CVector3(), CVector3(-25.0f, -25.0f, 200.0f),
		CVector3(25.0f, 25.0f, 225.0f), CVector3(), CVector3(), "Particle01.jpg");
	EngineFlare = Core->CreateFlareEffect(true, 1000.0f, 50.0f, CVector3(), "Flare02.jpg");
	EngineSmoke->Active = true;
	EngineFlare->Active = true;
*/
	RocketMesh = Core->CreateMesh("Rocket.mangal", "Rocket");
	RocketSmoke = Core->CreateParticleSystem(false, 40, 2, 0.035f, 30.0f, 1.5f, 1.65f,
		CVector3(), CVector3(), CVector3(), CVector3(-50.0f, -50.0f, -50.0f),
		CVector3(50.0f, 50.0f, 50.0f), CVector3(), CVector3(), "Particle01.jpg");
	RocketSmoke->Active = false;
	RocketFlare = Core->CreateFlareEffect(false, 1000.0f, 100.0f, CVector3(), "Flare01.jpg");
	RocketFlare->Active = false;

	EngineSound = FSOUND_Sample_Load(FSOUND_FREE, "Data\\Sounds\\Engine.wav", FSOUND_HW2D, 0, 0);
	RocketSound = FSOUND_Sample_Load(FSOUND_FREE, "Data\\Sounds\\Rocket.wav", FSOUND_HW2D, 0, 0);
	EngineSoundChannel = FSOUND_PlaySound(FSOUND_FREE, EngineSound);
}	//	void CPlayerShip::Init(...)

void CPlayerShip::DeInit(void) {
}	//	void CPlayerShip::DeInit(void)

void CPlayerShip::Update(void) {
	const float dt = Core->GetTimer()->GetDeltaTime();

	if (Core->GetInput()->GetKey(DIK_Z)) Torque.z += dt * 100.0f;
	if (Core->GetInput()->GetKey(DIK_X)) Torque.z -= dt * 100.0f;
	//Position.x += Core->GetInput()->GetMousePositionX() * 0.45f;
	//Position.y -= Core->GetInput()->GetMousePositionY() * 0.45f;

	if (Core->GetInput()->GetKey(DIK_UP)) {
		Speed.x += dt * 750.0f * cosf(DegToRad(Rotation.z+90.0f));
		Speed.y += dt * 750.0f * sinf(DegToRad(Rotation.z+90.0f));
	}
	if (Core->GetInput()->GetKey(DIK_DOWN)) {
		Speed.x += dt * 750.0f * cosf(DegToRad(Rotation.z-90.0f));
		Speed.y += dt * 750.0f * sinf(DegToRad(Rotation.z-90.0f));
	}
	if (Core->GetInput()->GetKey(DIK_LEFT)) {
		Speed.x += dt * 750.0f * cosf(DegToRad(Rotation.z+180.0f));
		Speed.y += dt * 750.0f * sinf(DegToRad(Rotation.z+180.0f));
	}
	if (Core->GetInput()->GetKey(DIK_RIGHT)) {
		Speed.x += dt * 750.0f * cosf(DegToRad(Rotation.z));
		Speed.y += dt * 750.0f * sinf(DegToRad(Rotation.z));
	}

	Rotation += Torque * dt;
	Position += Speed * dt;

	if (Torque.z > 5.0f) Torque.z -= dt * 25.0f;
	if (Torque.z < -5.0f) Torque.z += dt * 25.0f;
	if (Torque.z > 125.0f) Torque.z = 125.0f;
	if (Torque.z < -125.0f) Torque.z = -125.0f;

	if (Speed.x > 0.05f) Speed.x -= dt * 150.0f;
	if (Speed.x < 0.05f) Speed.x += dt * 150.0f;
	if (Speed.y > 0.05f) Speed.y -= dt * 150.0f;
	if (Speed.y < 0.05f) Speed.y += dt * 150.0f;

/*
	if (Core->GetInput()->GetKey(DIK_Z)) Rotation.z += dt * 35.0f;
	if (Core->GetInput()->GetKey(DIK_X)) Rotation.z -= dt * 35.0f;

	if (Core->GetInput()->GetKey(DIK_UP)) {
		Position.x += dt * 300.0f * cosf(DegToRad(Rotation.z+90.0f));
		Position.y += dt * 300.0f * sinf(DegToRad(Rotation.z+90.0f));
	}
	if (Core->GetInput()->GetKey(DIK_DOWN)) {
		Position.x += dt * 300.0f * cosf(DegToRad(Rotation.z-90.0f));
		Position.y += dt * 300.0f * sinf(DegToRad(Rotation.z-90.0f));
	}
	if (Core->GetInput()->GetKey(DIK_LEFT)) {
		Position.x += dt * 300.0f * cosf(DegToRad(Rotation.z+180.0f));
		Position.y += dt * 300.0f * sinf(DegToRad(Rotation.z+180.0f));
	}
	if (Core->GetInput()->GetKey(DIK_RIGHT)) {
		Position.x += dt * 300.0f * cosf(DegToRad(Rotation.z));
		Position.y += dt * 300.0f * sinf(DegToRad(Rotation.z));
	}
*/
/*
	Rotation.z = Position.x / 40.0f;
	Position.x += Core->GetInput()->GetMousePositionX() * 0.75f;
	Position.y -= Core->GetInput()->GetMousePositionY() * 0.75f;
*/
	if (Position.x > 460.0f) { Speed.x = 0.0f; Position.x = 460.0f; }
	if (Position.x < -460.0f) { Speed.x = 0.0f; Position.x = -460.0f; }
	if (Position.y > 460.0f) { Speed.y = 0.0f; Position.y = 460.0f; }
	if (Position.y < -460.0f) { Speed.y = 0.0f; Position.y = -460.0f; }

//	SetCursorPos(800, 600);

	if (Core->GetInput()->GetKey(DIK_SPACE) && !RocketLaunched) {
		RocketLaunched = true;
		RocketPosition = Position;
		RocketSpeed = CVector3(0.0f);
		RocketSmoke->Active = RocketFlare->Active = true;
		RocketSoundChannel = FSOUND_PlaySound(FSOUND_FREE, RocketSound);
	}	//	if (Core->GetInput()->GetKey(DIK_SPACE))
	if (RocketLaunched) {
		RocketSpeed.z += 3500.0f * dt;
		RocketPosition -= RocketSpeed * dt;
		RocketSmoke->Position = RocketFlare->Position = RocketPosition;

		if (RocketPosition.z < -9000.0f) {
			RocketLaunched = false;
			RocketSmoke->Active = RocketFlare->Active = false;
			FSOUND_StopSound(RocketSoundChannel);
			RocketSoundChannel = 0;
			RocketPosition = CVector3(0.0f);
		}	//	if (RocketPosition.z > 10000.0f)
	}	//	if (RocketLaunched)

//	EngineSmoke->Position = Position + CVector3(0.0f, 0.0f, 65.0f);
//	EngineFlare->Position = Position + CVector3(0.0f, 0.0f, 65.0f);
	Transform = Translate(Position) * RotateX(Rotation.x) * RotateY(Rotation.y) * RotateZ(Rotation.z);
}	//	void CPlayerShip::Update(void)

void CPlayerShip::Render(void) {
	//Core->GetRender()->RenderMesh(Mesh, Transform);
	if (RocketLaunched)
		Core->GetRender()->RenderMesh(RocketMesh,
		Translate(RocketPosition+CVector3(0.0f, 0.0f, -60.0f)));

	Core->GetRender()->BeginRenderSprites();
	//Core->GetRender()->RenderParticleSystem(EngineSmoke);
	Core->GetRender()->RenderParticleSystem(RocketSmoke);
	Core->GetRender()->EndRenderSprites();

	Core->GetRender()->BeginRenderSprites();
	//Core->GetRender()->RenderFlareEffect(EngineFlare);
	Core->GetRender()->RenderFlareEffect(RocketFlare);
	Core->GetRender()->EndRenderSprites();
}	//	void CPlayerShip::Render(void)