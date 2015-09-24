#ifndef Engine_h__
#define Engine_h__

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "Camera.h"
#include "Sky.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "StateMachine.h"
#include "Entity.h"
#include "xnacollision.h"
#include "MathHelper.h"
#include "TextureMgr.h"
#include "BasicModel.h"
#include "Sound.h"
#include <iostream>
#include <vector>
#include "Text.h"
#include "LevelSection.h"
#include "Player.h"


class Engine : public D3DApp
{
public:
	Engine(HINSTANCE hInstance);
	~Engine();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void CamFollowPlayer();
	void OnKeyUP(WPARAM btnState);
	void OnKeyDOWN(WPARAM btnState);
	void KeyboardHandler(float dt);
	void CameraHandler();
	void ClearScene();
	void RestoreStates();
	void ResetCamMainMenu();
	void ResetCamInGame();
	void IncProgress(float dt);
	void IncBugs(float dt);
	void DecBugs(float dt);
	void DecHP(float dt);
	void ClearVectors();

	//SCENE INITS
	void InitAll();
	void InsertAllIndices(std::vector<UINT>& indicies, std::vector<Entity*>& entitys);
	void InsertAllVertices(std::vector<Vertex::Basic32>& verts, UINT& k, std::vector<Entity*>& entitys);
	UINT TotalVertexCount(std::vector<Entity*>& entitys);
	UINT TotalIndiceCount(std::vector<Entity*>& entitys);
	void SetAllVertexOffsets(std::vector<Entity*>& entitys);
	void SetAllIndexOffsets(std::vector<Entity*>& entitys);
	void BuildVertexAndIndexBuffer(ID3D11Buffer** VB, ID3D11Buffer** IB, std::vector<Entity*>& entities);


	//GAME DRAWS
	void DrawMainMenu();
	void DrawAbout();
	void DrawPaused();
	void DrawGameOn();
	void DrawWin();
	void DrawLose();
	void DrawBossWin();
	void DrawBossLose();

	//UPDATES
	void UpdateMainMenu(float dt);
	void UpdateGame(float dt);

	//SPAWNERS
	void SpawnBug();
	void SpawnMushroom();
	void SpawnProjectile();
	void SpawnGhost();


	//BUTTON HANDLERS
	void BtnsMainMenu(float x, float y, bool clicked);
	void BtnsAbout(float x, float y, bool clicked);
	void BtnsPaused(float x, float y, bool clicked);
	void BtnsGameOn(float x, float y, bool clicked);
	void BtnsWin(float x, float y, bool clicked);
	void BtnsLose(float x, float y, bool clicked);
	void BtnsBossLose(float x, float y, bool clicked);
	void BtnsBossWin(float x, float y, bool clicked);
	bool InButton3D(float sx, float sy, Entity* button);
	bool InButton2D(float sx, float sy, Entity* button);

	//BUTTON FUNCITONS
	void Play();
	void About();
	void Music();
	void SoundFX();
	void AboutBack();
	void Quit();
	void Restart();

	//COLLISION
	bool CamOnPickUp(Entity* pickup);
	bool ProjectileBounds(Entity* proj);
	bool ProjectileHitBug(Entity* proj);
	bool ProjectileHitBoss(Entity* proj);
	bool ProjectileHitGhost(Entity* proj);
	bool GhostHitCam(Entity* ghost);
	bool BossHitCam(Entity* ghost);

private:
	Sky* mSky;
	TextureMgr mTexMgr;
	Camera mCam;
	Sound mSound;
	Player* mPlayer;

	ID3D11Buffer* mShapesVB;
	ID3D11Buffer* mShapesIB;



	//TODO:: Vector for Menu UI .. Level Objects .. Inventory ... Pickups... Enemies...
	std::vector<Entity*> mUI;
	std::vector<Entity*> mMain;		std::vector<Entity*> mMainBtns;
	std::vector<Entity*> mAbout;	std::vector<Entity*> mAboutBtns;
	std::vector<Entity*> mPaused;	std::vector<Entity*> mPausedBtns;


	std::vector<Text*> mTexts;
	std::vector<LevelSection*> mLevel;

	//Models
	std::vector<BasicModelInstance> mModelInstances;
	BasicModel* testModel;

	//Progress Bars 
	//Entity* mCompBar;


	//Spawn Timers
	float tickTimer; // Set this per second ..
	float bossTimer;
	float randBossTime;
	bool toRandSpot;
	bool toCam;
	float bugsWorth;
	float dmgAmount;
	float difficultyTimer;
	float farPlane;
	int spawnBugTime;
	int spawnMushTime;
	int speedBonusTime;
	int waitToClickTime;
	int mMoveSpeed;

// 	ID3D11ShaderResourceView* mFlareTexSRV;
// 	ID3D11ShaderResourceView* mRainTexSRV;
//	ID3D11ShaderResourceView* mRandomTexSRV;

	ParticleSystem mFire;
	ParticleSystem mRain;

	DirectionalLight mDirLights[3];
	DirectionalLight mDirLights2[3];
	DirectionalLight mDirLights3[3];

	ID3D11ShaderResourceView* mFloorTexSRV;

	Material mGridMat;
	Material mShadowMat;

	XMFLOAT4X4 mGridWorld;
	int mGridVertexOffset;
	UINT mGridIndexOffset;
	UINT mGridIndexCount;

	bool mWalkCamMode;
	bool mWireMode;
	bool mBFCull;
	bool fullyLoaded;
	bool exitable;

	POINT mLastMousePos;

	XMMATRIX mOrthoWorld;
};




#endif // Engine_h__
