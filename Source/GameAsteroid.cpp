#include "PreCompiled.h"

static CVector3 PositionMin(-600.0f, -600.0f, -5500.0f), PositionMax(600.0f, 600.0f, -3500.0f);
static CVector3 TorqueMin(-100.0f), TorqueMax(100.0f);

void CAsteroid::Init(const CCore *_Core, const CMesh *_Mesh) {
	this->Core = (CCore *)_Core;
	this->Mesh = (CMesh *)_Mesh;
	Destroyed = false;

	Position =
		CVector3(((float)rand()/(float)RAND_MAX)*(PositionMax.x-PositionMin.x)+PositionMin.x,
				 ((float)rand()/(float)RAND_MAX)*(PositionMax.y-PositionMin.y)+PositionMin.y,
				 ((float)rand()/(float)RAND_MAX)*(PositionMax.z-PositionMin.z)+PositionMin.z);
	Rotation =
		CVector3(((float)rand()/(float)RAND_MAX)*360.0f,
				 ((float)rand()/(float)RAND_MAX)*360.0f,
				 ((float)rand()/(float)RAND_MAX)*360.0f);
	Torque =
		CVector3(((float)rand()/(float)RAND_MAX)*(TorqueMax.x-TorqueMin.x)+TorqueMin.x,
				 ((float)rand()/(float)RAND_MAX)*(TorqueMax.y-TorqueMin.y)+TorqueMin.y,
				 ((float)rand()/(float)RAND_MAX)*(TorqueMax.z-TorqueMin.z)+TorqueMin.z);

}	//	void CAsteroid::Init(const CCore *Core, const CMesh *Mesh)

void CAsteroid::DeInit(void) {
}	//	void CAsteroid::DeInit(void)

void CAsteroid::SetDestroyed(const bool b) {
	Destroyed = b;

	if (b) {
	for (int i=0;  i<8;  i++) {
		DPosition[i] = Position;

		DSpeed[i] =
			CVector3(((float)rand()/(float)RAND_MAX)*(125.0f+125.0f)-125.0f,
					 ((float)rand()/(float)RAND_MAX)*(125.0f+125.0f)-125.0f,
					 ((float)rand()/(float)RAND_MAX)*(125.0f+125.0f)-125.0f+600.0f);
		DRotation[i] =
			CVector3(((float)rand()/(float)RAND_MAX)*360.0f,
					 ((float)rand()/(float)RAND_MAX)*360.0f,
					 ((float)rand()/(float)RAND_MAX)*360.0f);
		DTorque[i] =
			CVector3(((float)rand()/(float)RAND_MAX)*(TorqueMax.x-TorqueMin.x)+TorqueMin.x,
					 ((float)rand()/(float)RAND_MAX)*(TorqueMax.y-TorqueMin.y)+TorqueMin.y,
					 ((float)rand()/(float)RAND_MAX)*(TorqueMax.z-TorqueMin.z)+TorqueMin.z);
	}	//	for (int i=0;  i<8;  i++)
	
	TimeCounter = 0.0f;
	}
}	//	void CAsteroid::SetDestroyed(const bool b)

void CAsteroid::Update(void) {
	const float dt = Core->GetTimer()->GetDeltaTime();

	if (Destroyed) {
		for (int i=0;  i<8;  i++) {
			DPosition[i] += DSpeed[i] * dt;
			DRotation[i] += DTorque[i] * dt;
			DTransform[i] =
				Translate(DPosition[i]) * RotateX(DRotation[i].x) * RotateY(DRotation[i].y) * RotateZ(DRotation[i].z);
		}	//	for (int i=0;  i<8;  i++)

		TimeCounter += dt;

	if (TimeCounter >= 3.0f) {
	TimeCounter = 0.0f;
	Destroyed = false;
	Position =
		CVector3(((float)rand()/(float)RAND_MAX)*(PositionMax.x-PositionMin.x)+PositionMin.x,
				 ((float)rand()/(float)RAND_MAX)*(PositionMax.y-PositionMin.y)+PositionMin.y,
				 ((float)rand()/(float)RAND_MAX)*(PositionMax.z-PositionMin.z)+PositionMin.z);
	Rotation =
		CVector3(((float)rand()/(float)RAND_MAX)*360.0f,
				 ((float)rand()/(float)RAND_MAX)*360.0f,
				 ((float)rand()/(float)RAND_MAX)*360.0f);
	Torque =
		CVector3(((float)rand()/(float)RAND_MAX)*(TorqueMax.x-TorqueMin.x)+TorqueMin.x,
				 ((float)rand()/(float)RAND_MAX)*(TorqueMax.y-TorqueMin.y)+TorqueMin.y,
				 ((float)rand()/(float)RAND_MAX)*(TorqueMax.z-TorqueMin.z)+TorqueMin.z);
	}

		return;
	}	//	if (Destroyed)


	Position.z += 900.0f * dt;
	Rotation += Torque * dt;

	if (Position.z > 50.0f) {
	Position =
		CVector3(((float)rand()/(float)RAND_MAX)*(PositionMax.x-PositionMin.x)+PositionMin.x,
				 ((float)rand()/(float)RAND_MAX)*(PositionMax.y-PositionMin.y)+PositionMin.y,
				 ((float)rand()/(float)RAND_MAX)*(PositionMax.z-PositionMin.z)+PositionMin.z);
	Rotation =
		CVector3(((float)rand()/(float)RAND_MAX)*360.0f,
				 ((float)rand()/(float)RAND_MAX)*360.0f,
				 ((float)rand()/(float)RAND_MAX)*360.0f);
	Torque =
		CVector3(((float)rand()/(float)RAND_MAX)*(TorqueMax.x-TorqueMin.x)+TorqueMin.x,
				 ((float)rand()/(float)RAND_MAX)*(TorqueMax.y-TorqueMin.y)+TorqueMin.y,
				 ((float)rand()/(float)RAND_MAX)*(TorqueMax.z-TorqueMin.z)+TorqueMin.z);
	}	//		if (Asteroids[i]->Position.z > 500.0f)

	Transform = Translate(Position) * RotateX(Rotation.x) * RotateY(Rotation.y) * RotateZ(Rotation.z);
}	//	void CAsteroid::Update(void)

void CAsteroid::Render(void) {
	if (Destroyed) {
		for (int i=0;  i<8;  i++) {
			Core->GetRender()->RenderMesh(Mesh, DTransform[i]*Scale(CVector3(0.35f)));
		}	//	for (int i=0;  i<8;  i++)

		return;
	}	//	if (Destroyed)

	Core->GetRender()->RenderMesh(Mesh, Transform);
}	//	void CAsteroid::Render(void)