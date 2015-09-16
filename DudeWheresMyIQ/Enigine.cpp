#include "Engine.h"


Engine::Engine(HINSTANCE hInstance)
	: D3DApp(hInstance),
	mSky(0),
	mFloorTexSRV(0),
	mWalkCamMode(false),
	mWireMode(false),
	mBFCull(false),
	mPlayButt(0),
	mPausedButt(0),
	mTitleButt(0),
	mAboutButt(0),
	mBymeButt(0),
	mMusicButt(0),
	mSoundButt(0),
	mSOnButt(0),
	mMOffButt(0),
	mMOnButt(0),
	mSOffButt(0),
	mQuitButt(0),
	mRestartButt(0),
	mBackButt(0),
	mAboutMsgButt(0),
	testModel(0),
	spawnBugTime(0),
	spawnMushTime(0),
	speedBonusTime(0),
	mModeButt(0),
	mEasyButt(0),
	mMedButt(0),
	mHardButt(0),
	mInsaneButt(0),
	mMoveSpeed(500),
	tickTimer(0.0f),
	bossTimer(0.0f),
	randBossTime(1.0f),
	bugsWorth(0.03f),
	dmgAmount(0.01f),
	difficultyTimer(1.0f),
	waitToClickTime(0),
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


//TODO:: CLEAN UP THE MODEL CLASS WITH FUNCTIONALITY
// 	//Load Models
// 	testModel = new BasicModel(md3dDevice, mTexMgr, "Models\\Motherboard.obj", L"Textures\\");
// 	BasicModelInstance testInstance;
// 	testInstance.Model = testModel;
// 
// 	XMMATRIX modelScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
// 	XMMATRIX modelRot = XMMatrixRotationY(0.0f);
// 	XMMATRIX modelOffset = XMMatrixTranslation(0.0f, 10.0f, 0.0f);
// 
// 	XMStoreFloat4x4(&testInstance.World, modelScale*modelRot*modelOffset);
// 
// 	mModelInstances.push_back(testInstance);


	InitAll();

	mSky = new Sky(md3dDevice, L"Textures/ArstaBridge.dds", 5000.0f);

	

//FIRE EMITTER
//mRandomTexSRV = d3dHelper::CreateRandomTexture1DSRV(md3dDevice);
// 	std::vector<std::wstring> flares;
// 	flares.push_back(L"Textures\\flare0.dds");
// 	mFlareTexSRV = d3dHelper::CreateTexture2DArraySRV(md3dDevice, md3dImmediateContext, flares);
// 	mFire.Init(md3dDevice, Effects::FireFX, mFlareTexSRV, mRandomTexSRV, 500); 
// 	mFire.SetEmitPos(XMFLOAT3(0.0f, 1.0f, 120.0f));

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
	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 3000.0f);
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
	mPlayButt->Update(mCam, dt);
	mSoundButt->Update(mCam, dt);
	mMusicButt->Update(mCam, dt);
	mSOnButt->Update(mCam, dt);
	mSOffButt->Update(mCam, dt);
	mMOnButt->Update(mCam, dt);
	mMOffButt->Update(mCam, dt);
	mTitleButt->Update(mCam, dt);
	mAboutButt->Update(mCam, dt);
	mBackButt->Update(mCam, dt);
	mAboutMsgButt->Update(mCam, dt);
	mBymeButt->Update(mCam, dt);
	mModeButt->Update(mCam, dt);
	mEasyButt->Update(mCam, dt);
	mMedButt->Update(mCam, dt);
	mHardButt->Update(mCam, dt);
	mInsaneButt->Update(mCam, dt);

	if (*StateMachine::pMusicState == MusicState::MUSICON){ if  ( mSound.MusicPaused(1)){ mSound.PauseMusic(false,1); } }
	if (*StateMachine::pMusicState == MusicState::MUSICOFF){ if (!mSound.MusicPaused(1)){ mSound.PauseMusic(true,1); }}
}
void Engine::UpdateGame(float dt)
{
	//(*StateMachine::pGameState == GameState::GAMEON) ? mCursorOn = false : mCursorOn = true;

	mQuitButt->Update(mCam, dt);
	mRestartButt->Update(mCam, dt);
	mPausedButt->Update(mCam, dt);
	mBackButt->Update(mCam, dt);
	mYouWinButt->Update(mCam, dt);
	mYouLoseButt->Update(mCam, dt);
	mRetryButt->Update(mCam, dt);

	
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
	mCam.SetPosition(0.0f, 50.0f, 0.0f);
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
	mPlayButt		= new Entity(md3dDevice, 80.0f, 40.0f);
	mSoundButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mMusicButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mSOnButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mSOffButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mMOnButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mModeButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mEasyButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mMedButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mHardButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mInsaneButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mMOffButt		= new Entity(md3dDevice, 40.0f, 20.0f);
	mTitleButt		= new Entity(md3dDevice, 200.0f, 50.0f);
	mAboutButt		= new Entity(md3dDevice, 80.0f, 40.0f);
	mBymeButt		= new Entity(md3dDevice, 110.0f, 30.0f);
	mQuitButt		= new Entity(md3dDevice, 350.0f, 200.0f);
	mRestartButt	= new Entity(md3dDevice, 350.0f, 200.0f);
	mPausedButt		= new Entity(md3dDevice, 600.0f, 300.0f);
	mBackButt		= new Entity(md3dDevice, 80.0f, 40.0f);
	mAboutMsgButt	= new Entity(md3dDevice, 110.0f, 110.0f);

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
	mYouWinButt->LoadTexture(		md3dDevice, L"Textures/youwin.dds");
	mYouLoseButt->LoadTexture(		md3dDevice, L"Textures/youlose.dds");
	mRetryButt->LoadTexture(		md3dDevice, L"Textures/retry.dds");
	mModeButt->LoadTexture(			md3dDevice, L"Textures/mode.dds");
	mEasyButt->LoadTexture(			md3dDevice, L"Textures/easy.dds");
	mMedButt->LoadTexture(			md3dDevice, L"Textures/med.dds");
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








	SetAllVertexOffsets(); // EVENTUALLY PASS THE VECTOR TO IT 
	SetAllIndexOffsets();  // EVENTUALLY PASS THE VECTOR TO IT 

	UINT totalVertexCount = TotalVertexCount();
	UINT totalIndexCount  = TotalIndiceCount();
			 
	// pack vertices of all the meshes into one vertex buffer.
	std::vector<Vertex::Basic32> vertices(totalVertexCount);
	UINT k = 0;

	InsertAllVertices(vertices, k); // EVENTUALLY PASS THE VECTOR TO IT 


	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mShapesVB));


	// Pack the indices of all the meshes into one index buffer.
	std::vector<UINT> indices;

	InsertAllIndices(indices); // EVENTUALLY PASS THE VECTOR TO IT 

	//CREATE INDEX BUFFER
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mShapesIB));




	mSound.StreamMusic(1);
}
void Engine::InsertAllIndices(std::vector<UINT>& indices)
{
	//indices.insert(indices.end(), m.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), mPlayButt->mGrid.Indices.begin(), mPlayButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSoundButt->mGrid.Indices.begin(), mSoundButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMusicButt->mGrid.Indices.begin(), mMusicButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSOnButt->mGrid.Indices.begin(), mSOnButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSOffButt->mGrid.Indices.begin(), mSOffButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMOnButt->mGrid.Indices.begin(), mMOnButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMOffButt->mGrid.Indices.begin(), mMOffButt->mGrid.Indices.end());
	indices.insert(indices.end(), mTitleButt->mGrid.Indices.begin(), mTitleButt->mGrid.Indices.end());
	indices.insert(indices.end(), mAboutButt->mGrid.Indices.begin(), mAboutButt->mGrid.Indices.end());

	indices.insert(indices.end(), mBymeButt->mGrid.Indices.begin(), mBymeButt->mGrid.Indices.end());

	indices.insert(indices.end(), mQuitButt->mGrid.Indices.begin(), mQuitButt->mGrid.Indices.end());
	indices.insert(indices.end(), mRestartButt->mGrid.Indices.begin(), mRestartButt->mGrid.Indices.end());
	indices.insert(indices.end(), mPausedButt->mGrid.Indices.begin(), mPausedButt->mGrid.Indices.end());
	indices.insert(indices.end(), mBackButt->mGrid.Indices.begin(), mBackButt->mGrid.Indices.end());
	indices.insert(indices.end(), mAboutMsgButt->mGrid.Indices.begin(), mAboutMsgButt->mGrid.Indices.end());

	indices.insert(indices.end(), mYouWinButt->mGrid.Indices.begin(), mYouWinButt->mGrid.Indices.end());
	indices.insert(indices.end(), mYouLoseButt->mGrid.Indices.begin(), mYouLoseButt->mGrid.Indices.end());
	indices.insert(indices.end(), mRetryButt->mGrid.Indices.begin(), mRetryButt->mGrid.Indices.end());

	indices.insert(indices.end(), mModeButt->mGrid.Indices.begin(), mModeButt->mGrid.Indices.end());
	indices.insert(indices.end(), mEasyButt->mGrid.Indices.begin(), mEasyButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMedButt->mGrid.Indices.begin(), mMedButt->mGrid.Indices.end());
	indices.insert(indices.end(), mHardButt->mGrid.Indices.begin(), mHardButt->mGrid.Indices.end());
	indices.insert(indices.end(), mInsaneButt->mGrid.Indices.begin(), mInsaneButt->mGrid.Indices.end());

}
void Engine::InsertAllVertices(std::vector<Vertex::Basic32>& vertices, UINT& k)
{
	mPlayButt->LoadVertData(vertices, k);
	mSoundButt->LoadVertData(vertices, k);
	mMusicButt->LoadVertData(vertices, k);
	mSOnButt->LoadVertData(vertices, k);
	mSOffButt->LoadVertData(vertices, k);
	mMOnButt->LoadVertData(vertices, k);
	mMOffButt->LoadVertData(vertices, k);
	mTitleButt->LoadVertData(vertices, k);
	mAboutButt->LoadVertData(vertices, k);

	mBymeButt->LoadVertData(vertices, k);

	mQuitButt->LoadVertData(vertices, k);
	mRestartButt->LoadVertData(vertices, k);
	mPausedButt->LoadVertData(vertices, k);
	mBackButt->LoadVertData(vertices, k);
	mAboutMsgButt->LoadVertData(vertices, k);

	mYouWinButt->LoadVertData(vertices, k);
	mYouLoseButt->LoadVertData(vertices, k);
	mRetryButt->LoadVertData(vertices, k);

	mModeButt->LoadVertData(vertices, k);
	mEasyButt->LoadVertData(vertices, k);
	mMedButt->LoadVertData(vertices, k);
	mHardButt->LoadVertData(vertices, k);
	mInsaneButt->LoadVertData(vertices, k);
}
UINT Engine::TotalVertexCount()
{
	return 	mPlayButt->mGrid.Vertices.size()
		+ mSoundButt->mGrid.Vertices.size()
		+ mMusicButt->mGrid.Vertices.size()
		+ mSOnButt->mGrid.Vertices.size()
		+ mSOffButt->mGrid.Vertices.size()
		+ mMOnButt->mGrid.Vertices.size()
		+ mMOffButt->mGrid.Vertices.size()
		+ mTitleButt->mGrid.Vertices.size()
		+ mAboutButt->mGrid.Vertices.size()

		+ mBymeButt->mGrid.Vertices.size()

		+ mQuitButt->mGrid.Vertices.size()
		+ mRestartButt->mGrid.Vertices.size()
		+ mPausedButt->mGrid.Vertices.size()
		+ mBackButt->mGrid.Vertices.size()
		+ mAboutMsgButt->mGrid.Vertices.size()

		+ mYouWinButt->mGrid.Vertices.size()
		+ mYouLoseButt->mGrid.Vertices.size()
		+ mRetryButt->mGrid.Vertices.size()

		+ mModeButt->mGrid.Vertices.size()
		+ mEasyButt->mGrid.Vertices.size()
		+ mMedButt->mGrid.Vertices.size()
		+ mHardButt->mGrid.Vertices.size()
		+ mInsaneButt->mGrid.Vertices.size();
}
UINT Engine::TotalIndiceCount()
{
	return mPlayButt->mIndexCount
		+ mSoundButt->mIndexCount
		+ mMusicButt->mIndexCount
		+ mSOnButt->mIndexCount
		+ mSOffButt->mIndexCount
		+ mMOnButt->mIndexCount
		+ mMOffButt->mIndexCount
		+ mTitleButt->mIndexCount
		+ mAboutButt->mIndexCount

		+ mBymeButt->mIndexCount

		+ mQuitButt->mIndexCount
		+ mRestartButt->mIndexCount
		+ mPausedButt->mIndexCount
		+ mBackButt->mIndexCount
		+ mAboutMsgButt->mIndexCount

		+ mYouWinButt->mIndexCount
		+ mYouLoseButt->mIndexCount
		+ mRetryButt->mIndexCount

		+ mModeButt->mIndexCount
		+ mEasyButt->mIndexCount
		+ mMedButt->mIndexCount
		+ mHardButt->mIndexCount
		+ mInsaneButt->mIndexCount;
}
void Engine::SetAllVertexOffsets()
{
	mPlayButt->SetVertexOffset(0);
	mSoundButt->SetVertexOffset(mPlayButt->GetVertOffset() + mPlayButt->mGrid.Vertices.size());
	mMusicButt->SetVertexOffset(mSoundButt->GetVertOffset() + mSoundButt->mGrid.Vertices.size());
	mSOnButt->SetVertexOffset(mMusicButt->GetVertOffset() + mMusicButt->mGrid.Vertices.size());
	mSOffButt->SetVertexOffset(mSOnButt->GetVertOffset() + mSOnButt->mGrid.Vertices.size());
	mMOnButt->SetVertexOffset(mSOffButt->GetVertOffset() + mSOffButt->mGrid.Vertices.size());
	mMOffButt->SetVertexOffset(mMOnButt->GetVertOffset() + mMOnButt->mGrid.Vertices.size());
	mTitleButt->SetVertexOffset(mMOffButt->GetVertOffset() + mMOffButt->mGrid.Vertices.size());
	mAboutButt->SetVertexOffset(mTitleButt->GetVertOffset() + mTitleButt->mGrid.Vertices.size());

	mBymeButt->SetVertexOffset(mAboutButt->GetVertOffset() + mAboutButt->mGrid.Vertices.size());

	mQuitButt->SetVertexOffset(mBymeButt->GetVertOffset() + mBymeButt->mGrid.Vertices.size());
	mRestartButt->SetVertexOffset(mQuitButt->GetVertOffset() + mQuitButt->mGrid.Vertices.size());
	mPausedButt->SetVertexOffset(mRestartButt->GetVertOffset() + mRestartButt->mGrid.Vertices.size());
	mBackButt->SetVertexOffset(mPausedButt->GetVertOffset() + mPausedButt->mGrid.Vertices.size());
	mAboutMsgButt->SetVertexOffset(mBackButt->GetVertOffset() + mBackButt->mGrid.Vertices.size());


	mYouWinButt->SetVertexOffset(mAboutMsgButt->GetVertOffset() + mAboutMsgButt->mGrid.Vertices.size());
	mYouLoseButt->SetVertexOffset(mYouWinButt->GetVertOffset() + mYouWinButt->mGrid.Vertices.size());
	mRetryButt->SetVertexOffset(mYouLoseButt->GetVertOffset() + mYouLoseButt->mGrid.Vertices.size());


	mModeButt->SetVertexOffset(mRetryButt->GetVertOffset() + mRetryButt->mGrid.Vertices.size());
	mEasyButt->SetVertexOffset(mModeButt->GetVertOffset() + mModeButt->mGrid.Vertices.size());
	mMedButt->SetVertexOffset(mEasyButt->GetVertOffset() + mEasyButt->mGrid.Vertices.size());
	mHardButt->SetVertexOffset(mMedButt->GetVertOffset() + mMedButt->mGrid.Vertices.size());
	mInsaneButt->SetVertexOffset(mHardButt->GetVertOffset() + mHardButt->mGrid.Vertices.size());

}
void Engine::SetAllIndexOffsets()
{
		// Cache the index count of each object.
	mPlayButt->SetIndexOffset(0);
	mSoundButt->SetIndexOffset(		mPlayButt->GetIndOffset()		+ mPlayButt->mGrid.Indices.size());
	mMusicButt->SetIndexOffset(		mSoundButt->GetIndOffset()		+ mSoundButt->mGrid.Indices.size());
	mSOnButt->SetIndexOffset(		mMusicButt->GetIndOffset()		+ mMusicButt->mGrid.Indices.size());
	mSOffButt->SetIndexOffset(		mSOnButt->GetIndOffset()		+ mSOnButt->mGrid.Indices.size());
	mMOnButt->SetIndexOffset(		mSOffButt->GetIndOffset()		+ mSOffButt->mGrid.Indices.size());
	mMOffButt->SetIndexOffset(		mMOnButt->GetIndOffset()		+ mMOnButt->mGrid.Indices.size());
	mTitleButt->SetIndexOffset(		mMOffButt->GetIndOffset()		+ mMOffButt->mGrid.Indices.size());
	mAboutButt->SetIndexOffset(		mTitleButt->GetIndOffset()		+ mTitleButt->mGrid.Indices.size());

	mBymeButt->SetIndexOffset(mAboutButt->GetIndOffset() + mAboutButt->mGrid.Indices.size());

	mQuitButt->SetIndexOffset(mBymeButt->GetIndOffset() + mBymeButt->mGrid.Indices.size());
	mRestartButt->SetIndexOffset(	mQuitButt->GetIndOffset()		+ mQuitButt->mGrid.Indices.size());
	mPausedButt->SetIndexOffset(	mRestartButt->GetIndOffset()	+ mRestartButt->mGrid.Indices.size());
	mBackButt->SetIndexOffset(		mPausedButt->GetIndOffset()		+ mPausedButt->mGrid.Indices.size());
	mAboutMsgButt->SetIndexOffset(	mBackButt->GetIndOffset()		+ mBackButt->mGrid.Indices.size());


	mYouWinButt->SetIndexOffset(mAboutMsgButt->GetIndOffset() + mAboutMsgButt->mGrid.Indices.size());
	mYouLoseButt->SetIndexOffset(	mYouWinButt->GetIndOffset()		+ mYouWinButt->mGrid.Indices.size());
	mRetryButt->SetIndexOffset(		mYouLoseButt->GetIndOffset()	+ mYouLoseButt->mGrid.Indices.size());


	mModeButt->SetIndexOffset(mRetryButt->GetIndOffset() + mRetryButt->mGrid.Indices.size());
	mEasyButt->SetIndexOffset(		mModeButt->GetIndOffset()		+ mModeButt->mGrid.Indices.size());
	mMedButt->SetIndexOffset(		mEasyButt->GetIndOffset()		+ mEasyButt->mGrid.Indices.size());
	mHardButt->SetIndexOffset(		mMedButt->GetIndOffset()		+ mMedButt->mGrid.Indices.size());
	mInsaneButt->SetIndexOffset(	mHardButt->GetIndOffset()		+ mHardButt->mGrid.Indices.size());

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


		mPlayButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mSoundButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mMusicButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mTitleButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mAboutButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());

		mModeButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		switch (*StateMachine::pGameMode)
		{
		case GameMode::EASY:mEasyButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());		difficultyTimer = 1.0f;	break;
		case GameMode::MED:mMedButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());		difficultyTimer = 0.8f;	break;
		case GameMode::HARD:mHardButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());		difficultyTimer = 0.5f;	break;
		case GameMode::INSANE:mInsaneButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());	difficultyTimer = 0.2f;	break;
		}


		if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSOnButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }
		else{ mSOffButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }
		if (*StateMachine::pMusicState == MusicState::MUSICON){ mMOnButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }
		else { mMOffButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }
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
	RestoreStates();

	//SHADOWS *******************************************************************************************************

	//set shadow states
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
	md3dImmediateContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0); // changing 0 means overlaping draws
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

	//Set Shadow Constants
	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
	XMVECTOR toMainLight = -XMLoadFloat3(&mDirLights[0].Direction);
	XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
	float xOffSet = 43.0f; float zOffSet = 90.0f; float yOffSet = 0.1f; float sScale = 0.15f;

	mPlayButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSoundButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMusicButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSOnButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSOffButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMOnButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMOffButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mTitleButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet + 100.0f, mCam, mShadowMat);
	mAboutButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mModeButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale/2, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mEasyButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale/2, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	// Restore default states.
	RestoreStates();

	if(!fullyLoaded)fullyLoaded = true;
}
void Engine::DrawAbout()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	// Figure out which technique to use.  Skull does not have texture coordinates,
	// so we need a separate technique for it.
	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the grid.
		XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mGridMat);
		Effects::BasicFX->SetDiffuseMap(mFloorTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

		//DRAW BUTTS

		mAboutMsgButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mTitleButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mBackButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mBymeButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());

	}

	//SHADOWS *******************************************************************************************************

	//set shadow states
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
	md3dImmediateContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0); // changing 0 means overlaping draws
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

	//Set Shadow Constants
	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
	XMVECTOR toMainLight = -XMLoadFloat3(&mDirLights[0].Direction);
	XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
	float xOffSet = 43.0f; float zOffSet = 90.0f; float yOffSet = 0.1f; float sScale = 0.1f;

	mAboutMsgButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mTitleButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet+100.0f, mCam, mShadowMat);
	mBackButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mBymeButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);

	// Restore default states.
	RestoreStates();
}
void Engine::DrawPaused()
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
		mPausedButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mQuitButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mRestartButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
	}

	RestoreStates();
	DrawGameOn();
}
void Engine::DrawGameOn()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());


	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the grid.
		XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mGridMat);
		Effects::BasicFX->SetDiffuseMap(mFloorTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);


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
		mQuitButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mRestartButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
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
		mYouLoseButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mQuitButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mRetryButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
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
		mYouWinButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mQuitButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mRestartButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
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
/*		mBSOD->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);*/
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
	}
}
void Engine::KeyboardHandler(float dt)
{
	//
	// Control the camera.
	//
	if (*StateMachine::pGameState == GameState::GAMEON)
	{
		mCam.mUseConstraints = true;

		if (GetAsyncKeyState('W') & 0x8000)
			mCam.Walk(mMoveSpeed*dt);
	
		if (GetAsyncKeyState('S') & 0x8000)
			mCam.Walk(-mMoveSpeed*dt);
	
		if (GetAsyncKeyState('A') & 0x8000)
			mCam.Strafe(-mMoveSpeed*dt);
	
		if (GetAsyncKeyState('D') & 0x8000)
			mCam.Strafe(mMoveSpeed*dt);
	

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
	if (InButton3D(x, y, mPlayButt))
	{	
		mPlayButt->hovering = true;
		if (clicked)
		{
			ClearVectors();
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			*StateMachine::pGameState = GameState::GAMEON;
			if (!mSound.MusicPaused(1)){ mSound.PauseMusic(true, 1); }
			ResetCamInGame();
		}
	}
	else{ mPlayButt->hovering = false;  }

	if (InButton3D(x, y, mAboutButt))
	{
	
	
		mAboutButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			*StateMachine::pGameState = GameState::ABOUT;
		}
	}
	else{ mAboutButt->hovering = false; }

	if (InButton3D(x, y, mSoundButt))
	{
		mSoundButt->hovering = true;
		if (clicked)
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
	}
	else{ mSoundButt->hovering = false; }

	if (InButton3D(x, y, mMusicButt))
	{
		mMusicButt->hovering = true;
		if (clicked)
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
	}
	else{ mMusicButt->hovering = false; }

	if (InButton3D(x, y, mModeButt))
	{
		mModeButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			switch (*StateMachine::pGameMode)
			{
			case GameMode::EASY: *StateMachine::pGameMode	= GameMode::MED;	break;
			case GameMode::MED:*StateMachine::pGameMode		= GameMode::HARD;	break;
			case GameMode::HARD:*StateMachine::pGameMode	= GameMode::INSANE; break;
			case GameMode::INSANE:*StateMachine::pGameMode	= GameMode::EASY;	break;
			}
		}
	}
	else{ mModeButt->hovering = false; }

}
void Engine::BtnsAbout(float x, float y, bool clicked)
{
	if (InButton3D(x, y, mBackButt))
	{
		mBackButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			*StateMachine::pGameState = GameState::MAINMENU;
		}
	}
	else{ mBackButt->hovering = false; }
}
void Engine::BtnsPaused(float x, float y, bool clicked)
{
	if (!mSound.MusicPaused(2)){ mSound.PauseMusic(true, 2); }
	if (InButton2D(x, y, mQuitButt))
	{
		mQuitButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			*StateMachine::pGameState = GameState::MAINMENU;
			mWalkCamMode = false;
			ResetCamMainMenu();
			ClearVectors();
		}
	}
	else{ mQuitButt->hovering = false; }

	if (InButton2D(x, y, mRestartButt))
	{
		mRestartButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			*StateMachine::pGameState = GameState::GAMEON;
			ClearVectors();
			ResetCamInGame();
		}
	}
	else{ mRestartButt->hovering = false; }
}
void Engine::BtnsGameOn(float x, float y, bool clicked)
{
	//if (clicked){ SpawnProjectile(); }
}
void Engine::BtnsWin(float x, float y, bool clicked)
{
	if (InButton2D(x, y, mQuitButt))
	{
		mQuitButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			mWalkCamMode = false;
			*StateMachine::pGameState = GameState::MAINMENU;
			ResetCamMainMenu(); ClearVectors();
		}
	}
	else{ mQuitButt->hovering = false; }

	if (InButton2D(x, y, mRestartButt))
	{
		mRestartButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			*StateMachine::pGameState = GameState::GAMEON; ClearVectors(); ResetCamInGame();
		}
	}
	else{ mRestartButt->hovering = false; }

}
void Engine::BtnsLose(float x, float y, bool clicked)
{
	if (InButton2D(x, y, mQuitButt))
	{
		mQuitButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			mWalkCamMode = false;
			*StateMachine::pGameState = GameState::MAINMENU;
			ResetCamMainMenu(); ClearVectors();
		}
	}
	else{ mQuitButt->hovering = false; }

	if (InButton2D(x, y, mRetryButt))
	{
		mRetryButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			*StateMachine::pGameState = GameState::GAMEON; ClearVectors(); ResetCamInGame();
		}
	}
	else{ mRetryButt->hovering = false; }
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
	if (InButton2D(x, y, mQuitButt))
	{
		mQuitButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			mWalkCamMode = false;
			*StateMachine::pGameState = GameState::MAINMENU;
			ResetCamMainMenu(); ClearVectors();
		}
	}
	else{ mQuitButt->hovering = false; }

	if (InButton2D(x, y, mRestartButt))
	{
		mRestartButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSound.PlaySound(1); }
			*StateMachine::pGameState = GameState::GAMEON; ClearVectors(); ResetCamInGame();
		}
	}
	else{ mRestartButt->hovering = false; }
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
