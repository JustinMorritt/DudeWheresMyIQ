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
#include "Inventory.h"
#include "ShadowMap.h"
#include "Battle.h"


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
	void RebuildLevel();
	void ResetPlayer();
	static void NewBattle();

	//SCENE INITS
	void InitAll();
	static void InsertAllIndices(std::vector<UINT>& indicies, std::vector<Entity*>& entitys);
	static void InsertAllVertices(std::vector<Vertex::Basic32>& verts, UINT& k, std::vector<Entity*>& entitys);
	static UINT TotalVertexCount(std::vector<Entity*>& entitys);
	static UINT TotalIndiceCount(std::vector<Entity*>& entitys);
	static void SetAllVertexOffsets(std::vector<Entity*>& entitys);
	static void SetAllIndexOffsets(std::vector<Entity*>& entitys);
	static void BuildVertexAndIndexBuffer(ID3D11Buffer** VB, ID3D11Buffer** IB, std::vector<Entity*>& entities);


	//GAME DRAWS
	void DrawMainMenu();
	void DrawAbout();
	void DrawPaused();
	void DrawGameOn();
	void DrawWin();
	void DrawLose();
	void DrawBattle();
	void DrawInventory();

	//UPDATES
	void UpdateMainMenu(float dt);
	void UpdateGame(float dt);
	void UpdateBattle(float dt);
	void UpdateInventory(float dt);

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
	void BtnsInventory(float x, float y, bool clicked);
	void BtnsBattle(float x, float y, bool clicked);
	bool InButton3D(float sx, float sy, Entity* button);
	bool InButton2D(float sx, float sy, Entity* button);

	//BUTTON FUNCITONS
	void Play();
	void About();
	void Music();
	void SoundFX();
	void AboutBack();
	void FreeCam();
	void LockCam();
	void Quit();
	void Restart();
	void NextLevel();

	//COLLISION
	bool CamOnPickUp(Entity* pickup);
	bool ProjectileBounds(Entity* proj);
	bool ProjectileHitBug(Entity* proj);
	bool ProjectileHitBoss(Entity* proj);
	bool ProjectileHitGhost(Entity* proj);
	bool GhostHitCam(Entity* ghost);
	bool BossHitCam(Entity* ghost);

	//SHADOW MAP
	void DrawSceneToShadowMap();
	void BuildShadowTransform();

private:
	Sky* mSky;

	BoundingSphere mSceneBounds;
	static const int SMapSize = 2048;
	ShadowMap* mSmap;
	XMFLOAT4X4 mLightView;
	XMFLOAT4X4 mLightProj;
	XMFLOAT4X4 mShadowTransform;

	TextureMgr mTexMgr;
	Camera mCam;
	Sound mSound;
	Player* mPlayer;
	Inventory* mInventory;
	LevelSection* mLevel;
	static Battle* mBattle;

	ID3D11Buffer* mShapesVB;
	ID3D11Buffer* mShapesIB;



	//TODO:: Vector for Menu UI .. Level Objects .. Inventory ... Pickups... Enemies...
	std::vector<Entity*> mUI;
	std::vector<Entity*> mMain;		std::vector<Entity*> mMainBtns;
	std::vector<Entity*> mAbout;	std::vector<Entity*> mAboutBtns;
	std::vector<Entity*> mPaused;	std::vector<Entity*> mPausedBtns;
	std::vector<Entity*> mInv;		std::vector<Entity*> mInvBtns;
	std::vector<Entity*> mBattleBtns;


	std::vector<Text*> mTexts;
	std::vector<Text*> mBattleText;
	std::vector<Text*> mInventoryText;
	

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
	int mCurrLevel;
	bool mFreeCam;

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
