#include "Engine.h"
// Smart ItemS: newtons Cradle

Engine::Engine(HINSTANCE hInstance)
	: D3DApp(hInstance),
	mSky(0),
	mFloorTexSRV(0),
	mWalkCamMode(false),
	mWireMode(false),
	mBFCull(false),
	testModel(0),
	spawnBugTime(0),
	spawnMushTime(0),
	speedBonusTime(0),
	mMoveSpeed(500),
	tickTimer(0.0f),
	bossTimer(0.0f),
	randBossTime(1.0f),
	bugsWorth(0.03f),
	dmgAmount(0.01f),
	difficultyTimer(1.0f),
	waitToClickTime(0),
	farPlane(30000.0f),
	fullyLoaded(false),
	toRandSpot(false),
	toCam(false),
	exitable(false)
{
	mMainWndCaption = L"Dude Wheres My IQ?";
	mEnable4xMsaa = false;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 150.0f, -400.0f);
	mCam.Pitch(XM_PI / 6.5);


	mDirLights[0].Ambient  = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirLights[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

	mDirLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);



	mDirLights2[0].Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirLights2[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLights2[0].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights2[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.707f);
			  
	mDirLights2[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights2[1].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights2[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights2[1].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
			  
	mDirLights2[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights2[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights2[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights2[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);



	mDirLights3[0].Ambient = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	mDirLights3[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLights3[0].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights3[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.707f);
			  
	mDirLights3[1].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights3[1].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights3[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights3[1].Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
			
	mDirLights3[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights3[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights3[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights3[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);



	mShadowMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mShadowMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	mShadowMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);


	mOrthoWorld = XMMatrixOrthographicLH(mClientWidth, mClientHeight, -1000.0f, 1000.0f);
}

Engine::~Engine()
{
	if (md3dImmediateContext){ md3dImmediateContext->ClearState(); }
	
// 	ReleaseCOM(mRandomTexSRV);
// 	ReleaseCOM(mFlareTexSRV);
// 	ReleaseCOM(mRainTexSRV);

	SafeDelete(testModel);
	SafeDelete(mSky);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

bool Engine::Init()
{
	if(!D3DApp::Init())	return false;
	mSound.Initialize();
	Effects::InitAll(md3dDevice);	// Must init Effects first since InputLayouts depend on shader signatures.
	mTexMgr.Init(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);

	MathHelper::RandF(); //SEEDING RANDGEN

	Text::Init(&md3dDevice);
	LevelSection::Init(&md3dDevice);

	InitAll();

	mSky = new Sky(md3dDevice, L"Textures/ArstaBridge.dds", 5000.0f);

	//BUILD PLAYER
	mPlayer = new Player(&md3dDevice);
	std::vector<Entity*> tempVec; tempVec.push_back(mPlayer->mSelf);
	BuildVertexAndIndexBuffer(&mPlayer->mVB, &mPlayer->mIB, tempVec);
	mPlayer->mSelf->mUseAAB = true;
	mPlayer->InsertCollisionItems(mLevel[0]->mEntities); //INSERT LEVEL

	*StateMachine::pGameState	= GameState::MAINMENU;
	*StateMachine::pSoundState	= SoundState::SOUNDON;
	*StateMachine::pMusicState	= MusicState::MUSICON;
	*StateMachine::pGameMode	= GameMode::EASY;

	//mCam.SetConstraints(-1200.0f, 1200.0f, -1000.0f, 1000.0f, -1200.0f, 1200.0f);

	return true;
}

void Engine::OnResize()
{
	D3DApp::OnResize();
	mOrthoWorld = XMMatrixOrthographicLH(mClientWidth, mClientHeight, -1000.0f, 1000.0f);
	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, farPlane);
}

void Engine::UpdateScene(float dt)
{
	KeyboardHandler(dt);
	CameraHandler();

	//SWITCH ON THE MODE THE GAME IS IN 
	switch (*StateMachine::pGameState)
	{
	case GameState::ABOUT:		
	case GameState::MAINMENU:	UpdateMainMenu(dt); break;
	case GameState::GAMEON:
	case GameState::PAUSED:
	case GameState::WIN:
	case GameState::LOSE:		UpdateGame(dt);		break;
	}
}
void Engine::UpdateMainMenu(float dt)
{
	for (int i = 0; i < mUI.size(); i++)
	{
		mUI[i]->Update(mCam, dt);
	}

	for (int i = 0; i < mTexts.size(); i++)
	{
		mTexts[i]->Update(mCam, dt);
	}

	if (*StateMachine::pMusicState == MusicState::MUSICON){ if  ( mSound.MusicPaused(1)){ mSound.PauseMusic(false,1); } }
	if (*StateMachine::pMusicState == MusicState::MUSICOFF){ if (!mSound.MusicPaused(1)){ mSound.PauseMusic(true,1); }}
}
void Engine::UpdateGame(float dt)
{
	//(*StateMachine::pGameState == GameState::GAMEON) ? mCursorOn = false : mCursorOn = true;


	for (int i = 0; i < mPaused.size(); i++)
	{
		mPaused[i]->Update(mCam, dt);
	}


	for (int i = 0; i < mLevel.size(); i++)
	{
		mLevel[i]->Update(mCam, dt);
		
	}

	mPlayer->Update(mCam, dt);
	CamFollowPlayer();

	
	//TIMER STUFF / SPAWN RATES   *Spawn Before Update Or Youll Get a Flicker Later On Of it Not Translated Yet*
	tickTimer += dt;
	if (tickTimer >= difficultyTimer)
	{
		if (*StateMachine::pGameState == GameState::GAMEON)
		{

			if (*StateMachine::pMusicState == MusicState::MUSICON){ if (!mSound.PlayingMusic(2)){ mSound.StreamMusic(2); }
			if (mSound.MusicPaused(2)){ mSound.PauseMusic(false,2); }}


		}

// 		if (*StateMachine::pGameState == GameState::BOSSFIGHT)
// 		{
// 
// 			if (*StateMachine::pMusicState == MusicState::MUSICON){ if (!mSound.PlayingMusic(3)){ mSound.StreamMusic(3); }
// 			if (mSound.MusicPaused(3)){ mSound.PauseMusic(false, 3); }
// 			}
// 
// 
// 			spawnBugTime++;
// 			if (spawnBugTime == 1){ SpawnGhost(); spawnBugTime = 0; }
// 		}
	
		if (speedBonusTime > 0){ speedBonusTime--;	mMoveSpeed = 1000;	if (speedBonusTime == 0){ mMoveSpeed = 500; } }

		if (waitToClickTime > 0){ waitToClickTime--; }
		

		tickTimer = 0.0f;
	}


// 	if (GetAsyncKeyState('T') & 0x8000)
// 		mInvader->Walk(100.0f*dt);
// 	if (GetAsyncKeyState('H') & 0x8000)
// 		mInvader->Yaw(dt);
// 	if (GetAsyncKeyState('F') & 0x8000)
// 		mInvader->Yaw(-dt);
// 	if (GetAsyncKeyState('J') & 0x8000)
// 		mInvader->Walk(-100.0f*dt);

}





//HELPERS
void Engine::CameraHandler()
{
	// 
	// Clamp camera to terrain surface in walk mode.
	//
	if (mWalkCamMode)
	{
		XMFLOAT3 camPos = mCam.GetPosition();
		mCam.SetPosition(camPos.x, 50.0f, camPos.z);
	}

	mCam.UpdateViewMatrix();
}
void Engine::ClearScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Engine::RestoreStates()
{
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	// restore default states.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
}
void Engine::ResetCamMainMenu()
{
	mCam.ResetCam();
	mCam.SetPosition(0.0f, 150.0f, -400.0f);
	mCam.Pitch(XM_PI / 6.5);
}
void Engine::ResetCamInGame()
{
	mCam.ResetCam();
	mCam.SetPosition(0.0f, 20.0f, -400.0f);
	//mCam.Pitch(XM_PI / 2);
	mWalkCamMode = true;
}
void Engine::IncProgress(float dt)
{
// 	(mCompBar->currProgress < 1.0) ? mCompBar->currProgress += dt / 100 : mCompBar->currProgress = 1.0;
// 	if (mCompBar->currProgress == 1.0)
// 	{ 
// 		*StateMachine::pGameState = GameState::WIN; waitToClickTime = 2;
// 		if (*StateMachine::pSoundState == SoundState::SOUNDON)
// 		{
// 			mSound.PlaySound(4);
// 			mSound.PauseMusic(true, 2);
// 		}
// 	}
}
void Engine::IncBugs(float bug)
{
// 	if (mBugBar->currProgress < 1.0) 
// 	{
// 		mBugBar->currProgress += bug;
// 		if (mBugBar->currProgress >= 1.0)
// 		{
// 			mBugBar->currProgress = 1.0; *StateMachine::pGameState = GameState::LOSE; waitToClickTime = 2;
// 			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(3); mSound.PauseMusic(true, 2); }
// 		}
// 	} 
}  
void Engine::DecBugs(float bug)		
{
// 	if(mBugBar->currProgress > 0)mBugBar->currProgress -= bug;
// 	if(mBugBar->currProgress < 0)mBugBar->currProgress = 0.0f;
}
void Engine::DecHP(float hp)
{
// 	if (mHPBAR->currProgress > 0)mHPBAR->currProgress -= hp;
// 	if (mHPBAR->currProgress < 0)
// 	{ 
// 		mHPBAR->currProgress = 0.0f; waitToClickTime = 2;
// 	if (*StateMachine::pSoundState == SoundState::SOUNDON)
// 	{
// 		mSound.PlaySound(4); 
// 	}
// 	mSound.PauseMusic(true, 3);
// 	}
}
void Engine::ClearVectors()
{
// 	for (int i = 0; i < mInvaders.size(); i++)
// 	{
// 		delete mInvaders[i];
// 		mInvaders[i] = nullptr;
// 	}mInvaders.clear();
// 
// 	for (int i = 0; i < mMushrooms.size(); i++)
// 	{
// 		delete mMushrooms[i];
// 		mMushrooms[i] = nullptr;
// 	}mMushrooms.clear();
// 
// 	for (int i = 0; i < mProjectiles.size(); i++)
// 	{
// 		delete mProjectiles[i];
// 		mProjectiles[i] = nullptr;
// 	}mProjectiles.clear();
// 
// 	for (int i = 0; i < mGhosts.size(); i++)
// 	{
// 		delete mGhosts[i];
// 		mGhosts[i] = nullptr;
// 	}mGhosts.clear();
}
void Engine::CamFollowPlayer()
{
	mCam.SetPosition(mPlayer->mSelf->mPosition.x, mPlayer->mSelf->mPosition.y + 100.0f, mCam.GetPosition().z);
}



//COLLISION CASES
bool Engine::CamOnPickUp(Entity* pickup)
{
	//Convert from 0,0 at center of screen coordinates to 0,0 top left ...  cartesian to screen 
	float x, z;
	x = pickup->mPosition.x - pickup->mWidth / 2;
	z = pickup->mPosition.z - pickup->mWidth / 2;

	bool inX = false;
	bool inZ = false;

	if (mCam.GetPosition().x > x && mCam.GetPosition().x < x + pickup->mWidth)	{ inX = true; }
	if (mCam.GetPosition().z > z && mCam.GetPosition().z < z + pickup->mWidth)	{ inZ = true; }

	return(inX && inZ);
}
bool Engine::ProjectileBounds(Entity* proj)
{
	float x, y, z, maxBound = 4000.0f;

	x = abs(proj->mPosition.x);
	y = abs(proj->mPosition.y);
	z = abs(proj->mPosition.z);

	return((x < maxBound) && (y < maxBound) && (z < maxBound));
}
bool Engine::ProjectileHitBug(Entity* proj)
{
// 	bool hit = false;
// 	for (int i = 0; i < mInvaders.size(); i++)
// 	{
// 		if (XNA::IntersectSphereSphere(&mInvaders[i]->mSphereCollider, &proj->mSphereCollider))
// 		{
// 			hit = true; mInvaders[i]->mDead = true; DecBugs(bugsWorth);
// 		}
// 	}
// 	return hit;
	return true;
}
bool Engine::ProjectileHitBoss(Entity* proj)
{
// 	bool hit = false;
// 
// 	if (XNA::IntersectSphereSphere(&mBoss->mSphereCollider, &proj->mSphereCollider))
// 	{
// 		hit = true; DecHP(dmgAmount);
// 	}
// 	return hit;
	return true;
}
bool Engine::ProjectileHitGhost(Entity* proj)
{
// 	bool hit = false;
// 	for (int i = 0; i < mGhosts.size(); i++)
// 	{
// 		if (XNA::IntersectSphereSphere(&mGhosts[i]->mSphereCollider, &proj->mSphereCollider))
// 		{
// 			hit = true; //mGhosts[i]->mDead = true;
// 		}
// 	}
// 	return hit;
	return true;
}
bool Engine::GhostHitCam(Entity* ghost)
{
	return XNA::IntersectSphereSphere(&mCam.mSphereCollider, &ghost->mSphereCollider);
}
bool Engine::BossHitCam(Entity* boss)
{
	return XNA::IntersectSphereSphere(&mCam.mSphereCollider, &boss->mSphereCollider);
}



//GAME INITS
void Engine::InitAll()
{
	//MAKE BUTTONS
	Entity* mPlayButt		= new Entity(0,"play",	 80.0f, 40.0f);		mUI.push_back(mPlayButt	); 		mMain.push_back(mPlayButt);			mMainBtns.push_back(mPlayButt);
	Entity* mSoundButt		= new Entity(0,"sound",  40.0f, 20.0f);		mUI.push_back(mSoundButt); 		mMain.push_back(mSoundButt);		mMainBtns.push_back(mSoundButt);
	Entity* mMusicButt		= new Entity(0,"music",  40.0f, 20.0f);		mUI.push_back(mMusicButt); 		mMain.push_back(mMusicButt);		mMainBtns.push_back(mMusicButt);
	Entity* mSOnButt		= new Entity(0,"Son",	 40.0f, 20.0f);		mUI.push_back(mSOnButt	);		mMain.push_back(mSOnButt	);
	Entity* mSOffButt		= new Entity(0,"Soff",	 40.0f, 20.0f);		mUI.push_back(mSOffButt	); 		mMain.push_back(mSOffButt	);
	Entity* mMOnButt		= new Entity(0,"Mon",	 40.0f, 20.0f);		mUI.push_back(mMOnButt	); 		mMain.push_back(mMOnButt	);
	Entity* mModeButt		= new Entity(0,"mode",	 40.0f, 20.0f);		mUI.push_back(mModeButt); 											
	Entity* mEasyButt		= new Entity(0,"easy",	 40.0f, 20.0f);		mUI.push_back(mEasyButt);
	Entity* mMedButt		= new Entity(0,"med",	 40.0f, 20.0f);		mUI.push_back(mMedButt);
	Entity* mHardButt		= new Entity(0,"hard",	 40.0f, 20.0f);		mUI.push_back(mHardButt);
	Entity* mInsaneButt		= new Entity(0,"insane", 40.0f, 20.0f);		mUI.push_back(mInsaneButt);
	Entity* mMOffButt		= new Entity(0, "Moff",  40.0f, 20.0f);		mUI.push_back(mMOffButt); 		mMain.push_back(mMOffButt);
	Entity* mTitleButt		= new Entity(0, "title", 200.0f, 50.0f);	mUI.push_back(mTitleButt); 		mMain.push_back(mTitleButt);		mAbout.push_back(mTitleButt);
	Entity* mAboutButt		= new Entity(0,"about",  80.0f, 40.0f);		mUI.push_back(mAboutButt);		mMain.push_back(mAboutButt	);		mMainBtns.push_back(mAboutButt);
	Entity* mBymeButt		= new Entity(0,"byme",	 110.0f, 30.0f);		mUI.push_back(mBymeButt); 		mAbout.push_back(mBymeButt	);
	Entity* mQuitButt		= new Entity(0,"quit",	 350.0f, 200.0f);		mUI.push_back(mQuitButt); 		mPaused.push_back(mQuitButt);		mPausedBtns.push_back(mQuitButt);
	Entity* mRestartButt	= new Entity(0,"restart", 350.0f, 200.0f);	mUI.push_back(mRestartButt); 	mPaused.push_back(mRestartButt);	mPausedBtns.push_back(mRestartButt);
	Entity* mPausedButt		= new Entity(0,"paused", 600.0f, 300.0f);	mUI.push_back(mPausedButt); 	mPaused.push_back(mPausedButt);
	Entity* mBackButt		= new Entity(0,"back",	 80.0f, 40.0f);		mUI.push_back(mBackButt);		mAbout.push_back(mBackButt);		mAboutBtns.push_back(mBackButt);
	Entity* mAboutMsgButt	= new Entity(0,"aboutmsg", 110.0f, 110.0f);	mUI.push_back(mAboutMsgButt); 	mAbout.push_back(mAboutMsgButt);

	mPlayButt->LoadTexture(			md3dDevice, L"Textures/play.dds");
	mSoundButt->LoadTexture(		md3dDevice, L"Textures/sound.dds");
	mMusicButt->LoadTexture(		md3dDevice, L"Textures/music.dds");
	mSOnButt->LoadTexture(			md3dDevice, L"Textures/on.dds");
	mSOffButt->LoadTexture(			md3dDevice, L"Textures/off.dds");
	mMOnButt->LoadTexture(			md3dDevice, L"Textures/on.dds");
	mMOffButt->LoadTexture(			md3dDevice, L"Textures/off.dds");
	mTitleButt->LoadTexture(		md3dDevice, L"Textures/title.dds");
	mAboutButt->LoadTexture(		md3dDevice, L"Textures/about.dds");
	mBymeButt->LoadTexture(			md3dDevice, L"Textures/byme.dds");
	mQuitButt->LoadTexture(			md3dDevice, L"Textures/quit.dds");
	mRestartButt->LoadTexture(		md3dDevice, L"Textures/restart.dds");
	mPausedButt->LoadTexture(		md3dDevice, L"Textures/paused.dds");
	mBackButt->LoadTexture(			md3dDevice, L"Textures/back.dds");
	mAboutMsgButt->LoadTexture(		md3dDevice, L"Textures/aboutmsg.dds");
	mModeButt->LoadTexture(			md3dDevice, L"Textures/mode.dds");
	mMedButt->LoadTexture(			md3dDevice, L"Textures/med.dds");
	mEasyButt->LoadTexture(			md3dDevice, L"Textures/easy.dds");
	mHardButt->LoadTexture(			md3dDevice, L"Textures/hard.dds");
	mInsaneButt->LoadTexture(		md3dDevice, L"Textures/insane.dds");

	//3D UI STUFF
	mPlayButt->SetPos(0.0f, 50.0f, -200.0f);
	mPlayButt->Pitch(XM_PI / 4.5);

	mSoundButt->SetPos(-140.0f, 50.0f, -200.0f);
	mSoundButt->Pitch(XM_PI / 4.5);

	mSOnButt->SetPos(-85.0f, 50.0f, -200.0f);
	mSOnButt->Pitch(XM_PI / 4.5);

	mSOffButt->SetPos(-85.0f, 50.0f, -200.0f);
	mSOffButt->Pitch(XM_PI / 4.5);

	mMusicButt->SetPos(-140.0f, 50.0f, -150.0f);
	mMusicButt->Pitch(XM_PI / 4.5);

	mMOnButt->SetPos(-85.0f, 50.0f, -150.0f);
	mMOnButt->Pitch(XM_PI / 4.5);

	mMOffButt->SetPos(-85.0f, 50.0f, -150.0f);
	mMOffButt->Pitch(XM_PI / 4.5);

	mTitleButt->SetPos(0.0f, 110.0f, -200.0f);
	mTitleButt->Pitch(XM_PI / 4.5);

	mAboutButt->SetPos(100.0f, 50.0f, -200.0f);
	mAboutButt->Pitch(XM_PI / 4.5);

	mBymeButt->SetPos(-120.0f, 50.0f, -180.0f);
	mBymeButt->Pitch(XM_PI / 4.5);

	mBackButt->SetPos(120.0f, 50.0f, -180.0f);
	mBackButt->Pitch(XM_PI / 4.5);

	mAboutMsgButt->SetPos(0.0f, 50.0f, -240.0f);
	mAboutMsgButt->Pitch(XM_PI / 4.5);

	mModeButt->SetPos(-20.0f, 50.0f,  -280.0f);
	mModeButt->Pitch(XM_PI / 4.5);
	mEasyButt->SetPos(20.0f, 50.0f, -280.0f);
	mEasyButt->Pitch(XM_PI / 4.5);
	mMedButt->SetPos(20.0f, 50.0f, -280.0f);
	mMedButt->Pitch(XM_PI / 4.5);
	mHardButt->SetPos(20.0f, 50.0f, -280.0f);
	mHardButt->Pitch(XM_PI / 4.5);
	mInsaneButt->SetPos(20.0f, 50.0f, -280.0f);
	mInsaneButt->Pitch(XM_PI / 4.5);

	//2D UI STUFF
	mRestartButt->SetPos(-700.0f, 100.0f, -90.0f);
	mRestartButt->Pitch(XM_PI / 4.5);

	mPausedButt->SetPos(0.0f, 150.0f, -90.0f);
	mPausedButt->Pitch(XM_PI / 4);

	mQuitButt->SetPos(700.0f, 100.0f, -90.0f);
	mQuitButt->Pitch(XM_PI / 4);




	BuildVertexAndIndexBuffer(&mShapesVB, &mShapesIB, mUI);



	Text* t = new Text("Wassup? / hey new line kcdkdcmdck!? ", 0.0f, 0.0f, 0.0f, 20.0f, 0, true); mTexts.push_back(t);
	BuildVertexAndIndexBuffer(&t->mVB, &t->mIB, t->mText);
	

	LevelSection* L = new LevelSection("1234 9fkfm kf k k k df", 0.0f, 0.0f, 0.0f, 20.0f); mLevel.push_back(L);
	BuildVertexAndIndexBuffer(&L->mVB, &L->mIB, L->mEntities);














	mSound.StreamMusic(1);
}
void Engine::InsertAllIndices(std::vector<UINT>& indices, std::vector<Entity*>& entitys)
{
	for (int i = 0; i < entitys.size(); i++)
	{
		indices.insert(indices.end(), entitys[i]->mGrid.Indices.begin(), entitys[i]->mGrid.Indices.end());
	}
}
void Engine::InsertAllVertices(std::vector<Vertex::Basic32>& vertices, UINT& k, std::vector<Entity*>& entitys)
{
	for (int i = 0; i < entitys.size(); i++)
	{
		entitys[i]->LoadVertData(vertices, k);
	}
}
UINT Engine::TotalVertexCount(std::vector<Entity*>& entitys)
{
	UINT ret = 0;
	for (int i = 0; i < entitys.size(); i++)
	{
		ret += entitys[i]->mGrid.Vertices.size();
	}
	return ret;
}
UINT Engine::TotalIndiceCount(std::vector<Entity*>& entitys)
{
	UINT ret = 0;
	for (int i = 0; i < entitys.size(); i++)
	{
		ret += entitys[i]->mIndexCount;
	}
	return ret;
}
void Engine::SetAllVertexOffsets(std::vector<Entity*>& entitys)
{
	for (int i = 0; i < entitys.size(); i++)
	{
		if (i != 0)
		{
			entitys[i]->SetVertexOffset(entitys[i - 1]->GetVertOffset() + entitys[i - 1]->mGrid.Vertices.size());
		}
		else
		{
			entitys[i]->SetVertexOffset(0);
		}
	}
}
void Engine::SetAllIndexOffsets(std::vector<Entity*>& entitys)
{
		// Cache the index count of each object.
	for (int i = 0; i < entitys.size(); i++)
	{
		if (i != 0)
		{
			entitys[i]->SetIndexOffset(entitys[i - 1]->GetIndOffset() + entitys[i - 1]->mGrid.Indices.size());
		}
		else
		{
			entitys[i]->SetIndexOffset(0);
		}
	}
}
void Engine::BuildVertexAndIndexBuffer(ID3D11Buffer** VB, ID3D11Buffer** IB, std::vector<Entity*>& entities)
{
	SetAllVertexOffsets(entities);
	SetAllIndexOffsets(entities);

	UINT totalVertexCount = TotalVertexCount(entities);
	UINT totalIndexCount = TotalIndiceCount(entities);

	// pack vertices of all the meshes into one vertex buffer.
	std::vector<Vertex::Basic32> vertices(totalVertexCount);
	UINT k = 0;
	InsertAllVertices(vertices, k, entities); // EVENTUALLY PASS THE VECTOR TO IT 

	//CREATE VERTEX BUFFER
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, VB));


	// Pack the indices of all the meshes into one index buffer.
	std::vector<UINT> indices;
	InsertAllIndices(indices, entities); // EVENTUALLY PASS THE VECTOR TO IT 

	//CREATE INDEX BUFFER
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, IB));
}



//GAME DRAWS
void Engine::DrawScene()
{
	ClearScene();
	mSky->Draw(md3dImmediateContext, mCam); //Draw Sky First So The Z Buffer Takes Effect  ... Otherwise it Draws Over I

	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	if( mWireMode )	md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

	//SWITCH ON THE MODE THE GAME IS IN 
	switch (*StateMachine::pGameState)
	{
	case GameState::MAINMENU:	DrawMainMenu(); break;
	case GameState::ABOUT:		DrawAbout();	break;
	case GameState::PAUSED:		DrawPaused();	break;
	case GameState::WIN:		DrawWin();		break;
	case GameState::LOSE:		DrawLose();		break;
	case GameState::GAMEON:		DrawGameOn();	break;
	}

	RestoreStates();
	HR(mSwapChain->Present(0, 0));
}
void Engine::DrawMainMenu()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;

	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);


		for (int i = 0; i < mMain.size(); i++)
		{
			if (mMain[i]->mLabel == "easy"){
				if (*StateMachine::pGameMode == GameMode::EASY){ mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }			continue;
			}
			if (mMain[i]->mLabel == "med"){
				if (*StateMachine::pGameMode == GameMode::MED){ mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }				continue;
			}
			if (mMain[i]->mLabel == "hard"){
				if (*StateMachine::pGameMode == GameMode::HARD){ mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }			continue;
			}
			if (mMain[i]->mLabel == "insane"){
				if (*StateMachine::pGameMode == GameMode::INSANE){ mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }			continue;
			}
			if (mMain[i]->mLabel == "Son"){
				if (*StateMachine::pSoundState == SoundState::SOUNDON){ mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }		continue;
			}
			if (mMain[i]->mLabel == "Soff"){
				if (*StateMachine::pSoundState == SoundState::SOUNDOFF){ mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }	continue;
			}
			if (mMain[i]->mLabel == "Mon"){
				if (*StateMachine::pMusicState == MusicState::MUSICON){ mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }		continue;
			}
			if (mMain[i]->mLabel == "Moff"){
				if (*StateMachine::pMusicState == MusicState::MUSICOFF){mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());}		continue;
			}
			mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		}
	}

	//DRAW 2D STUFF
	Effects::BasicFX->SetDirLights(mDirLights3);
	activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
	md3dImmediateContext->OMSetDepthStencilState(RenderStates::ZBufferDisabled, 0); // changing 0 means overlaping draws
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		for (int i = 0; i < mTexts.size(); i++)
		{
			mTexts[i]->DrawText2D(&activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		}
	}

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
// 	XMMATRIX toTexSpace(
// 		0.5f, 0.0f, 0.0f, 0.0f,
// 		0.0f, -0.5f, 0.0f, 0.0f,
// 		0.0f, 0.0f, 1.0f, 0.0f,
// 		0.5f, 0.5f, 0.0f, 1.0f);
// 	stride = sizeof(Vertex::PosNormalTexTan);
// 	ID3DX11EffectTechnique* tech = Effects::BasicFX->Light3TexTech;
// 	tech->GetDesc(&techDesc);
// 	for (UINT p = 0; p < techDesc.Passes; ++p)
// 	{
// 		for (UINT modelIndex = 0; modelIndex < mModelInstances.size(); ++modelIndex)
// 		{
// 			world = XMLoadFloat4x4(&mModelInstances[modelIndex].World);
// 			worldInvTranspose = MathHelper::InverseTranspose(world);
// 			worldViewProj = world*view*proj;
// 
// 			Effects::BasicFX->SetWorld(world);
// 			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
// 			Effects::BasicFX->SetWorldViewProj(worldViewProj);
// 			Effects::BasicFX->SetWorldViewProjTex(worldViewProj*toTexSpace);
// 			Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
// 
// 			for (UINT subset = 0; subset < mModelInstances[modelIndex].Model->SubsetCount; ++subset)
// 			{
// 				Effects::BasicFX->SetMaterial(mModelInstances[modelIndex].Model->Mat[subset]);
// 				Effects::BasicFX->SetDiffuseMap(mModelInstances[modelIndex].Model->DiffuseMapSRV[subset]);
// 				//Effects::BasicFX->SetNormalMap(mModelInstances[modelIndex].Model->NormalMapSRV[subset]);
// 
// 				tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
// 				mModelInstances[modelIndex].Model->ModelMesh.Draw(md3dImmediateContext, subset);
// 			}
// 		}
// 	}


	//SHADOWS *******************************************************************************************************

// 	//set shadow states
// 	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
// 	md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
// 	md3dImmediateContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0); // changing 0 means overlaping draws
// 	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
// 	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
// 
// 	//Set Shadow Constants
// 	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
// 	XMVECTOR toMainLight = -XMLoadFloat3(&mDirLights[0].Direction);
// 	XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
// 	float xOffSet = 43.0f; float zOffSet = 90.0f; float yOffSet = 0.1f; float sScale = 0.15f;
// 
// 	mPlayButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mSoundButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mMusicButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mSOnButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mSOffButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mMOnButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mMOffButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mTitleButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet + 100.0f, mCam, mShadowMat);
// 	mAboutButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mModeButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale/2, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mEasyButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale/2, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	// Restore default states.
	RestoreStates();

	if(!fullyLoaded)fullyLoaded = true;
}
void Engine::DrawAbout()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

		//DRAW BUTTS
		for (int i = 0; i < mAbout.size(); i++)
		{
			if (mAbout[i]->mLabel == "aboutmsg" || mAbout[i]->mLabel == "title" || mAbout[i]->mLabel == "back" || mAbout[i]->mLabel == "byme")
			{
				mAbout[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
			}
		}
	}

	//SHADOWS *******************************************************************************************************

// 	//set shadow states
// 	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
// 	md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
// 	md3dImmediateContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0); // changing 0 means overlaping draws
// 	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
// 	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
// 
// 	//Set Shadow Constants
// 	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
// 	XMVECTOR toMainLight = -XMLoadFloat3(&mDirLights[0].Direction);
// 	XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
// 	float xOffSet = 43.0f; float zOffSet = 90.0f; float yOffSet = 0.1f; float sScale = 0.1f;
// 
// 	mAboutMsgButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mTitleButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet+100.0f, mCam, mShadowMat);
// 	mBackButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 	mBymeButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
// 
// 	// Restore default states.
	RestoreStates();
}
void Engine::DrawPaused()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);

	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);


	//DRAW 2D STUFF
	Effects::BasicFX->SetDirLights(mDirLights2);
	activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
	md3dImmediateContext->OMSetDepthStencilState(RenderStates::ZBufferDisabled, 0); // changing 0 means overlaping draws
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		for (int i = 0; i < mPaused.size(); i++)
		{
			mPaused[i]->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		}
	}

	RestoreStates();
	DrawGameOn();
}
void Engine::DrawGameOn()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);


		Effects::BasicFX->SetDirLights(mDirLights2);
		activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
// 			for (int i = 0; i < mTexts.size(); i++)
// 			{
// 				mTexts[i]->DrawText3D(&activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
// 			}

			for (int i = 0; i < mLevel.size(); i++)
			{
				mLevel[i]->Draw(&activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
			}

			mPlayer->Draw(&activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		}



		//DRAW BUTTS
		Effects::BasicFX->SetDirLights(mDirLights2);
		activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
		md3dImmediateContext->OMSetDepthStencilState(RenderStates::ZBufferDisabled, 0); // changing 0 means overlaping draws

		if (*StateMachine::pGameState == GameState::GAMEON)
		{

		}
		else
		{
// 			mHPBAR->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
// 			mHPButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
// 			mHPOL->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		}

	}
}
void Engine::DrawWin()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights2);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		for (int i = 0; i < mMain.size(); i++)
		{
			if (mMain[i]->mLabel == "quit" || mMain[i]->mLabel == "restart")
			{
				mMain[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
			}
		}
	}

	RestoreStates();
	DrawGameOn();
}
void Engine::DrawLose()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights2);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		


	}

	RestoreStates();
	DrawGameOn();
}
void Engine::DrawBossWin()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights2);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		
	}

	RestoreStates();
	DrawGameOn();
}
void Engine::DrawBossLose()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights2);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{



	}
}


//KEYBOARD/MOUSE
void Engine::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;
		
		if (waitToClickTime == 0)
		{	
			switch (*StateMachine::pGameState)
			{
			case GameState::MAINMENU:	BtnsMainMenu(x, y, true);		break;
			case GameState::ABOUT:		BtnsAbout(x, y, true);			break;
			case GameState::PAUSED:		BtnsPaused(x, y, true);			break;
			case GameState::WIN:		BtnsWin(x, y, true);			break;
			case GameState::LOSE:		BtnsLose(x, y, true);			break;
			case GameState::GAMEON:		BtnsGameOn(x, y, true);			break;
			}
		}
	}
}
void Engine::OnMouseUp(WPARAM btnState, int x, int y)
{
	//ReleaseCapture();
}
void Engine::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (fullyLoaded)
	{
		switch (*StateMachine::pGameState)
		{
		case GameState::MAINMENU:	BtnsMainMenu(x, y, false);		break;
		case GameState::ABOUT:		BtnsAbout(x, y, false);			break;
		case GameState::PAUSED:		BtnsPaused(x, y, false);		break;
		case GameState::WIN:		BtnsWin(x, y, false);			break;
		case GameState::LOSE:		BtnsLose(x, y, false);			break;
		case GameState::GAMEON:		BtnsGameOn(x, y, false);		break;
		}
	}
	if (*StateMachine::pGameState == GameState::GAMEON)
	{
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
	
		mCam.mPitch += dy; //Increment internal pitch 
	
		if (mCam.mPitch > -XM_PI/2 && mCam.mPitch < XM_PI/4){ mCam.Pitch(dy); } //LIMITS UP AND DOWN
		mCam.mPitch = MathHelper::Clamp(mCam.mPitch, -XM_PI / 2, XM_PI/4);
			
		mCam.RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;

	//Mouse Wrap around
	if (*StateMachine::pGameState == GameState::GAMEON)
	{
		if (x <= 3)
		{
			SetCursorPos(mClientWidth, y);
			mLastMousePos.x = mClientWidth;
			mLastMousePos.y = y;
			return;
		}
		else if (x >= mClientWidth - 5)
		{
			SetCursorPos(5, y);
			mLastMousePos.x = 5;
			mLastMousePos.y = y;
			return;
		}
	}
}
void Engine::OnKeyUP(WPARAM btnState)
{
	switch (btnState)
	{
	case 0x31:(mWireMode)	? mWireMode = false : mWireMode = true; break;	// 1 KEY
	case 0x32:(mBFCull)		? mBFCull	= false	: mBFCull	= true; break;	// 2 KEY
	case 0x50:if (*StateMachine::pGameState == GameState::GAMEON || *StateMachine::pGameState == GameState::PAUSED)
		{(*StateMachine::pGameState == GameState::PAUSED) ? *StateMachine::pGameState = GameState::GAMEON : *StateMachine::pGameState = GameState::PAUSED;}break;
	case 0x57: mPlayer->GoOut = false; mPlayer->SlOut = true; break; // W
	case 0x41: mPlayer->GoBW  = false; mPlayer->SlBW  = true; break; // A
	case 0x53: mPlayer->GoIn  = false; mPlayer->SlIn  = true; break; // S
	case 0x44: mPlayer->GoFW  = false; mPlayer->SlFW  = true; break; // D
	}
}
void Engine::OnKeyDOWN(WPARAM btnState)
{
	switch (btnState)
	{
	case 0x20: mPlayer->Jump(); break; //SPACE
	}
}
void Engine::KeyboardHandler(float dt)
{
	//
	// Control the camera.
	//
	if (*StateMachine::pGameState == GameState::GAMEON)
	{
		//mCam.mUseConstraints = true;

		if (GetAsyncKeyState('W') & 0x8000)
		{
			//mCam.Walk(mMoveSpeed*dt);
			mPlayer->GoOut = true;
		}
		
	
		if (GetAsyncKeyState('S') & 0x8000)
		
		{
		//	mCam.Walk(-mMoveSpeed*dt);
			mPlayer->GoIn = true;
		}
	
		if (GetAsyncKeyState('A') & 0x8000)
		{
			//mCam.Strafe(-mMoveSpeed*dt);
			mPlayer->GoBW = true;
		}
			
	
		if (GetAsyncKeyState('D') & 0x8000)
		{
		//	mCam.Strafe(mMoveSpeed*dt);
			mPlayer->GoFW = true;
		}
			






		if (GetAsyncKeyState('I') & 0x8000)
		{
			mCam.Walk(mMoveSpeed*dt);
		}
		if (GetAsyncKeyState('K') & 0x8000)
		{
			mCam.Walk(-mMoveSpeed*dt);
		}
		if (GetAsyncKeyState('J') & 0x8000)
		{
			mCam.Strafe(-mMoveSpeed*dt);
		}
		if (GetAsyncKeyState('L') & 0x8000)
		{
			mCam.Strafe(mMoveSpeed*dt);
		}


	//
	// Walk/fly mode
	//
	if (GetAsyncKeyState('2') & 0x8000)
		mWalkCamMode = true;
	if (GetAsyncKeyState('3') & 0x8000)
		mWalkCamMode = false;

	}else
	{
		mCam.mUseConstraints = false;
	}
	//
	// Reset particle systems.
	//
	// 	if (GetAsyncKeyState('R') & 0x8000)
	// 	{
	// 		mFire.Reset();
	// 		mRain.Reset();
	// 	}
}




//BUTTON HANDLERS
void Engine::BtnsMainMenu(float x, float y, bool clicked)
{
	for (int i = 0; i < mMainBtns.size(); i++)
	{
		if (mMainBtns[i]->mLabel == "play"){ if (InButton3D(x, y, mMainBtns[i])){ mMainBtns[i]->hovering = true;  if (clicked){  Play(); } } else{ mMainBtns[i]->hovering = false; }  continue; }
		if (mMainBtns[i]->mLabel == "about"){ if (InButton3D(x, y, mMainBtns[i])){ mMainBtns[i]->hovering = true; if (clicked){ About(); } } else{ mMainBtns[i]->hovering = false; } continue; }
		if (mMainBtns[i]->mLabel == "music"){ if (InButton3D(x, y, mMainBtns[i])){ mMainBtns[i]->hovering = true; if (clicked){ Music(); } } else{ mMainBtns[i]->hovering = false; } continue; }
		if (mMainBtns[i]->mLabel == "sound"){ if (InButton3D(x, y, mMainBtns[i])){ mMainBtns[i]->hovering = true; if (clicked){ SoundFX(); } } else{ mMainBtns[i]->hovering = false; } continue; }
	}
}
void Engine::BtnsAbout(float x, float y, bool clicked)
{
	if (InButton3D(x, y, mAboutBtns[0])){ mAboutBtns[0]->hovering = true;  if (clicked){  AboutBack(); } } else{ mAboutBtns[0]->hovering = false; }
}
void Engine::BtnsPaused(float x, float y, bool clicked)
{
	for (int i = 0; i < mPausedBtns.size(); i++)
	{
		if (mPausedBtns[i]->mLabel == "quit")		{ if (InButton2D(x, y, mPausedBtns[i])){ mPausedBtns[i]->hovering = true;  if (clicked){ Quit(); } }	else{ mPausedBtns[i]->hovering = false; } continue; }
		if (mPausedBtns[i]->mLabel == "restart")	{ if (InButton2D(x, y, mPausedBtns[i])){ mPausedBtns[i]->hovering = true;  if (clicked){ Restart(); } } else{ mPausedBtns[i]->hovering = false; } continue; }
	}
}
void Engine::BtnsGameOn(float x, float y, bool clicked)
{
	//if (clicked){ SpawnProjectile(); }
}
void Engine::BtnsWin(float x, float y, bool clicked)
{
// 	if (InButton2D(x, y, mQuitButt))
// 	{
// 		mQuitButt->hovering = true;
// 		if (clicked)
// 		{
// 			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
// 			mWalkCamMode = false;
// 			*StateMachine::pGameState = GameState::MAINMENU;
// 			ResetCamMainMenu(); ClearVectors();
// 		}
// 	}
// 	else{ mQuitButt->hovering = false; }
// 
// 	if (InButton2D(x, y, mRestartButt))
// 	{
// 		mRestartButt->hovering = true;
// 		if (clicked)
// 		{
// 			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
// 			*StateMachine::pGameState = GameState::GAMEON; ClearVectors(); ResetCamInGame();
// 		}
// 	}
// 	else{ mRestartButt->hovering = false; }

}
void Engine::BtnsLose(float x, float y, bool clicked)
{
// 	if (InButton2D(x, y, mQuitButt))
// 	{
// 		mQuitButt->hovering = true;
// 		if (clicked)
// 		{
// 			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
// 			mWalkCamMode = false;
// 			*StateMachine::pGameState = GameState::MAINMENU;
// 			ResetCamMainMenu(); ClearVectors();
// 		}
// 	}
// 	else{ mQuitButt->hovering = false; }

// 	if (InButton2D(x, y, mRetryButt))
// 	{
// 		mRetryButt->hovering = true;
// 		if (clicked)
// 		{
// 			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
// 			*StateMachine::pGameState = GameState::GAMEON; ClearVectors(); ResetCamInGame();
// 		}
// 	}
// 	else{ mRetryButt->hovering = false; }
}
void Engine::BtnsBossLose(float x, float y, bool clicked)
{
// 	if (InButton2D(x, y, mBSOD) && exitable)
// 	{
// 		if (clicked)
// 		{
// 			mCompBar->currProgress = 0.0f;
// 			mBugBar->currProgress = 0.0f;
// 			mBoss->currProgress = 0.0f;
// 			mWalkCamMode = false;
// 			*StateMachine::pGameState = GameState::MAINMENU;
// 			ResetCamMainMenu(); ClearVectors();
// 			exitable = false;
// 		}
// 	}
}
void Engine::BtnsBossWin(float x, float y, bool clicked)
{
// 	if (InButton2D(x, y, mQuitButt))
// 	{
// 		mQuitButt->hovering = true;
// 		if (clicked)
// 		{
// 			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
// 			mWalkCamMode = false;
// 			*StateMachine::pGameState = GameState::MAINMENU;
// 			ResetCamMainMenu(); ClearVectors();
// 		}
// 	}
// 	else{ mQuitButt->hovering = false; }
// 
// 	if (InButton2D(x, y, mRestartButt))
// 	{
// 		mRestartButt->hovering = true;
// 		if (clicked)
// 		{
// 			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
// 			*StateMachine::pGameState = GameState::GAMEON; ClearVectors(); ResetCamInGame();
// 		}
// 	}
// 	else{ mRestartButt->hovering = false; }
}
bool Engine::InButton3D(float x, float y, Entity* button)
{
	XMMATRIX P = mCam.Proj();

	// Compute picking ray in view space.
	int newWidth, newHeight;
	float fs;
	if (mFullScreen){ newWidth = m_ScreenWidth; newHeight = m_ScreenHeight; fs = 1.0f; }
	else
	{
		fs = 1.0f;
		newWidth	= mClientWidth   ;
		newHeight	= mClientHeight  ;
	}

	float vx = (+2.0f*x / newWidth - fs) / P(0, 0);
	float vy = (-2.0f*y / newHeight + fs) / P(1, 1);


	// Ray definition in view space.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Tranform ray to local space of Mesh.
	XMMATRIX V = mCam.View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = XMLoadFloat4x4(&button->mWorld);
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// Make the ray direction unit length for the intersection tests.
	rayDir = XMVector3Normalize(rayDir);

	float tmin = 0.0f; // The Returned Distance
	if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &button->mMeshBox, &tmin))
	{
		//WE ARE IN THE MESH .. DO WHATEVER YOU WANT
		return true;
	}
	return false;
}
bool Engine::InButton2D(float sx, float sy, Entity* button)
{
	//Convert from 0,0 at center of screen coordinates to 0,0 top left ...  cartesian to screen 
	float x,y;
	x = (button->mPosition.x - button->mWidth/2) + (mClientWidth / 2);
	y = (mClientHeight / 2) - (button->mPosition.y + button->mHeight/2);


	bool inX = false;
	bool inY = false;

 	if (sx > x && sx < x + button->mWidth)	{ inX = true; }
 	if (sy > y && sy < y + button->mHeight)	{ inY = true; }


	return(inX && inY);
}

//BUTOON FUNCTIONS
void Engine::Play()
{
	ClearVectors();
	if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
	*StateMachine::pGameState = GameState::GAMEON;
	if (!mSound.MusicPaused(1)){ mSound.PauseMusic(true, 1); }
	ResetCamInGame();
}
void Engine::About()
{
	if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
	*StateMachine::pGameState = GameState::ABOUT;
}
void Engine::Music()
{
	if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
	if (*StateMachine::pMusicState == MusicState::MUSICON)
	{
		*StateMachine::pMusicState = MusicState::MUSICOFF;
	} 
	else
	{
		*StateMachine::pMusicState = MusicState::MUSICON;
	}
}
void Engine::SoundFX()
{
	if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
	if (*StateMachine::pSoundState == SoundState::SOUNDON)
	{
		*StateMachine::pSoundState = SoundState::SOUNDOFF;
	}
	else
	{
		*StateMachine::pSoundState = SoundState::SOUNDON;
	}
}
void Engine::AboutBack()
{
	if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
	*StateMachine::pGameState = GameState::MAINMENU;
}
void Engine::Quit()
{
	if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
	*StateMachine::pGameState = GameState::MAINMENU;
	mWalkCamMode = false;
	ResetCamMainMenu();
	ClearVectors();
}
void Engine::Restart()
{
	if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
	*StateMachine::pGameState = GameState::GAMEON;
	ClearVectors();
	ResetCamInGame();
}

//SPAWNERS 
void Engine::SpawnBug()
{
// 	//Varrying spawn sides.. 1000z or -1000z then Rand -1000 to 1000 on X orrrr
// 	//						 1000x or -1000x and  Rand -1000 to 1000 on the Z ..
// 	float outSkirtZ;
// 	float outSkirtX;
// 	if (MathHelper::RandF() > 0.5)
// 	{
// 		MathHelper::RandF() > 0.5 ? outSkirtZ = -1000 : outSkirtZ = 1000;
// 		outSkirtX = MathHelper::RandF(-1000.0f, 1000.0f);
// 	}
// 	else
// 	{
// 		MathHelper::RandF() > 0.5 ? outSkirtX = -1000 : outSkirtX = 1000;
// 		outSkirtZ = MathHelper::RandF(-1000.0f, 1000.0f);
// 	}
// //	float randSize = MathHelper::RandF(10.0f, 50.0f); // TODO... IMPLEMENT A RANDOM SCALE SIZE;
// 	Entity* Invader = new Entity(md3dDevice, 50.0f, 50.0f, false, true);
// 
// 	float num = MathHelper::RandF();
// 	num > 0.75 ? Invader->UseTexture(mInvader->mTexSRV)	 :
// 	num > 0.50 ? Invader->UseTexture(mInvader2->mTexSRV) :
// 	num > 0.25 ? Invader->UseTexture(mInvader3->mTexSRV) :
// 	Invader->UseTexture(mInvader4->mTexSRV);
// 
// 	Invader->SetUpAnimation(6.0f, 1.0f, 1.0f);
// 
// 	Invader->reverseLook = true;
// 	Invader->SetPos(outSkirtX, 30.0f, outSkirtZ);
// 	Invader->SetGoToPoint(0.0f, 30.0f, 0.0f); // Go to Center 
// 	Invader->SetSphereCollider(20.0f);
// 	Invader->SetVertexOffset(mInvader->GetVertOffset());
// 	Invader->SetIndexOffset(mInvader->mIndexOffset);
// 	Invader->mIndexCount = mInvader->mIndexCount;
// 	Invader->mMeshBox = mInvader->mMeshBox;
// 	mInvaders.push_back(Invader);

}
void Engine::SpawnMushroom()
{
// 	float outSkirtZ;
// 	float outSkirtX;
// 
// 	outSkirtX = MathHelper::RandF(-500.0f, 500.0f);
// 	outSkirtZ = MathHelper::RandF(-500.0f, 500.0f);
// 	
// 	Entity* Mush = new Entity(md3dDevice, 50.0f, 50.0f, false, true);
// 	Mush->UseTexture(mMushroom->mTexSRV);
// 	Mush->reverseLook = true;
// 	Mush->SetPos(outSkirtX, 20.0f, outSkirtZ);
// 
// 	Mush->SetVertexOffset(mMushroom->GetVertOffset());
// 	Mush->SetIndexOffset(mMushroom->mIndexOffset);
// 	Mush->mIndexCount = mMushroom->mIndexCount;
// 	Mush->mMeshBox = mMushroom->mMeshBox;
// 	mMushrooms.push_back(Mush);
}
void Engine::SpawnProjectile()
{
// 	float outSkirtZ, outSkirtX, outSkirtY;
// 	
// 	outSkirtX = mCam.GetPosition().x;
// 	outSkirtZ = mCam.GetPosition().z;
// 	outSkirtY = mCam.GetPosition().y;
// 
// 	Entity* Proj = new Entity(md3dDevice, 10.0f, 10.0f, true);
// 	Proj->UseTexture(mProjectile->mTexSRV);
// 
// 	Proj->SetPos(outSkirtX, outSkirtY - 15.0f, outSkirtZ);
// 	Proj->SetSphereCollider(5.0f);
// 	Proj->SetVertexOffset(mProjectile->GetVertOffset());
// 	Proj->SetIndexOffset(mProjectile->mIndexOffset);
// 	Proj->mIndexCount = mProjectile->mIndexCount;
//  	
// 	Proj->useTexTrans = true;
// 	Proj->texTransMult = { 1.0f, 0.0f, 0.0f };
// 	Proj->origTexScale = { 2.0f, 2.0f, 2.0f };
// 
// 	Proj->mLook = mCam.GetLook();
// 	
// 	Proj->mMeshBox = mProjectile->mMeshBox;
// 	mProjectiles.push_back(Proj);
}
void Engine::SpawnGhost()
{
// 	float outSkirtZ, outSkirtX, outSkirtY;
// 
// 	outSkirtX = mBoss->mPosition.x;
// 	outSkirtZ = mBoss->mPosition.z;
// 	outSkirtY = mBoss->mPosition.y;
// 
// 	Entity* Ghost = new Entity(md3dDevice, 10.0f, 0.0f, 0.0f, false, true);
// 	Ghost->reverseLook = true;
// 	float num = MathHelper::RandF();
// 	num > 0.80 ? Ghost->UseTexture(mGhost1->mTexSRV) :
// 	num > 0.60 ? Ghost->UseTexture(mGhost2->mTexSRV) :
// 	num > 0.40 ? Ghost->UseTexture(mGhost3->mTexSRV) :
// 	num > 0.20 ? Ghost->UseTexture(mGhost4->mTexSRV) :
// 	Ghost->UseTexture(mGhost5->mTexSRV);
// 
// 	
// 
// 	Ghost->SetPos(outSkirtX + MathHelper::RandF(-60.0f, 60.0f), outSkirtY - 20.0f, outSkirtZ + MathHelper::RandF(-60.0f, 60.0f));
// 	Ghost->SetGoToPoint(mCam.GetPosition().x, mCam.GetPosition().y, mCam.GetPosition().z); // Go to Center 
// 	Ghost->SetSphereCollider(30.0f);
// 	Ghost->SetVertexOffset(mGhost1->GetVertOffset());
// 	Ghost->SetIndexOffset(mGhost1->mIndexOffset);
// 	Ghost->mIndexCount = mGhost1->mIndexCount;
// 
// 	//XMVECTOR cam = XMVectorNegate(XMLoadFloat3(&mCam.GetLook()));
// 	//XMStoreFloat3(&Ghost->mLook, cam);
// 	
// 	Ghost->mMeshBox = mGhost1->mMeshBox;
// 	mGhosts.push_back(Ghost);
}
