#include "PreCompiled.h"

static CCore *Core = 0;
static float w = 0.0f, h = 0.0f;
static CFont *Font = 0;
static CFlareEffect *Star = 0, *Star2 = 0;
static CParticleSystem *Fog = 0;
static CPlayerShip PlayerShip;
static CTexture *HUD = 0, *CH = 0;
static std::vector<CAsteroid *> Asteroids;
static float Health = 100.0f, LevelProgress = 0.0f, Scores = 0.0f;
static FSOUND_SAMPLE *RocketTouchedAsteroid, *Thud;
static int ThudChannel;
static FMUSIC_MODULE *Music;
static std::vector<CVector3> Stars;
static CMesh *AsteroidMesh[3];

static bool InMenu = true;
static int CursorPos = 1;
static FSOUND_SAMPLE *MenuTick;
static FMUSIC_MODULE *MenuMusic;
static CTexture *MenuTex;
static bool InGame = false;

static const int EndPause = 7000;

static void Menu(void) {
	if (FMUSIC_IsPlaying(MenuMusic) == FALSE)
		FMUSIC_PlaySong(MenuMusic);
	FMUSIC_SetPaused(MenuMusic, false);
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
unsigned long StartTicks = 0;
	while (true) {
		StartTicks = GetTickCount();

		Core->GetInput()->Update();

		if (Core->GetInput()->GetKey(DIK_UP)) { 
			FSOUND_PlaySound(FSOUND_FREE, MenuTick);
			CursorPos--; 
			Core->GetInput()->SetKey(DIK_UP, false); 
			Sleep(200);
		}
		if (Core->GetInput()->GetKey(DIK_DOWN)) {
			FSOUND_PlaySound(FSOUND_FREE, MenuTick);
			CursorPos++; 
			Core->GetInput()->SetKey(DIK_DOWN, false); 
			Sleep(200);
		}
		if (Core->GetInput()->GetKey(DIK_RETURN)) {
			FSOUND_PlaySound(FSOUND_FREE, MenuTick);
			Sleep(200);
			if (CursorPos == 1) {
				Core->GetRender()->BeginScene();
				Core->GetRender()->SetMode(CRender::RM_2D);
				Core->GetRender()->BeginRenderSprites();
				if (!InGame)
				Core->GetRender()->WriteText(Font, CVector2(160.0f, h-260.0f),
					CVector3(1.0f), "Загрузка... Подождите пожалуйста!");
				Core->GetRender()->EndRenderSprites();
				Core->GetRender()->EndScene();
				if (!InGame)
				Sleep(2000);
				FMUSIC_SetPaused(MenuMusic, true);
				InGame = true;
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				Core->GetTimer()->Update();
				return;
			}
			if (CursorPos == 3) {
				Core->GetRender()->BeginScene();
				Core->GetRender()->SetMode(CRender::RM_2D);
				Core->GetRender()->BeginRenderSprites();
				Core->GetRender()->WriteText(Font, CVector2(300.0f, h-260.0f),
					CVector3(1.0f), "Пока!!! :)");
				Core->GetRender()->WriteText(Font, CVector2(180.0f, h-290.0f),
					CVector3(0.0f, 1.0f, 0.0f), "Asteroid Fields 3 - будет жив!");
				Core->GetRender()->WriteText(Font, CVector2(210.0f, h-390.0f),
					CVector3(1.0f, 0.0f, 1.0f), "От resurected_timofei'а =)");
				Core->GetRender()->EndRenderSprites();
				Core->GetRender()->EndScene();
				Sleep(EndPause);
				exit(1);
			}

			if (CursorPos == 2) {
				Core->GetRender()->BeginScene();
				Core->GetRender()->SetMode(CRender::RM_2D);
				Core->GetRender()->BeginRenderSprites();

Core->GetRender()->WriteText(Font, CVector2(75.0f,  h-70.0f), CVector3(1.0f, 0.0f, 1.0f), "Информация об игре Asteroid Fields 2 (v1.1).");
Core->GetRender()->WriteText(Font, CVector2(45.0f, h-140.0f), CVector3(1.0f), "Игра Asteroid Fields 2 была создана Рузиевым");
Core->GetRender()->WriteText(Font, CVector2(45.0f, h-170.0f), CVector3(1.0f), "Тимуром aka resurected_timofei, ведущим про-");
Core->GetRender()->WriteText(Font, CVector2(45.0f, h-200.0f), CVector3(1.0f), "граммистом команды Mangal Development.");

Core->GetRender()->WriteText(Font, CVector2(45.0f, h-270.0f), CVector3(1.0f), "Основная цель игры состоит в прохождении вс-");
Core->GetRender()->WriteText(Font, CVector2(45.0f, h-300.0f), CVector3(1.0f), "его астероидного поля на вашем пути, облетая");
Core->GetRender()->WriteText(Font, CVector2(45.0f, h-330.0f), CVector3(1.0f), "астероиды или уничтожая их.");

Core->GetRender()->WriteText(Font, CVector2(45.0f, h-400.0f), CVector3(1.0f), "Управление:");
Core->GetRender()->WriteText(Font, CVector2(45.0f, h-430.0f), CVector3(1.0f), "Z, X     -  управление повёрнутостью корабля");
Core->GetRender()->WriteText(Font, CVector2(45.0f, h-460.0f), CVector3(1.0f), "Стрелки  -  управление кораблём");
Core->GetRender()->WriteText(Font, CVector2(45.0f, h-490.0f), CVector3(1.0f), "Пробел   -  огонь ракетой!");

Core->GetRender()->WriteText(Font, CVector2(45.0f, h-560.0f), CVector3(0.0f, 1.0f, 0.0f), "Желаю успехов в деле уничтожения астероидов! :)");

				Core->GetRender()->EndRenderSprites();

				Core->GetRender()->EndScene();

				Core->GetInput()->Update();
				while (!Core->GetInput()->GetKey(DIK_ESCAPE)) {
					Sleep(20);
					Core->GetInput()->Update();
				}
			}
		}

		if (CursorPos < 1) CursorPos = 3;
		if (CursorPos > 3) CursorPos = 1;

		Core->GetRender()->BeginScene();
		Core->GetRender()->SetMode(CRender::RM_2D);
		Core->GetRender()->BeginRenderSprites();

		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, MenuTex->ID);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f,1.0f); glVertex2f(w, h);
		glTexCoord2f(0.0f,1.0f); glVertex2f(0.0f, h);
		glTexCoord2f(0.0f,0.0f); glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f,0.0f); glVertex2f(w, 0.0f);
		glEnd();

		Core->GetRender()->EndRenderSprites();

		Core->GetRender()->BeginRenderSprites();

		Core->GetRender()->WriteText(Font, CVector2(310.0f, h-360.0f),
			CVector3(1.0f, 1.0f, 0.0f), "Главное меню");
		Core->GetRender()->WriteText(Font, CVector2(180.0f, h-430.0f-CursorPos*30.0f),
			CVector3(1.0f, 1.0f, 0.0f), "--->                     <---");
		if (!InGame)
		Core->GetRender()->WriteText(Font, CVector2(270.0f, h-460.0f),
			CVector3(1.0f), "Начать новую игру");
		else
		Core->GetRender()->WriteText(Font, CVector2(270.0f, h-460.0f),
			CVector3(1.0f), "Продолжить игру");
		Core->GetRender()->WriteText(Font, CVector2(270.0f, h-490.0f),
			CVector3(1.0f), "Информация об игре");
		Core->GetRender()->WriteText(Font, CVector2(270.0f, h-520.0f),
			CVector3(1.0f), "Выйти из игры");

		Core->GetRender()->EndRenderSprites();
		Core->GetRender()->EndScene();

			while ((GetTickCount() - StartTicks) < 20);
	}	//	while (true)
}	//	static void Menu(void)

static bool Init(void) {
	srand(timeGetTime());

	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
	//FSOUND_SetOutput(FSOUND_OUTPUT_NOSOUND);
	FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
	FSOUND_Init(44100, 32, 0);
	FSOUND_SetSFXMasterVolume(150);

	ShowCursor(FALSE);
	w = (float)Core->GetWindowWidth();
	h = (float)Core->GetWindowHeight();

	Font = Core->CreateFont("LucidaConsoleBold.bmp", 30.0f, 15.0f);

	Core->GetRender()->SetProjection2D(Ortho(0.0f, w, 0.0f, h, -1.0f, 1.0f));
	Core->GetRender()->SetModelView2D(CMatrix());
	Core->GetRender()->SetProjection3D(Perspective(60.0f, w/h, 20.0f, 10000.0f));
	Core->GetRender()->SetModelView3D(Translate(CVector3(0.0f, 0.0f, -550.0f)));

	MenuTick = FSOUND_Sample_Load(FSOUND_FREE, "Data\\Sounds\\Tick.wav", FSOUND_HW2D, 0, 0);
	MenuMusic = FMUSIC_LoadSong("Data\\Musics\\IGELKOTT.MOD");
	FMUSIC_PlaySong(MenuMusic);
	FMUSIC_SetMasterVolume(MenuMusic, 50);
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	//	============================================================================
	//	Mangal Development logo at startup
	CTexture *MangalLogo = Core->CreateTexture("Mangal.jpg");

	Core->GetRender()->BeginScene();
	Core->GetRender()->SetMode(CRender::RM_2D);
	Core->GetRender()->BeginRenderSprites();

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, MangalLogo->ID);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f,1.0f); glVertex2f(w, h);
	glTexCoord2f(0.0f,1.0f); glVertex2f(0.0f, h);
	glTexCoord2f(0.0f,0.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f,0.0f); glVertex2f(w, 0.0f);
	glEnd();

	Core->GetRender()->EndRenderSprites();
	Core->GetRender()->EndScene();
	Sleep(7000);
	//	Mangal Development logo at startup
	//	============================================================================
	//	============================================================================
	//	Asteroid Fields 2 logo at startup
	CTexture *AF2Logo = Core->CreateTexture("AF2.jpg");

	Core->GetRender()->BeginScene();
	Core->GetRender()->SetMode(CRender::RM_2D);
	Core->GetRender()->BeginRenderSprites();

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, AF2Logo->ID);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f,1.0f); glVertex2f(w, h);
	glTexCoord2f(0.0f,1.0f); glVertex2f(0.0f, h);
	glTexCoord2f(0.0f,0.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f,0.0f); glVertex2f(w, 0.0f);
	glEnd();

	Core->GetRender()->EndRenderSprites();
	Core->GetRender()->EndScene();
	Sleep(7000);
	//	Asteroid Fields 2 logo at startup
	//	============================================================================


	FMUSIC_SetLooping(MenuMusic, true);
	MenuTex = Core->CreateTexture("Menu.tga", true);

	Menu();

	PlayerShip.Init(Core);

	Fog = Core->CreateParticleSystem(true, 275, 1, 0.025f, 75.0f, 0.1f, 0.12f,
		CVector3(0.0f, 0.0f, -2000.0f), CVector3(-600.0f),
		CVector3(600.0f), CVector3(0.0f, 0.0f, 350.0f), CVector3(0.0f, 0.0f, 400.0f),
		CVector3(), CVector3(), "Particle02.jpg");

	HUD = Core->CreateTexture("HUD.tga", true);
	CH = Core->CreateTexture("CrossHair.tga", true);


	AsteroidMesh[0] = Core->CreateMesh("Asteroid01.mangal", "Asteroid");
	AsteroidMesh[1] = Core->CreateMesh("Asteroid02.mangal", "Asteroid");
	AsteroidMesh[2] = Core->CreateMesh("Asteroid03.mangal", "Asteroid");
	Asteroids.resize(45);
	for (int i=0;  i<(int)Asteroids.size();  i++) {
		Asteroids[i] = new CAsteroid;
		Asteroids[i]->Init(Core, AsteroidMesh[rand()%3]);
	}	//	for (int i=0;  i<(int)Asteroids.size();  i++)

	RocketTouchedAsteroid = FSOUND_Sample_Load(FSOUND_FREE, "Data\\Sounds\\Explode.wav", FSOUND_HW2D, 0, 0);
	Thud = FSOUND_Sample_Load(FSOUND_FREE, "Data\\Sounds\\Thud.wav", FSOUND_HW2D, 0, 0);
	ThudChannel = FSOUND_PlaySound(FSOUND_FREE, Thud);

	Music = FMUSIC_LoadSong("Data\\Musics\\commando2.mod");
	FMUSIC_SetLooping(Music, true);
	FMUSIC_PlaySong(Music);
	FMUSIC_SetMasterVolume(Music, 100);

	Star = Core->CreateFlareEffect(false, 0.0f, 2000.0f, CVector3(3000.0f, 3000.0f, -7500.0f), "Flare01.jpg");
	Star2 = Core->CreateFlareEffect(false, 0.0f, 1200.0f, CVector3(-3000.0f, 0.0f, -7500.0f), "Flare01.jpg");

	Stars.resize(350);
	for (int i=0;  i<(int)Stars.size();  i++)
		Stars[i] = Random(CVector3(-6000.0f, -6000.0f, -8000.0f), CVector3(6000.0f, 6000.0f, -7000.0f));
	glPointSize(1.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glClearColor(0.55f, 0.05f, 0.05f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	FMUSIC_PlaySong(Music);
	FMUSIC_SetPaused(Music, false);

	return true;
}	//	static bool Init(void)

static void DeInit(void) {
	FSOUND_Close();
}	//	static void Init(void)

static void Update(void) {
	FSOUND_Update();
	if (Core->GetInput()->GetKey(DIK_ESCAPE)) {
		FMUSIC_SetPaused(Music, true);
		FSOUND_StopSound(PlayerShip.EngineSoundChannel);
		Menu();
		PlayerShip.EngineSoundChannel = FSOUND_PlaySound(FSOUND_FREE, PlayerShip.EngineSound);
		FMUSIC_SetPaused(Music, false);
		//Core->StopMainLoop();
	}

	PlayerShip.Update();
	for (int i=0;  i<(int)Asteroids.size();  i++) {
		if (!Asteroids[i]->IsDestroyed())
		if (SpheresCollision(Asteroids[i]->GetPosition(), 75.0f,
			PlayerShip.GetRocketPosition()+CVector3(0.0f, 0.0f, -25.0f), 25.0f)) {
				PlayerShip.SetRocketLaunched(false);
				Asteroids[i]->SetDestroyed(true);
				FSOUND_PlaySound(FSOUND_FREE, RocketTouchedAsteroid);
				Scores += 100.0f;
			}	//	if (SpheresCollision(...))

		Asteroids[i]->Update();

		if (Asteroids[i]->GetPosition().z > -150.0f)
			if (SpheresCollision(Asteroids[i]->GetPosition(), 70.0f,
				PlayerShip.GetPosition(), 30.0f)) {
				PlayerShip.Position =
					PlayerShip.Position - (Asteroids[i]->GetPosition() - PlayerShip.Position)
					* Core->GetTimer()->GetDeltaTime() * 7.5f;
				PlayerShip.Position.z = 0;
				Health -= Core->GetTimer()->GetDeltaTime() * 70.0f;

				if (FSOUND_IsPlaying(ThudChannel) == FALSE)
					ThudChannel = FSOUND_PlaySound(FSOUND_FREE, Thud);
			}
	}	//	for (int i=0;  i<(int)Asteroids.size();  i++)
	
	LevelProgress += Core->GetTimer()->GetDeltaTime() * 0.375f;
	Scores += Core->GetTimer()->GetDeltaTime() * 6.65f;

	if (LevelProgress >= 100.0f) {
		MessageBox(0, "Вы выиграли!\nВы прошли всё астероидное поле!", "Asteroid Fields 2",
			MB_ICONQUESTION | MB_OK | MB_SYSTEMMODAL);
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
				Core->GetRender()->BeginScene();
				Core->GetRender()->SetMode(CRender::RM_2D);
				Core->GetRender()->BeginRenderSprites();
				Core->GetRender()->WriteText(Font, CVector2(300.0f, h-260.0f),
					CVector3(1.0f), "Пока!!! :)");
				Core->GetRender()->WriteText(Font, CVector2(180.0f, h-290.0f),
					CVector3(0.0f, 1.0f, 0.0f), "Asteroid Fields 3 - будет жив!");
				Core->GetRender()->WriteText(Font, CVector2(210.0f, h-390.0f),
					CVector3(1.0f, 0.0f, 1.0f), "От resurected_timofei'а =)");
				Core->GetRender()->EndRenderSprites();
				Core->GetRender()->EndScene();
				Sleep(EndPause);
				exit(1);
	}
	if (Health <= 0.0f) {
		MessageBox(0, "Вы проиграли!\nВашему кораблю нанесён максимальный ущерб!",
			"Asteroid Fields 2",
			MB_ICONQUESTION | MB_OK | MB_SYSTEMMODAL);
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
				Core->GetRender()->BeginScene();
				Core->GetRender()->SetMode(CRender::RM_2D);
				Core->GetRender()->BeginRenderSprites();
				Core->GetRender()->WriteText(Font, CVector2(300.0f, h-260.0f),
					CVector3(1.0f), "Пока!!! :)");
				Core->GetRender()->WriteText(Font, CVector2(180.0f, h-290.0f),
					CVector3(0.0f, 1.0f, 0.0f), "Asteroid Fields 3 - будет жив!");
				Core->GetRender()->WriteText(Font, CVector2(210.0f, h-390.0f),
					CVector3(1.0f, 0.0f, 1.0f), "От resurected_timofei'а =)");
				Core->GetRender()->EndRenderSprites();
				Core->GetRender()->EndScene();
				Sleep(EndPause);
				exit(1);
	}


	static float AstTimer;
	AstTimer += Core->GetTimer()->GetDeltaTime();
	if (AstTimer >= 1.75f) {
		CAsteroid *Asteroid = new CAsteroid;
		Asteroid->Init(Core, AsteroidMesh[rand()%3]);

		Asteroids.push_back(Asteroid);
		AstTimer = 0.0f;
	}
}	//	static void Update(void)

static void Draw(void) {
	//Core->GetRender()->SetModelView3D(Translate(CVector3(0.0f, 0.0f, -500.0f)) * RotateX(CamAngleX) * RotateY(CamAngleY));
	Core->GetRender()->SetModelView3D(Inverse(PlayerShip.GetTransform().m));
	Core->GetRender()->SetMode(CRender::RM_3D);

	float LightPos[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	for (int i=0;  i<(int)Asteroids.size();  i++) Asteroids[i]->Render();
	PlayerShip.Render();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glBegin(GL_POINTS);
		for(int i=0;  i<(int)Stars.size();  i++)
		glVertex3fv(&Stars[i].x);
	glEnd();
	glPopAttrib();

	Core->GetRender()->BeginRenderSprites();
	Core->GetRender()->RenderParticleSystem(Fog);
	Core->GetRender()->EndRenderSprites();

	Core->GetRender()->BeginRenderSprites();
	Core->GetRender()->RenderFlareEffect(Star);
	Core->GetRender()->RenderFlareEffect(Star2);
	Core->GetRender()->EndRenderSprites();

//	----------------------------------------------------------------------------------
//	----------------------------------------------------------------------------------
	Core->GetRender()->BeginRenderSprites();
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, CH->ID);

	CMatrix ModelView3D = Core->GetRender()->GetModelView3D();

	CVector3 Right(ModelView3D.m[0], ModelView3D.m[4], ModelView3D.m[8]);
	CVector3 Up(ModelView3D.m[1], ModelView3D.m[5], ModelView3D.m[9]);

	CVector3 Coo[4];
	Coo[0] = (-Right-Up)*10.0f; Coo[1] = (Right-Up)*10.0f;
	Coo[2] = (Right+Up)*10.0f; Coo[3] = (-Right+Up)*10.0f;
	CVector3 Pos = PlayerShip.GetPosition() + CVector3(0.0f, 0.0f, -100.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f,1.0f); glVertex3f((Pos+Coo[0]).x, (Pos+Coo[0]).y, (Pos+Coo[0]).z);
	glTexCoord2f(0.0f,1.0f); glVertex3f((Pos+Coo[1]).x, (Pos+Coo[1]).y, (Pos+Coo[1]).z);
	glTexCoord2f(0.0f,0.0f); glVertex3f((Pos+Coo[2]).x, (Pos+Coo[2]).y, (Pos+Coo[2]).z);
	glTexCoord2f(1.0f,0.0f); glVertex3f((Pos+Coo[3]).x, (Pos+Coo[3]).y, (Pos+Coo[3]).z);
	glEnd();
	Core->GetRender()->EndRenderSprites();
//	----------------------------------------------------------------------------------
//	----------------------------------------------------------------------------------

	Core->GetRender()->SetMode(CRender::RM_2D);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_FOG);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 1.0f, 0.0f);
	glRectf(7.5f, 240.0f, 17.5f, 250.0f + LevelProgress * 2.0f);
	glRectf(w-17.5f, 240.0f, w-7.5f, 250.0f + Health * 2.0f);
	glColor3f(0.0f, 0.5f, 0.0f);
	glRectf(7.5f, 240.0f, 17.5f, 250.0f + 100.0f * 2.0f);
	glRectf(w-17.5f, 240.0f, w-7.5f, 250.0f + 100.0f * 2.0f);
	glPopAttrib();

	Core->GetRender()->BeginRenderSprites();

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, HUD->ID);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f,1.0f); glVertex2f(w, h);
	glTexCoord2f(0.0f,1.0f); glVertex2f(0.0f, h);
	glTexCoord2f(0.0f,0.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f,0.0f); glVertex2f(w, 0.0f);
	glEnd();

	Core->GetRender()->WriteText(Font, CVector2(w-270.0f, h-90.0f),
		CVector3(0.0f, 1.0f, 0.0f), "%i", Core->GetTimer()->GetUpdatesPerSecond());
	Core->GetRender()->WriteText(Font, CVector2(280.0f, h-40.0f),
		CVector3(0.0f, 1.0f, 0.0f), "Asteroid Fields 2");
	Core->GetRender()->WriteText(Font, CVector2(120.0f, 10.0f),
		CVector3(0.0f, 1.0f, 0.0f), "%i", int(LevelProgress));
	Core->GetRender()->WriteText(Font, CVector2(w-165.0f, 10.0f),
		CVector3(0.0f, 1.0f, 0.0f), "%i", int(Health));
	Core->GetRender()->WriteText(Font, CVector2(235.0f, h-90.0f),
		CVector3(0.0f, 1.0f, 0.0f), "%i", int(Scores));

	Core->GetRender()->EndRenderSprites();
}	//	static void Draw(void)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	int nShowCmd) {

	if (!(Core = new CCore)) return -1;
	if (!Core->Init(hInstance, "Settings.cfg", Init, DeInit, Update, Draw)) return -1;
	Core->MainLoop();
	if (Core) { Core->DeInit(); delete Core; Core = 0; }

	return 0;
}	//	int WINAPI WinMain(...)
