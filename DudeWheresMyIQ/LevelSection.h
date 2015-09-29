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

	//OBSTACLES
	void MakeSpinBlock(float x, float y, float z, float w, float h, float d);
	void MakeSpinBlock2(float x, float y, float z, float w, float h, float d);
	void MakeUpDownBlock(float x, float y, float z, float w, float h, float d);
	void MakeSpinningStairs(float x, float y, float z, float w, float h, float d);
	void MakeFlippingBlock(float x, float y, float z, float w, float h, float d);
	void MakeRollingBlock(float x, float y, float z, float w, float h, float d);
	void MakeSideToSideBlock(float x, float y, float z, float w, float h, float d);
	void MakeMovingStairs(float x, float y, float z, float w, float h, float d);
	void MakeMovingStairs2(float x, float y, float z, float w, float h, float d);
	void MakeSpindal(float x, float y, float z, float w, float h, float d);
	void MakeSideSpindal(float x, float y, float z, float w, float h, float d);

	void Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt, XMMATRIX& shadow);
	void DrawShad(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj);
	void ShutDown();
	
	static ID3D11ShaderResourceView* mGrass;
	static ID3D11ShaderResourceView* mBush;

	static ID3D11Device* mDevice;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	std::vector<Entity*> mEntities;
};
#endif // LevelSection_h__

