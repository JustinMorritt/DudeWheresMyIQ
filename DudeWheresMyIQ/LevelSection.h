#ifndef LevelSection_h__
#define LevelSection_h__

#include <iostream>
#include <vector>
#include "Entity.h"
#include "d3dUtil.h"
#include "MathHelper.h"
#include "Inventory.h"

class LevelSection
{
public:
	LevelSection(float x, float y, float z, float size);
	~LevelSection();
	static void Init(ID3D11Device** device);
	void Update(const Camera& cam, float dt);
	void MakeChunk(float x, float y, float z, float w, float h, float d);
	void MakeObstacle(float x, float y, float z, float w, float h, float d);
	void MakeRandomEnemy(float x, float y, float z, float w, float h, float d);

	//OBSTACLES
	void MakeSpinBlock(float x, float y, float z, float w, float h, float d);
	void MakeSpinBlock2(float x, float y, float z, float w, float h, float d);
	void MakeUpDownBlock(float x, float y, float z, float w, float h, float d);
	void MakeSpinningStairs(float x, float y, float z, float w, float h, float d);
	void MakeDoubleStairClimb(float x, float y, float z, float w, float h, float d);
	void MakeFlippingBlock(float x, float y, float z, float w, float h, float d);
	void MakeRollingBlock(float x, float y, float z, float w, float h, float d);
	void MakeSideToSideBlock(float x, float y, float z, float w, float h, float d);
	void MakeMovingStairs(float x, float y, float z, float w, float h, float d);
	void MakeMovingStairs2(float x, float y, float z, float w, float h, float d);
	void MakeSpindal(float x, float y, float z, float w, float h, float d);
	void MakeSideSpindal(float x, float y, float z, float w, float h, float d);
	void PulseBall(float x, float y, float z, float w, float h, float d);
	void EarthOrbit(float x, float y, float z, float w, float h, float d);
	void MovingBalls(float x, float y, float z, float w, float h, float d);

	void Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt, XMMATRIX& shadow);
	void DrawShad(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj);
	void ShutDown();
	
	static ID3D11ShaderResourceView* mGrass;
	static ID3D11ShaderResourceView* mBush;
	static ID3D11ShaderResourceView* mEarth;
	static ID3D11ShaderResourceView* mMoon;
	static ID3D11ShaderResourceView* mBeachBall;

	//ENEMYS
	static ID3D11ShaderResourceView* mChick1;
	static ID3D11ShaderResourceView* mChick2;
	static ID3D11ShaderResourceView* mChick3;
	static ID3D11ShaderResourceView* mChick4;
	static ID3D11ShaderResourceView* mDude1;
	static ID3D11ShaderResourceView* mDude2;
	static ID3D11ShaderResourceView* mDude3;

	static ID3D11Device* mDevice;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	std::vector<Entity*> mEntities;
};
#endif // LevelSection_h__

