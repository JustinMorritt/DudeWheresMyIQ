#include "LevelSection.h"

ID3D11ShaderResourceView* LevelSection::mGrass;
ID3D11ShaderResourceView* LevelSection::mBush;
ID3D11Device* LevelSection::mDevice;

LevelSection::LevelSection(float x, float y, float z, float size)
{
	float origX = x;
	float origY = y;
	float currX = x;
	float currY = y;
	float totalWidth = 0.0f;
	float textHeight = size;
	float finalWidth = 0.0f;
	float mult = 10.0f;

	MakeChunk(currX, currY, z, size*mult, size, size*mult); currX += size * mult; //Initial Landing

	int num = MathHelper::RandI(25.0f, 25.0f);
	for (int i = 0; i < num; i++)
	{
		float rand = MathHelper::RandF();
		rand > 0.3 ? MakeChunk(currX, currY, z, size*mult, size, size*mult) : MakeObstacle(currX, currY, z, size*mult, size, size*mult);
		rand > 0.8 ? currX += size * mult*1.5 : currX += size * mult;  //MOVE NEXT AND RAND SPACING 
		rand > 0.5f ? currY += size : currY -= size ;				   //UP AND DOWN
		totalWidth += size; 
	}
}

LevelSection::~LevelSection()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	for (int i = 0; i < mEntities.size(); i++)
	{
		delete mEntities[i];
		mEntities[i] = nullptr;
		mEntities.erase(mEntities.begin() + i);
	}
	std::vector<Entity*> mEntities;
}

void LevelSection::Init(ID3D11Device** device)
{
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/grass.dds", 0, 0, &mGrass, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/bush.dds", 0, 0, &mBush, 0));
	mDevice = *device;
}

void LevelSection::Update(const Camera& cam, float dt)
{
	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->Update(cam, dt);
		if (mEntities[i]->mDead)
		{
			delete mEntities[i];
			mEntities[i] = nullptr;
			mEntities.erase(mEntities.begin() + i);
		}
	}
}

void LevelSection::MakeChunk(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	if (MathHelper::RandF() > 0.5){ Entity* B = new Entity(2, "bush", 60.0f, 30.0f); B->UseTexture(mBush); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 5.0f, z + (MathHelper::RandF(-d / 2, d / 2)));   mEntities.push_back(B); }
	if (MathHelper::RandF() > 0.2){ Entity* B = new Entity(4, "beer", 30.0f, 25.0f, 1.0f); B->SetToSpin(MathHelper::RandF(1.0f,10.0f), true); B->UseTexture(Inventory::mBeer); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2))); B->mUseAABOnce = true; B->mUseAAB = true;  mEntities.push_back(B); }
}

void LevelSection::MakeObstacle(float x, float y, float z, float w, float h, float d)
{

	float rand = MathHelper::RandF();
	rand > 0.8 ? MakeSpinBlock2(x, y, z, w, h, d)		:
	rand > 0.7 ? MakeMovingStairs2(x, y, z, w, h, d)	:
	rand > 0.6 ? MakeMovingStairs(x, y, z, w, h, d)		:
	rand > 0.5 ? MakeSpinBlock(x, y, z, w, h, d)		:
	rand > 0.4 ? MakeUpDownBlock(x, y, z, w, h, d)		:
	rand > 0.3 ? MakeFlippingBlock(x, y, z, w, h, d)	:
	rand > 0.2 ? MakeRollingBlock(x, y, z, w, h, d)		:
	rand > 0.1 ? MakeSideToSideBlock(x, y, z, w, h, d)	:
	MakeSpinningStairs(x, y, z, w, h, d);

}
void LevelSection::MakeSpinBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d / 4); E->SetToSpin(1.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}



void LevelSection::MakeSpinBlock2(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d / 4); E->SetToSpin(1.0f, true);   E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	MathHelper::RandF() > 0.5 ? E->SetToFlip(1.0f, true) : E->SetToRoll(1.0f, true);
}

void LevelSection::MakeUpDownBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->SetToGoUpAndDown(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}

void LevelSection::MakeSpinningStairs(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y;
	float currRot = 0.0f;
	for (int i = 0; i < 10; i++)
	{
		E = new Entity(3, "ground", w/4, h, d*1.5); E->Yaw(currRot); E->SetToSpin(-0.3f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, currY, z);  mEntities.push_back(E);
		currY += h*1.5;
		currRot += 0.2f;
	}
}

void LevelSection::MakeFlippingBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->SetToFlip(0.5f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);

}

void LevelSection::MakeRollingBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->SetToRoll(1.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}

void LevelSection::MakeSideToSideBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}

void LevelSection::MakeMovingStairs(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y+h;
	float currPos = (w/8)*-6.0f;
	for (int i = 0; i < 7; i++)
	{
		E = new Entity(3, "ground", w / 8, h/1.5, d); E->SetPos(x, currY, z); E->Strafe(currPos); E->SetBackAndForth(50.0f, 130.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true;   mEntities.push_back(E);
		currY += h;
		currPos += w/8;
	}
}

void LevelSection::MakeMovingStairs2(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y + h;
	float currPos = (d / 8)*-6.0f;
	for (int i = 0; i < 7; i++)
	{
		E = new Entity(3, "ground", w, h / 1.5, d/8); E->SetPos(x, currY, z); E->Walk(currPos); E->SetSideToSide(50.0f, 130.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true;   mEntities.push_back(E);
		currY += h;
		currPos += d / 8;
	}
}

void LevelSection::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt, XMMATRIX& shadow)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->SetShadTrans(shadow);
		mEntities[i]->Draw(*activeTech, context, pass, camera, dt);
	}
}

void LevelSection::DrawShad(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->DrawShadow(*activeTech, context, camera, lightView, lightProj);
	}
}

void LevelSection::ShutDown()
{
	if (mGrass){ mGrass->Release(); mGrass = 0; }
}


